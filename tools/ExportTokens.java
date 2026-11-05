// Headless: decompile every function and dump the decompiler's token tree with type and
// symbol information as JSON lines, for tools/lift.py (Ghidra C -> compilable C).
//   <outdir>/<program>.tokens.jsonl   one function per line
//   <outdir>/<program>.types.json     data types, called/defined function signatures, globals
//@category REA
import ghidra.app.script.GhidraScript;
import ghidra.app.decompiler.*;
import ghidra.program.model.address.*;
import ghidra.program.model.data.*;
import ghidra.program.model.listing.*;
import ghidra.program.model.pcode.*;
import ghidra.program.model.symbol.*;
import java.io.*;
import java.util.*;

public class ExportTokens extends GhidraScript {
    private final Map<String, String> types = new LinkedHashMap<>();   // id -> json
    private final Map<Long, String> funcs = new TreeMap<>();           // addr -> json
    private final Map<Long, String> globals = new TreeMap<>();

    private static String q(String s) {
        if (s == null) return "null";
        StringBuilder b = new StringBuilder("\"");
        for (char c : s.toCharArray()) {
            if (c == '"' || c == '\\') b.append('\\').append(c);
            else if (c < 0x20) b.append(String.format("\\u%04x", (int) c));
            else b.append(c);
        }
        return b.append('"').toString();
    }

    private String typeId(DataType dt) {
        if (dt == null) return "null";
        String id = dt.getPathName();
        if (types.containsKey(id)) return q(id);
        types.put(id, "null");  // break cycles
        StringBuilder j = new StringBuilder("{");
        j.append("\"name\":").append(q(dt.getName())).append(",\"size\":").append(dt.getLength());
        if (dt instanceof Pointer) {
            j.append(",\"k\":\"ptr\",\"t\":").append(typeId(((Pointer) dt).getDataType()));
        } else if (dt instanceof Array) {
            Array a = (Array) dt;
            j.append(",\"k\":\"arr\",\"n\":").append(a.getNumElements())
             .append(",\"t\":").append(typeId(a.getDataType()));
        } else if (dt instanceof TypeDef) {
            j.append(",\"k\":\"td\",\"t\":").append(typeId(((TypeDef) dt).getBaseDataType()));
        } else if (dt instanceof Composite) {
            Composite c = (Composite) dt;
            j.append(",\"k\":\"").append(dt instanceof Union ? "union" : "struct").append("\"");
            j.append(",\"fields\":[");
            boolean first = true;
            for (DataTypeComponent comp : c.getDefinedComponents()) {
                if (!first) j.append(',');
                first = false;
                j.append("{\"name\":").append(q(comp.getFieldName())).append(",\"off\":")
                 .append(comp.getOffset()).append(",\"t\":").append(typeId(comp.getDataType()))
                 .append(",\"bits\":").append(comp.isBitFieldComponent()).append('}');
            }
            j.append(']');
        } else if (dt instanceof FunctionDefinition) {
            j.append(",\"k\":\"fn\"");
        } else if (dt instanceof ghidra.program.model.data.Enum) {
            j.append(",\"k\":\"enum\"");
        } else if (dt instanceof AbstractFloatDataType) {
            j.append(",\"k\":\"float\"");
        } else if (dt instanceof AbstractIntegerDataType) {
            j.append(",\"k\":\"int\",\"signed\":").append(((AbstractIntegerDataType) dt).isSigned());
        } else {
            j.append(",\"k\":\"base\"");
        }
        types.put(id, j.append('}').toString());
        return q(id);
    }

    private String sig(Function f) {
        StringBuilder j = new StringBuilder("{");
        j.append("\"addr\":").append(f.getEntryPoint().getOffset());
        j.append(",\"name\":").append(q(f.getName(true)));
        j.append(",\"ext\":").append(f.isExternal());
        String orig = null;
        if (f.isExternal()) {
            ExternalLocation el = f.getExternalLocation();
            if (el != null) orig = el.getOriginalImportedName();
            if (orig == null && el != null) orig = el.getLabel();
        }
        j.append(",\"import\":").append(q(orig));
        j.append(",\"src\":").append(q(f.getSignatureSource().toString()));
        j.append(",\"varargs\":").append(f.hasVarArgs());
        j.append(",\"ret\":").append(typeId(f.getReturnType()));
        j.append(",\"params\":[");
        Parameter[] ps = f.getParameters();
        for (int i = 0; i < ps.length; i++) {
            if (i > 0) j.append(',');
            j.append("{\"name\":").append(q(ps[i].getName())).append(",\"t\":")
             .append(typeId(ps[i].getDataType())).append('}');
        }
        return j.append("]}").toString();
    }

    private void addFunc(Function f) {
        if (f == null) return;
        long a = f.getEntryPoint().getOffset();
        if (f.isExternal()) a = -1 - Math.abs(f.getName().hashCode() * 31L + f.getID());
        if (!funcs.containsKey(a)) funcs.put(a, sig(f));
    }

    private String funcRef(Function f) {
        if (f == null) return "null";
        if (f.isThunk()) {
            Function t = f.getThunkedFunction(true);
            if (t != null) f = t;
        }
        addFunc(f);
        if (f.isExternal()) {
            ExternalLocation el = f.getExternalLocation();
            String orig = el != null ? el.getOriginalImportedName() : null;
            if (orig == null && el != null) orig = el.getLabel();
            return "{\"ext\":" + q(orig) + "}";
        }
        return "{\"addr\":" + f.getEntryPoint().getOffset() + "}";
    }

    private void addGlobal(Address ad, HighVariable hv, String text) {
        long a = ad.getOffset();
        if (globals.containsKey(a)) return;
        Symbol s = getSymbolAt(ad);
        DataType dt = hv != null ? hv.getDataType() : null;
        Data d = getDataAt(ad);
        globals.put(a, "{\"addr\":" + a + ",\"text\":" + q(text) + ",\"sym\":" +
                q(s != null ? s.getName(true) : null) + ",\"t\":" + typeId(dt) +
                ",\"data\":" + typeId(d != null ? d.getDataType() : null) +
                ",\"func\":" + (getFunctionAt(ad) != null) + "}");
    }

    @Override
    public void run() throws Exception {
        String[] args = getScriptArgs();
        File outDir = new File(args.length > 0 ? args[0] : "/tmp/lift");
        outDir.mkdirs();
        String name = currentProgram.getName();
        DecompInterface ifc = new DecompInterface();
        DecompileOptions opts = new DecompileOptions();
        ifc.setOptions(opts);
        ifc.toggleCCode(true);
        ifc.toggleSyntaxTree(true);
        ifc.setSimplificationStyle("decompile");
        ifc.openProgram(currentProgram);
        int n = 0, fail = 0;
        try (PrintWriter out = new PrintWriter(new BufferedWriter(new FileWriter(
                new File(outDir, name + ".tokens.jsonl"))))) {
            FunctionIterator it = currentProgram.getFunctionManager().getFunctions(true);
            while (it.hasNext() && !monitor.isCancelled()) {
                Function f = it.next();
                if (f.isExternal() || f.isThunk()) continue;
                addFunc(f);
                DecompileResults r = ifc.decompileFunction(f, 120, monitor);
                StringBuilder j = new StringBuilder("{\"addr\":").append(f.getEntryPoint().getOffset());
                j.append(",\"name\":").append(q(f.getName(true)));
                if (r == null || !r.decompileCompleted() || r.getCCodeMarkup() == null) {
                    j.append(",\"error\":").append(q(r == null ? "null" : r.getErrorMessage())).append('}');
                    out.println(j);
                    fail++;
                    continue;
                }
                List<ClangNode> leaves = new ArrayList<>();
                r.getCCodeMarkup().flatten(leaves);
                j.append(",\"toks\":[");
                boolean first = true;
                for (ClangNode node : leaves) {
                    if (!(node instanceof ClangToken)) continue;
                    ClangToken t = (ClangToken) node;
                    if (!first) j.append(',');
                    first = false;
                    String k = t.getClass().getSimpleName().replace("Clang", "").replace("Token", "");
                    j.append("[").append(q(k)).append(',').append(q(t.getText()));
                    StringBuilder x = new StringBuilder();
                    if (t instanceof ClangBreak) {
                        x.append("\"indent\":").append(((ClangBreak) t).getIndent());
                    } else if (t instanceof ClangTypeToken) {
                        x.append("\"t\":").append(typeId(((ClangTypeToken) t).getDataType()));
                    } else if (t instanceof ClangFieldToken) {
                        ClangFieldToken ft = (ClangFieldToken) t;
                        x.append("\"t\":").append(typeId(ft.getDataType()))
                         .append(",\"off\":").append(ft.getOffset());
                    } else if (t instanceof ClangFuncNameToken) {
                        PcodeOp op = t.getPcodeOp();
                        Function target = null;
                        if (op != null && (op.getOpcode() == PcodeOp.CALL)) {
                            target = getFunctionAt(op.getInput(0).getAddress());
                        } else if (op == null) {
                            target = f;
                        }
                        if (target == null) {
                            List<Function> fs = getGlobalFunctions(t.getText());
                            if (fs != null && fs.size() == 1) target = fs.get(0);
                        }
                        x.append("\"fn\":").append(funcRef(target));
                        if (op != null) x.append(",\"call\":true");
                    } else if (t instanceof ClangVariableToken) {
                        ClangVariableToken vt = (ClangVariableToken) t;
                        HighVariable hv = vt.getHighVariable();
                        Varnode vn = vt.getVarnode();
                        String cls = hv == null ? "none" : hv.getClass().getSimpleName();
                        x.append("\"hv\":").append(q(cls));
                        if (hv != null) x.append(",\"t\":").append(typeId(hv.getDataType()));
                        Address ga = null;
                        if (hv instanceof HighGlobal) {
                            HighSymbol hs = hv.getSymbol();
                            if (hs != null && hs.getStorage() != null && hs.getStorage().getMinAddress() != null)
                                ga = hs.getStorage().getMinAddress();
                            else if (hv.getRepresentative() != null)
                                ga = hv.getRepresentative().getAddress();
                        } else if (hv == null && vn != null && vn.getAddress().isMemoryAddress()) {
                            ga = vn.getAddress();
                        }
                        if (ga == null && hv == null) {
                            SymbolIterator si = currentProgram.getSymbolTable().getSymbols(t.getText());
                            if (si.hasNext()) {
                                Symbol s = si.next();
                                if (!si.hasNext() && s.getAddress().isMemoryAddress()) ga = s.getAddress();
                            }
                        }
                        if (ga != null && ga.isMemoryAddress()) {
                            x.append(",\"g\":").append(ga.getOffset());
                            Function gf = getFunctionAt(ga);
                            if (gf != null) x.append(",\"fn\":").append(funcRef(gf));
                            else addGlobal(ga, hv, t.getText());
                        }
                        if (vn != null && vn.isConstant()) x.append(",\"const\":true");
                    } else if (t instanceof ClangLabelToken) {
                        x.append("\"label\":true");
                    }
                    if (x.length() > 0) j.append(",{").append(x).append('}');
                    j.append(']');
                }
                j.append("]}");
                out.println(j);
                n++;
            }
        }
        try (PrintWriter tw = new PrintWriter(new BufferedWriter(new FileWriter(
                new File(outDir, name + ".types.json"))))) {
            tw.println("{\"types\":{");
            boolean first = true;
            for (Map.Entry<String, String> e : types.entrySet()) {
                if (!first) tw.println(",");
                first = false;
                tw.print(q(e.getKey()) + ":" + e.getValue());
            }
            tw.println("},\"funcs\":[");
            first = true;
            for (String s : funcs.values()) {
                if (!first) tw.println(",");
                first = false;
                tw.print(s);
            }
            tw.println("],\"globals\":[");
            first = true;
            for (String s : globals.values()) {
                if (!first) tw.println(",");
                first = false;
                tw.print(s);
            }
            tw.println("]}");
        }
        ifc.dispose();
        println("EXPORTED-TOKENS " + name + " functions=" + n + " failed=" + fail);
    }
}
