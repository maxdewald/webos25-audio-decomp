// Headless: decompile functions with their callers. args: <out.c> <name|0xaddr>...
//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import java.io.*;

public class DecompFuncs extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        DecompInterface ifc = new DecompInterface();
        ifc.setOptions(new DecompileOptions());
        ifc.toggleCCode(true);
        ifc.openProgram(currentProgram);
        try (PrintWriter out = new PrintWriter(new FileWriter(args[0]))) {
            for (int i = 1; i < args.length; i++) {
                Function f = null;
                if (args[i].startsWith("0x")) f = getFunctionContaining(toAddr(Long.parseUnsignedLong(args[i].substring(2), 16)));
                else for (Function g : getGlobalFunctions(args[i])) f = g;
                if (f == null) { out.println("/* not found: " + args[i] + " */"); continue; }
                StringBuilder callers = new StringBuilder();
                for (Reference r : getReferencesTo(f.getEntryPoint())) {
                    Function c = getFunctionContaining(r.getFromAddress());
                    callers.append(c == null ? "?" : c.getName()).append('@').append(r.getFromAddress()).append(' ');
                }
                DecompileResults res = ifc.decompileFunction(f, 180, monitor);
                out.println("\n/* @" + f.getEntryPoint() + " " + f.getName() + " size=" + f.getBody().getNumAddresses()
                        + "\n * callers: " + callers + "*/");
                out.print(res != null && res.decompileCompleted() ? res.getDecompiledFunction().getC()
                        : "/* decompile failed: " + (res == null ? "null" : res.getErrorMessage()) + " */\n");
            }
        }
        ifc.dispose();
    }
}
