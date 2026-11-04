// Print listing (address, bytes, mnemonic) for a range: args <start_hex> <end_hex>
//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.Reference;
import java.io.PrintWriter;

public class ListInsns extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] a = getScriptArgs();
        Address s = toAddr(Long.parseLong(a[0], 16)), e = toAddr(Long.parseLong(a[1], 16));
        Listing l = currentProgram.getListing();
        CodeUnitIterator it = l.getCodeUnits(new AddressSet(s, e), true);
        PrintWriter output = a.length > 2 ? new PrintWriter(a[2]) : null;
        int count = 0;
        try {
            while (it.hasNext()) {
                CodeUnit cu = it.next();
                StringBuilder hex = new StringBuilder();
                for (byte value : cu.getBytes()) hex.append(String.format("%02x", value));
                StringBuilder line = new StringBuilder(cu.getAddress() + "\t" + hex + "\t" + cu);
                for (Reference reference : getReferencesFrom(cu.getAddress())) {
                    Address target = reference.getToAddress();
                    line.append("\tref=").append(target);
                    if (!reference.getReferenceType().isData()) continue;
                    try {
                        int value = currentProgram.getMemory().getInt(target);
                        line.append(String.format(" value=0x%08x", value));
                        Address pointer = toAddr(Integer.toUnsignedLong(value));
                        StringBuilder text = new StringBuilder();
                        for (int index = 0; index < 160; index++) {
                            int character = currentProgram.getMemory().getByte(pointer.add(index)) & 0xff;
                            if (character == 0) {
                                if (text.length() > 1) line.append(" string=").append(text);
                                break;
                            }
                            if (character < 32 || character > 126) break;
                            text.append((char) character);
                        }
                    } catch (Exception ignored) { }
                }
                if (output == null) println("LIST " + line);
                else output.println(line);
                count++;
            }
        } finally {
            if (output != null) output.close();
        }
        println("LIST_EXPORTED code_units=" + count);
    }
}
