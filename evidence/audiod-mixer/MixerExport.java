import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.DecompInterface;
import ghidra.app.decompiler.DecompileResults;
import ghidra.app.decompiler.DecompileOptions;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.address.Address;
import ghidra.program.model.data.DWordDataType;
import ghidra.program.model.data.MutabilitySettingsDefinition;
import java.io.*;
import java.util.*;

public class MixerExport extends GhidraScript {
    public void run() throws Exception {
        String[] arguments = getScriptArgs();
        File output = new File(arguments[0]);
        output.mkdirs();
        Set<Function> selected = new LinkedHashSet<>();
        try (PrintWriter index = new PrintWriter(new File(output, "functions.tsv"));
             PrintWriter literals = new PrintWriter(new File(output, "mixer-literals.tsv"))) {
            index.println("image_base\t" + currentProgram.getImageBase());
            index.println("sha256\t" + currentProgram.getExecutableSHA256());
            FunctionIterator functions = currentProgram.getFunctionManager().getFunctions(true);
            while (functions.hasNext()) {
                Function function = functions.next();
                if (function.isExternal() || function.isThunk()) continue;
                index.println(function.getEntryPoint() + "\t" + function.getBody().getNumAddresses() + "\t" + function.getName(true));
                if (arguments.length == 1 && function.getName(true).matches(".*(setMixer|[Mm]ixing|updateMixer|checkMixer|setMix).*")) selected.add(function);
            }
            DataIterator data = currentProgram.getListing().getDefinedData(true);
            while (data.hasNext()) {
                Data item = data.next();
                if (!item.hasStringValue() || item.getValue() == null) continue;
                String text = item.getValue().toString();
                if (!text.equals("setMixer") && !text.equals("mix")) continue;
                literals.println("STRING\t" + item.getAddress() + "\t" + text);
                for (Reference reference : getReferencesTo(item.getAddress())) {
                    Function owner = getFunctionContaining(reference.getFromAddress());
                    literals.println("XREF\t" + reference.getFromAddress() + "\t" + (owner == null ? "NO_FUNCTION" : owner.getName(true)));
                    if (arguments.length == 1 && owner != null) selected.add(owner);
                }
            }
        }
        for (int index = 1; index < arguments.length; index++) {
            Address address = toAddr(arguments[index]);
            Function function = getFunctionContaining(address);
            if (function != null) selected.add(function);
            else println("NO_FUNCTION " + address);
        }
        DecompInterface decompiler = new DecompInterface();
        DecompileOptions options = new DecompileOptions();
        options.setRespectReadOnly(true);
        decompiler.setOptions(options);
        for (Function function : selected) {
            disassemble(function.getEntryPoint());
            CreateFunctionCmd.fixupFunctionBody(currentProgram, function, monitor);
            InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
            while (instructions.hasNext()) {
                Instruction instruction = instructions.next();
                if (!instruction.getMnemonicString().startsWith("ldr") || !instruction.toString().contains("pc")) continue;
                for (Reference reference : instruction.getReferencesFrom()) {
                    Address target = reference.getToAddress();
                    if (reference.getReferenceType().isData() && getDataAt(target) == null && getInstructionContaining(target) == null) {
                        try { createData(target, new DWordDataType()); } catch (Exception ignored) { }
                    }
                    Data literal = getDataAt(target);
                    if (reference.getReferenceType().isData() && literal != null) {
                        MutabilitySettingsDefinition.DEF.setChoice(literal, MutabilitySettingsDefinition.CONSTANT);
                    }
                }
            }
        }
        decompiler.openProgram(currentProgram);
        String suffix = arguments.length == 1 ? "initial" : arguments[1];
        try (PrintWriter code = new PrintWriter(new File(output, "focus-" + suffix + ".c"));
             PrintWriter references = new PrintWriter(new File(output, "refs-" + suffix + ".tsv"));
             PrintWriter assembly = new PrintWriter(new File(output, "insns-" + suffix + ".tsv"))) {
            code.println("/* LOCAL STATIC EVIDENCE: Ghidra pseudocode, not original source. SHA256 " + currentProgram.getExecutableSHA256() + " image base " + currentProgram.getImageBase() + " */");
            for (Function function : selected) {
                code.println("\n/* @" + function.getEntryPoint() + " " + function.getName(true) + " */");
                DecompileResults result = decompiler.decompileFunction(function, 45, monitor);
                if (result.decompileCompleted() && result.getDecompiledFunction() != null) code.println(result.getDecompiledFunction().getC());
                else code.println("/* FAILED " + result.getErrorMessage() + " */");
                for (Reference reference : getReferencesTo(function.getEntryPoint())) {
                    Function owner = getFunctionContaining(reference.getFromAddress());
                    references.println(function.getEntryPoint() + "\t" + reference.getFromAddress() + "\t" + reference.getReferenceType() + "\t" + (owner == null ? "NO_FUNCTION" : owner.getEntryPoint() + " " + owner.getName(true)));
                }
                InstructionIterator instructions = currentProgram.getListing().getInstructions(function.getBody(), true);
                while (instructions.hasNext()) {
                    Instruction instruction = instructions.next();
                    assembly.println(function.getEntryPoint() + "\t" + instruction.getAddress() + "\t" + instruction);
                }
            }
        } finally {
            decompiler.dispose();
        }
        println("MIXER_EXPORT functions=" + selected.size() + " image_base=" + currentProgram.getImageBase());
    }
}