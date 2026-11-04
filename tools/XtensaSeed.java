// Pre-script: create functions at every 4-byte aligned windowed-ABI prologue `entry a1, N` (36 x1 xx).
//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.*;
import ghidra.program.model.mem.*;

public class XtensaSeed extends GhidraScript {
    @Override
    public void run() throws Exception {
        Memory mem = currentProgram.getMemory();
        int n = 0;
        for (MemoryBlock b : mem.getBlocks()) {
            if (!b.isInitialized()) continue;
            Address a = b.getStart();
            long len = b.getSize();
            byte[] buf = new byte[(int) len];
            b.getBytes(a, buf);
            for (int i = 0; i + 3 <= len; i += 4) {
                int b0 = buf[i] & 0xff, b1 = buf[i + 1] & 0xff, b2 = buf[i + 2] & 0xff;
                int frame = ((b2 << 4) | (b1 >> 4)) << 3;
                if (b0 != 0x36 || (b1 & 0xf) != 1 || frame == 0 || frame > 0x4000) continue;
                Address fa = a.add(i);
                if (getFunctionAt(fa) != null) continue;
                DisassembleCommand dc = new DisassembleCommand(fa, null, true);
                if (!dc.applyTo(currentProgram, monitor)) continue;
                if (createFunction(fa, null) != null) n++;
            }
        }
        println("XTENSA_SEED functions=" + n);
    }
}
