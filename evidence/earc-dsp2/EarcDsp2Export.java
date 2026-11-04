import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.Address;
import ghidra.program.model.address.AddressSet;
import ghidra.program.model.listing.Instruction;
import ghidra.program.model.listing.InstructionIterator;
import ghidra.program.model.pcode.PcodeOp;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileReader;
import java.io.PrintWriter;

public class EarcDsp2Export extends GhidraScript {
    @Override
    public void run() throws Exception {
        File output = new File(getScriptArgs()[0]).getCanonicalFile();
        if (!output.getName().equals("earc-dsp2")) throw new IllegalArgumentException("Wrong output directory");
        if (!currentProgram.getLanguageID().toString().equals("Xtensa:LE:32:hifi_lg")) {
            throw new IllegalArgumentException("Unexpected language: " + currentProgram.getLanguageID());
        }
        try (PrintWriter info = new PrintWriter(new File(output, "ghidra-program.tsv"))) {
            info.println("ghidra_version\t" + getGhidraVersion());
            info.println("program\t" + currentProgram.getName());
            info.println("language\t" + currentProgram.getLanguageID());
            info.println("executable_sha256\t" + currentProgram.getExecutableSHA256());
            info.println("image_base\t" + currentProgram.getImageBase());
        }
        try (BufferedReader ranges = new BufferedReader(new FileReader(new File(output, "selected-ranges.tsv")));
             PrintWriter counts = new PrintWriter(new File(output, "assembly-coverage.tsv")))) {
            counts.println("stem\tinstructions\tinstruction_bytes\topaque_flix_bundles\topaque_flix_bytes\tlast_instruction");
            String row;
            while ((row = ranges.readLine()) != null) {
                String[] fields = row.split("\t");
                Address start = toAddr(Long.parseLong(fields[0], 16));
                Address end = toAddr(Long.parseLong(fields[1], 16) - 1);
                String stem = fields[2];
                runScript("ListInsns.java", new String[] {fields[0], end.toString(), new File(output, stem + ".asm.tsv").getPath()});
                InstructionIterator instructions = currentProgram.getListing().getInstructions(new AddressSet(start, end), true);
                int instructionCount = 0;
                int byteCount = 0;
                int flixCount = 0;
                int flixBytes = 0;
                String last = "";
                try (PrintWriter pcode = new PrintWriter(new File(output, stem + ".pcode.tsv")))) {
                    pcode.println("address\tbytes\tassembly\tpcode");
                    while (instructions.hasNext()) {
                        Instruction instruction = instructions.next();
                        instructionCount++;
                        byteCount += instruction.getLength();
                        last = instruction.getAddress().toString();
                        if (instruction.getMnemonicString().equals("FLIX")) {
                            flixCount++;
                            flixBytes += instruction.getLength();
                        }
                        StringBuilder bytes = new StringBuilder();
                        for (byte value : instruction.getBytes()) bytes.append(String.format("%02x", value));
                        StringBuilder operations = new StringBuilder();
                        for (PcodeOp operation : instruction.getPcode()) {
                            if (operations.length() > 0) operations.append(" ; ");
                            operations.append(operation.toString().replace('\t', ' '));
                        }
                        pcode.println(instruction.getAddress() + "\t" + bytes + "\t" + instruction + "\t" + operations);
                    }
                }
                counts.println(stem + "\t" + instructionCount + "\t" + byteCount + "\t" + flixCount + "\t" + flixBytes + "\t" + last);
                println("EARC_DSP2_EXPORTED " + stem + " instructions=" + instructionCount + " flix=" + flixCount);
            }
        }
    }
}