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

package streamit.scheduler2.print;

import java.io.File;
import java.io.FileOutputStream;
import java.io.DataOutputStream;
import streamit.library.Filter;
import streamit.scheduler2.iriter.FilterIter;
import streamit.scheduler2.iriter.PipelineIter;
import streamit.scheduler2.iriter.SplitJoinIter;
import streamit.scheduler2.iriter.FeedbackLoopIter;
import streamit.scheduler2.iriter.IteratorBase;
import streamit.scheduler2.iriter.SplitterIter;
import streamit.scheduler2.iriter.JoinerIter;
import streamit.scheduler2.iriter.Iterator;

// Yousun Ko
import java.util.List;
import streamit.library.Stream;
import streamit.library.FeedbackLoop;

public class PrintGraph extends streamit.misc.AssertedClass
{
    /**
     * The maximum number of phases to show (for split/join).
     */
    private static final int MAX_PHASES_TO_SHOW = 16;
    /**
     * Whether or not we are printing I/O rates.
     */

    public void printProgram(Iterator iter)
    {
        // commented out following assertion A.D.
        // it is false for a number of programs since the outermost
        // level is NOT guaranteed to be a pipeine and we have examples
        // of outer levels that are SplitJoins or FeedbackLoops.
        //assert iter.isPipeline() != null;

        File outputFile;
        FileOutputStream fileOutputStream;
        DataOutputStream outputStream;

        // print a graph both with and without I/O rates
        String filename = getName(iter) + ".edges.out";
        try {
            outputFile = new File(filename);
            fileOutputStream = new FileOutputStream(outputFile);
            outputStream = new DataOutputStream(fileOutputStream);

            printStream(iter, outputStream);
        } catch (Throwable e) {
            ERROR(e);
        }
    }

    void printStream(Iterator iter, DataOutputStream outputStream)
    {
        try {
            if (iter.isPipeline() != null) {
                printPipe(iter.isPipeline(), outputStream);
            }
            if (iter.isSplitJoin() != null) {
                printSJ(iter.isSplitJoin(), outputStream);
            }
            if (iter.isFeedbackLoop() != null) {
                printFL(iter.isFeedbackLoop(), outputStream);
            }
        } catch (Throwable e) {
            ERROR(e);
        }
    }

    String getName(Object obj)
    {
        String name = obj.toString();
        // if there is a dollar sign in the returned name, then this
        // indicates an anonymous class in the library.  We don't want
        // to label the anonymous nodes, so just return empty in this
        // case.
        if (name.indexOf("$")>-1) {
            name = "";
        } else {
            name = name
                .replace('@', '_')
                .replace('.', '_')
                .replace(' ', '_')
                .replace('=', '_')
                .replace(',', '_');
        }

        return name;
    }

    String getName(IteratorBase iter)
    {
        return getName(iter.getObject());
    }

    String getUniqueName(IteratorBase iter)
    {
        String name = getName(iter.getObject()) + "_" + iter.getObject().hashCode();
        Object obj = iter.getObject();
        // add the I/O rates
        if (obj instanceof Filter) {
            Filter filter = (Filter)obj;

            // add steady rates
            int pop = filter.getSteadyPopSummary();
            int peek = filter.getSteadyPeekSummary();
            int push = filter.getSteadyPushSummary();

            name += ("@" + 
                     "pop_" + pop + "_" +
                     (pop==peek ? "" : "peek_" + peek + "_") +
                     "push_" + push);
        }
        return name;
    }

    String getNameForSplit(SplitterIter iter) {
        if (iter.getSplitterNumWork()>1) {
            StringBuffer result = new StringBuffer("splitter with " + iter.getSplitterNumWork() + " phases:\\n");
            int numPhasesToShow = Math.min(MAX_PHASES_TO_SHOW, iter.getSplitterNumWork());
            for (int j=0; j<numPhasesToShow; j++) {
                int[] weights = iter.getSplitPushWeights(j);
                result.append("p" + j + ": (");
                for (int i=0; i<weights.length; i++) { 
                    result.append(weights[i]);
                    if (i!=weights.length-1) {
                        result.append(", ");
                    } else {
                        result.append(")");
                        if (j!=numPhasesToShow-1) {
                            result.append("\\n");
                        } else if (numPhasesToShow<iter.getSplitterNumWork()) {
                            result.append("\\n(last " + (iter.getSplitterNumWork()-numPhasesToShow) + " phases not shown)\\n");
                        }
                    }
                }
            }
            return result.toString();
        } else {
            // detect duplicate if pop is unary
            if (iter.getSplitPop(0)==1) {
                // duplicate splitter
                return "duplicate";
            } else if (iter.getSplitPop(0)==0) {
                // null splitter
                return "roundrobin(0)";
            } else {
                // roundrobin splitter... enumerate weights
                int weights[] = iter.getSplitPushWeights(0);
                StringBuffer result = new StringBuffer("roundrobin(");
                for (int i=0; i<weights.length; i++) { 
                    result.append(weights[i]);
                    if (i!=weights.length-1) {
                        result.append(", ");
                    } else {
                        result.append(")");
                    }
                }
                return result.toString();
            }
        }
    }

    String getNameForJoin(JoinerIter iter) {
        if (iter.getJoinerNumWork()>1) {
            StringBuffer result = new StringBuffer("joiner with " + iter.getJoinerNumWork() + " phases:\\n");
            int numPhasesToShow = Math.min(MAX_PHASES_TO_SHOW, iter.getJoinerNumWork());
            for (int j=0; j<numPhasesToShow; j++) {
                int[] weights = iter.getJoinPopWeights(j);
                result.append("p" + j + ": (");
                for (int i=0; i<weights.length; i++) { 
                    result.append(weights[i]);
                    if (i!=weights.length-1) {
                        result.append(", ");
                    } else {
                        result.append(")");
                        if (j!=numPhasesToShow-1) {
                            result.append("\\n");
                        } else if (numPhasesToShow<iter.getJoinerNumWork()) {
                            result.append("\\n(last " + (iter.getJoinerNumWork()-numPhasesToShow) + " phases not shown)\\n");
                        }
                    }
                }
            }
            return result.toString();
        } else if (iter.getJoinPush(0)==0) {
            // null joiner
            return "roundrobin(0)";
        } else {
            int weights[] = iter.getJoinPopWeights(0);
            StringBuffer result = new StringBuffer("roundrobin(");
            for (int i=0; i<weights.length; i++) { 
                result.append(weights[i]);
                if (i!=weights.length-1) {
                    result.append(", ");
                } else {
                    result.append(")");
                }
            }
            return result.toString();
        }
    }

    // Yousun Ko: return pop rate of a splitter
    String getPopRateForSplit(SplitterIter iter) {
        assert iter.getSplitterNumWork()<=1;
       
        // detect duplicate if pop is unary
        if (iter.getSplitPop(0)==1) {   
          return "(1)";
        }else if (iter.getSplitPop(0)==0) {
          // null splitter
          return "(0)";
        } else {
          int weights[] = iter.getSplitPushWeights(0);
          int sum_weights = 0;
          for(int i=0; i < weights.length; i++){
            sum_weights+=weights[i];
          }
          return "("+sum_weights+")";
        }
    }

    // Yousun Ko: return 'idx'th push rate of a splitter 
    String getPushRateForSplit(SplitterIter iter, int idx) {
        assert iter.getSplitterNumWork()<=1;

        // detect duplicate if pop is unary
        if (iter.getSplitPop(0)==1) {
          // duplicate splitter
          return "(1)";
        } else if (iter.getSplitPop(0)==0) {
          // null splitter
          return "(0)";
        } else {
          // roundrobin splitter... enumerate weights
          int weights[] = iter.getSplitPushWeights(0);
          return "("+weights[idx]+")";
        }
    }

    // Yousun Ko: return 'idx'th pop rate of a joiner
    String getPopRateForJoin(JoinerIter iter, int idx) {
        assert iter.getJoinerNumWork()<=1;
        if (iter.getJoinPush(0)==0) {
            // null joiner
            return "(0)";
        } else {
            int weights[] = iter.getJoinPopWeights(0);
            return "("+weights[idx]+")";
        }
    }

    // Yousun Ko: return push rate of a joiner
    String getPushRateForJoin(JoinerIter iter) {
        assert iter.getJoinerNumWork()<=1;
        if (iter.getJoinPush(0)==0) {
            // null joiner
            return "(0)";
        } else {
            int weights[] = iter.getJoinPopWeights(0);
            int sum_weights = 0; 
            for (int i=0; i<weights.length; i++) { 
                sum_weights+=weights[i];
            }
            return "("+sum_weights+")";
        }
    }

    void printPipe(PipelineIter pipeline, DataOutputStream outputStream)
    {
        try {
            String lastPrintedName = null;
            String lastPrintedRate = null;
            Iterator lastPrintedIter = null;

            // Walk through each of the elements in the pipeline.
            for (int nChild = 0; nChild < pipeline.getNumChildren(); nChild++) {
                Iterator child = pipeline.getChild(nChild);
                assert child != null;

                String topChildName = getUniqueTopStreamName(child);
                String bottomChildName = getUniqueBottomStreamName(child);

                // Yousun Ko: additional information for splitjoin edges
                String topChildRate = "";
                String bottomChildRate = "";

                // Yousun Ko: Iterator to search for the real(Top/Bottom)Child
                Iterator realTopChild = child;
                Iterator realBottomChild = child;
                while (realTopChild.isPipeline() != null) {
                    realTopChild = realTopChild.isPipeline().getChild(0);
                }
                while(realBottomChild.isPipeline() != null){
                    realBottomChild = realBottomChild.isPipeline().getChild(
                            realBottomChild.isPipeline().getNumChildren()-1);
                }

                // Yousun Ko: topChild is a splitter
                if (realTopChild.isSplitJoin() != null){
                  topChildRate = getPopRateForSplit(realTopChild.isSplitJoin());
                }
                // Yousun Ko: bottomChild is joiner
                if (realBottomChild.isSplitJoin() != null){
                  bottomChildRate = getPushRateForJoin(realBottomChild.isSplitJoin());
                }
                // Yousun Ko: topChild is joiner
                if (realTopChild.isFeedbackLoop() != null){
                  topChildRate = getPopRateForJoin(realTopChild.isFeedbackLoop(),0);
                }
                // Yousun Ko: bottomChild is splitter
                if (realBottomChild.isFeedbackLoop() != null){
                  bottomChildRate = getPushRateForSplit(realBottomChild.isFeedbackLoop(),0);
                }

                if ((lastPrintedName != null) && (topChildName !=null)){
                  String edgeType = ((Stream)child.getObject()).getIncomingType();
                  printEdge(edgeType, 
                            lastPrintedName+lastPrintedRate, topChildName+topChildRate, 
                            outputStream);
                }

                // Yousun Ko: in-node traversal
                printStream(pipeline.getChild(nChild), outputStream);

                // Update the known edges.
                lastPrintedName = bottomChildName;
                lastPrintedRate = bottomChildRate;
                lastPrintedIter = child;
            }
        } catch (Throwable e) {
            ERROR(e);
        }
    }

    void printSJ(SplitJoinIter sj, DataOutputStream outputStream)
    {
        try {
            String splitName =
                getUniqueTopStreamName(sj.getUnspecializedIter());
            String joinName =
                getUniqueBottomStreamName(sj.getUnspecializedIter());

            // ...and walk through the body.
            int nChild;
            for (nChild = 0; nChild < sj.getNumChildren(); nChild++) {
                Iterator child = sj.getChild(nChild);

                // Yousun Ko: additional information for splitjoin edges
                String topChildName = getUniqueTopStreamName(child);
                String bottomChildName = getUniqueBottomStreamName(child);
                String topChildRate = "";
                String bottomChildRate = "";

                // Yousun Ko: Iterator to search for the real(Top/Bottom)Child
                Iterator realTopChild = child;
                Iterator realBottomChild = child;
                while(realTopChild.isPipeline() != null) {
                    realTopChild=realTopChild.isPipeline().getChild(0);
                }
                while(realBottomChild.isPipeline() != null){
                    realBottomChild=realBottomChild.isPipeline().getChild(
                            realBottomChild.isPipeline().getNumChildren()-1);
                }

                // Yousun Ko: topChild is splitter
                if (realTopChild.isSplitJoin() != null){
                    topChildRate = getPopRateForSplit(realTopChild.isSplitJoin());
                }
                // Yousun Ko: bottomChild is joiner
                if (realBottomChild.isSplitJoin() != null){
                    bottomChildRate = getPushRateForJoin(realBottomChild.isSplitJoin());
                }
                // Yousun Ko: topChild is joiner
                if (realTopChild.isFeedbackLoop() != null){
                    topChildRate = getPopRateForJoin(realTopChild.isFeedbackLoop(),0);
                }
                // Yousun Ko: bottomChild is splitter
                if (realBottomChild.isFeedbackLoop() != null){
                    bottomChildRate = getPushRateForSplit(realBottomChild.isFeedbackLoop(),0);
                }

                String inType = ((Stream)child.getObject()).getIncomingType();
                String outType = ((Stream)child.getObject()).getOutgoingType();

                // Yousun Ko: print split->child[i]
                if (inType != null){
                    printEdge(inType,
                        splitName + getPushRateForSplit(sj,nChild),
                        getUniqueTopStreamName(child)+topChildRate,
                        outputStream);
                }

                // Yousun Ko: in-node traversal, print child[i]
                printStream(child, outputStream);

                // Yousun Ko: print child[i]->join
                if (outType != null){
                    printEdge(outType,
                        getUniqueBottomStreamName(child)+bottomChildRate,
                        joinName + getPopRateForJoin(sj, nChild),
                        outputStream);
                }
            }

        } catch (Throwable e) {
            ERROR(e);
        }
    }

    void printFL(FeedbackLoopIter flIter, DataOutputStream outputStream)
    {
        try {
            // Yousun Ko: Iterator to search for the real(Top/Bottom)Child
            Iterator bodyIter = flIter.getBodyChild();
            Iterator loopIter = flIter.getLoopChild();
            Iterator jsIter = flIter.getUnspecializedIter();
            FeedbackLoop fl = (FeedbackLoop)flIter.getUnspecializedIter().getObject();

            // Yousun Ko: traverse body and loop 
            printStream(flIter.getBodyChild(), outputStream);
            printStream(flIter.getLoopChild(), outputStream);

            String edgeType;
            // Yousun Ko: joiner
            edgeType = ((Stream)bodyIter.getObject()).getIncomingType();
            printEdge(edgeType,
                    getUniqueTopStreamName(jsIter)+getPushRateForJoin(flIter),
                    getUniqueTopStreamName(bodyIter),
                    outputStream);

            // Yousun Ko: splitter
            edgeType = ((Stream)bodyIter.getObject()).getOutgoingType();
            printEdge(edgeType,
                    getUniqueBottomStreamName(bodyIter),
                    getUniqueBottomStreamName(jsIter)+getPopRateForSplit(flIter),
                    outputStream);

            // Yousun Ko: splitter
            edgeType = ((Stream)loopIter.getObject()).getIncomingType();
            printEdge(edgeType,
                    getUniqueBottomStreamName(jsIter)+getPushRateForSplit(flIter,1),
                    getUniqueTopStreamName(loopIter),
                    outputStream);

            edgeType = ((Stream)loopIter.getObject()).getOutgoingType();
            String enqueueStr = "=";
            
            // Yousun Ko: append queue values.
            List<Object> enqueue = fl.getEnqueued();
            assert enqueue != null;
            enqueueStr += "{";
            for (int i = 0; i < fl.getEnqueued().size(); i++) {
                if (i != 0) {
                    enqueueStr += ",";
                }
                enqueueStr += "{";
                enqueueStr += enqueue.get(i);
                enqueueStr += "}";
            }
            enqueueStr += "}";

            // Yousun Ko: joiner
            printEdge(edgeType,
                    getUniqueBottomStreamName(loopIter),
                    getUniqueTopStreamName(jsIter)+getPopRateForJoin(flIter,1)+enqueueStr,
                    outputStream);
        } catch (Throwable e) {
            ERROR(e);
        }
    }

    void printEdge(String type,
                   String from,
                   String to,
                   DataOutputStream outputStream)
    {
        if (from == null || to == null)
            return;
        try {
            outputStream.writeBytes(type + " " + from + " -> " + to + "\n");
        } catch (Throwable e) {
            ERROR(e);
        }
    }

    String getUniqueTopStreamName(Iterator iter)
    {
        if (iter.isFilter() != null) {
            return getUniqueName(iter);
        }
        if (iter.isPipeline() != null) {
            return getUniqueTopStreamName(iter.isPipeline().getChild(0));
        }
        if (iter.isSplitJoin() != null) {
            return getUniqueName(iter) + "_split";
        }
        if (iter.isFeedbackLoop() != null) {
            return getUniqueName(iter) + "_join";
        }

        assert false;
        return null;
    }

    String getUniqueBottomStreamName(Iterator iter)
    {
        if (iter.isFilter() != null) {
            return getUniqueName(iter);
        }
        if (iter.isPipeline() != null) {
            return getUniqueBottomStreamName(
                    iter.isPipeline().getChild(
                        iter.isPipeline().getNumChildren() - 1));
        }
        if (iter.isSplitJoin() != null) {
            return getUniqueName(iter) + "_join";
        }
        if (iter.isFeedbackLoop() != null) {
            return getUniqueName(iter) + "_split";
        }

        assert false;
        return null;
    }

}
