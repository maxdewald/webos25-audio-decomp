//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.address.AddressRangeIterator;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import com.google.gson.GsonBuilder;
import java.io.*;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.util.*;

public class EarcDsp1Export extends GhidraScript {
    private File output;
    private final List<Map<String, Object>> literals = new ArrayList<>();
    private final List<Map<String, Object>> references = new ArrayList<>();
    private final Map<Long, Function> selected = new TreeMap<>();
    private final Set<Long> primary = new HashSet<>();

    private Map<String, Object> record(Object... pairs) {
        Map<String, Object> result = new LinkedHashMap<>();
        for (int index = 0; index < pairs.length; index += 2)
            result.put((String) pairs[index], pairs[index + 1]);
        return result;
    }

    private String hex(byte[] bytes) {
        StringBuilder result = new StringBuilder();
        for (byte value : bytes) result.append(String.format("%02x", value & 255));
        return result.toString();
    }

    private String textAt(Address address) {
        StringBuilder result = new StringBuilder();
        try {
            for (int index = 0; index < 320; index++) {
                int character = currentProgram.getMemory().getByte(address.add(index)) & 255;
                if (character == 0) return result.length() > 1 ? result.toString() : null;
                if ((character < 32 && character != 9 && character != 10 && character != 13) || character > 126)
                    return null;
                result.append((char) character);
            }
        } catch (Exception ignored) { }
        return null;
    }

    private void saveJson(String filename, Object value) throws Exception {
        try (Writer writer = new OutputStreamWriter(new FileOutputStream(new File(output, filename)), StandardCharsets.UTF_8)) {
            new GsonBuilder().setPrettyPrinting().disableHtmlEscaping().create().toJson(value, writer);
        }
    }

    private Function targetFunction(Address address) {
        Function function = getFunctionAt(address);
        return function == null ? getFunctionContaining(address) : function;
    }

    private void addFunctions(AddressSet range) {
        FunctionIterator functions = currentProgram.getFunctionManager().getFunctionsOverlapping(range);
        while (functions.hasNext()) {
            Function function = functions.next();
            selected.put(function.getEntryPoint().getOffset(), function);
            primary.add(function.getEntryPoint().getOffset());
        }
    }

    private void exportListing(String name, Address start, Address end, boolean context) throws Exception {
        AddressSet range = new AddressSet(start, end.subtract(1));
        File directory = new File(output, context ? "context" : "ranges");
        directory.mkdirs();
        long covered = 0;
        int opaque = 0;
        int instructionCount = 0;
        try (PrintWriter listing = new PrintWriter(new File(directory, start + "_" + name + ".asm"), "UTF-8")) {
            listing.println("RANGE\t" + start + "\t" + end + "\t" + name + "\tend_exclusive");
            CodeUnitIterator units = currentProgram.getListing().getCodeUnits(range, true);
            while (units.hasNext()) {
                CodeUnit unit = units.next();
                covered += unit.getLength();
                StringBuilder line = new StringBuilder(unit.getAddress() + "\t" + hex(unit.getBytes()) + "\t" + unit);
                if (unit instanceof Instruction) instructionCount++;
                if (unit.getMnemonicString().toLowerCase().contains("opaque")) opaque++;
                for (Reference reference : getReferencesFrom(unit.getAddress())) {
                    Address target = reference.getToAddress();
                    Function callee = targetFunction(target);
                    Map<String, Object> referenceRecord = record("site", unit.getAddress().toString(),
                        "target", target.toString(), "reference_type", reference.getReferenceType().toString(),
                        "target_saved_function", callee == null ? null : callee.getName(),
                        "range", name, "context", context);
                    references.add(referenceRecord);
                    line.append("\tref=").append(target);
                    if (callee != null && reference.getReferenceType().isCall() && !context && !callee.isExternal())
                        selected.put(callee.getEntryPoint().getOffset(), callee);
                    if (!reference.getReferenceType().isData()) continue;
                    String direct = textAt(target);
                    if (direct != null) {
                        literals.add(record("site", unit.getAddress().toString(), "target", target.toString(),
                            "kind", "direct_ascii", "text", direct, "range", name, "context", context));
                    }
                    try {
                        int value = currentProgram.getMemory().getInt(target);
                        Address pointer = toAddr(Integer.toUnsignedLong(value));
                        String text = textAt(pointer);
                        Map<String, Object> literal = record("site", unit.getAddress().toString(),
                            "slot", target.toString(), "value", String.format("%08x", value),
                            "kind", "referenced_little_endian_u32", "range", name, "context", context);
                        if (text != null) {
                            literal.put("text", text);
                            line.append("\tstring=").append(text.replace("\n", "\\n").replace("\r", "\\r"));
                        }
                        Function pointed = getFunctionAt(pointer);
                        if (pointed != null) {
                            literal.put("pointed_saved_function", pointed.getName());
                            if (!context && !pointed.isExternal()) selected.put(pointed.getEntryPoint().getOffset(), pointed);
                        }
                        literals.add(literal);
                        line.append(String.format("\tvalue=0x%08x", value));
                    } catch (Exception ignored) { }
                }
                listing.println(line);
            }
        }
        coverage.add(record("name", name, "start", start.toString(), "end_exclusive", end.toString(),
            "span_bytes", end.subtract(start), "listed_bytes", covered, "instruction_count", instructionCount,
            "opaque_instruction_count", opaque, "context", context));
    }

    private final List<Map<String, Object>> coverage = new ArrayList<>();

    @Override
    public void run() throws Exception {
        String[] arguments = getScriptArgs();
        output = new File(arguments[0]);
        if (!isRunningHeadless()) throw new IllegalStateException("Headless read-only export required");
        List<String[]> ranges = new ArrayList<>();
        for (String line : Files.readAllLines(new File(output, "ranges.tsv").toPath(), StandardCharsets.UTF_8)) {
            String[] fields = line.split("\t");
            ranges.add(fields);
            addFunctions(new AddressSet(toAddr(Long.parseLong(fields[0], 16)), toAddr(Long.parseLong(fields[1], 16) - 1)));
        }
        for (String[] range : ranges)
            exportListing(range[2], toAddr(Long.parseLong(range[0], 16)), toAddr(Long.parseLong(range[1], 16)), false);
        DecompInterface decompiler = new DecompInterface();
        decompiler.toggleCCode(true);
        decompiler.toggleSyntaxTree(false);
        decompiler.openProgram(currentProgram);
        List<Map<String, Object>> functionRecords = new ArrayList<>();
        int failures = 0;
        try (PrintWriter code = new PrintWriter(new File(output, "focused.c"), "UTF-8")) {
            code.println("/* Local Ghidra read-only export. Saved names/types and synthetic boundaries are untrusted. */");
            for (Function function : selected.values()) {
                if (monitor.isCancelled()) throw new InterruptedException("Export cancelled");
                List<String> bodyRanges = new ArrayList<>();
                AddressRangeIterator bodies = function.getBody().getAddressRanges();
                while (bodies.hasNext()) {
                    AddressRange body = bodies.next();
                    bodyRanges.add(body.getMinAddress() + ".." + body.getMaxAddress());
                    if (!primary.contains(function.getEntryPoint().getOffset()))
                        exportListing(function.getName().replaceAll("[^A-Za-z0-9_]", "_"), body.getMinAddress(), body.getMaxAddress().add(1), true);
                }
                DecompileResults result = decompiler.decompileFunction(function, 30, monitor);
                boolean success = result != null && result.decompileCompleted() && result.getDecompiledFunction() != null;
                code.println("\n/* @" + function.getEntryPoint() + " " + function.getName() + " saved_body=" + bodyRanges + " */");
                if (success) code.print(result.getDecompiledFunction().getC());
                else {
                    failures++;
                    code.println("/* Decompile failed: " + (result == null ? "null" : result.getErrorMessage()) + " */");
                }
                functionRecords.add(record("entry", function.getEntryPoint().toString(), "saved_name", function.getName(),
                    "saved_body", bodyRanges, "synthetic", function.getName().startsWith("__blk"),
                    "selected_by_range", primary.contains(function.getEntryPoint().getOffset()), "decompile_completed", success,
                    "decompile_message", result == null ? "null" : result.getErrorMessage()));
            }
        } finally {
            decompiler.dispose();
        }
        saveJson("functions.json", functionRecords);
        saveJson("literals.json", literals);
        saveJson("references.json", references);
        saveJson("coverage.json", coverage);
        saveJson("program.json", record("provider", "local fallback, not REA Evidence", "program", currentProgram.getName(),
            "ghidra_version", getGhidraVersion(), "language", currentProgram.getLanguageID().toString(),
            "executable_sha256", currentProgram.getExecutableSHA256(), "image_base", currentProgram.getImageBase().toString(),
            "exported_functions", functionRecords.size(), "decompile_failures", failures,
            "semantics_limit", "FLIX opaque8 has no semantics; fallthrough and decompiler types are not proof of complete control/data flow"));
        println("EARC_DSP1_EXPORTED functions=" + functionRecords.size() + " failures=" + failures + " literals=" + literals.size());
    }
}