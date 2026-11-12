// Pre-script: create named functions from <addr_hex>\t<name> (path given as script arg).
//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.app.cmd.disassemble.DisassembleCommand;
import ghidra.program.model.address.Address;
import ghidra.program.model.listing.Function;
import ghidra.program.model.symbol.SourceType;
import java.nio.file.*;

public class ApplySyms extends GhidraScript {
    @Override
    public void run() throws Exception {
        int n = 0;
        for (String ln : Files.readAllLines(Paths.get(getScriptArgs()[0]))) {
            if (ln.startsWith("#") || ln.isBlank()) continue;
            String[] p = ln.split("\t", 2);
            Address a = toAddr(Long.parseUnsignedLong(p[0], 16));
            new DisassembleCommand(a, null, true).applyTo(currentProgram, monitor);
            Function f = getFunctionAt(a);
            if (f == null) f = createFunction(a, p[1]);
            else f.setName(p[1], SourceType.IMPORTED);
            if (f != null) n++;
        }
        println("APPLY_SYMS named=" + n);
    }
}
