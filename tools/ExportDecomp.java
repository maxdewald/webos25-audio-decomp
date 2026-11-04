// Headless: decompile every function and write <outdir>/<program>.c plus a function index.
//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.symbol.*;
import ghidra.program.model.data.*;
import java.io.*;
import java.util.*;

public class ExportDecomp extends GhidraScript {
    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        File outDir = new File(args.length > 0 ? args[0] : "/tmp/decomp");
        outDir.mkdirs();
        String name = currentProgram.getName();
        DecompInterface ifc = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        ifc.setOptions(opts);
        ifc.toggleCCode(true);
        ifc.toggleSyntaxTree(false);
        ifc.setSimplificationStyle("decompile");
        ifc.openProgram(currentProgram);

        try (PrintWriter c = new PrintWriter(new FileWriter(new File(outDir, name + ".c")));
             PrintWriter idx = new PrintWriter(new FileWriter(new File(outDir, name + ".functions.tsv")))) {
            c.println("/* Ghidra " + getGhidraVersion() + " pseudocode for " + name +
                      " sha256=" + currentProgram.getExecutableSHA256() + " */");
            FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
            int n = 0, fail = 0;
            while (it.hasNext() && !monitor.isCancelled()) {
                Function f = it.next();
                if (f.isExternal() || f.isThunk()) continue;
                DecompileResults r = ifc.decompileFunction(f, 120, monitor);
                String body;
                if (r != null && r.decompileCompleted() && r.getDecompiledFunction() != null) {
                    body = r.getDecompiledFunction().getC();
                } else {
                    body = "/* decompile failed: " + (r == null ? "null" : r.getErrorMessage()) + " */\n";
                    fail++;
                }
                c.println("\n/* @" + f.getEntryPoint() + " " + f.getName(true) + " */");
                c.print(body);
                idx.println(f.getEntryPoint() + "\t" + f.getBody().getNumAddresses() + "\t" + f.getName(true));
                n++;
            }
            println("EXPORTED " + name + " functions=" + n + " failed=" + fail);
        }
        try (PrintWriter s = new PrintWriter(new FileWriter(new File(outDir, name + ".strings.tsv")))) {
            DataIterator di = currentProgram.getListing().getDefinedData(true);
            while (di.hasNext()) {
                Data d = di.next();
                if (!d.hasStringValue()) continue;
                Object v = d.getValue();
                if (v == null) continue;
                String str = v.toString().replace("\n", "\\n").replace("\t", "\\t");
                StringBuilder refs = new StringBuilder();
                for (Reference ref : getReferencesTo(d.getAddress())) {
                    Function rf = getFunctionContaining(ref.getFromAddress());
                    if (rf != null) refs.append(rf.getName(true)).append(',');
                }
                s.println(d.getAddress() + "\t" + str + "\t" + refs);
            }
        }
        ifc.dispose();
    }
}
