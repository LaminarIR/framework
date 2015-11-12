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

import java.util.ArrayList;
import java.util.List;

/**
 * Front-end visitor pass that marks true state vars of an actor.
 * A true state var must keep the state information of an actor, 
 * not a constant value storage.
 * 
 */

public class MarkTrueStateVars extends FEReplacer
{
    private boolean updated;
    private boolean isAddr;
    private String varToCheck;

    public MarkTrueStateVars (){
        this.updated = false;
        this.isAddr = false;
        this.varToCheck = "";
    }

    @Override
	public Object visitStreamSpec(StreamSpec spec)
    {
        // check all actor field vars whether the var is updated during steady-state
        // iterations. If the var is updated, then mark the var as the true state var.
        List<FieldDecl> newFields = new ArrayList<FieldDecl>();
        boolean changed = false;
        
        for (Iterator<FieldDecl> iter = spec.getVars().iterator(); iter.hasNext(); )
        {
            FieldDecl field = iter.next();
            List<Type> types = field.getTypes();
            List<String> names = field.getNames();
            List<Expression> inits = field.getInits();
            List<Boolean> states = new ArrayList<Boolean>();

            for(int i = 0; i < field.getNumFields(); i++)
            {
                varToCheck = field.getName(i);

                updated = false;
                isAddr = false;
                // check the work function
                for (Iterator<Function> funcIter = spec.getFuncs().iterator(); funcIter.hasNext(); )
                {
                    Function oldFunc = funcIter.next();
                    if (oldFunc.getCls() != Function.FUNC_INIT)
                    {
                        Function newFunc = (Function)oldFunc.accept(this);
                    }
                }
                states.add(updated);
            }
            FieldDecl newVar = new FieldDecl(field.getContext(), types, names, inits, states);
            newFields.add(newVar);
        }

        return new StreamSpec(spec.getContext(), spec.getType(),
                              spec.getStreamType(), spec.getName(), spec.getParams(),
                              newFields, spec.getFuncs(), spec.isStateful());
    }

    @Override
    public Object visitExprArray(ExprArray exp){
        boolean oldIsAddr = isAddr;
        Expression base = (Expression)exp.getBase().accept(this);
        // array offsets are not addresses
        isAddr = false;
        Expression offset = (Expression)exp.getOffset().accept(this);
        isAddr = oldIsAddr;
        return exp;
    }

    @Override
	public Object visitExprUnary(ExprUnary exp)
    {
        int op = exp.getOp();
        if (op == ExprUnary.UNOP_PREINC ||
            op == ExprUnary.UNOP_POSTINC ||
            op == ExprUnary.UNOP_PREDEC ||
            op == ExprUnary.UNOP_POSTDEC){
            boolean oldIsAddr = isAddr;
            isAddr = true;
            Expression newExpr = (Expression)exp.getExpr().accept(this);
            isAddr = oldIsAddr;
        }
        return exp;
    }

    @Override
    public Object visitExprVar(ExprVar exp){
        if (isAddr && varToCheck.equals(exp.getName())){
            updated = true;
        }
        return exp;
    }

    @Override
	public Object visitStmtAssign(StmtAssign stmt)
    {    
        boolean oldIsAddr = isAddr;
        isAddr = true;
        Expression newLHS = (Expression)stmt.getLHS().accept(this);
        isAddr = oldIsAddr;
        return stmt;
    }
}
