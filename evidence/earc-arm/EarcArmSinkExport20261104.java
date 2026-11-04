import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.decompiler.DecompileResults;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressRange;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import ghidra.program.model.symbol.Symbol;
import ghidra.program.model.symbol.SymbolIterator;
import ghidra.program.model.data.*;
import com.google.gson.GsonBuilder;
import java.io.*;
import java.nio.file.*;
import java.util.*;

public class EarcArmSinkExport20261104 extends GhidraScript {
    private String address(Address value) {
        return value == null ? null : value.toString();
    }

    private String hex(byte[] values) {
        StringBuilder result = new StringBuilder();
        for (byte value : values) result.append(String.format("%02x", value & 255));
        return result.toString();
    }

    private Map<String, Object> reference(Reference ref) {
        Map<String, Object> result = new LinkedHashMap<>();
        result.put("from", address(ref.getFromAddress()));
        result.put("to", address(ref.getToAddress()));
        result.put("type", ref.getReferenceType().toString());
        Symbol symbol = getSymbolAt(ref.getToAddress());
        result.put("symbol", symbol == null ? null : symbol.getName(true));
        return result;
    }

    private List<String> functionNames(Set<Function> functions) {
        List<String> result = new ArrayList<>();
        for (Function function : functions) {
            result.add(address(function.getEntryPoint()) + " " + function.getName(true));
        }
        Collections.sort(result);
        return result;
    }

    @Override
    public void run() throws Exception {
        Path output = Paths.get(getScriptArgs()[0]).toAbsolutePath();
        Files.createDirectories(output);
        long[] seeds = {0x18240, 0x27c40, 0x26fc4, 0x2441c, 0x24924, 0x26c6c,
            0x271f8, 0x2673c, 0x262a0, 0x282d0, 0x24b2c, 0x17004};
        Map<String, Object> report = new LinkedHashMap<>();
        report.put("evidence_origin", "local Ghidra existing database, not REA MCP");
        report.put("representation", "uncertain recovered C pseudocode, not original source");
        report.put("target_executed", false);
        report.put("database_modified_by_script", false);
        report.put("ghidra_version", getGhidraVersion());
        report.put("program", currentProgram.getName());
        report.put("executable_path_from_database", currentProgram.getExecutablePath());
        report.put("database_executable_sha256", currentProgram.getExecutableSHA256());
        report.put("image_base", address(currentProgram.getImageBase()));
        report.put("language", currentProgram.getLanguageID().toString());
        report.put("compiler_spec", currentProgram.getCompilerSpec().getCompilerSpecID().toString());
        List<Object> symbols = new ArrayList<>();
        SymbolIterator symbolIterator = currentProgram.getSymbolTable().getAllSymbols(true);
        while (symbolIterator.hasNext()) {
            Symbol symbol = symbolIterator.next();
            if (!symbol.getName().contains("set_caps")) continue;
            Map<String, Object> item = new LinkedHashMap<>();
            item.put("name", symbol.getName(true));
            item.put("address", address(symbol.getAddress()));
            item.put("type", symbol.getSymbolType().toString());
            symbols.add(item);
        }
        report.put("set_caps_symbols", symbols);
        List<Object> types = new ArrayList<>();
        Iterator<DataType> typeIterator = currentProgram.getDataTypeManager().getAllDataTypes();
        Set<String> requestedTypes = new HashSet<>(Arrays.asList("lgao", "aren_pcm_info_t",
            "aren_msgpid_to_signal_t", "LGAO_AUDIO_START_INFO_T", "LGAO_CLOCK_TYPE_T",
            "lgao_event", "lgao_mq_item", "aren_clock_type_t"));
        while (typeIterator.hasNext()) {
            DataType type = typeIterator.next();
            if (!requestedTypes.contains(type.getName())) continue;
            Map<String, Object> item = new LinkedHashMap<>();
            item.put("path", type.getPathName());
            item.put("length", type.getLength());
            item.put("definition_from_database", type.toString());
            DataType underlying = type;
            while (underlying instanceof TypeDef) underlying = ((TypeDef) underlying).getBaseDataType();
            if (underlying instanceof Composite) {
                List<Object> fields = new ArrayList<>();
                for (DataTypeComponent field : ((Composite) underlying).getComponents()) {
                    Map<String, Object> member = new LinkedHashMap<>();
                    member.put("offset", field.getOffset());
                    member.put("length", field.getLength());
                    member.put("name", field.getFieldName());
                    member.put("type", field.getDataType().getPathName());
                    fields.add(member);
                }
                item.put("fields", fields);
            }
            types.add(item);
        }
        report.put("recovered_types_not_runtime_verified", types);
        DecompInterface decompiler = new DecompInterface();
        decompiler.setOptions(new DecompileOptions());
        decompiler.toggleCCode(true);
        decompiler.toggleSyntaxTree(false);
        decompiler.setSimplificationStyle("decompile");
        if (!decompiler.openProgram(currentProgram)) throw new IOException(decompiler.getLastMessage());
        List<Object> functions = new ArrayList<>();
        Map<String, Object> literals = new TreeMap<>();
        int failures = 0;
        try (PrintWriter code = new PrintWriter(Files.newBufferedWriter(output.resolve("sink-focused.c")));
             PrintWriter assembly = new PrintWriter(Files.newBufferedWriter(output.resolve("sink-focused.arm.tsv")))) {
            code.println("/* LOCAL GHIDRA UNCERTAIN DECOMPILATION. Not original source. No target execution. */");
            assembly.println("function\taddress\tbytes\tlisting\treferences");
            for (long seed : seeds) {
                Function function = getFunctionAt(toAddr(seed));
                if (function == null) throw new IOException("Missing existing function at " + Long.toHexString(seed));
                Map<String, Object> item = new LinkedHashMap<>();
                item.put("name", function.getName(true));
                item.put("entry", address(function.getEntryPoint()));
                item.put("signature_from_database", function.getSignature().toString());
                item.put("body_bytes", function.getBody().getNumAddresses());
                List<Object> ranges = new ArrayList<>();
                for (AddressRange range : function.getBody()) {
                    ranges.add(Arrays.asList(address(range.getMinAddress()), address(range.getMaxAddress())));
                }
                item.put("body_ranges_inclusive", ranges);
                item.put("callers_database_direct", functionNames(function.getCallingFunctions(monitor)));
                item.put("callees_database_direct", functionNames(function.getCalledFunctions(monitor)));
                List<Object> incoming = new ArrayList<>();
                for (Reference ref : getReferencesTo(function.getEntryPoint())) incoming.add(reference(ref));
                item.put("entry_references", incoming);
                DecompileResults result = decompiler.decompileFunction(function, 90, monitor);
                boolean complete = result != null && result.decompileCompleted() && result.getDecompiledFunction() != null;
                item.put("decompile_completed", complete);
                item.put("decompile_message", result == null ? "null result" : result.getErrorMessage());
                code.println("\n/* @" + function.getEntryPoint() + " " + function.getName(true) + " */");
                if (complete) code.println(result.getDecompiledFunction().getC());
                else { failures++; code.println("/* DECOMPILE FAILED */"); }
                List<Object> outgoing = new ArrayList<>();
                int units = 0;
                CodeUnitIterator iterator = currentProgram.getListing().getCodeUnits(function.getBody(), true);
                while (iterator.hasNext()) {
                    CodeUnit unit = iterator.next();
                    StringBuilder refs = new StringBuilder();
                    for (Reference ref : getReferencesFrom(unit.getAddress())) {
                        outgoing.add(reference(ref));
                        refs.append(ref.getReferenceType()).append(":").append(ref.getToAddress()).append(" ");
                        Address target = ref.getToAddress();
                        if (!ref.getReferenceType().isData() || !currentProgram.getMemory().contains(target)) continue;
                        if (!literals.containsKey(address(target))) {
                            Map<String, Object> literal = new LinkedHashMap<>();
                            literal.put("address", address(target));
                            Data data = getDataContaining(target);
                            if (data != null) {
                                literal.put("defined_at", address(data.getAddress()));
                                literal.put("data_type", data.getDataType().getPathName());
                                literal.put("value", String.valueOf(data.getValue()));
                            }
                            try {
                                byte[] bytes = new byte[16];
                                int count = currentProgram.getMemory().getBytes(target, bytes);
                                literal.put("analysis_memory_bytes", hex(Arrays.copyOf(bytes, count)));
                                literal.put("u32_le", String.format("0x%08x", currentProgram.getMemory().getInt(target)));
                            } catch (Exception error) { literal.put("read_error", error.toString()); }
                            literals.put(address(target), literal);
                        }
                    }
                    assembly.println(function.getName(true) + "\t" + unit.getAddress() + "\t" + hex(unit.getBytes()) + "\t" + unit + "\t" + refs);
                    units++;
                }
                item.put("listing_code_units", units);
                item.put("outgoing_references", outgoing);
                functions.add(item);
                println("EARC_FUNCTION " + function.getEntryPoint() + " " + function.getName(true) + " complete=" + complete);
            }
        } finally { decompiler.dispose(); }
        report.put("functions", functions);
        report.put("decompile_failures", failures);
        report.put("literals", literals);
        report.put("coverage_limits", Arrays.asList("Existing database function boundaries and types retained without reanalysis",
            "set_caps may be inlined; no independent function is invented", "Indirect calls and runtime side effects are not established",
            "Literal bytes are Ghidra analysis memory; original ELF identity is checked separately"));
        try (Writer writer = Files.newBufferedWriter(output.resolve("sink-focused.coverage.json"))) {
            new GsonBuilder().setPrettyPrinting().serializeNulls().create().toJson(report, writer);
        }
        println("EARC_EXPORTED functions=" + functions.size() + " failed=" + failures);
        if (failures != 0) throw new IOException("Incomplete decompilation");
    }
}