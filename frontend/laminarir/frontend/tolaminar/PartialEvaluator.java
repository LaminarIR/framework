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

/**
 * Traverse a front-end tree and do partial evaluation on exprs of integers. 
 */

public class PartialEvaluator implements FEVisitor {

    // list of constant variables 
    private ConstVarList constList;

    // set if the partial evaluation was successful
    private boolean resolved;

    public PartialEvaluator(ConstVarList constList){
        this.constList = constList;
        this.resolved = true;
    }

    public boolean isResolved(){
        return resolved;
    }

    @Override
    public Object visitExprArray (ExprArray exp) {
        if (exp.getComponent() instanceof ExprVar) {
            // try to evaluate dim offsets
            String name = ((ExprVar)exp.getComponent()).getName();
            String dim = "";
            int numDims = 0;
            Expression expr = (Expression)exp;
            while (expr instanceof ExprArray){
                int intDim= (int) ((ExprArray)expr).getOffset().accept(this);
                if (!resolved){
                    return 0;
                }
                dim = "["+Integer.toString(intDim)+"]"+dim;
                expr = ((ExprArray)expr).getBase();
                numDims++;
            }
            // dim offsets are all evaluated
            for(int i = 0; i < constList.size(); i++){
                if (name.equals(constList.getName(i))){
                    // do not evaluate when the array is not int
                    Type type = constList.getType(i);
                    if(numDims != ((TypeArray)type).getDims()){
                        resolved = false;
                        return 0;
                    }
                    while (type instanceof TypeArray){
                        type = ((TypeArray)type).getBase();
                    }
                    if (!(type instanceof TypePrimitive
                        && (((TypePrimitive)type).getType()) 
                            == TypePrimitive.TYPE_INT)){
                        resolved = false;
                        return 0;
                    }else{
                        return Integer.parseInt(constList.getVal(name+dim));
                    }
                }
            }
        }
        
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprArrayInit(ExprArrayInit exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprBinary(ExprBinary exp) {
        int lhs = (int) exp.getLeft().accept(this);
        int rhs = (int) exp.getRight().accept(this);

        switch (exp.getOp()) {
            case ExprBinary.BINOP_ADD:
                return lhs + rhs;
            case ExprBinary.BINOP_SUB:
                return lhs - rhs;
            case ExprBinary.BINOP_MUL:
                return lhs * rhs;
            case ExprBinary.BINOP_DIV:
                if (rhs == 0){
                    resolved = false;
                    return 0;
                }else{
                    return lhs / rhs;
                }
            case ExprBinary.BINOP_MOD:
                if (rhs == 0){
                    resolved = false;
                    return 0;
                }else{
                    return lhs % rhs;
                }

            case ExprBinary.BINOP_BAND:
                return lhs & rhs;
            case ExprBinary.BINOP_BOR:
                return lhs | rhs;
            case ExprBinary.BINOP_BXOR:
                return lhs ^ rhs;
            case ExprBinary.BINOP_LSHIFT:
                return lhs << rhs;
            case ExprBinary.BINOP_RSHIFT:
                return lhs >> rhs;
            case ExprBinary.BINOP_AND:
            case ExprBinary.BINOP_OR:
            case ExprBinary.BINOP_EQ:
            case ExprBinary.BINOP_NEQ:
            case ExprBinary.BINOP_LT:
            case ExprBinary.BINOP_LE:
            case ExprBinary.BINOP_GT:
            case ExprBinary.BINOP_GE:
            default:
                resolved = false;
                return 0;
        }
    }


    @Override
    public Object visitExprComplex(ExprComplex exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprComposite(ExprComposite exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprConstBoolean(ExprConstBoolean exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprConstChar(ExprConstChar exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprConstFloat(ExprConstFloat exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprConstInt(ExprConstInt exp) {
        return exp.getVal();
    }

    @Override
    public Object visitExprConstStr(ExprConstStr exp) {
        resolved = false;
        return 0;
    }   

    @Override
    public Object visitExprDynamicToken(ExprDynamicToken exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprField(ExprField exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprFunCall(ExprFunCall exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprHelperCall(ExprHelperCall exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprIter(ExprIter exprIter) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprPeek(ExprPeek exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprPop(ExprPop exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprRange(ExprRange exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprTernary(ExprTernary exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprTypeCast(ExprTypeCast exp) {
        resolved = false;
        return 0;
    }

    @Override
    public Object visitExprUnary(ExprUnary exp) {
        int val = (int) exp.getExpr().accept(this);
        switch (exp.getOp()) {
            case ExprUnary.UNOP_NOT:
                resolved = false;
                return 0;
            case ExprUnary.UNOP_NEG:
                return val*(-1);
            case ExprUnary.UNOP_PREINC: // pre-increment
            case ExprUnary.UNOP_POSTINC:// post-increment
                return val+1;
            case ExprUnary.UNOP_PREDEC: // pre-decrement
            case ExprUnary.UNOP_POSTDEC:// post-decrement
                return val-1;
            case ExprUnary.UNOP_COMPLEMENT:
                resolved = false;
                return 0;
            default:
                assert false : exp;
                resolved = false;
                return 0;
        }
    }

    @Override
    // Yousun Ko: search for the const param value when var expr found
    public Object visitExprVar(ExprVar exp) {
        String name = exp.getName();
        if (exp.isPropagatable()){
            for(int i = 0; i < constList.size(); i++){
                String constName = constList.getName(i);
                Type constType = constList.getType(i);
                if (constName.equals(name)
                    && constType instanceof TypePrimitive
                    && (((TypePrimitive)constType).getType()) 
                        == TypePrimitive.TYPE_INT){
                    return Integer.parseInt(constList.getVal(i));
                }
            }
        }
        // Yousun Ko: no const value is found for the var expr
        resolved = false;
        return 0;
    }

    @Override
    public Object visitFieldDecl(FieldDecl field) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitFunction(Function func) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitFuncWork(FuncWork func) {
        // Yousun Ko: this shouldn't be reached.
        return visitFunction(func);
    }

    @Override
    public Object visitProgram(Program prog) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitSCAnon(SCAnon creator) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitSCSimple(SCSimple creator) {   
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitSJDuplicate(SJDuplicate sj) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitSJRoundRobin(SJRoundRobin sj) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitSJWeightedRR(SJWeightedRR sj) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtAdd(StmtAdd stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtAssign(StmtAssign stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtBlock(StmtBlock stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtBody(StmtBody stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtBreak(StmtBreak stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtContinue(StmtContinue stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtDoWhile(StmtDoWhile stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtEmpty(StmtEmpty stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtEnqueue(StmtEnqueue stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtExpr(StmtExpr stmt) {
        int result = (int) stmt.getExpression().accept(this);
        return result;
    }

    @Override
    public Object visitStmtFor(StmtFor stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtIfThen(StmtIfThen stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtJoin(StmtJoin stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtLoop(StmtLoop stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtPush(StmtPush stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtReturn(StmtReturn stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtSendMessage(StmtSendMessage stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtHelperCall(StmtHelperCall stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtSplit(StmtSplit stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtVarDecl(StmtVarDecl stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStmtWhile(StmtWhile stmt) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStreamSpec(StreamSpec spec) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    public Object visitTypeHelper(TypeHelper th) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitStreamType(StreamType type) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }

    @Override
    public Object visitOther(FENode node) {
        // Yousun Ko: this shouldn't be reached.
        resolved = false;
        return 0;
    }
}
