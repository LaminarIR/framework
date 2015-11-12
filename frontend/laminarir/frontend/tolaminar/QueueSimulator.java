/* ---------------------------------------------------------------------------
 * LaminarIR 
 *
 * This file is distributed under the MIT Open Source License.
 * See LICENSE.LaminarIR for details. 
 * ---------------------------------------------------------------------------
 */

package laminarir.frontend.tolaminar;

import java.util.Iterator;
import java.util.List;
import laminarir.frontend.nodes.*;
import java.util.ArrayList;
/**
 * Traverse a front-end tree and count queue operations. 
 */

public class QueueSimulator implements FEVisitor {

    private StreamSpec ss;

    // list of constant variables
    private ConstVarList constList;

    private String commType;

    private boolean branch;

    public QueueSimulator(StreamSpec spec, ConstVarList constList, String commType){
        this.ss = spec;
        // deep copy the constList to not affect the given constList
        this.constList = new ConstVarList(constList);
        this.commType = commType;
        branch = false;
    }

    private boolean isTopLevelSpec(StreamSpec spec) {
        StreamType st = spec.getStreamType();
        return spec.getType() != StreamSpec.STREAM_GLOBAL
                && st != null
                && st.getIn() instanceof TypePrimitive
                && ((TypePrimitive) st.getIn()).getType() == TypePrimitive.TYPE_VOID
                && st.getOut() instanceof TypePrimitive
                && ((TypePrimitive) st.getOut()).getType() == TypePrimitive.TYPE_VOID;
    }

    @Override
    public Object visitExprArray (ExprArray exp) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps baseCtr = (NumQueueOps) exp.getBase().accept(this);
        result.add(baseCtr);
        NumQueueOps offsetCtr = (NumQueueOps) exp.getOffset().accept(this);
        result.add(offsetCtr);
        return result;
    }

    @Override
    public Object visitExprArrayInit (ExprArrayInit exp) {
        NumQueueOps result = new NumQueueOps();
        List<Expression> elems = exp.getElements();
        for (Expression e : elems) {
            NumQueueOps ctr = (NumQueueOps) e.accept(this);
            result.add(ctr);
        }
        return result;
    }

    @Override
    public Object visitExprBinary(ExprBinary exp) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps leftCtr = (NumQueueOps) exp.getLeft().accept(this);
        result.add(leftCtr);
        NumQueueOps rightCtr = (NumQueueOps) exp.getRight().accept(this);
        result.add(rightCtr);

        return result;
    }

    @Override
    public Object visitExprComplex(ExprComplex exp) {
        NumQueueOps result = new NumQueueOps();
        // We should never see one of these at this point.
        assert false : exp;
        // If we do, print something vaguely intelligent:
        if (exp.getReal() != null){
            NumQueueOps realCtr = (NumQueueOps) exp.getReal().accept(this);
            result.add(realCtr);
        }
        if (exp.getImag() != null){
            NumQueueOps imagCtr = (NumQueueOps) exp.getImag().accept(this);
            result.add(imagCtr);
        }
        return result;
    }

    @Override
    public Object visitExprComposite(ExprComposite exp) {
        // We should never see one of these at this point.
        // assert false : exp;
        // If we do, print something vaguely intelligent:
        return new NumQueueOps();    
    }

    @Override
    public Object visitExprConstBoolean(ExprConstBoolean exp) {
        return new NumQueueOps();    
    }

    @Override
    public Object visitExprConstChar(ExprConstChar exp) {
        return new NumQueueOps();    
    }

    @Override
    public Object visitExprConstFloat(ExprConstFloat exp) {
        return new NumQueueOps();    
    }

    @Override
    public Object visitExprConstInt(ExprConstInt exp) {
        return new NumQueueOps();    
    }

    @Override
    public Object visitExprConstStr(ExprConstStr exp) {
        return new NumQueueOps();    
    }

    @Override
    public Object visitExprDynamicToken(ExprDynamicToken exp) {
        return new NumQueueOps();    
    }

    @Override
    public Object visitExprField(ExprField exp) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps leftCtr = (NumQueueOps) exp.getLeft().accept(this);
        result.add(leftCtr);
        return result;
    }

    @Override
    public Object visitExprFunCall(ExprFunCall exp) {
        NumQueueOps result = new NumQueueOps();
        for ( Object param : exp.getParams() ) {
            NumQueueOps paramCtr = (NumQueueOps) ((Expression)param).accept(this);
            result.add(paramCtr);
        }
        return result;
    }

    @Override
    public Object visitExprHelperCall(ExprHelperCall exp) {
        NumQueueOps result = new NumQueueOps();
        for (Expression param : exp.getParams()) {
            NumQueueOps paramCtr = (NumQueueOps) ((Expression)param).accept(this);
            result.add(paramCtr);
        }
        return result;
    }

    @Override
    public Object visitExprIter(ExprIter exprIter) {
        return new NumQueueOps();
    }

    @Override
    public Object visitExprPeek(ExprPeek exp) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps exprCtr = (NumQueueOps) exp.getExpr().accept(this);
        // Yousun Ko: not resolvable if the peek index is not constant
        PartialEvaluator idxEval = new PartialEvaluator(constList);
        int idx = (int) exp.getExpr().accept(idxEval);
        if (!idxEval.isResolved()){
            result.inResolved = false;
        }
        result.numPeek = 1;
        result.add(exprCtr);
        return result;
    }

    @Override
    public Object visitExprPop(ExprPop exp) {
        NumQueueOps result = new NumQueueOps();
        result.numPop = 1;
        return result;
    }

    @Override
    public Object visitExprRange(ExprRange exp) {
        NumQueueOps result = new NumQueueOps();

        NumQueueOps minCtr = (NumQueueOps) exp.getMin().accept(this);
        result.add(minCtr);
        NumQueueOps aveCtr = (NumQueueOps) exp.getAve().accept(this);
        result.add(aveCtr);
        NumQueueOps maxCtr = (NumQueueOps) exp.getMax().accept(this);
        result.add(maxCtr);
        return result;
    }

    @Override
    public Object visitExprTernary(ExprTernary exp) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps a = (NumQueueOps) exp.getA().accept(this);
        result.add(a);
        NumQueueOps b = (NumQueueOps) exp.getB().accept(this);
        result.add(b);
        NumQueueOps c = (NumQueueOps) exp.getC().accept(this);
        result.add(c);
        switch (exp.getOp()) {
            case ExprTernary.TEROP_COND:
                return result;
            default:
                assert false : exp;
                return new NumQueueOps();
        }
    }

    @Override
    public Object visitExprTypeCast(ExprTypeCast exp) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps exprCtr = (NumQueueOps) exp.getExpr().accept(this);
        result.add(exprCtr);
        return result;
    }

    @Override
    public Object visitExprUnary(ExprUnary exp) {
        // Yousun Ko: drop the var from the constList in case when 
        //            op causes value update in the middle of a statement
        int op = exp.getOp(); 
        if (op == ExprUnary.UNOP_PREINC ||
            op == ExprUnary.UNOP_POSTINC ||
            op == ExprUnary.UNOP_PREDEC ||
            op == ExprUnary.UNOP_POSTDEC) {
            if (exp.getExpr() instanceof ExprVar){
                constList.remove(((ExprVar)exp.getExpr()).getName());
            }
        }

        NumQueueOps result = new NumQueueOps();
        NumQueueOps child = (NumQueueOps) exp.getExpr().accept(this);
        result.add(child);
        return result;
    }

    @Override
    public Object visitExprVar(ExprVar exp) {
        return new NumQueueOps();
    }

    @Override
    public Object visitFieldDecl(FieldDecl field) {
        NumQueueOps result = new NumQueueOps();
        for (int i = 0; i < field.getNumFields(); i++) {
            if (field.getInit(i) != null){
                NumQueueOps ctr = (NumQueueOps) field.getInit(i).accept(this);
                result.add(ctr);
            }
        }
        return result;
    }

    @Override
    public Object visitFunction(Function func) {
        NumQueueOps result = new NumQueueOps();

        NumQueueOps bodyCtr = (NumQueueOps) func.getBody().accept(this);
        if (func.getCls() == Function.FUNC_HELPER){
            // Yousun Ko: not resolvable when helper functions contain push/pop
            if (bodyCtr.numPop != 0){
                bodyCtr.inResolved = false;
            }
            if (bodyCtr.numPush != 0){
                bodyCtr.outResolved = false;
            }
        }

        result.add(bodyCtr);
        return result;
    }

    @Override
    public Object visitFuncWork(FuncWork func) {
        // Nothing special here; we get to ignore the I/O rates.
        return visitFunction(func);
    }

    @Override
    public Object visitProgram(Program prog) {
        NumQueueOps result = new NumQueueOps();
        
        for (TypeHelper th : prog.getHelpers()) {
            NumQueueOps helperCtr = (NumQueueOps) visitTypeHelper(th);
            result.add(helperCtr);
        }

        // Yousun Ko: generate actor definitions
        StreamSpec main = null;
        for (Iterator<StreamSpec> iter = prog.getStreams().iterator(); iter.hasNext();) {
            StreamSpec spec = iter.next();            
            if (isTopLevelSpec(spec)) {
                assert main == null : "Found more than one top-level stream";
                main = spec;
                iter.remove();
            } else {
                NumQueueOps specCtr= (NumQueueOps)spec.accept(this);
                result.add(specCtr);
            }
        }
        assert main != null : "Did not find any top-level stream";
        NumQueueOps mainCtr = (NumQueueOps) main.accept(this);
        result.add(mainCtr);
        return result;
    }

    public Object visitTypeHelper(TypeHelper th) {
        NumQueueOps result = new NumQueueOps();
        int num = th.getNumFuncs();
        for (int i = 0; i < num; i++) {
            NumQueueOps funcCtr = (NumQueueOps) th.getFunction(i).accept(this);
            result.add(funcCtr);
        }
        return result;
    }


    @Override
    public Object visitSCAnon(SCAnon creator) {
        NumQueueOps result = new NumQueueOps();
        assert false : "NodesToLaminarIR run before NameAnonymousStreams";
        NumQueueOps specCtr = (NumQueueOps) creator.getSpec().accept(this);
        result.add(specCtr);
        return result;
    }

    @Override
    public Object visitSCSimple(SCSimple creator) {   
        NumQueueOps result = new NumQueueOps();
        for (Expression param : creator.getParams()) {
            NumQueueOps paramCtr = (NumQueueOps) param.accept(this);
            result.add(paramCtr);
        }
        return result;
    }

    @Override
    public Object visitSJDuplicate(SJDuplicate sj) {
        return new NumQueueOps();
    }

    @Override
    public Object visitSJRoundRobin(SJRoundRobin sj) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps sjCtr = (NumQueueOps) sj.getWeight().accept(this);
        result.add(sjCtr);
        return result;
    }

    @Override
    public Object visitSJWeightedRR(SJWeightedRR sj) {
        NumQueueOps result = new NumQueueOps();
        for (Expression weight  : sj.getWeights()) {            
            NumQueueOps sjCtr = (NumQueueOps) weight.accept(this);
            result.add(sjCtr);
        }
        return result;
    }

    @Override
    public Object visitStmtAdd(StmtAdd stmt) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps scCtr = (NumQueueOps) doStreamCreator("add",stmt.getCreator());
        result.add(scCtr);
        return result;
    }

    @Override
    public Object visitStmtAssign(StmtAssign stmt) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps lhs = (NumQueueOps) stmt.getLHS().accept(this);
        result.add(lhs);
        NumQueueOps rhs = (NumQueueOps) stmt.getRHS().accept(this);
        result.add(rhs);

        // update const param list
        if (stmt.getLHS() instanceof ExprVar){
            String varLHS = ((ExprVar)stmt.getLHS()).getName();

            PartialEvaluator rhsEval = new PartialEvaluator(constList);
            int rhsVal = (int) stmt.getRHS().accept(rhsEval);

            if( !branch && rhsEval.isResolved() ){
                String newVal = null;

                if (stmt.getOp() == 0){
                    newVal = Integer.toString(rhsVal);
                } else {
                    PartialEvaluator lhsEval = new PartialEvaluator(constList);
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
                if(newVal != null && !ss.isStateVar(varLHS)){
                    constList.add(new TypePrimitive(TypePrimitive.TYPE_INT), 
                        varLHS, newVal);
                }else{
                    constList.remove(varLHS);
                }
            }else{
                constList.remove(varLHS);
            }
        }

        return result;
    }

    @Override
    public Object visitStmtBlock(StmtBlock stmt) {
        NumQueueOps result = new NumQueueOps();
        for ( Statement s : stmt.getStmts()) {
            NumQueueOps sCtr = (NumQueueOps) s.accept(this);
            result.add(sCtr);
        }
        return result;
    }

    @Override
    public Object visitStmtBody(StmtBody stmt) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps scCtr = (NumQueueOps) doStreamCreator("setBody", stmt.getCreator());
        result.add(scCtr);
        return result;
    }

    @Override
    public Object visitStmtBreak(StmtBreak stmt) {
        return new NumQueueOps();
    }

    @Override
    public Object visitStmtContinue(StmtContinue stmt) {
        return new NumQueueOps();
    }

    @Override
    public Object visitStmtDoWhile(StmtDoWhile stmt) {

        boolean oldBranch = branch;
        branch = true;

        NumQueueOps result = new NumQueueOps();
        NumQueueOps bodyCtr = (NumQueueOps) stmt.getBody().accept(this);
        result.add(bodyCtr);
        NumQueueOps condCtr = (NumQueueOps) stmt.getCond().accept(this);
        // Yousun Ko: not resolvable, if push/pop are invoked in loop-body 
        if (bodyCtr.numPop != 0){
            bodyCtr.inResolved = false;
        }
        if (bodyCtr.numPush != 0){
            bodyCtr.outResolved = false;
        }
        result.add(bodyCtr);

        branch = oldBranch;

        return result;
    }

    @Override
    public Object visitStmtEmpty(StmtEmpty stmt) {
        return new NumQueueOps();
    }

    @Override
    public Object visitStmtEnqueue(StmtEnqueue stmt) {
        return new NumQueueOps();
    }

    @Override
    public Object visitStmtExpr(StmtExpr stmt) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps exprCtr = (NumQueueOps) stmt.getExpression().accept(this);
        result.add(exprCtr);
        return result;
    }

    @Override
    public Object visitStmtFor(StmtFor stmt) {
        NumQueueOps result = new NumQueueOps();
        if (stmt.getInit() != null){
            NumQueueOps initCtr = (NumQueueOps) stmt.getInit().accept(this);
            result.add(initCtr);
        }
        if (stmt.getCond() != null){
            NumQueueOps condCtr = (NumQueueOps) stmt.getCond().accept(this);
            result.add(condCtr);
        }
        if (stmt.getIncr() != null){
            NumQueueOps incrCtr = (NumQueueOps) stmt.getIncr().accept(this);
            result.add(incrCtr);
        }
        List<Integer> iters = getIters(stmt);

        // Yousun Ko: do not unroll when commType is FIFO
        if (commType.equals("fifo")){
            iters = null;
        }
        String iterVar = getIterVar(stmt);
        NumQueueOps bodyCtr;
        // Yousun Ko: loop iteration is not resolved
        if (iters == null){ 
            boolean oldBranch = branch;
            branch = true;
            // Yousun Ko: constantness of the iterVar ends here
            constList.remove(iterVar);
            bodyCtr = (NumQueueOps) stmt.getBody().accept(this);
            if (bodyCtr.numPop != 0){
                bodyCtr.inResolved = false;
            }
            if (bodyCtr.numPush != 0){
                bodyCtr.outResolved = false;
            }
            // Yousun Ko: constantness of the iterVar ends here
            constList.remove(iterVar);
            branch = oldBranch;
        // Yousun Ko: loop iteration is 0. reset the ctr.
        } else if (iters.size() == 0){  
            bodyCtr = new NumQueueOps();
        // Yousun Ko: multiply iters.size() to the NumQueueOps
        } else {
            // Yousun Ko: add the iter var as a const param for partial evaluation
            //          : dummy initial value 0 to check resolvability.
            if (!ss.isStateVar(iterVar)){
                constList.add(new TypePrimitive(TypePrimitive.TYPE_INT), iterVar, 
                    Integer.toString(iters.get(0)));
            }
            bodyCtr = (NumQueueOps) stmt.getBody().accept(this);
            // Yousun Ko: liveness of the iter var ends here
            constList.remove(iterVar);

            if (bodyCtr.inResolved){
                bodyCtr.numPop *= iters.size();
            }
            if (bodyCtr.outResolved){
                bodyCtr.numPush *= iters.size();
            }
        }
        result.add(bodyCtr);
        return result;
    }

    /** Yousun Ko: calculate loop iteration of generic for-loops. E.g., 
      * 1) LT and LE with increment, and
      * 2) GT and GE with decrement.
      * Regard other possibilities as not resolvable.
      */
    public List<Integer> getIters(StmtFor stmt){
        // Yousun Ko: name of the iterative variable
        String iterVar = null;
        // Yousun Ko: to calculate loop iter num
        int lowerBound = 0;
        int upperBound = 0;
        int interval = 0;
        // Yousun Ko: partial evaluators
        PartialEvaluator initEval = new PartialEvaluator(constList);
        int initVal = 0;
        PartialEvaluator condEval = new PartialEvaluator(constList);
        int condVal = 0;
        int condOp = -1;
        PartialEvaluator incrEval = new PartialEvaluator(constList);


        boolean resolved = true;
        // Yousun Ko: evalute init
        if (resolved && stmt.getInit() != null){
            // Yousun Ko: e.g., int i = 0
            if (stmt.getInit() instanceof StmtVarDecl) {
                assert ((StmtVarDecl)stmt.getInit()).getNumVars() == 1;
                iterVar = ((StmtVarDecl)stmt.getInit()).getName(0);
                initVal = (int) ((StmtVarDecl)stmt.getInit()).getInit(0).accept(initEval);
                if (!initEval.isResolved()){
                    resolved = false;
                }
            // Yousun Ko: e.g., i = 0
            } else if (stmt.getInit() instanceof StmtAssign) {
                iterVar = (String)((StmtAssign)stmt.getInit()).getLHS().toString();
                initVal = (int)((StmtAssign)stmt.getInit()).getRHS().accept(initEval);
                if (!initEval.isResolved()){
                    resolved = false;
                }
            } else {
            // Yousun Ko: other stmt types are regarded as not-resolvable
                resolved = false;
            }
        }

        // Yousun Ko: evaluate cond
        if (resolved && stmt.getCond() != null){
            if (stmt.getCond() instanceof ExprBinary){
                // Yousun Ko: record op for later use.
                condOp = ((ExprBinary)stmt.getCond()).getOp();
                // Yousun Ko: check if the iteration variable is consistant
                //          : iter var should be on the left side
                String localIterVar = 
                    (String)((ExprBinary)stmt.getCond()).getLeft().toString();
                if (!iterVar.equals(localIterVar)) {
                    resolved = false;        
                }
                condVal = (int) ((ExprBinary)stmt.getCond()).getRight().accept(condEval);
                if (!condEval.isResolved()){
                    resolved = false;
                }
            } else {
            // Yousun Ko: Other expr are regarded as not-resolvable
                resolved = false;
            }
        }


        // Yousun Ko: evaluate incr
        if (resolved && stmt.getIncr() != null){
            // Yousun Ko: e.g., i += 1
            if (stmt.getIncr() instanceof StmtAssign){
                String localIterVar = (String)((StmtAssign)stmt.getIncr()).getLHS().toString();
                if (!iterVar.equals(localIterVar)) {
                    resolved = false;        
                }
                
                interval = (int) ((StmtAssign)stmt.getIncr()).getRHS().accept(incrEval);
                if (!incrEval.isResolved()){
                    resolved = false;
                }
                int op = ((StmtAssign)stmt.getIncr()).getOp(); 
                if (op == ExprBinary.BINOP_ADD){
                }else if (op == ExprBinary.BINOP_SUB){
                    interval = interval*(-1);
                }else{
                    resolved = false;
                }
            // Yousun Ko : e.g., i++
            }else if (stmt.getIncr() instanceof StmtExpr){
                Expression expr = ((StmtExpr)stmt.getIncr()).getExpression();
                if (expr instanceof ExprUnary){
                    String localIterVar = (String)((ExprUnary) expr).getExpr().toString();
                    if (!iterVar.equals(localIterVar)) {
                        resolved = false;        
                    }

                    int op = ((ExprUnary)expr).getOp();
                    if (op == ExprUnary.UNOP_POSTINC
                            || op == ExprUnary.UNOP_PREINC){
                        interval = 1;
                    }else if (op == ExprUnary.UNOP_POSTDEC
                            || op == ExprUnary.UNOP_PREDEC){
                        interval = -1;
                    }else{
                        resolved = false;
                    }
                }else{
                    resolved = false;
                }
            } else {
                resolved = false;
            }
        }
        // Yousun Ko: check if the iterVar has been updated in body
        if (resolved){
            // Yousun Ko: backup the previous values
            ConstVarList oldConstList = new ConstVarList(constList);
            constList.add(new TypePrimitive(TypePrimitive.TYPE_INT), iterVar, Integer.toString(initVal));
            NumQueueOps bodyCtr = (NumQueueOps) stmt.getBody().accept(this);
            ConstVarList newConstList = new ConstVarList(constList);
            // Yousun Ko: if iterVar is updated within a loop, then 
            if (!(Integer.toString(initVal).equals(newConstList.getVal(iterVar)))){
                resolved = false;
            }

            // Yousun Ko: restore the constList
            constList = new ConstVarList(oldConstList);
        }

        // Yousun Ko: do traverse
        List<Integer> iters = new ArrayList<Integer>();
        if (resolved) {
            // Yousun Ko: calculate the number of iteration
            if (condOp == ExprBinary.BINOP_LE){
                for ( int i = initVal; i <= condVal; i+=interval ) {
                    iters.add(i);
                }
            }else if (condOp == ExprBinary.BINOP_LT){
                for ( int i = initVal; i < condVal; i+=interval ) {
                    iters.add(i);
                }
            }else if (condOp == ExprBinary.BINOP_GE){
                for ( int i = initVal; i >= condVal; i+=interval ) {
                    iters.add(i);
                }
            }else if (condOp == ExprBinary.BINOP_GT){
                for ( int i = initVal; i > condVal; i+=interval ) {
                    iters.add(i);
                }
            // Yousun Ko: Other expr are regarded as not-resolvable
            }else{
                return null;
            }
            return iters;
        } else {  // not resolved
            return null;
        }
    }

    /** Yousun Ko: return the iter var 
      */
    public String getIterVar(StmtFor stmt){
        // Yousun Ko: name of the iterative variable
        String iterVar = null;

        boolean resolved = true;
        // Yousun Ko: evalute init
        if (resolved && stmt.getInit() != null){
            // Yousun Ko: e.g., int i = 0
            if (stmt.getInit() instanceof StmtVarDecl) {
                assert ((StmtVarDecl)stmt.getInit()).getNumVars() == 1;
                iterVar = ((StmtVarDecl)stmt.getInit()).getName(0);
            // Yousun Ko: e.g., i = 0
            } else if (stmt.getInit() instanceof StmtAssign) {
                iterVar = (String)((StmtAssign)stmt.getInit()).getLHS().toString();
            }
        }

        // Yousun Ko: evaluate cond
        if (resolved && stmt.getCond() != null){
            if (stmt.getCond() instanceof ExprBinary){
                String localIterVar = 
                    (String)((ExprBinary)stmt.getCond()).getLeft().toString();
                if(iterVar == null){
                    iterVar = localIterVar;
                }else if (!iterVar.equals(localIterVar)) {
                    resolved = false;        
                }
            }
        }


        // Yousun Ko: evaluate incr
        if (resolved && stmt.getIncr() != null){
            // Yousun Ko: e.g., i += 1
            if (stmt.getIncr() instanceof StmtAssign){
                String localIterVar = (String)((StmtAssign)stmt.getIncr()).getLHS().toString();
                if (iterVar == null){
                    iterVar = localIterVar;
                } else if (!iterVar.equals(localIterVar)) {
                    resolved = false;        
                }
            // Yousun Ko : e.g., i++
            }else if (stmt.getIncr() instanceof StmtExpr){
                Expression expr = ((StmtExpr)stmt.getIncr()).getExpression();
                if (expr instanceof ExprUnary){
                    String localIterVar = (String)((ExprUnary) expr).getExpr().toString();
                    if (iterVar == null) {
                        iterVar = localIterVar;
                    } else if (!iterVar.equals(localIterVar)) {
                        resolved = false;        
                    }
                }
            }
        }

        if(!resolved){
            System.err.println("Error: invalid for statement found.");
            System.exit(1);
            return null;
        }else{
            return iterVar;
        }
    }

    @Override
    public Object visitStmtIfThen(StmtIfThen stmt) {
        assert stmt.getCond() != null;

        boolean oldBranch = branch;
        branch = true;

        NumQueueOps result = new NumQueueOps();
        NumQueueOps condCtr = (NumQueueOps) stmt.getCond().accept(this);
        result.add(condCtr);
        NumQueueOps consCtr = (NumQueueOps) stmt.getCons().accept(this);
        result.add(consCtr);
        NumQueueOps altCtr = new NumQueueOps();
        if (stmt.getAlt() != null){
            altCtr = (NumQueueOps) stmt.getAlt().accept(this);
        }
        // Do not add altCtr to result. 
        // Only reset resolved flag when qOp numbers not match.
        int status = consCtr.examines(altCtr);
        if (status != NumQueueOps.RESOLVED){
            if (status == NumQueueOps.BOTH_NOT_RESOLVED){
                result.inResolved = false;
                result.outResolved = false;
            }else if (status == NumQueueOps.IN_NOT_RESOLVED){
                result.inResolved = false;
            }else if (status == NumQueueOps.OUT_NOT_RESOLVED){
                result.outResolved = false;
            }
        }

        branch = oldBranch;

        return result;
    }

    @Override
    public Object visitStmtJoin(StmtJoin stmt) {
        assert stmt.getJoiner() != null;
        NumQueueOps result = new NumQueueOps();
        NumQueueOps joinerCtr = (NumQueueOps) stmt.getJoiner().accept(this);
        result.add(joinerCtr);
        return result;
    }

    @Override
    public Object visitStmtLoop(StmtLoop stmt) {
        assert stmt.getCreator() != null;
        NumQueueOps result = new NumQueueOps();
        NumQueueOps scCtr = (NumQueueOps) doStreamCreator("setLoop", stmt.getCreator());
        result.add(scCtr);
        return result;
    }

    private Object doStreamCreator(String how, StreamCreator sc) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps scCtr = (NumQueueOps) sc.accept(this);
        result.add(scCtr);
        return result;
    }

    @Override
    public Object visitStmtPush(StmtPush stmt) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps valCtr = (NumQueueOps) stmt.getValue().accept(this);
        result.add(valCtr);
        result.numPush += 1;
        return result;
    }

    @Override
    public Object visitStmtReturn(StmtReturn stmt) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps valCtr = (NumQueueOps) stmt.getValue().accept(this);
        result.add(valCtr);
        return result;
    }

    @Override
    public Object visitStmtSendMessage(StmtSendMessage stmt) {
        NumQueueOps result = new NumQueueOps();
        NumQueueOps recCtr = (NumQueueOps) stmt.getReceiver().accept(this);
        result.add(recCtr);

        if (stmt.getMinLatency() == null) {
            if (stmt.getMaxLatency() != null){
                NumQueueOps maxCtr = (NumQueueOps) stmt.getMaxLatency().accept(this);
                result.add(maxCtr);
            }
        } else {
            Expression max = stmt.getMaxLatency();
            if (max == null){
                max = new ExprBinary(null, ExprBinary.BINOP_MUL,
                        stmt.getMinLatency(), new ExprConstInt(null, 100));
            }
            NumQueueOps minCtr = (NumQueueOps) stmt.getMinLatency().accept(this);
            result.add(minCtr);
            NumQueueOps maxCtr = (NumQueueOps) max.accept(this);
            result.add(maxCtr);
        }
        for ( Expression param : stmt.getParams() ) {
            NumQueueOps paramCtr = (NumQueueOps) param.accept(this);
            result.add(paramCtr);
        }
        return result;
    }

    @Override
    public Object visitStmtHelperCall(StmtHelperCall stmt) {
        NumQueueOps result = new NumQueueOps();
        for ( Expression param : stmt.getParams()) {
            NumQueueOps paramCtr = (NumQueueOps) param.accept(this);
            result.add(paramCtr);
        }
        return result;
    }

    @Override
    public Object visitStmtSplit(StmtSplit stmt) {
        assert stmt.getSplitter() != null;
        NumQueueOps result = new NumQueueOps();
        NumQueueOps splitCtr = (NumQueueOps) stmt.getSplitter().accept(this);
        result.add(splitCtr);
        return result;
    }

    @Override
    public Object visitStmtVarDecl(StmtVarDecl stmt) {
        NumQueueOps result = new NumQueueOps();
        for (int i = 0; i < stmt.getNumVars(); i++) {
            if (stmt.getInit(i) != null){
                NumQueueOps initCtr = (NumQueueOps) stmt.getInit(i).accept(this);
                result.add(initCtr);

                PartialEvaluator rhsEval = new PartialEvaluator(constList);
                int rhsVal = (int) stmt.getInit(i).accept(rhsEval);

                constList.remove(stmt.getName(i));

                if(rhsEval.isResolved() && !branch
                       && !ss.isStateVar(stmt.getName(i)) && commType.equals("direct")){
                    constList.add(new TypePrimitive(TypePrimitive.TYPE_INT),
                            stmt.getName(i),Integer.toString(rhsVal));
                }
            }
        }
        return result;
    }

    @Override
    public Object visitStmtWhile(StmtWhile stmt) {
        assert stmt.getCond() != null;
        assert stmt.getBody() != null;

        boolean oldBranch = branch;
        branch = true;

        NumQueueOps result = new NumQueueOps();
        NumQueueOps condCtr = (NumQueueOps) stmt.getCond().accept(this);
        result.add(condCtr);
        NumQueueOps bodyCtr = (NumQueueOps) stmt.getBody().accept(this);
        // Yousun Ko: not resolvable, if push/pop are invoked in loop-body 
        if (bodyCtr.numPop != 0){
            bodyCtr.inResolved = false;
        }
        if (bodyCtr.numPush != 0){
            bodyCtr.outResolved = false;
        }
        result.add(bodyCtr);

        branch = oldBranch;

        return result;
    }

    @Override
    public Object visitStreamSpec(StreamSpec spec) {
        return new NumQueueOps();
    }

    @Override
    public Object visitStreamType(StreamType type) {
        return new NumQueueOps();
    }

    @Override
    public Object visitOther(FENode node) {
        NumQueueOps result = new NumQueueOps();
        
        if (node instanceof ExprJavaConstructor) {
            return result;
        }
        
        if (node instanceof StmtIODecl) {
            StmtIODecl ap = (StmtIODecl) node;
            if (ap.getPeek() == null) {
                // by default, peek==pop
                if (ap.getPop() != null) {
                    NumQueueOps stmtCtr = (NumQueueOps) ap.getPop().accept(this);
                    result.add(stmtCtr);
                }
            } else {
                NumQueueOps stmtCtr = (NumQueueOps) ap.getPeek().accept(this);
                result.add(stmtCtr);
            }
            if (ap.getPop() != null){
                NumQueueOps stmtCtr = (NumQueueOps) ap.getPop().accept(this);
                result.add(stmtCtr);
            }
            if (ap.getPush() != null){
                NumQueueOps stmtCtr = (NumQueueOps) ap.getPush().accept(this);
                result.add(stmtCtr);
            }
            return result;
        }
        return result;
    }
}
