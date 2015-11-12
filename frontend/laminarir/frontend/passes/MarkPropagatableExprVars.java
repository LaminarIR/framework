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

package laminarir.frontend.passes;

import java.util.Iterator;
import java.util.Map;

import laminarir.frontend.nodes.*;

import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.ArrayList;
import java.util.List;

/**
 * Front-end visitor pass that marks propagatable ExprVars
 * for later constant propagation. Default status of propagatable
 * of all ExprVar are false. Thus, traverse ExprVars where it has
 * to be propagated.
 */

public class MarkPropagatableExprVars extends FEReplacer
{
    // tells the visitor whether the current node can be propagated
    // or not.
    private boolean doProp;

    public MarkPropagatableExprVars ()
    {
        this.doProp = false;    
    }

    protected Expression visitExpression(Expression expr){
        return (Expression)expr.accept(this);
    }

    @Override
	public Object visitStreamSpec(StreamSpec spec)
    {
        List<FieldDecl> newVars = new ArrayList<FieldDecl>();
        List<Function> newFuncs = new ArrayList<Function>();
        List<Parameter> newParams = new ArrayList<Parameter>();

        for(Iterator<Parameter> iter = spec.getParams().iterator(); iter.hasNext(); )
        {

            boolean oldDoProp = doProp;
            doProp = true;
            Parameter oldParam = iter.next();
            Type newType = oldParam.getType();
            if (newType instanceof TypeArray){
                newType = visitTypeArray((TypeArray)newType);
            }
            doProp = oldDoProp;
            Parameter newParam = new Parameter(newType, oldParam.getName());
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

        return new StreamSpec(spec.getContext(), spec.getType(),
                              spec.getStreamType(), spec.getName(), newParams,
                              newVars, newFuncs, spec.isStateful());
        
    }

    @Override
	public Object visitExprArray (ExprArray exp)
    {
        boolean oldDoProp = doProp;
        doProp = false;
        Expression base = visitExpression(exp.getBase());

        // Yousun Ko: do propagation always on array offset
        doProp = true;
        Expression offset = visitExpression(exp.getOffset());
        doProp = oldDoProp;

        if (base == exp.getBase() && offset == exp.getOffset())
            return exp;
        else
            return new ExprArray(exp.getContext(), base, offset);
    }
    
    @Override
	public Object visitExprArrayInit ( ExprArrayInit exp )
    {
        boolean hasChanged = false;
        boolean oldDoProp = doProp;
        doProp = true;
        List<Expression> newElements = new ArrayList<Expression>();
        for (Iterator<Expression> iter = exp.getElements().iterator(); iter.hasNext(); )
            {
                Expression element = iter.next();
                Expression newElement = visitExpression(element);
                newElements.add(newElement);
                if (element != newElement) hasChanged = true;
            }
        doProp = oldDoProp;

        if (!hasChanged) return exp;
        return new ExprArrayInit(exp.getContext(), newElements);
    }

    @Override
	public Object visitExprBinary(ExprBinary exp)
    {
        boolean oldDoProp = doProp;
        doProp = true;
        Expression left = visitExpression(exp.getLeft());
        Expression right = visitExpression(exp.getRight());
        doProp = oldDoProp;

        if (left == exp.getLeft() && right == exp.getRight())
            return exp;
        else
            return new ExprBinary(exp.getContext(), exp.getOp(), left, right);
    }
   
    @Override
    public Object visitExprFunCall(ExprFunCall exp)
    {   
        boolean hasChanged = false;
        List<Object> newParams = new ArrayList<Object>();
        boolean oldDoProp = doProp;
        doProp = true;
        for (Iterator<Object> iter = exp.getParams().iterator(); iter.hasNext(); )
            {
                Expression param = (Expression)iter.next();
                Expression newParam = visitExpression(param);
                newParams.add(newParam);
                if (param != newParam) hasChanged = true;
            }
        doProp = oldDoProp;
        if (!hasChanged) return exp;
        return new ExprFunCall(exp.getContext(), exp.getName(), newParams);
    }   

    @Override
	public Object visitExprPeek(ExprPeek exp)
    {
        boolean oldDoProp = doProp;
        doProp = true;
        Expression expr = visitExpression(exp.getExpr());
        doProp = oldDoProp;

        if (expr == exp.getExpr())
            return exp;
        else
            return new ExprPeek(exp.getContext(), expr);
    }

    @Override
	public Object visitExprRange(ExprRange exp) 
    {
        boolean oldDoProp = doProp;
        doProp = true;
        Expression min = visitExpression(exp.getMin());
        Expression ave = visitExpression(exp.getAve());
        Expression max = visitExpression(exp.getMax());
        doProp = oldDoProp;

        if (min == exp.getMin() && ave == exp.getAve() && max == exp.getMax()) {
            return exp;
        } else {
            return new ExprRange(exp.getContext(), min, ave, max);
        }
    }
    
    @Override
	public Object visitExprTernary(ExprTernary exp)
    {
        boolean oldDoProp = doProp;
        doProp = true;
        Expression a = visitExpression(exp.getA());
        Expression b = visitExpression(exp.getB());
        Expression c = visitExpression(exp.getC());
        doProp = oldDoProp;

        if (a == exp.getA() && b == exp.getB() && c == exp.getC())
            return exp;
        else
            return new ExprTernary(exp.getContext(), exp.getOp(), a, b, c);
    }
    
    @Override
	public Object visitExprTypeCast(ExprTypeCast exp)
    {
        boolean oldDoProp = doProp;
        doProp = true;
        Expression expr = visitExpression(exp.getExpr());
        doProp = oldDoProp;

        if (expr == exp.getExpr())
            return exp;
        else
            return new ExprTypeCast(exp.getContext(), exp.getType(), expr);
    }

    @Override
	public Object visitExprUnary(ExprUnary exp)
    {
        boolean oldDoProp = doProp;
        doProp = false;
        Expression expr = visitExpression(exp.getExpr());
        doProp = oldDoProp;
        if (expr == exp.getExpr())
            return exp;
        else
            return new ExprUnary(exp.getContext(), exp.getOp(), expr);
    }

    @Override
	public Object visitExprVar(ExprVar exp) 
    {
        if (doProp == true){
            return new ExprVar(exp.getContext(), exp.getName(), doProp);
        }else{
            return exp;
        }
    }

    @Override
	public Object visitFieldDecl(FieldDecl field)
    {
        int num = field.getNumFields();
        List<Expression> newInits = new ArrayList<Expression>(num);
        List<Type> newTypes = new ArrayList<Type>(num);
        for (int i = 0; i < num; i++)
            {
                boolean oldDoProp = doProp;
                doProp = true;
                Expression init = field.getInit(i);
                if (init != null)
                    init = (Expression)init.accept(this);
                newInits.add(init);

                Type type = field.getType(i);
                if (type instanceof TypeArray) {
                    // visit base recursively
                    type = visitTypeArray((TypeArray)type);
                }
                newTypes.add(type);
                doProp = oldDoProp;
            }
        return new FieldDecl(field.getContext(), newTypes,
                             field.getNames(), newInits, field.getStates());
    }

    private Type visitTypeArray(TypeArray type){
        Type newBase = type.getBase();
        if (newBase instanceof TypeArray){
            newBase = visitTypeArray((TypeArray)newBase);
        }
        return new TypeArray(newBase,
                    (Expression)type.getLength().accept(this));
    }

    @Override
	public Object visitStmtAssign(StmtAssign stmt)
    {   

        boolean oldDoProp = doProp;
        doProp = false;
        Expression newLHS = visitExpression(stmt.getLHS());

        doProp = true;
        Expression newRHS = visitExpression(stmt.getRHS());
        doProp = doProp;

        if (newLHS == stmt.getLHS() && newRHS == stmt.getRHS())
            return stmt;
        return new StmtAssign(stmt.getContext(), newLHS, newRHS,
                              stmt.getOp());
    }
   
    @Override
	public Object visitStmtPush(StmtPush stmt)
    {
        boolean oldDoProp = doProp;
        doProp = true;
        Expression newValue = visitExpression(stmt.getValue());
        doProp = oldDoProp;

        if (newValue == stmt.getValue()) return stmt;
        return new StmtPush(stmt.getContext(), newValue);
    }
    
    @Override
	public Object visitStmtReturn(StmtReturn stmt)
    {
        boolean oldDoProp = doProp;
        doProp = true;
        Expression newValue = stmt.getValue() == null ? null :
            visitExpression(stmt.getValue());
        doProp = oldDoProp;
        if (newValue == stmt.getValue()) return stmt;
        return new StmtReturn(stmt.getContext(), newValue);
    }
    
    @Override
	public Object visitStmtHelperCall(StmtHelperCall stmt) 
    {
        boolean hasChanged = false;
        List<Expression> newParams = new ArrayList<Expression>();
        
        boolean oldDoProp = doProp;
        doProp = true;
        for (Iterator<Expression> iter = stmt.getParams().iterator(); iter.hasNext(); ) {
            Expression param = iter.next();
            Expression newParam = visitExpression(param);
            newParams.add(newParam);
            if (param != newParam) hasChanged = true;
        }
        doProp = oldDoProp;
        if (!hasChanged) return stmt;
        return new StmtHelperCall(stmt.getContext(), stmt.getHelperPackage(),
                                  stmt.getName(), newParams);
    }

    @Override
	public Object visitStmtVarDecl(StmtVarDecl stmt)
    {
        List<Expression> newInits = new ArrayList<Expression>();
        List<Type> newTypes = new ArrayList<Type>();

        boolean oldDoProp = doProp;
        doProp = true;
        for (int i = 0; i < stmt.getNumVars(); i++)
        {
            Expression init = stmt.getInit(i);
            if (init != null)
                init = visitExpression(init);
            newInits.add(init);

            Type type = stmt.getType(i);
            if (type instanceof TypeArray) {
                // visit base recursively
                type = visitTypeArray((TypeArray)type);
            }
            newTypes.add(type);
        }
        doProp = oldDoProp;

        return new StmtVarDecl(stmt.getContext(), newTypes,
                               stmt.getNames(), newInits);
    }
}
