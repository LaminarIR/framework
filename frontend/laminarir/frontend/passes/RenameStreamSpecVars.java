/* ---------------------------------------------------------------------------
 * LaminarIR 
 *
 * This file is distributed under the MIT Open Source License.
 * See LICENSE.LaminarIR for details.
 * ---------------------------------------------------------------------------
 */

package laminarir.frontend.passes;

import java.util.Iterator;
import java.util.Map;

import laminarir.frontend.nodes.*;

// Yousun Ko
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.ArrayList;
import java.util.List;

/**
 * Front-end visitor pass that renames keywords when there is
 * conflict with local var names.
 */

public class RenameStreamSpecVars extends FEReplacer
{

    // Yousun Ko: maintains program keywords
    String instName;
    private List<String> specVars;

    public RenameStreamSpecVars()
    {
        instName = null;
        specVars = new ArrayList<String>();
    }

    private Type visitTypeArray(TypeArray type){
        Type newBase = type.getBase();
        if (newBase instanceof TypeArray){
            newBase = visitTypeArray((TypeArray)newBase);
        }
        return new TypeArray(newBase,
                    (Expression)type.getLength().accept(this));
    }


    /**
     * Top level visitor, append instance ID to distinguish state vars 
     * of different instances.
     * 
     */
    @Override
	public Object visitStreamSpec(StreamSpec spec)
    {
        if (spec.getInstName() == null){
            System.err.println("Error: instance name of given StreamSpec is not set.");
            System.exit(1);
        }

        instName = spec.getInstName();

        List<FieldDecl> newVars = new ArrayList<FieldDecl>();
        List<Function> newFuncs = new ArrayList<Function>();
        List<Parameter> newParams = new ArrayList<Parameter>();

        for (FieldDecl field: spec.getVars()){
            for(int i = 0; i < field.getNumFields(); i++){
                specVars.add(field.getName(i));
            }
        }

        for (Parameter param: spec.getParams()){
            specVars.add(param.getName());
        }

        boolean changed = false;
        
        for (Iterator<Parameter> iter = spec.getParams().iterator(); iter.hasNext(); )
        {
            Parameter oldParam = iter.next();
            Type newType = oldParam.getType();
            if (newType instanceof TypeArray) {
                // visit base recursively
                newType = visitTypeArray((TypeArray)newType);
            }

            Parameter newParam = new Parameter(newType, 
                                            oldParam.getName()+"_"+instName);
            newParams.add(newParam);
        }

        for (Iterator<FieldDecl> iter = spec.getVars().iterator(); iter.hasNext(); )
            {
                FieldDecl oldVar = iter.next();
                FieldDecl newVar = (FieldDecl)oldVar.accept(this);
                newVars.add(newVar);
            }
        for (Iterator<Function> iter = spec.getFuncs().iterator(); iter.hasNext(); )
            {
                Function oldFunc = iter.next();
                Function newFunc = (Function)oldFunc.accept(this);
                newFuncs.add(newFunc);
            }

        StreamSpec newSpec =  new StreamSpec(spec.getContext(), spec.getType(),
                              spec.getStreamType(), spec.getName(), newParams,
                              newVars, newFuncs, spec.isStateful());
        newSpec.setInstName(instName);
        return newSpec;
    }

    @Override
	public Object visitFunction(Function func)
    {
        Statement newBody = (Statement)super.visitStmtBlock((StmtBlock)func.getBody());
        Function newFunc = new Function(func.getContext(), func.getCls(),
                func.getName(), func.getReturnType(), func.getParams(), 
                newBody, func.getPeekRate(), func.getPopRate(), func.getPushRate());
        return newFunc;
    }

    @Override
    public Object visitExprVar(ExprVar exp){
        String name = exp.getName();
        for(String var: specVars){
            if (name.equals(var)){
                return super.visitExprVar(
                        new ExprVar(exp.getContext(), 
                            name+"_"+instName, exp.isPropagatable()));
            }
        }
        return super.visitExprVar(exp);
    }

    @Override
	public Object visitFieldDecl(FieldDecl field)
    {
        List<Type> newTypes = new ArrayList<Type>();
        List<String> newNames = new ArrayList<String>(field.getNames());
        List<Expression> newInits = new ArrayList<Expression>();
        List<Boolean> newStates = new ArrayList<Boolean>();

        for (int i = 0; i < field.getNumFields(); i++){
            Expression init = field.getInit(i);
            if(init != null){
                init = (Expression)init.accept(this);
            }
            newInits.add(init);
            for (String var: specVars){
                if (field.getNames().get(i).equals(var)){
                    String newName = field.getNames().get(i)+"_"+instName;
                    newNames.set(i, newName);
                }
            }
            Type type = field.getType(i);
            if (type instanceof TypeArray) {
                // visit base recursively
                type = visitTypeArray((TypeArray)type);
            }
            newTypes.add(type);
            newStates.add(field.getState(i));
        }
        FieldDecl newField = new FieldDecl(field.getContext(),
                newTypes, newNames, newInits, newStates);
        return newField;
    }

    @Override
	public Object visitStmtVarDecl(StmtVarDecl stmt)
    {
        List<Expression> newInits = new ArrayList<Expression>();
        List<Type> newTypes = new ArrayList<Type>();

        for (int i = 0; i < stmt.getNumVars(); i++)
            {
                Expression init = stmt.getInit(i);
                if (init != null)
                    init = (Expression)init.accept(this);
                newInits.add(init);

                Type type = stmt.getType(i);
                if (type instanceof TypeArray) {
                    // visit base recursively
                    type = visitTypeArray((TypeArray)type);
                }
                newTypes.add(type);
            }

        return new StmtVarDecl(stmt.getContext(), newTypes,
                               stmt.getNames(), newInits);
    }

}
