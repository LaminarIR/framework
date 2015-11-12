/* ---------------------------------------------------------------------------
 * LaminarIR 
 *
 * This file is distributed under the MIT Open Source License.
 * See LICENSE.LaminarIR for details. 
 * ---------------------------------------------------------------------------
 */

package laminarir.frontend.nodes;

/**
 * Stores number of queue operations given by QueueSimulator
 */
public class NumQueueOps {

    public int numPop;
    public int numPush;
    public int numPeek;
    public boolean inResolved;
    public boolean outResolved;

    public static final int RESOLVED = 1;
    public static final int IN_NOT_RESOLVED = 2;
    public static final int OUT_NOT_RESOLVED = 3;
    public static final int BOTH_NOT_RESOLVED = 4;

    public NumQueueOps(){
        this.numPop = 0;
        this.numPush = 0;
        this.numPeek = 0;
        this.inResolved = true;
        this.outResolved = true;
    }

    public void add(NumQueueOps qOps) {
        this.numPop  = numPop + qOps.numPop;
        this.numPush = numPush+ qOps.numPush;
        this.numPeek = numPeek+ qOps.numPeek;

        if (!qOps.inResolved) {
            this.inResolved = false;
        }
        if (!qOps.outResolved) {
            this.outResolved = false;
        }
    }

    // examine whether push and pop positions are resolvable.
    public int examines(NumQueueOps inNumQ){
        boolean inResolved=true;
        boolean outResolved=true;
        if ((numPop != inNumQ.numPop) || 
                !(inResolved && inNumQ.inResolved)){
            inResolved = false;
        }

        if ((numPush != inNumQ.numPush) || 
                !(outResolved && inNumQ.outResolved)){
            outResolved = false;
        }

        if (!inResolved && !outResolved){
            return BOTH_NOT_RESOLVED;
        }
        if (!inResolved){
            return IN_NOT_RESOLVED;
        }
        if (!outResolved){
            return OUT_NOT_RESOLVED;
        }
        return RESOLVED;
    }
 

    public boolean equals(NumQueueOps qOps) {
        if (this.numPop != qOps.numPop){
            return false;
        }
        if (this.numPush != qOps.numPush){
            return false;
        }
        if (this.inResolved != qOps.inResolved){
            return false;    
        }
        if (this.outResolved != qOps.outResolved){
            return false;    
        }
        return true;
    }

    public String toString(){
       String result="";
       result += "+ numPop  : "+numPop+"\n";
       result += "+ numPush : "+numPush+"\n";
       result += "+ numPeek : "+numPeek+"\n";
       result += "+ inResolved: "+inResolved+"\n";
       result += "+ outResolved: "+outResolved+"\n";
       return result;
    }
}


