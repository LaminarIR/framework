/*
 * Copyright 2003 by the Massachusetts Institute of Technology.
 *
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of M.I.T. not be used in
 * advertising or publicity pertaining to distribution of the
 * software without specific, written prior permission.
 * M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is"
 * without express or implied warranty.
 */

/* Code related to LaminarIR:
 * See file LICENSE.LaminarIR which has been distributed with this file.
 */

package laminarir.frontend.tolaminar;

import laminarir.frontend.nodes.*;

import java.util.Iterator;
import java.util.List;
import java.util.ListIterator;
import java.util.Map;
import java.util.HashMap;
import java.util.Set;
import java.util.HashSet;

import java.io.BufferedReader;
import java.io.FileReader;
import java.io.IOException;
import java.io.FileNotFoundException;

import java.util.ArrayList;
import java.util.Collections;

import java.util.regex.Matcher;
import java.util.regex.Pattern;

import laminarir.frontend.passes.RenameStreamSpecVars;


 /**
 * Traverse a StreamIt front-end tree and produce LaminarIR code.  
 * This file is derived from streamit.frontend.tojava.NodesToJava.java
 * and modified for the LaminarIR Framework. 
 *
 * Information of the original code, streamit.frontend.tojava.NodesToJava.java
 * @author  David Maze &lt;dmaze@cag.lcs.mit.edu&gt;
 * @version $Id: NodesToJava.java,v 1.128 2007-06-25 02:38:58 thies Exp $
 */

public class NodesToLaminarIR implements FEVisitor
{
    // Set inside visitStreamSpec
    private StreamSpec ss;

    // A string consisting of an even number of spaces.
    private String indent;

    // set inside visitStreamSpec if this class generated using static 
    //(a.k.a. global) keyword.
    private boolean global;

    // Yousun Ko: accumulates global code
    private String globalCode;

    // Yousun Ko: a flag to indicate if the visitor is in a branch
    //            true when in StmtIfThen, StmtDowhile, StmtWhile
    private boolean branch;

    // Yousun Ko: set if queue positions are resolved.
    private boolean inResolved;
    private boolean outResolved;

    // Yousun Ko: store communication type, direct or fifo
    private String commType = null;

    // Yousun Ko: randomize input
    private boolean randomize = false;

    // Yousun Ko: do sanitycheck
    private boolean sanitycheck = false;

    // Yousun Ko: file names to read
    private String graphFile = null;
    private String constFile = null;
    private String pathFile = null;

    // Yousun Ko: record name of global helper functions
    List <String> helperFuncs;

    // Yousun Ko: record global const variables
    ConstVarList globalConstList;
    // Yousun Ko: container for the current constant var list
    ConstVarList curConstList;

    // Yousun Ko: record prog variables
    String KEY_SEED;
    String KEY_SUM;

    public NodesToLaminarIR (String commType, boolean randomize, boolean sanitycheck, String graphFile, String constFile, String pathFile) 
    {
        this.ss = null;
        this.indent = "";
        this.global = false;

        this.branch = false;
        this.inResolved = true;
        this.outResolved = true;
        this.commType = commType;
        this.randomize = randomize;
        this.sanitycheck = sanitycheck;
        this.graphFile = graphFile;
        this.constFile = constFile;
        this.pathFile = pathFile;
        this.globalCode = "";
        this.helperFuncs = new ArrayList<String>();
        this.globalConstList = new ConstVarList();
        this.curConstList = new ConstVarList();
        this.KEY_SEED = "";
        this.KEY_SUM = "";
    }

    // Add two spaces to the indent.
    private void addIndent() 
    {
        indent += "  ";
    }
    
    // Remove two spaces from the indent.
    private void unIndent()
    {
        indent = indent.substring(2);
    }

    // Determine if this is that top level of th program
    // There is no built-in marker for the top-level stream, but
    // we can recognize it as the only void -> void stream spec other than
    // a static (global).
    private boolean isTopLevelSpec(StreamSpec spec) {
        StreamType st = spec.getStreamType();
        return
            spec.getType() != StreamSpec.STREAM_GLOBAL &&
            st != null &&
            st.getIn() instanceof TypePrimitive &&
            ((TypePrimitive)st.getIn()).getType() == TypePrimitive.TYPE_VOID &&
            st.getOut() instanceof TypePrimitive &&
            ((TypePrimitive)st.getOut()).getType() == TypePrimitive.TYPE_VOID;
    }

    // Yousun Ko: extract dimension from a type
    private String extractDimInType(Type type){
        String result = "";
        if (type instanceof TypeArray){
            String strType = convertType(type) + " ";
            int dimIdx = strType.indexOf("[");
            assert dimIdx != 0: "No dimension found in TypeArray: "+type.toString();
            result = strType.substring(dimIdx, strType.length());
        }
        return result;
    }

    // Yousun Ko: extract base type of a type
    private String extractBaseInType(Type type){
        if (type instanceof TypeArray){
            while (type instanceof TypeArray){
                type = ((TypeArray) type).getBase();
            }
        }
        return convertType(type);
    }

    // Yousun Ko: C does not determine behaviour of uninitialized non-static variables.
    //            Generate initial value when not given by declaration.
    private String generateInitVal(Type type) {
        // extract the base type when it's array
        boolean isArray = false;
        if (type instanceof TypeArray) {
            isArray = true;
            while (type instanceof TypeArray){
                type = ((TypeArray) type).getBase();
            }
        }
        String initVal = null;
        if (type instanceof TypePrimitive) {
            switch (((TypePrimitive) type).getType()) {
                case TypePrimitive.TYPE_BOOLEAN:
                case TypePrimitive.TYPE_BIT:
                case TypePrimitive.TYPE_INT:
                    initVal = "0";
                    break;
                case TypePrimitive.TYPE_FLOAT:
                case TypePrimitive.TYPE_DOUBLE:
                case TypePrimitive.TYPE_FLOAT2:
                case TypePrimitive.TYPE_FLOAT3:
                case TypePrimitive.TYPE_FLOAT4:
                    initVal = "0.0";
                    break;
                default:
                    System.err.println("Error: wrong type is given, "+type);
                    System.exit(1);
                    return null;
            }
        } else {
            System.err.println("Error: wrong type is given, "+type);
            System.exit(1);
            return null;
        }

        if (isArray){
            initVal = "{"+initVal+"}";
        }
        return initVal;
    }

    // Convert a Type to a String.  If visitors weren't so generally
    // useless for other operations involving Types, we'd use one here.
    public String convertType(Type type)
    {
        // This is So Wrong in the greater scheme of things.
        if (type instanceof TypeArray)
            {
                // declare arrays like int[10][10] foo;
                return convertTypeFull(type);
            }
        else if (type instanceof TypeStruct)
            {
                return ((TypeStruct)type).getName();
            }
        else if (type instanceof TypeStructRef)
            {
                return ((TypeStructRef)type).getName();
            }
        else if (type instanceof TypePrimitive)
            {
                switch (((TypePrimitive)type).getType())
                    {
                    case TypePrimitive.TYPE_BOOLEAN:
                    case TypePrimitive.TYPE_BIT:
                    case TypePrimitive.TYPE_INT: return "int";
                    case TypePrimitive.TYPE_FLOAT: return "float";
                    case TypePrimitive.TYPE_DOUBLE: return "double";
                    case TypePrimitive.TYPE_COMPLEX:
                    case TypePrimitive.TYPE_FLOAT2: 
                    case TypePrimitive.TYPE_FLOAT3: 
                    case TypePrimitive.TYPE_FLOAT4: 
                        System.err.println("Error: complex types are not supported.");
                        return null;
                    case TypePrimitive.TYPE_VOID: return "void";
                    default: assert false : type; return null;
                    }
            }
        else if (type instanceof TypePortal)
            {
                System.err.println("Error: portal is not supported.");
                return null;
            }
        else
            {
                assert false : type;
                return null;
            }
    }

    // Yousun Ko: convert streamit data types to C types
    public String convertType(String type){
        if (type.equals("boolean") ||
                type.equals("bit") ||
                type.equals("int")) {
            return "int";
        } else if (type.equals("float")) {
            return "float";
        } 

        // not supported types reach here including complex
        System.err.println("Error: unsupported type is given, "+type);
        System.exit(-1);
        return null;
    }


    public String convertTypeFull(Type type) {
        return convertTypeFull(type, true);
    }

    // Do the same conversion, but including array dimensions.
    public String convertTypeFull(Type type, boolean includePrimitive)
    {
        if (type instanceof TypeArray)
            {
                TypeArray array = (TypeArray)type;
                String output = "";
                // first get primitive type
                if (includePrimitive) {
                    Type primitive = array;
                    while (primitive instanceof TypeArray) {
                        primitive = ((TypeArray)primitive).getBase();
                    }
                    output = convertTypeFull(primitive);
                }
                return output +
                    "[" + (String)array.getLength().accept(this) + "]"
                    + convertTypeFull(array.getBase(), false);
        
            }
        if (includePrimitive) {
            return convertType(type);
        } else {
            return "";
        }
    }

    // Get a constructor for some type.
    public String makeConstructor(Type type)
    {
        System.err.println("Error: no construction should have reached.");
        return null;
    }

    // Get a Java Class object corresponding to a type.
    public String typeToClass(Type t)
    {
        if (t instanceof TypePrimitive)
            {
                switch (((TypePrimitive)t).getType())
                    {
                    case TypePrimitive.TYPE_BOOLEAN:
                        return "Boolean.TYPE";
                    case TypePrimitive.TYPE_BIT:
                        return "Integer.TYPE";
                    case TypePrimitive.TYPE_INT:
                        return "Integer.TYPE";
                    case TypePrimitive.TYPE_FLOAT:
                        return "Float.TYPE";
                    case TypePrimitive.TYPE_DOUBLE:
                        return "Double.TYPE";
                    case TypePrimitive.TYPE_VOID:
                        return "Void.TYPE";
                    case TypePrimitive.TYPE_COMPLEX:
                        System.err.println("Error: complex types are not supported.");
                        return null;
                    default:
                        assert false : t;
                        return null;
                    }
            }
        else
            {
                assert false : t;
                return null;
            }
    }

    private String annotatedFunction(String name, Type type)
    {
        String prefix = "", suffix = "";
        // Check for known suffixes:
        if (type instanceof TypePrimitive)
            {
                switch (((TypePrimitive)type).getType())
                    {
                    case TypePrimitive.TYPE_BOOLEAN:
                        suffix = "Bool";
                        break;
                    case TypePrimitive.TYPE_BIT:
                        suffix = "Int";
                        break;
                    case TypePrimitive.TYPE_INT:
                        suffix = "Int";
                        break;
                    case TypePrimitive.TYPE_FLOAT:
                        suffix = "Float";
                        break;
                    case TypePrimitive.TYPE_DOUBLE:
                        suffix = "Double";
                        break;
                    case TypePrimitive.TYPE_COMPLEX:
                        System.err.println("Error: complex types are not supported.");
                        return null;
                    default:
                        assert false : type;
                    }
            }
        else if (name.startsWith("inputChannel"))
            {
                prefix = "(" + convertType(type) + ")";
            }
        return prefix + name + suffix;
    }

    // Return a representation of a list of Parameter objects.
    public String doParams(List params, String prefix)
    {
        String result = "(";
        boolean first = true;
        for (Iterator iter = params.iterator(); iter.hasNext(); )
            {
                Parameter param = (Parameter)iter.next();
                if (!first) result += ", ";
                if (prefix != null) result += prefix + " ";

                // Yousun Ko: convert type
                String dim = extractDimInType(param.getType());
                String strType = extractBaseInType(param.getType());
                result += strType+" ";
                result += param.getName()+dim;
                first = false;
            }
        result += ")";
        return result;
    }

    // Return a representation of lhs = rhs, with no trailing semicolon.
    public String doAssignment(Expression lhs, Expression rhs,
                               SymbolTable symtab)
    {
        // If the left-hand-side is a complex variable, we need to
        // properly decompose the right-hand side.
        // We can use a null stream type here since the left-hand
        // side shouldn't contain pushes, pops, or peeks.
        GetExprType eType = new GetExprType(symtab, ss.getStreamType(),
                                            new java.util.HashMap<String, TypeStruct>(),
                                            new java.util.HashMap<String, TypeHelper>());
        Type lhsType = (Type)lhs.accept(eType);
        if (lhsType.isComplex())
            {
                Expression real = new ExprField(lhs.getContext(), lhs, "real");
                Expression imag = new ExprField(lhs.getContext(), lhs, "imag");
                // If the right hand side is complex too (at this point
                // just test the run-time type of the expression), then we
                // should do field copies; otherwise we only have a real part.
                if (rhs instanceof ExprComplex)
                    {
                        ExprComplex cplx = (ExprComplex)rhs;
                        return real.accept(this) + " = " +
                            cplx.getReal().accept(this) + ";\n" +
                            imag.accept(this) + " = " +
                            cplx.getImag().accept(this);
                    }
                else
                    return real.accept(this) + " = " +
                        rhs.accept(this) + ";\n" +
                        imag.accept(this) + " = 0.0";
            }
        else if (lhsType.isComposite()) 
            {
                Expression x = new ExprField(lhs.getContext(), lhs, "x");
                Expression y = new ExprField(lhs.getContext(), lhs, "y");
                Expression z = new ExprField(lhs.getContext(), lhs, "z");
                Expression w = new ExprField(lhs.getContext(), lhs, "w");
                // If the right hand side is composite too, then we 
                // should do field copies.
                if (rhs instanceof ExprComposite) {
                    ExprComposite cpst = (ExprComposite)rhs;
                    String result = x.accept(this) + " = " +
                        cpst.getX().accept(this) + ";\n" +
                        y.accept(this) + " = " +
                        cpst.getY().accept(this); 
                    Expression z1 = cpst.getZ();
                    if (z1 != null) result +=  ";\n" + z.accept(this) + " = " +
                                        z1.accept(this); 
                    Expression w1 = cpst.getW();
                    if (w1 != null) result +=  ";\n" + w.accept(this) + " = " +
                                        w1.accept(this); 
                    return result;
                } 
                else
                    throw new RuntimeException("type not compatible");
            }
        else
            {
                // Might want to special-case structures and arrays;
                // ignore for now.
                return lhs.accept(this) + " = " + rhs.accept(this);
            }
    }

    public Object visitExprArray(ExprArray exp)
    {
        // Yousun Ko: try partial evaluation
        PartialEvaluator expEval = new PartialEvaluator(curConstList);
        int expVal = (int) exp.accept(expEval);
        String result = "";
        if (expEval.isResolved()){
            result = Integer.toString(expVal);
        }else{
            result += (String) exp.getBase().accept(
                    this);
            result += "[";
            result += (String) exp.getOffset().accept(this);
            result += "]";
        }
        return result;
    }
    
    public Object visitExprArrayInit(ExprArrayInit exp)
    {
        StringBuffer sb = new StringBuffer();
        sb.append("{");

        List elems = exp.getElements();
        for (int i=0; i<elems.size(); i++) {
            sb.append((String)((Expression)elems.get(i)).accept(this));
            if (i!=elems.size()-1) {
                sb.append(",");
            }
            // leave blank line for multi-dim arrays
            if (exp.getDims()>1) {
                sb.append("\n");
            }
        }
    
        sb.append("}");

        return sb.toString();
    }
    
    public Object visitExprBinary(ExprBinary exp)
    {
        String result;
        String op = null;
        result = "(";
        result += (String)exp.getLeft().accept(this);
        switch (exp.getOp())
            {
            case ExprBinary.BINOP_ADD: op = "+"; break;
            case ExprBinary.BINOP_SUB: op = "-"; break;
            case ExprBinary.BINOP_MUL: op = "*"; break;
            case ExprBinary.BINOP_DIV: op = "/"; break;
            case ExprBinary.BINOP_MOD: op = "%"; break;
            case ExprBinary.BINOP_AND: op = "&&"; break;
            case ExprBinary.BINOP_OR:  op = "||"; break;
            case ExprBinary.BINOP_EQ:  op = "=="; break;
            case ExprBinary.BINOP_NEQ: op = "!="; break;
            case ExprBinary.BINOP_LT:  op = "<"; break;
            case ExprBinary.BINOP_LE:  op = "<="; break;
            case ExprBinary.BINOP_GT:  op = ">"; break;
            case ExprBinary.BINOP_GE:  op = ">="; break;
            case ExprBinary.BINOP_BAND:op = "&"; break;
            case ExprBinary.BINOP_BOR: op = "|"; break;
            case ExprBinary.BINOP_BXOR:op = "^"; break;
            case ExprBinary.BINOP_LSHIFT: op = "<<"; break;
            case ExprBinary.BINOP_RSHIFT: op = ">>"; break;
            default: assert false : exp; break;
            }
        result += " " + op + " ";
        result += (String)exp.getRight().accept(this);
        result += ")";
        return result;
    }

    public Object visitExprComplex(ExprComplex exp)
    {
        // We should never see one of these at this point.
        System.err.println("Error: complex types are not supported.");
        return null;
    }

    public Object visitExprComposite(ExprComposite exp)
    {
        // We should never see one of these at this point.
        //assert false : exp;
        // If we do, print something vaguely intelligent:
        System.err.println("Error: composites are not supported.");
        return null;
    }

    public Object visitExprConstBoolean(ExprConstBoolean exp)
    {
        if (exp.getVal())
            return "1";
        else
            return "0";
    }

    public Object visitExprConstChar(ExprConstChar exp)
    {
        return "'" + exp.getVal() + "'";
    }

    public Object visitExprConstFloat(ExprConstFloat exp)
    {
        return Double.toString(exp.getVal()) + "f";
    }

    public Object visitExprConstInt(ExprConstInt exp)
    {
        return Integer.toString(exp.getVal());
    }
    
    public Object visitExprConstStr(ExprConstStr exp)
    {
        return exp.getVal();
    }

    public Object visitExprDynamicToken(ExprDynamicToken exp) {
        System.err.println("Error: dynamic rate is not supported.");
        return null;
    }

    public Object visitExprField(ExprField exp)
    {
        String result = "";
        result += (String)exp.getLeft().accept(this);
        result += ".";
        result += (String)exp.getName();
        return result;
    }

    public Object visitExprFunCall(ExprFunCall exp)
    {
        String result;
        String name = exp.getName();
        boolean mathFunction = false;
        // Local function?
        if (ss.getFuncNamed(name) != null) {
            result = name + "(";
        }
        // look for print and println statements; assume everything
        // else is a math function
        else if (name.equals("ArrayMemoizer.initArray")) {
            result = "ArrayMemoizer.initArray(";
        } else if (name.equals("print")) {
            return "";
        } else if (name.equals("println")) {
            return "";
        // Yousun Ko: add more function calls to be supported
        //          : begin
        } else if (name.equals("printf")) {
            result = "printf(";
        } else if (name.equals("scanf")) {
            result = "scanf(";
        } else if (name.equals("srand")) {
            result = "srand(";
        } else if (name.equals("rand_r")) {
            result = "rand_r(";
        } else if (name.equals("floor")) {
            result = "floor(";
        } else if (name.equals("sqrt")) {
            result = "sqrt(";
        } else if (name.equals("sin")) {
            result = "sin(";
        } else if (name.equals("cos")) {
            result = "cos(";
        } else if (name.equals("tan")) {
            result = "tan(";
        } else if (name.equals("asin")) {
            result = "asin(";
        } else if (name.equals("acos")) {
            result = "acos(";
        } else if (name.equals("atan")) {
            result = "atan(";
        } else if (name.equals("RTS_filereader_readInt")) {
            result = name +"(";
        } else if (name.equals("RTS_filereader_readFloat")) {
            result = name +"(";
        } else if (name.equals("RTS_filewriter_writeInt")) {
            result = name +"(";
        } else if (name.equals("RTS_filewriter_writeFloat")) {
            result = name +"(";
        // Yousun: ends
        } else {
            System.err.println("Error: unsupported function call, "+name);
            return null;
        }
        boolean first = true;
        for (Iterator iter = exp.getParams().iterator(); iter.hasNext(); )
            {
                Expression param = (Expression)iter.next();
                if (!first) result += ", ";
                first = false;
                result += (String)param.accept(this);
            }
        result += ")";

        return result;
    }

    public Object visitExprHelperCall(ExprHelperCall exp)
    {
        String result = exp.getHelperPackage() + '.' + exp.getName() + '(';
        boolean first = true;
        for (Iterator iter = exp.getParams().iterator(); iter.hasNext(); )
            {
                Expression param = (Expression)iter.next();
                if (!first) result += ", ";
                first = false;
                result += (String)param.accept(this);
            }
        result += ")";
        return result;
    }

	@Override
	public Object visitExprIter(ExprIter exprIter) {
        System.err.println("Error: iter() is disabled.");
        return null;
	}

    public Object visitExprPeek(ExprPeek exp)
    {
        String result = "";
        if (commType.equals("fifo")){
            result += "peek("+ (String) exp.getExpr().accept(this) +")";
        }else{
            if (inResolved){
                PartialEvaluator idxEval = new PartialEvaluator(curConstList);
                int peekIdx = (int) exp.getExpr().accept(idxEval);
                result+="@"+ss.getPeekInToken(peekIdx);
            } else {
                String peekIdx = (String) exp.getExpr().accept(this);
                result = ss.getInArrayName()+"["+ss.getInArrayCtr()+"+"+peekIdx+"]";
            }
        }
        return result;
    }
    
    public Object visitExprPop(ExprPop exp)
    {
        String result = "";
        if (commType.equals("fifo")){
            result += "pop()";
        }else{
            if (inResolved){
                result = "@"+ss.getNextInToken();
            }else{
                result = ss.getInArrayName()+"["+ss.getInArrayCtr()+"++]";
            }
        }
        return result;
    }

    public Object visitExprRange(ExprRange exp) {
        System.err.println("Error: dynamic rate is not supported.");
        return null;
    }

    public Object visitExprTernary(ExprTernary exp)
    {
        String a = (String)exp.getA().accept(this);
        String b = (String)exp.getB().accept(this);
        String c = (String)exp.getC().accept(this);
        switch (exp.getOp())
            {
            case ExprTernary.TEROP_COND:
                return "(" + a + " ? " + b + " : " + c + ")";
            default:
                assert false : exp;
                return null;
            }
    }

    public Object visitExprTypeCast(ExprTypeCast exp)
    {
        return "((" + convertType(exp.getType()) + ")(" +
            (String)exp.getExpr().accept(this) + "))";
    }

    public Object visitExprUnary(ExprUnary exp)
    {
        // Yousun Ko: drop the var from the constList in case when
        //            op causes value update in the middle of a statement
        int op = exp.getOp();
        if (op == ExprUnary.UNOP_PREINC ||
            op == ExprUnary.UNOP_POSTINC ||
            op == ExprUnary.UNOP_PREDEC ||
            op == ExprUnary.UNOP_POSTDEC){
            if (exp.getExpr() instanceof ExprVar){
                curConstList.remove(((ExprVar)exp.getExpr()).getName());
            }
        }

        String child = (String)exp.getExpr().accept(this);
        String result = null;

        switch(exp.getOp())
            {
            case ExprUnary.UNOP_NOT:        result = "!" + child; break;
            case ExprUnary.UNOP_NEG:        result = "-" + child; break;
            case ExprUnary.UNOP_PREINC:     result = "++" + child; break;
            case ExprUnary.UNOP_POSTINC:    result = child + "++"; break;
            case ExprUnary.UNOP_PREDEC:     result = "--" + child; break;
            case ExprUnary.UNOP_POSTDEC:    result = child + "--"; break;
            case ExprUnary.UNOP_COMPLEMENT: result = "~" + child; break;
            default: assert false : exp;    result = null; break;
            }

        return result;
    }

    public Object visitExprVar(ExprVar exp)
    {
        String result = exp.getName();
        if (exp.isPropagatable()){
            PartialEvaluator expEval = new PartialEvaluator(curConstList);
            int expVal = (int) exp.accept(expEval);
            if (expEval.isResolved()){
                result = Integer.toString(expVal);
            }
        }
        return result;
    }

    public Object visitFieldDecl(FieldDecl field)
    {
        // Assume all of the fields have the same type.
        String result = indent;
        if (global) {
            result += "const ";
        }
        
        // Yousun Ko: convert java style array declaration into C style
        Type fieldType = field.getType(0);
        String dim = extractDimInType(fieldType);
        String strType = extractBaseInType(fieldType);
        result += strType +" ";

        for (int i = 0; i < field.getNumFields(); i++)
            {
                if (i > 0) result += ", ";

                String fieldName = field.getName(i);
                String fieldVal = null;
                result += fieldName+dim;
                if (field.getInit(i) != null) {
                    fieldVal = (String) field.getInit(i).accept(this);
                    result += " = " + fieldVal;
                } else {
                    // Yousun Ko: generate init value automatically.
                    //          : though do not update the const var list.
                    result += " = " + generateInitVal(fieldType);
                }

                // Yousun Ko: add the var to the list of the global constant vars 
                //            when the var is field of global streamspec 
                //            and the value is known.
                if (global && fieldVal != null && !ss.isStateVar(fieldName)){
                    globalConstList.add(fieldType, fieldName, fieldVal);
                }
            }
        result += ";";
        result += "\n";
        return result;
    }

    public Object visitFunction(Function func)
    {
        String result = indent;

        if (ss == null) { // A helper function
            result += "static ";
            if (func.getCls() == Function.FUNC_NATIVE) result += "native ";
            result += convertType(func.getReturnType()) + " ";
            result += func.getName();
            result += doParams(func.getParams(), null);
            if (func.getCls() == Function.FUNC_NATIVE) 
                result += ";\n"; 
            else
                result += " " + (String)func.getBody().accept(this) + "\n";
            return result;
        }

        //
        // Yousun Ko: prepare body block modification

        // Yousun Ko: copy body for possibility of prepending variable declarations
        FEContext context = func.getContext();
        StmtBlock oldBody = (StmtBlock) func.getBody();
        List newStmts = new ArrayList();
            
        // Yousun Ko: examine queue position resolvability of the whole work body 
        QueueSimulator bodyCtr = new QueueSimulator(ss, curConstList, commType);
        NumQueueOps numQBody = (NumQueueOps) oldBody.accept(bodyCtr);

        //
        // Yousun Ko: body block modifications to be prepended

        // Yousun Ko: init function
        if (func.getCls() == Function.FUNC_INIT){
            result += "init:\n"+indent;

            // Yousun Ko: add original body block
            newStmts.addAll(oldBody.getStmts());
        // Yousun Ko: work function
        } else if (func.getCls() == Function.FUNC_WORK){
            result += "firing:\n"+indent;
            // check the resolvability of queue positions
            inResolved = true;
            outResolved = true;

            // Yousun Ko: generate random inputs when the node is source
            if (randomize
                && ss.getStreamType() != null
                && ss.getStreamType().getIn() instanceof TypePrimitive
                && ((TypePrimitive) ss.getStreamType().getIn()).getType() 
                    == TypePrimitive.TYPE_VOID) {
                ss.setOutTokenCtr(1);
                for(int i = 0; i < ss.getOutputDataRate(); i++){
                    if (commType.equals("fifo")){
                        newStmts.add(new StmtPush(context, 
                            new ExprFunCall(context,"rand_r", 
                                new ExprVar(context, "&seed"))));
                    }else{
                        newStmts.add(new StmtAssign(context, 
                            new ExprVar(context, "@"+ss.getNextOutToken()),
                            new ExprFunCall(context,"rand_r", 
                                new ExprVar(context, "&seed")),
                            0));
                    }
                }
                ss.setOutTokenCtr(1);

            }else{

                // input queue position is not resolved
                if (!numQBody.inResolved && commType.equals("direct")){
                    ss.setInTokenCtr(1);
                    List <Expression> exprInTokens = new ArrayList<Expression>();
                    int numInTokens = ss.getInputDataRate();
                    if (ss.getPeekDataRate() > ss.getInputDataRate()){
                        numInTokens = ss.getPeekDataRate();
                    }
                    for(int i = 0; i < numInTokens; i++){
                        exprInTokens.add(new ExprVar(context, "@"+ss.getNextInToken()));
                    }
                    ss.setInTokenCtr(1);
                    // Yousun: add an array decl size of input data rate
                    newStmts.add(new StmtVarDecl(context, 
                        new TypeArray(ss.getStreamType().getIn(), 
                            new ExprConstInt(context, numInTokens)),
                        ss.getInArrayName(), 
                        new ExprArrayInit(context, exprInTokens)));
                    newStmts.add(new StmtVarDecl(context,
                        new TypePrimitive(TypePrimitive.TYPE_INT),
                        ss.getInArrayCtr(),
                        new ExprConstInt(context, 0)));
                    inResolved = false;
                }
                // output queue position is not resolved
                if (!numQBody.outResolved && commType.equals("direct")){
                    // Yousun: add an array decl size of output data rate
                    List <Expression> exprOutVals = new ArrayList<Expression>();
                    for(int i = 0; i < ss.getOutputDataRate(); i++){
                        exprOutVals.add(new ExprConstInt(context, 0));
                    }
                    // Yousun: add an array decl size of input data rate
                    newStmts.add(new StmtVarDecl(context, 
                        new TypeArray(ss.getStreamType().getIn(), 
                            new ExprConstInt(context, ss.getOutputDataRate())),
                        ss.getOutArrayName(), 
                        new ExprArrayInit(context, exprOutVals)));
                    newStmts.add(new StmtVarDecl(context,
                        new TypePrimitive(TypePrimitive.TYPE_INT),
                        ss.getOutArrayCtr(),
                        new ExprConstInt(context, 0)));
                    outResolved = false;
                }
                // Yousun Ko: add summing up code in the sink node
                //            examine if the stream is sink node
                if (commType.equals("fifo") && ss.getStreamType() != null
                    && ss.getStreamType().getOut() instanceof TypePrimitive
                    && ((TypePrimitive) ss.getStreamType().getOut()).getType() 
                        == TypePrimitive.TYPE_VOID) {
                    for(int i = 0; i < ss.getInputDataRate(); i++){
                        String curToken = ss.getNextInToken();
                        newStmts.add(new StmtAssign(context, 
                            new ExprVar(context, KEY_SUM),
                            new ExprPeek(context, new ExprConstInt(context, i)),
                            ExprBinary.BINOP_ADD));
                    }
                }

                // Yousun Ko: add original body block
                newStmts.addAll(oldBody.getStmts());
            }

        // Yousun Ko: helper function
        } else if (func.getCls() == Function.FUNC_HELPER) {
            boolean found = false;
            String funcName = func.getName();
            for (String name : helperFuncs){
                if (funcName.equals(name)){
                    found = true;
                }
            }

            if (!found){
                // do not modify any streamspec variable values while 
                // traversig helper functions
                boolean oldBranch = branch;
                branch = true;
                // Yousun Ko: check if helper function contains push/pop
                QueueSimulator helperCtr = new QueueSimulator(ss, curConstList, commType);
                NumQueueOps numQHelper = (NumQueueOps) func.accept(helperCtr);
                assert numQHelper.numPop == 0
                    && numQHelper.numPush == 0
                    : "Error: helper function "+funcName+" contains queue operations.";

                helperFuncs.add(funcName);
                globalCode += indent+convertType(func.getReturnType()) + " ";
                globalCode += func.getName();
                globalCode += doParams(func.getParams(), null) + " ";
                String bodyCode = (String) func.getBody().accept(this);
                // Yousun Ko: add state variables and parameters of actor
                globalCode += "{\n";
                for( Parameter param: ss.getParams() ){
                    Type paramType = param.getType();
                    String paramName = param.getName();
                    
                    String dim = extractDimInType(paramType);
                    globalCode += indent+extractBaseInType(paramType)+" ";
                    globalCode += paramName+dim;
                    globalCode += "="+curConstList.getVal(paramName)+";\n";
                }
                
                for (FieldDecl varDecl: ss.getVars()){
                    globalCode += (String) varDecl.accept(this);
                }

                globalCode += (String) bodyCode.substring(1);
                globalCode += "\n";
                branch=oldBranch;
            }

            return "";

        // Yousun Ko: not supported functions
        } else {
            System.err.println("Error: Function type of "+func.getName()+" is not supported.");
            System.exit(1);
        }

        //
        // Yousun Ko: body block modifications to be appended

        // Yousun Ko: add copy operation when output queue position is not resolved
        if(func.getCls() == Function.FUNC_WORK){
            if(!numQBody.outResolved && commType.equals("direct")){
                ss.setOutTokenCtr(1);
                for(int i = 0; i < ss.getOutputDataRate(); i++){
                    newStmts.add(new StmtAssign(context, 
                            new ExprVar(context, "@"+ss.getNextOutToken()),
                            new ExprArray(context, new ExprVar(context, 
                                    ss.getOutArrayName()), new ExprConstInt(context, i)),
                            0));
                }
                ss.setOutTokenCtr(1);
            }

            // Yousun Ko: add summing up code in the sink node
            //            examine if the stream is sink node
            if (ss.getStreamType() != null
                && ss.getStreamType().getOut() instanceof TypePrimitive
                && ((TypePrimitive) ss.getStreamType().getOut()).getType() 
                    == TypePrimitive.TYPE_VOID) {
                assert ss.getStreamType().getIn() instanceof TypePrimitive: 
                    "Input data type of sink is not primitive";
                ss.setInTokenCtr(1);
                for(int i = 0; i < ss.getInputDataRate(); i++){
                    String curToken = ss.getNextInToken();
                    if (commType.equals("direct")){
                        newStmts.add(new StmtAssign(context, 
                            new ExprVar(context, KEY_SUM),
                            new ExprVar(context, "@"+curToken),
                            ExprBinary.BINOP_ADD));
                    }
                    // Yousun Ko: add printf if sanitycheck is on
                    if (sanitycheck){
                        String sinkInType = convertType(convertType(ss.getStreamType().getIn()));
                        String outputFormat = "%d";
                        if (sinkInType.equals("float")){
                            outputFormat = "%f";
                        }
                        if (commType.equals("direct")){
                            newStmts.add(new StmtExpr(context, new ExprConstStr(context, 
                                "printf(\""+outputFormat+"\", @"+curToken+");")));
                        }
                    }
                }
                ss.setInTokenCtr(1);
            }
        }

        Statement newBody = new StmtBlock(oldBody.getContext(), newStmts);

        String prefix = null;

        result += (String) newBody.accept(this);
        result += "\n";
            
        // initialize resolved for the later use
        if(func.getCls() == Function.FUNC_WORK){
            inResolved = true;
            outResolved = true;
        }

        return result;
    }
    
    public Object visitFuncWork(FuncWork func)
    {
        // Nothing special here; we get to ignore the I/O rates.
        return visitFunction(func);
    }

    
    // Yousun Ko: helper functions for visitProgram
    //          : begin
    private String extractEdges(){
        String edges = "";
        BufferedReader graphReader = null;
        try {
            graphReader = new BufferedReader(new FileReader(graphFile));
        } catch (FileNotFoundException e) {
            System.err.println("Unable to open file: "+graphFile);
            System.exit(-1);
        }
        
        while (true) {
            String line = null;
            try {
                line = graphReader.readLine();
            } catch(Exception e) {
                e.printStackTrace();
            }
            
            if (line == null) break;
            String[] tokens = line.split(" ");
            assert tokens.length == 4: "Invalid graph information: "+line;
            String type = convertType(tokens[0]);
            String from = tokens[1];
            String to = tokens[3];
            String delay = "";

            // trim additional information from the predecessor
            int atIdx = from.indexOf("@");
            if (atIdx != -1){
                from = from.substring(0, atIdx);
            }

            // trim additional information from the successor
            atIdx = to.indexOf("@");
            if (atIdx != -1){
                to = to.substring(0, atIdx);
            }
            edges += indent + type + " " + from + " -> " + to + delay + ";\n";
        }
        return edges;
    }

    private List<String> extractNodesFromGraph(){
        List<String> nodes = new ArrayList<String>();
        BufferedReader graphReader = null;
        try {
            graphReader = new BufferedReader(new FileReader(graphFile));
        }catch (FileNotFoundException e){
            System.err.println("Unable to open file: "+graphFile);
            System.exit(-1);
        }
        
        while (true) {
            String line = null;
            try {
                line = graphReader.readLine();
            } catch(Exception e){
                e.printStackTrace();
            }
            if (line == null) break;
            String[] tokens = line.split(" ");
            assert tokens.length == 4: "Invalid graph information: "+line;
            nodes.add(tokens[1]);
            nodes.add(tokens[3]);
        }
        // Yousun Ko: eliminate duplicates
        HashSet<String> list2set = new HashSet<String>(nodes);
        nodes = new ArrayList<String>(list2set);
        return nodes;
    }


    // Yousun Ko: extract const values of parameters for const propagation
    private void extractAndSetParamVals(StreamSpec spec){
        // convertType may need to access spec, so set it global
        StreamSpec oldSS = ss;
        ss = spec;
        List<Parameter> params = spec.getParams();
        BufferedReader constReader = null;
        try {
            constReader = new BufferedReader(new FileReader(constFile));
        }catch (FileNotFoundException e){
            System.err.println("Unable to open file: "+constFile);
            System.exit(-1);
        }

        // set patterns for boolean values
        Pattern patTrue = Pattern.compile("\\btrue\\b");
        Pattern patFalse = Pattern.compile("\\bfalse\\b");

        // clear out and set with the default and global constant vars
        curConstList = new ConstVarList(globalConstList);
        // fill up the constant variables of the instance
        while (true) {
            String line = null;
            try {
                line = constReader.readLine();
            } catch(Exception e){
                e.printStackTrace();
            }

            if (line == null) break;
            String[] tokens = line.split(",");
            assert tokens.length == 3: "Invalid constnat information: "+line;
            // found const var of the instance
            if (spec.getInstName().equals(tokens[0])){
              Type constParamType;
              // search for the var
              for (Parameter param: params) {
                String constParamName = tokens[1]+"_"+spec.getInstName();
                if (constParamName.equals(param.getName())){
                  constParamType = param.getType();
                  String constParamVal = "";
                  // if input parameter is array, declare inside the filter bodys
                  if (constParamType instanceof TypeArray){
                    constParamVal = 
                        line.substring(tokens[0].length()+tokens[1].length()+2, line.length());
                    constParamVal = constParamVal.replace("[","{");
                    constParamVal = constParamVal.replace("]","}");
                  }else{
                    constParamVal = tokens[2];
                  }
                  
                  // convert boolean values into int values if exist
                  Matcher matFalse = patFalse.matcher(constParamVal);
                  constParamVal = matFalse.replaceAll("0");
                  Matcher matTrue = patTrue.matcher(constParamVal);
                  constParamVal = matTrue.replaceAll("1");

                  // add the var to the current const var list
                  if (!ss.isStateVar(constParamName)){
                    curConstList.add(constParamType, constParamName, constParamVal);
                  }
                }
              }
            }
        }
        ss = oldSS;
    }

    private String extractInNode(String node){
        String incomingNodeFull = null;
        String incomingNode = null;
        BufferedReader graphReader = null;
        try {
            graphReader = new BufferedReader(new FileReader(graphFile));
        }catch (FileNotFoundException e){
            System.err.println("Unable to open file: "+graphFile);
            System.exit(-1);
        }
        
        while (true) {
            String line = null;
            try {
                line = graphReader.readLine();
            } catch(Exception e){
                e.printStackTrace();
            }

            if (line == null) break;
            String[] tokens = line.split(" ");
            assert tokens.length == 4: "Invalid graph information: "+line;
            if (node.equals(tokens[3])){
                assert incomingNodeFull == null: "Multiple incoming node found: "+node;
                incomingNodeFull = tokens[1];
            }
        }

        // source
        if (incomingNodeFull == null){
            return null;
        }
        
        // trim additional information
        int atIdx = incomingNodeFull.indexOf("@");
        if (atIdx == -1){
            incomingNode = incomingNodeFull;
        }else{
            incomingNode = incomingNodeFull.substring(0, atIdx);
        }
        
        // eliminate delay initialization
        int assignIdx = incomingNode.indexOf("=");
        if (assignIdx != -1){
            incomingNode = incomingNode.substring(0,assignIdx);
        }

        // eliminate weight information of splitjoin
        String patStr = "\\((\\d+)\\)";
        Pattern pat = Pattern.compile(patStr);
        Matcher mat = pat.matcher(incomingNode);
        incomingNode = mat.replaceFirst("");
        return incomingNode;
    }

    private String extractOutNode(String node){
        String outgoingNodeFull = null;
        String outgoingNode = null;
        BufferedReader graphReader = null;
        try {
            graphReader = new BufferedReader(new FileReader(graphFile));
        }catch (FileNotFoundException e){
            System.err.println("Unable to open file: "+graphFile);
            System.exit(-1);
        }
        
        while (true) {
            String line = null;
            try {
                line = graphReader.readLine();
            } catch(Exception e){
                e.printStackTrace();
            }

            if (line == null) break;
            String[] tokens = line.split(" ");
            assert tokens.length == 4: "Invalid graph information: "+line;
            if (node.equals(tokens[1])){
                assert outgoingNodeFull == null: "Multiple outgoing node found: "+node;
                outgoingNodeFull = tokens[3];
            }
        }
       
        // sink
        if (outgoingNodeFull == null){
            return null;
        }

        // trim additional information
        int atIdx = outgoingNodeFull.indexOf("@");
        if (atIdx == -1){
            outgoingNode = outgoingNodeFull;
        }else{
            outgoingNode = outgoingNodeFull.substring(0, atIdx);
        }

        // eliminate delay initialization
        int assignIdx = outgoingNode.indexOf("=");
        if (assignIdx != -1){
            outgoingNode = outgoingNode.substring(0,assignIdx);
        }

        // eliminate weight information of splitjoin
        String patStr = "\\((\\d+)\\)";
        Pattern pat = Pattern.compile(patStr);
        Matcher mat = pat.matcher(outgoingNode);
        outgoingNode = mat.replaceFirst("");
        return outgoingNode;
    }

    private Type extractInType(String node){
        String incomingType = null;
        BufferedReader graphReader = null;
        try {
            graphReader = new BufferedReader(new FileReader(graphFile));
        }catch (FileNotFoundException e){
            System.err.println("Unable to open file: "+graphFile);
            System.exit(-1);
        }
        
        while (true) {
            String line = null;
            try {
                line = graphReader.readLine();
            } catch(Exception e){
                e.printStackTrace();
            }

            if (line == null) break;
            String[] tokens = line.split(" ");
            assert tokens.length == 4: "Invalid graph information: "+line;
            if (node.equals(tokens[3])){
                incomingType = tokens[0];
            }
        }

        // source
        if (incomingType == null){
            return null;
        }
        
        if (incomingType.equals("boolean")){
            return new TypePrimitive(TypePrimitive.TYPE_BOOLEAN);
        } else if (incomingType.equals("bit")){
            return new TypePrimitive(TypePrimitive.TYPE_BIT);
        } else if (incomingType.equals("int")) {
            return new TypePrimitive(TypePrimitive.TYPE_INT);
        }else if (incomingType.equals("float")) {
            return new TypePrimitive(TypePrimitive.TYPE_FLOAT);
        }else{
            System.err.println("Error: unsupported type detected, "+incomingType);
            System.exit(1);
        }
        return null;
    }

    private Type extractOutType(String node){
        String outgoingType = null;
        BufferedReader graphReader = null;
        try {
            graphReader = new BufferedReader(new FileReader(graphFile));
        }catch (FileNotFoundException e){
            System.err.println("Unable to open file: "+graphFile);
            System.exit(-1);
        }
        
        while (true) {
            String line = null;
            try {
                line = graphReader.readLine();
            } catch(Exception e){
                e.printStackTrace();
            }

            if (line == null) break;
            String[] tokens = line.split(" ");
            assert tokens.length == 4: "Invalid graph information: "+line;
            if (node.equals(tokens[1])){
                outgoingType = tokens[0];
            }
        }

        // source
        if (outgoingType == null){
            return null;
        }

        if (outgoingType.equals("boolean")){
            return new TypePrimitive(TypePrimitive.TYPE_BOOLEAN);
        } else if (outgoingType.equals("bit")){
            return new TypePrimitive(TypePrimitive.TYPE_BIT);
        } else if (outgoingType.equals("int")) {
            return new TypePrimitive(TypePrimitive.TYPE_INT);
        }else if (outgoingType.equals("float")) {
            return new TypePrimitive(TypePrimitive.TYPE_FLOAT);
        }else{
            System.err.println("Error: unsupported type detected, "+outgoingType);
            System.exit(1);
        }

        return null;
    }

    private String extractFileName(String type){
        BufferedReader filenameReader = null;
        try {
            filenameReader = new BufferedReader(new FileReader(pathFile));
        }catch (FileNotFoundException e){
            System.err.println("Unable to open file: "+pathFile);
            System.exit(-1);
        }
        
        while (true) {
            String line = null;
            try {
                line = filenameReader.readLine();
            } catch(Exception e){
                e.printStackTrace();
            }

            if (line == null) break;
            String[] tokens = line.split(",");
            assert tokens.length == 2: "Invalid file path information: "+line;
            if (tokens[0].equals(type)){
                return tokens[1];
            }
        }
        return null;
    }

    private int extractRate(String name, String type){
        int rate;
        String patStr = "(?<="+type+"_)(\\d+)";
        Pattern pat = Pattern.compile(patStr);
        Matcher mat = pat.matcher(name);
        if (mat.find()){
            rate = Integer.parseInt(mat.group(0));
        }else{
            rate = 0;
        }
        return rate;
    }
    // Yousun Ko: helper functions for visitProgram
    //          : end

    public Object visitProgram(Program prog) {
        // Nothing special here either. Just accumulate all of the
        // structures and streams.
        String result = "";
        String sdfCode = "";

        boolean isFileIn = false;
        String inType = null;
        boolean isFileOut = false;
        String outType = null;

        // Yousun Ko: no struct support


        // Yousun Ko: traverse global streams first
        StreamSpec main = null;
        TypePrimitive sinkInType = null;

        // Yousun Ko: add default preamble
        globalCode = "preamble {\n";
        globalCode += "#include <stdio.h>\n";
        globalCode += "#include <stdlib.h>\n";
        globalCode += "#include <math.h>\n";
        globalCode += "\n";
        if (randomize){
            globalCode += "static unsigned int "+prog.getKeySeed()+";\n";
            globalCode += "\n";
        }

        global = true;

        // Yousun Ko: generate helper functions that does not access data
        for (TypeHelper th : prog.getHelpers()) {
            globalCode += visitTypeHelper(th);
        }

        for (Iterator iter = prog.getStreams().iterator(); iter.hasNext();) {
            StreamSpec spec = ((StreamSpec) iter.next());
            if (isTopLevelSpec(spec)) {
                assert main == null : "Found more than one top-level stream";
                main = spec;
            } else {
                if (spec.getType() == StreamSpec.STREAM_FILTER){
                    // Yousun Ko: examine if the stream is sink node
                    if (spec.getStreamType() != null
                        && spec.getStreamType().getOut() instanceof TypePrimitive
                        && ((TypePrimitive) spec.getStreamType().getOut()).getType() 
                            == TypePrimitive.TYPE_VOID) {
                    assert spec.getStreamType().getIn() instanceof TypePrimitive: 
                        "Input data type of sink is not primitive";
                    sinkInType = (TypePrimitive) spec.getStreamType().getIn();
                    }
                }
                // Yousun Ko: visit if the stream is global
                else if (spec.getType() == StreamSpec.STREAM_GLOBAL){
                    globalCode += spec.accept(this);
                }
            }
        }
        global = false;
        assert main != null : "Did not find any top-level stream";

        // Yousun Ko: set prog variables
        KEY_SEED = prog.getKeySeed();
        KEY_SUM = prog.getKeySum();

        // Yousun Ko: begin program
        String progName = main.getName();
        sdfCode = "sdf "+progName+" {\n";
        addIndent();

        // Yousun Ko: generate edges
        sdfCode+= extractEdges();;
        sdfCode+="\n";

        // Yousun Ko: extract list of nodes
        List<String> nodes = extractNodesFromGraph();

        // Yousun Ko: generate actor definitions
        for( int i = 0; i < nodes.size() ; i++ ){
            String nodeFullName = nodes.get(i);
            int atIdx = nodeFullName.indexOf("@");
            if (atIdx == -1) {   // current node is not an instance of a filter
                continue;
            }

            String instName = nodeFullName.substring(0, atIdx);
            int barIdx = instName.lastIndexOf("_");
            assert barIdx != -1: "Wrong node name is given: "+nodeFullName;
            String defName = instName.substring(0, barIdx);

            StreamSpec nodeSpec = null;
            // Yousun Ko: find node specification
            for(int j = 0; j < prog.getStreams().size(); j++){
                if (prog.getStreams().get(j).getName().equals(defName)){
                    nodeSpec = prog.getStreams().get(j);
                }
            }

            // Yousun Ko: node specification is not found
            if (nodeSpec == null) {
                // IO type of the nodeSpec
                StreamType idStreamType = new StreamType(new FEContext(), 
                        extractInType(nodeFullName),
                        extractOutType(nodeFullName));

                if (defName.equals("streamit_library_Identity")){
                    // stmts of work function for id
                    Statement stmt = new StmtPush(new FEContext(), new ExprPop(new FEContext()));
                    List<Statement> stmts = new ArrayList<Statement>();
                    stmts.add(stmt);
                    Statement body = new StmtBlock(new FEContext(), stmts);
                    Function idWorkFunc = new Function(new FEContext(), Function.FUNC_WORK, 
                            "work", extractOutType(nodeFullName), null, body, null, null, null);
                    List<Function> idFuncs = new ArrayList<Function>();
                    idFuncs.add(idWorkFunc);
                
                    // create a new nodeSpec
                    nodeSpec = new StreamSpec(new FEContext(), StreamSpec.STREAM_FILTER,
                            idStreamType, defName, new ArrayList<Parameter>(),
                            new ArrayList<FieldDecl>(), idFuncs);
                } else if (defName.equals("streamit_library_io_FileReader")){
                    isFileIn = true;

                    // stmts of work function for FileReader
                    // push(RTS_filereader_readInt()) or
                    // push(RTS_filereader_readFloat())
                    inType = convertType(convertType(extractOutType(nodeFullName)));
                    String funcName = "";
                    if (inType.equals("int")){
                        funcName="RTS_filereader_readInt";
                    }else if (inType.equals("float")){
                        funcName="RTS_filereader_readFloat";
                    }
                    
                    Statement stmt = new StmtPush(new FEContext(), 
                                        new ExprFunCall(new FEContext(), funcName, new ArrayList<Object>()));
                    List<Statement> stmts = new ArrayList<Statement>();
                    stmts.add(stmt);
                    Statement body = new StmtBlock(new FEContext(), stmts);
                    Function workFunc = new Function(new FEContext(), Function.FUNC_WORK, 
                            "work", extractOutType(nodeFullName), null, body, null, null, null);
                    List<Function> funcs = new ArrayList<Function>();
                    funcs.add(workFunc);
                
                    // create a new nodeSpec
                    nodeSpec = new StreamSpec(new FEContext(), StreamSpec.STREAM_FILTER,
                            idStreamType, defName, new ArrayList<Parameter>(),
                            new ArrayList<FieldDecl>(), funcs);
                } else if (defName.equals("streamit_library_io_FileWriter")){
                    isFileOut = true;

                    // stmts of work function for FileReader
                    // push(RTS_filewriter_writeInt()) or
                    // push(RTS_filewriter_writeFloat())
                    sinkInType = (TypePrimitive)extractInType(nodeFullName);
                    outType = convertType(convertType(sinkInType));
                    String funcName = "";
                    if (outType.equals("int")){
                        funcName="RTS_filewriter_writeInt";
                    }else if (outType.equals("float")){
                        funcName="RTS_filewriter_writeFloat";
                    }
                    
                    Statement stmt = new StmtExpr(new FEContext(), 
                                        new ExprFunCall(new FEContext(), funcName, new ExprPop(new FEContext())));
                    List<Statement> stmts = new ArrayList<Statement>();
                    stmts.add(stmt);
                    Statement body = new StmtBlock(new FEContext(), stmts);
                    Function workFunc = new Function(new FEContext(), Function.FUNC_WORK, 
                            "work", extractOutType(nodeFullName), null, body, null, null, null);
                    List<Function> funcs = new ArrayList<Function>();
                    funcs.add(workFunc);
                
                    // create a new nodeSpec
                    nodeSpec = new StreamSpec(new FEContext(), StreamSpec.STREAM_FILTER,
                            idStreamType, defName, new ArrayList<Parameter>(),
                            new ArrayList<FieldDecl>(), funcs);
                }else{
                    System.err.println("Error: no node specification found for, "+nodeFullName);
                    System.exit(1);
                }
            }
            
            nodeSpec.setInstName(instName);
            // Yousun Ko: rename streamspec specific variables (params, fields)
            nodeSpec = (StreamSpec)nodeSpec.accept(new RenameStreamSpecVars());
            // Yousun Ko: prepare the streamspec field values
            extractAndSetParamVals(nodeSpec);
            nodeSpec.setInputDataRate(extractRate(nodeFullName, "pop"));
            nodeSpec.setPeekDataRate(extractRate(nodeFullName, "peek"));
            nodeSpec.setOutputDataRate(extractRate(nodeFullName, "push"));
            nodeSpec.setIncomingNode(extractInNode(nodeFullName));
            nodeSpec.setOutgoingNode(extractOutNode(nodeFullName));
            // Yousun Ko: generate code for the node
            sdfCode += nodeSpec.accept(this);
        }


        // Yousun Ko: include fileIO rts functions
        if ( isFileIn || isFileOut ){
            globalCode += "#include \"rts/fileIO.h\"\n";
        }

        // Yousun Ko: append sum decl and enclose preamble
        if (sinkInType == null) {
            System.out.println("Warning: No sink node detected. Sum variable is deactivated.");
        }else{
            globalCode += "volatile ";
            switch (sinkInType.getType()) {
                case TypePrimitive.TYPE_BOOLEAN:
                case TypePrimitive.TYPE_BIT:
                case TypePrimitive.TYPE_INT:
                    globalCode+= "long long "+prog.getKeySum()+" = 0;\n";
                    break;
                case TypePrimitive.TYPE_FLOAT:
                case TypePrimitive.TYPE_DOUBLE:
                    globalCode+= "double "+prog.getKeySum()+" = 0.0;\n";
                    break;
                default:
                    assert false : sinkInType.getType();
            }
        }

        globalCode += "}\n";

        // Yousun Ko: end program
        unIndent();
        sdfCode += "}\n";
        
        // Yousun Ko: accumulate codes
        result += globalCode;
        result += "\n";
        result += sdfCode;
        result += "\n";

        // Yousun Ko: add default postamble
        result += "postamble {\n";
        result += "int main(){\n";
        addIndent();

        // Yousun Ko: open i/o files
        if (isFileIn){
            String inputFileName = extractFileName("InputFile");
            result += indent+"RTS_filereader_open(\""+inputFileName+"\", \""+inType+"\");\n";
        }
        if (isFileOut){
            String outputFileName = extractFileName("OutputFile");
            result += indent+"RTS_filewriter_open(\""+outputFileName+"\", \""+outType+"\");\n";
        }

        result += indent+progName+"();\n";

        // Yousun Ko: close i/o files
        if (isFileIn){
            result += indent+"RTS_filereader_close(\""+inType+"\");\n";
        }
        if (isFileOut){
            result += indent+"RTS_filewriter_close(\""+outType+"\");\n";
        }


        if (sinkInType != null) {
            result += indent+"printf(\"sum: ";
            switch (sinkInType.getType()) {
                case TypePrimitive.TYPE_BOOLEAN:
                case TypePrimitive.TYPE_BIT:
                case TypePrimitive.TYPE_INT:
                    result+= "%lld";
                    break;
                case TypePrimitive.TYPE_FLOAT:
                case TypePrimitive.TYPE_DOUBLE:
                    result+= "%f";
                    break;
                default:
                    assert false : sinkInType.getType();
            }
            result += "\\n\", "+prog.getKeySum()+");\n";
        }

        result += indent+"return 0;\n";
        unIndent();
        result += "}\n";

        result += "}\n";

        return result;
    }

    public Object visitSCAnon(SCAnon creator) {
        System.err.println("Error: NodesToLaminarIR run before NameAnonymousStreams.");
        return null;
    }
    
    public Object visitSCSimple(SCSimple creator)
    {
        // Hacked to make FileReader/Writer<bit> work
        boolean hardcoded_BitFileFlag = (creator.getName().equals("FileReader") ||
                                         creator.getName().equals("FileWriter"));
        
        String result;
        result = "new " + creator.getName() + "(";
        boolean first = true;
        for (Iterator iter = creator.getParams().iterator(); iter.hasNext(); )
            {
                Expression param = (Expression)iter.next();
                if (!first) result += ", ";
                result += (String)param.accept(this);
                first = false;
            }
        for (Iterator iter = creator.getTypes().iterator(); iter.hasNext(); )
            {
                Type type = (Type)iter.next();
                if (!first) result += ", ";
                // Hacked to make FileReader/Writer<bit> work
                if ((type instanceof TypePrimitive) && 
                    (((TypePrimitive) type).getType() == TypePrimitive.TYPE_BIT) && 
                    hardcoded_BitFileFlag) {
                    result += "Bit.TYPE, Bit.TREAT_AS_BITS"; 
                }
                else
                    result += typeToClass(type);
                first = false;
            }
        result += ")";
        return result;
    }

    public Object visitSJDuplicate(SJDuplicate sj)
    {
        System.err.println("Error: redundant statement found, DUPLICATE().");
        return null;
    }

    public Object visitSJRoundRobin(SJRoundRobin sj)
    {
        System.err.println("Error: redundant statement found, ROUND_ROBIN().");
        return null;
    }

    public Object visitSJWeightedRR(SJWeightedRR sj)
    {
        System.err.println("Error: redundant statement found, WEIGHTED_ROUND_ROBIN().");
        return null;
    }

    private Object doStreamCreator(String how, StreamCreator sc)
    {
        // If the stream creator involves registering with a portal,
        // we need a temporary variable.
        List portals = sc.getPortals();
        if (portals.isEmpty()) {
            // basic behavior: put expression in-line.
            //System.err.println("basic \"" + ((SCSimple)sc).getName() + "\"");
            return how + "(" + (String)sc.accept(this) + ")";
        }

        // Yousun Ko: found portal
        System.err.println("Error: dynamic functionality is not supported.");
        return null;
    }
    
    public Object visitStmtAdd(StmtAdd stmt)
    {
        System.err.println("Error: redundant statement found, add().");
        return null;
    }
    
    public Object visitStmtAssign(StmtAssign stmt)
    {
        String op;
        switch(stmt.getOp())
            {
            case ExprBinary.BINOP_ADD: op = " += "; break;
            case ExprBinary.BINOP_SUB: op = " -= "; break;
            case ExprBinary.BINOP_MUL: op = " *= "; break;
            case ExprBinary.BINOP_DIV: op = " /= "; break;
            case ExprBinary.BINOP_LSHIFT: op = " <<= "; break;
            case ExprBinary.BINOP_RSHIFT: op = " >>= "; break;
            case 0: op = " = "; break;
            default: assert false: stmt; op = " = "; break;
            }
        String lhs = (String)stmt.getLHS().accept(this);
        String rhs = (String)stmt.getRHS().accept(this);

        // update the const param list
        if(stmt.getLHS() instanceof ExprVar){
            String varLHS = ((ExprVar)stmt.getLHS()).getName();

            // examine to add the new value
            PartialEvaluator rhsEval = new PartialEvaluator(curConstList);
            int rhsVal = (int) stmt.getRHS().accept(rhsEval);
            if ( !branch && rhsEval.isResolved() ) {
                // if 1) the stmt is assignment,
                //    2) rhs value are resolved and
                //    3) not in branch
                // update or add lhs var to the constParam
                // Otherwise, remove lhs var from the constParam
                
                String newVal = null;

                if (stmt.getOp() == 0) {
                    newVal = Integer.toString(rhsVal);
                } else {
                    PartialEvaluator lhsEval = new PartialEvaluator(curConstList);
                    int lhsVal = (int) stmt.getLHS().accept(lhsEval);
                    if (lhsEval.isResolved()){
                        switch (stmt.getOp()) {
                           case ExprBinary.BINOP_ADD:
                               newVal = Integer.toString(lhsVal + rhsVal);
                               break;
                           case ExprBinary.BINOP_SUB:
                               newVal = Integer.toString(lhsVal - rhsVal);
                               break;
                           case ExprBinary.BINOP_MUL:
                               newVal = Integer.toString(lhsVal * rhsVal);
                               break;
                           case ExprBinary.BINOP_DIV:
                               if (rhsVal != 0) {
                                   newVal = Integer.toString(lhsVal / rhsVal);
                               }
                               break;
                           case ExprBinary.BINOP_LSHIFT:
                               newVal = Integer.toString(lhsVal << rhsVal);
                               break;
                           case ExprBinary.BINOP_RSHIFT:
                               newVal = Integer.toString(lhsVal >> rhsVal);
                               break;
                           default:
                               break;
                        }
                    }
                }

                // add only when newVal is known
                if(newVal != null
                  && !ss.isStateVar(varLHS)){
                    curConstList.add(new TypePrimitive(TypePrimitive.TYPE_INT), 
                        varLHS, newVal);
                }else{
                    curConstList.remove(varLHS);
                }
            }else{
                curConstList.remove(varLHS);
            }
        }

        // Assume both sides are the right type.
        return lhs + op + rhs;
    }

    public Object visitStmtBlock(StmtBlock stmt)
    {
        // Put context label at the start of the block, too.
        String result = "{";
        result += "\n";
        addIndent();
        for (Iterator iter = stmt.getStmts().iterator(); iter.hasNext(); )
            {
                Statement s = (Statement)iter.next();
                String line = indent;
                line += (String)s.accept(this);
                if (!(s instanceof StmtIfThen)) {
                    line += ";";
                }
                line += "\n";
                result += line;
            }
        unIndent();
        result += indent + "}";
        return result;
    }

    public Object visitStmtBody(StmtBody stmt)
    {
        System.err.println("Error: redundant statement found, setBody().");
        return null;
    }
    
    public Object visitStmtBreak(StmtBreak stmt)
    {
        return "break";
    }
    
    public Object visitStmtContinue(StmtContinue stmt)
    {
        return "continue";
    }

    public Object visitStmtDoWhile(StmtDoWhile stmt)
    {

        boolean oldBranch = branch;
        branch = true;

        String result = "do ";
        result += (String)stmt.getBody().accept(this);
        result += "while (" + (String)stmt.getCond().accept(this) + ")";

        branch = oldBranch;

        return result;
    }

    public Object visitStmtEmpty(StmtEmpty stmt)
    {
        return "";
    }

    public Object visitStmtEnqueue(StmtEnqueue stmt)
    {
        System.err.println("Error: redundant statement found, enqueue().");
        return null;
    }
    
    public Object visitStmtExpr(StmtExpr stmt)
    {
        String result = (String)stmt.getExpression().accept(this);
        // Gross hack to strip out leading class casts,
        // since they'll illegal (JLS 14.8).
        if (result.length() > 0 && result.charAt(0) == '(' &&
            Character.isUpperCase(result.charAt(1)))
            result = result.substring(result.indexOf(')') + 1);
        return result;
    }

    public Object visitStmtFor(StmtFor stmt)
    {
        // Yousun Ko: get number of iteration
        QueueSimulator loopCtr = new QueueSimulator(ss, curConstList, commType);
        NumQueueOps numQLoop = (NumQueueOps) stmt.accept(loopCtr);
        List<Integer> iters = loopCtr.getIters(stmt);
        // Yousun Ko: do not unroll if no pop/push/peek found
        if (iters != null && iters.size() != 0 
                && numQLoop.numPop == 0 
                && numQLoop.numPush == 0 
                && numQLoop.numPeek == 0){
            iters = null;  
        }
        // Yousun Ko: do not unroll when commType is FIFO
        if (commType.equals("fifo")){
            iters = null;
        }
        // Yousun Ko: do traverse
        String result = "";
        String iterVar = loopCtr.getIterVar(stmt);
        if (iters != null){
            // Yousun Ko: unroll the loop
            result+="\n";
            for( int i : iters ){
                // Yousun Ko: add iter var of the iteration as a const param for partial evaluation
                if (!ss.isStateVar(iterVar)){
                    curConstList.add(new TypePrimitive(TypePrimitive.TYPE_INT), iterVar, Integer.toString(i));
                }
                result += indent+(String) stmt.getBody().accept(this)+"\n";
                // Yousun Ko: liveness of the iter var ends here
                curConstList.remove(iterVar);
            }

        } else {    // Yousun Ko: not resolved
            boolean oldBranch = branch;
            branch = true;
            // constantness of the iterVar ends here
            curConstList.remove(iterVar);

            // Yousun Ko: take the iter var decl out (java -> c style)
            if (stmt.getInit() instanceof StmtVarDecl){
                result += "{\n";
                result += indent+ "int "+iterVar+";\n";
                result += indent+ "for ("+ iterVar + "="    
                    + (String) ((StmtVarDecl)stmt.getInit()).getInit(0).accept(this);
            }else{
                result += "for (";
                if (stmt.getInit() != null)
                    result += (String) stmt.getInit().accept(
                            this);
            }

            // constantness of the iterVar ends here
            curConstList.remove(iterVar);

            result += "; ";
            if (stmt.getCond() != null)
                result += (String) stmt.getCond().accept(
                        this);
            result += "; ";
            if (stmt.getIncr() != null)
                result += (String) stmt.getIncr().accept(
                        this);
            result += ") ";
            result += (String) stmt.getBody().accept(
                    this);
            if (stmt.getInit() instanceof StmtVarDecl){
                result += "\n"+indent+"}";
            }
            branch = oldBranch;
        }
        return result;
    }

    public Object visitStmtIfThen(StmtIfThen stmt)
    {
        // must have an if part...
        assert stmt.getCond() != null;

        boolean oldBranch = branch;
        branch = true;

        int tmp_inTokenCtr = ss.getInTokenCtr();
        int tmp_outTokenCtr = ss.getOutTokenCtr();
        String result = "if (" + (String)stmt.getCond().accept(this) + ") ";
        result += (String)stmt.getCons().accept(this);
        ss.setInTokenCtr(tmp_inTokenCtr);
        ss.setOutTokenCtr(tmp_outTokenCtr);
        if (stmt.getAlt() != null)
            result += " else " + (String)stmt.getAlt().accept(this);

        branch = oldBranch;

        return result;
    }

    public Object visitStmtJoin(StmtJoin stmt)
    {
        System.err.println("Error: redundant statement found, setJoiner().");
        return null;
    }
    
    public Object visitStmtLoop(StmtLoop stmt)
    {
        System.err.println("Error: redundant statement found, setLoop().");
        return null;
    }

    public Object visitStmtPush(StmtPush stmt)
    {
        String result = "";
        String value = (String)stmt.getValue().accept(this);
        if (commType.equals("fifo")){
            result += "push("+value+")";
        }else{
            if (outResolved){
                result = "@"+ss.getNextOutToken();
            }else{
                result = ss.getOutArrayName()+"["+ss.getOutArrayCtr()+"++]";
            }
            result+="="+value;
        }
        return result;
    }

    public Object visitStmtReturn(StmtReturn stmt)
    {
        if (stmt.getValue() == null) return "return";
        return "return " + (String)stmt.getValue().accept(this);
    }

    public Object visitStmtSendMessage(StmtSendMessage stmt)
    {
        System.err.println("Error: dynamic functionality is not supported.");
        return null;
    }

    public Object visitStmtHelperCall(StmtHelperCall stmt) 
    {
        String result = stmt.getHelperPackage() + '.' + stmt.getName() + '(';
        boolean first = true;
        for (Iterator iter = stmt.getParams().iterator(); iter.hasNext(); ) {
            Expression param = (Expression)iter.next();
            if (!first) result += ", ";
            first = false;
            result += (String)param.accept(this);
        }
        result += ")";
        return result;
    }

    public Object visitStmtSplit(StmtSplit stmt)
    {
        System.err.println("Error: redundant statement found, setSplitter().");
        return null;
    }

    public Object visitStmtVarDecl(StmtVarDecl stmt)
    {
        String result = "";

        // Yousun Ko: convert java style array declaration into c style
        String dim = extractDimInType(stmt.getType(0));
        String strType = extractBaseInType(stmt.getType(0));
        result += strType+" ";

        for (int i = 0; i < stmt.getNumVars(); i++) {
            if (i > 0)
                result += ", ";
            String name = stmt.getName(i)+dim;
            result += name;
            if (stmt.getInit(i) != null) {
                PartialEvaluator rhsEval = new PartialEvaluator(curConstList);
                int rhsVal = (int) stmt.getInit(i).accept(rhsEval);
                String rhs = (String) stmt.getInit(i).accept(this);

                // if 1) the stmt is assignment,
                //    2) rhs value is resolved and
                //    3) not in branch
                // update or add lhs var to the constParam
                // Otherwise, remove lhs var from the constParam
                curConstList.remove(name);

                if (rhsEval.isResolved() && !branch
                        && !ss.isStateVar(name) && commType.equals("direct")){
                    // if partial evaluation is resolved then the var is type int
                    curConstList.add(new TypePrimitive(TypePrimitive.TYPE_INT), 
                            name, Integer.toString(rhsVal));
                }
                result += " = " + rhs;
            } else {
                // Yousun Ko: generate init value automatically.
                //          : though do not update the const var list.
                result += " = " + generateInitVal(stmt.getType(0));
            }
        }

        return result;
    }

    public Object visitStmtWhile(StmtWhile stmt)
    {
        assert stmt.getCond() != null;
        assert stmt.getBody() != null;

        boolean oldBranch = branch;
        branch = true;

        String result = "while (" + (String)stmt.getCond().accept(this) +
            ") " + (String)stmt.getBody().accept(this);

        branch = oldBranch;
        return result;
    }

    /**
     * For a non-anonymous StreamSpec, check to see if it has any
     * message handlers.  If it does, then generate a Java interface
     * containing the handlers named (StreamName)Interface, and
     * a portal class named (StreamName)Portal.
     */
    private String maybeGeneratePortal(StreamSpec spec)
    {
        List<Function> handlers = new java.util.ArrayList<Function>();
        for (Iterator iter = spec.getFuncs().iterator(); iter.hasNext(); )
            {
                Function func = (Function)iter.next();
                if (func.getCls() == Function.FUNC_HANDLER)
                    handlers.add(func);
            }
        if (handlers.isEmpty())
            return null;
        
        // Yousun Ko: found portal
        System.err.println("Error: dynamic functionality is not supported.");
        return null;
    }

    /**
     * For a non-anonymous StreamSpec in the library path, generate
     * extra functions we need to construct the object.  In the
     * compiler path, generate an empty constructor.
     */
    private String maybeGenerateConstruct(StreamSpec spec)
    {
        StringBuffer result = new StringBuffer();
        
        // The StreamSpec at this point has no parameters; we need to
        // find the parameters of the init function.
        Function init = spec.getInitFunc();
        // (ASSERT: init != null)
        List params = init.getParams();

        if (spec.getType() == StreamSpec.STREAM_GLOBAL) {
            return result.toString();
        }

        System.err.println("Error: redundant functionality found, construct().");
        return null;
    }

    public Object visitStreamSpec(StreamSpec spec)
    {
        // Anonymous classes look different from non-anonymous ones.
        // This appears in two places: (a) as a top-level (named)
        // stream; (b) in an anonymous stream creator (SCAnon).
        //
        // However... 
        // Any code here for anonymous streams should be obsolete since 
        // NameAnonymousStreams should have been run before NodesToJava.

        // Yousun Ko: generates actor functions
        String result = "";

        boolean isActor = false;

        if (spec.getName() != null && !isTopLevelSpec(spec)){
            if (spec.getType() == StreamSpec.STREAM_FILTER) {
                isActor = true;
                result += indent + "actor " + spec.getInstName() + "\n";
                result += indent + "{\n";
                addIndent();
            }
        } else {
            assert false : "NodesToLaminarIR run before NameAnonymousStreams";
        }

        // At this point we get to ignore wholesale the stream type, except
        // that we want to save it.
        StreamSpec oldSS = ss;
        ss = spec;

        // Output field definitions:         
        if ( global || isActor ){
            if (isActor && (spec.getVars().size() != 0 
                    || spec.getParams().size() != 0
                    || curConstList.size() != 0)){
                result += indent + "state:\n";
                result += indent + "{\n";
                // add declarations of parameters
                for(Parameter param: spec.getParams()){
                    Type paramType = param.getType();
                    String paramName = param.getName();

                    String dim = extractDimInType(paramType);
                    result += indent+extractBaseInType(paramType)+" ";
                    result += paramName+dim;
                    result += "="+curConstList.getVal(paramName)+";\n";
                }
            }

            for (Iterator iter = spec.getVars().iterator(); iter.hasNext(); )
            {
                FieldDecl varDecl = (FieldDecl)iter.next();
                result += (String) varDecl.accept(this);
            }

            if (isActor && (spec.getVars().size() != 0
                    || spec.getParams().size() != 0
                    || curConstList.size() != 0)){
                result+= indent + "}\n";
            }
        }

        // Output method definitions:
        if (isActor) {
            List<Function> funcs = spec.getFuncs();
            // Yousun Ko: add seeding process when the node is source
            if (randomize
                && spec.getStreamType() != null
                && spec.getStreamType().getIn() instanceof TypePrimitive
                && ((TypePrimitive) spec.getStreamType().getIn()).getType() 
                    == TypePrimitive.TYPE_VOID) {

                FEContext context = spec.getContext();
                List newStmts = new ArrayList();
                // printf("Enter seed: ");
                newStmts.add(new StmtExpr(context, 
                                new ExprFunCall(context, "printf" , 
                                    new ExprConstStr(context, "\"Enter seed: \""))));
                // scanf ("%d", &seed);
                newStmts.add(new StmtExpr(context,
                                new ExprFunCall(context, "scanf",
                                    new ExprConstStr(context, "\"%d\""),
                                    new ExprVar(context, "&seed"))));
                // srand (seed);
                newStmts.add(new StmtExpr(context,
                                new ExprFunCall(context, "srand",
                                    new ExprVar(context, "seed"))));

                // search for the init function
                boolean init_found = false;
                for (int i = 0; i < funcs.size(); i++){
                    Function func = funcs.get(i);
                    if (func.getCls() == Function.FUNC_INIT){
                        funcs.set(i, new Function(func.getContext(), func.getCls(),
                                    null, new TypePrimitive(TypePrimitive.TYPE_VOID),
                                    Collections.<Parameter> emptyList(), 
                                    new StmtBlock(func.getBody().getContext(), newStmts),
                                    null,null,null));
                        init_found = true;
                    }
                }
                
                // prepend the new init function when no init func is found
                if (!init_found){
                    funcs.add(0, new Function(context, Function.FUNC_INIT,
                                null, new TypePrimitive(TypePrimitive.TYPE_VOID),
                                Collections.<Parameter> emptyList(), 
                                new StmtBlock(context, newStmts),
                                null,null,null));
                }
            }

            for (Function func : funcs) {
                // Input token list
                if(func.getCls() == Function.FUNC_WORK
                  &&isActor 
                  && spec.getIncomingNode()!= null){
                    int popRate = spec.getInputDataRate();
                    int peekRate = spec.getPeekDataRate();

                    // pop
                    result += indent + "input:\n";
                    addIndent();
                    result += indent + spec.getIncomingNode()+": ";
                    spec.setInTokenCtr(1);
                    for (int i = 0; i < popRate; i++){
                        if (i != 0){
                            result+=",";
                        }
                        result+=spec.getNextInToken();
                    }
                    result+=";\n";
                    unIndent();

                    // peek
                    if (peekRate-popRate >0){
                        result+= indent + "peek:\n";
                        addIndent();
                        result += indent + spec.getIncomingNode()+": ";
                        for(int i = 0; i < peekRate-popRate; i++){
                            if(i != 0){
                                result+=",";
                            }
                            result+=spec.getNextInToken();
                        }
                        result+=";\n";
                        unIndent();
                    }
                    
                    // reset the counter for firing function
                    spec.setInTokenCtr(1);
                }

                result += (String) func.accept(this);

                // Output token list
                if(func.getCls() == Function.FUNC_WORK
                  && isActor 
                  && spec.getOutgoingNode() != null){
                    result += indent + "output:\n";
                    addIndent();
                    result += indent + spec.getOutgoingNode()+": ";
                    spec.setOutTokenCtr(1);
                    for (int i = 0; i < spec.getOutputDataRate(); i++){
                        if (i != 0){
                            result+=",";
                        }
                        result+=spec.getNextOutToken();
                    }
                    result+=";\n";
                    spec.setOutTokenCtr(1);
                    unIndent();
                }
            }
        }
        
        ss = oldSS;

        if (isActor){
            unIndent();
            isActor = false;
            result += indent+"}\n\n";
        }
        
        return result;
    }

    public Object visitTypeHelper(TypeHelper th) {
        System.err.println("Error: redundant feature found, helper type.");
        return null;
    }
    
    public Object visitStreamType(StreamType type)
    {
        // Nothing to do here.
        return "";
    }
    
    public Object visitOther(FENode node)
    {
        if (node instanceof ExprJavaConstructor)
            {
                ExprJavaConstructor jc = (ExprJavaConstructor)node;
                return makeConstructor(jc.getType());
            }
        if (node instanceof StmtIODecl) {
            StmtIODecl ap = (StmtIODecl) node;
            String result;
            if (ap.isPrework()) {
                System.err.println("Error: prework is not supported.");
                return null;
            } else if (ap.isWork()) {
                System.err.println("Error: redundant statement found, addSteadyPhase()");
                return null;
            } else {
                System.err.println("Error: redundant statement found, annotateIORate()");
                return null;
            }
        }
        if (node instanceof StmtSetTypes) {
            System.err.println("Error: redundant statement found, setIOTypes()");
            return null;
        } else {
            assert false : node;
            return "";
        }
    }
}
