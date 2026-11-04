// Post-analysis: disassemble undefined gaps between consecutive known function entries
// (code reached via unrecovered jump tables), then re-derive each function body.
//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.app.cmd.function.CreateFunctionCmd;
import ghidra.program.model.address.*;
import ghidra.program.model.listing.*;
import java.util.*;

public class XtensaFill extends GhidraScript {
    @Override
    public void run() throws Exception {
        Listing l = currentProgram.getListing();
        List<Function> fs = new ArrayList<>();
        currentProgram.getFunctionManager().getFunctions(true).forEach(fs::add);
        int filled = 0;
        for (int pass = 0; pass < 3; pass++) {
            for (int i = 0; i + 1 < fs.size(); i++) {
                Address s = fs.get(i).getEntryPoint(), e = fs.get(i + 1).getEntryPoint().subtract(1);
                if (e.subtract(s) > 0x4000) continue;
                Address a = s;
                while (a.compareTo(e) <= 0) {
                    Data d = l.getUndefinedDataAt(a);
                    if (d != null) {
                        int b = currentProgram.getMemory().getByte(a) & 0xff;
                        if (b != 0) {
                            DisassembleCommand dc = new DisassembleCommand(a, new AddressSet(a, e), true);
                            if (dc.applyTo(currentProgram, monitor) && l.getInstructionAt(a) != null) filled++;
                        }
                    }
                    CodeUnit cu = l.getCodeUnitAt(a);
                    a = a.add(cu == null ? 1 : cu.getLength());
                }
            }
        }
        // Blocks only reachable through FLIX branch slots (opaque to SLEIGH) become
        // separate functions "<owner>__blk_<addr>" so the decompiler emits them.
        int blocks = 0;
        for (int i = 0; i + 1 < fs.size(); i++) {
            Function f = fs.get(i);
            Address s = f.getEntryPoint(), e = fs.get(i + 1).getEntryPoint().subtract(1);
            if (e.subtract(s) > 0x4000) continue;
            Instruction prev = null;
            for (Instruction ins : l.getInstructions(new AddressSet(s, e), true)) {
                Address a = ins.getAddress();
                boolean startsRun = prev == null ? false
                    : !prev.getMaxAddress().add(1).equals(a) || !prev.hasFallthrough();
                prev = ins;
                if (!startsRun || a.equals(s)) continue;
                if (getFunctionContaining(a) != null) continue;
                if (getReferencesTo(a).length > 0) continue;
                if (createFunction(a, f.getName() + "__blk_" + a) != null) blocks++;
            }
        }
        println("XTENSA_FILL hidden_blocks=" + blocks);
        println("XTENSA_FILL disassembled_gaps=" + filled);
    }
}
