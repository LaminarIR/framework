/* ---------------------------------------------------------------------------
 * LaminarIR 
 *
 * This file is distributed under the MIT Open Source License.
 * See LICENSE.LaminarIR for details.
 * ---------------------------------------------------------------------------
 */

package laminarir.frontend.passes;

import laminarir.frontend.nodes.*;

import java.util.Iterator;
import java.util.Map;

// Yousun Ko
import java.util.regex.Matcher;
import java.util.regex.Pattern;
import java.util.ArrayList;
import java.util.List;

/**
 * Front-end visitor pass that renames keywords when there is
 * conflict with local var names.
 */

public class RenameKeywords extends FEReplacer
{

    // Yousun Ko: maintains program keywords
    private List<String> progKeyPats;
    private List<Integer> progKeyPrefixes;

    // Yousun Ko: maintains streamspec keywords
    private List<String> specKeyPats;
    private List<Integer> specKeyPrefixes;

    public RenameKeywords()
    {
        progKeyPats = new ArrayList<String>();
        progKeyPrefixes = new ArrayList<Integer>();

        specKeyPats = new ArrayList<String>();
        specKeyPrefixes = new ArrayList<Integer>();
    }

    private void examine(String varName){
        // Yousun Ko: examine program keywords
        Pattern uPat = Pattern.compile("_");
        for(int i = 0; i < progKeyPats.size(); i++){
            Pattern pat = Pattern.compile(progKeyPats.get(i));
            Matcher mat = pat.matcher(varName);
            if (mat.find()){
                // Yousun Ko: varName is "_"* and the keyword
                if (mat.group(0).length() == varName.length()){
                    Matcher uMat = uPat.matcher(varName);
                    int counter = 1;
                    while (uMat.find()){
                        counter++;
                    }
                    if (counter > progKeyPrefixes.get(i)){
                        progKeyPrefixes.set(i, counter);
                    }
                }
            }
        }

        // Yousun Ko: examine streamspec keywords
        for(int i = 0; i < specKeyPats.size(); i++){
            Pattern pat = Pattern.compile(specKeyPats.get(i));
            Matcher mat = pat.matcher(varName);
            if (mat.find()){
                // Yousun Ko: varName is "_"* and the keyword
                if (mat.group(0).length() == varName.length()){
                    Matcher uMat = uPat.matcher(varName);
                    int counter = 1;
                    while (uMat.find()){
                        counter++;
                    }
                    if (counter > specKeyPrefixes.get(i)){
                        specKeyPrefixes.set(i, counter);
                    }
                }
            }
        }
    }

    /**
     * Top level visitor, sets up some useful info.
     * 
     */
    @Override
	public Object visitProgram(Program prog) {

        // Yousun Ko: set program keywords
        // prog.sum
        progKeyPats.add("(_*)"+prog.getKeySum());
        progKeyPrefixes.add(0);
        // prog.seed
        progKeyPats.add("(_*)"+prog.getKeySeed());
        progKeyPrefixes.add(0);



        // Yousun Ko: set initial spec keywords
        // Yousun Ko: initial spec keyword patterns by StreamSpec constructor
        List<String> initSpecKeyPats = new ArrayList<String>();
        
        // Yousun Ko: initial spec keyword prefixes by traversing global ids
        List<Integer> initSpecKeyPrefixes = new ArrayList<Integer>();

        // Yousun Ko: global stream spec init val by global func names and vars
        StreamSpec dummy = prog.getStreams().get(0);

        // spec.x
        initSpecKeyPats.add("(_*)"+dummy.getInTokenPrefix()+"(\\d+)");
        initSpecKeyPrefixes.add(0);
        // spec.in_array
        initSpecKeyPats.add("(_*)"+dummy.getInArrayName());
        initSpecKeyPrefixes.add(0);
        // spec.in_array_ctr
        initSpecKeyPats.add("(_*)"+dummy.getInArrayCtr());
        initSpecKeyPrefixes.add(0);

        // spec.y
        initSpecKeyPats.add("(_*)"+dummy.getOutTokenPrefix()+"(\\d+)");
        initSpecKeyPrefixes.add(0);
        // spec.out_array
        initSpecKeyPats.add("(_*)"+dummy.getOutArrayName());
        initSpecKeyPrefixes.add(0);
        // spec.out_array_ctr
        initSpecKeyPats.add("(_*)"+dummy.getOutArrayCtr());
        initSpecKeyPrefixes.add(0);

        // prepare examination
        specKeyPats = new ArrayList<String>(initSpecKeyPats);
        specKeyPrefixes = new ArrayList<Integer>(initSpecKeyPrefixes);

        for (Iterator iter = prog.getHelpers().iterator(); iter.hasNext();) {
            TypeHelper helper = (TypeHelper) iter.next();
            examine(helper.getName());        
        }

        for (Iterator<StreamSpec> iter = prog.getStreams().iterator(); iter.hasNext();) {
            StreamSpec spec = iter.next();
            if (spec.getType() == StreamSpec.STREAM_GLOBAL){
                for (FieldDecl var : spec.getVars()) {
                    int num = var.getNumFields();
                    for (int y = 0; y < num; y++) {
                        examine(var.getName(y));
                    }
                }
                visitStreamSpec(spec);
            }
        }
        
        // Yousun Ko: global varNames and funcNames are examined. 
        initSpecKeyPrefixes = new ArrayList<Integer>(specKeyPrefixes);

        List<StreamSpec> newStreams = new ArrayList<StreamSpec>();

        // Yousun Ko: do actual examination
        for (Iterator<StreamSpec> iter = prog.getStreams().iterator(); iter.hasNext();) {
            StreamSpec spec = iter.next();

            // Yousun Ko: initialize pattern list
            specKeyPats = new ArrayList<String>(initSpecKeyPats);

            // Yousun Ko: load initial prefixes
            specKeyPrefixes = new ArrayList<Integer>(initSpecKeyPrefixes);

            visitStreamSpec(spec);
            
            // Yousun Ko: update spec keywords
            for(int i = 0; i < specKeyPrefixes.size(); i++){
                int numPrefixes = specKeyPrefixes.get(i);
                String prefix = "";
                String name = "";
                while (numPrefixes>0){
                    numPrefixes--;
                    prefix="_"+prefix;
                }
                switch(i){
                    case 0: // spec.x
                         name = spec.getInTokenPrefix();
                        spec.setInTokenPrefix(prefix+name);
                        break;
                    case 1: // spec.in_array
                        name = spec.getInArrayName();
                        spec.setInArrayName(prefix+name);
                        break;
                    case 2: // spec.in_array_ctr
                        name = spec.getInArrayCtr();
                        spec.setInArrayCtr(prefix+name);
                        break;
                    case 3: // spec.y
                        name = spec.getOutTokenPrefix();
                        spec.setOutTokenPrefix(prefix+name);
                        break;
                    case 4: // spec.out_array
                        name = spec.getOutArrayName();
                        spec.setOutArrayName(prefix+name);
                        break;
                    case 5: // spec.out_array_ctr
                        name = spec.getOutArrayCtr();
                        spec.setOutArrayCtr(prefix+name);
                        break;
                    default:
                        assert false: "Error: length of specKeyPrefixes is not 6.";
                }
            }
            newStreams.add(spec);
        }
       
        Program newProg = new Program(prog.getContext(), 
                                newStreams, 
                                prog.getStructs(), 
                                prog.getHelpers());

        // Yousun Ko: update prog keywords
        for(int i = 0; i < progKeyPrefixes.size(); i++){
            int numPrefixes = progKeyPrefixes.get(i);
            String prefix = "";
            String name = "";
            while (numPrefixes>0){
                numPrefixes--;
                prefix="_"+prefix;
            }

            switch(i){
                case 0: // prog.sum
                    name = prog.getKeySum();
                    newProg.setKeySum(prefix+name);
                    break;
                case 1: // prog.seed
                    name = prog.getKeySeed();
                    newProg.setKeySeed(prefix+name);
            }
        }
        return newProg;
    }


    @Override
	public Object visitFieldDecl(FieldDecl field)
    {
        for (int i = 0; i < field.getNumFields(); i++){
            // Yousun Ko
            examine(field.getName(i));
        }
        return super.visitFieldDecl(field);
    }

    @Override
	public Object visitFunction(Function func)
    {
        for (Iterator iter = func.getParams().iterator(); iter.hasNext(); )
        {
            Parameter param = (Parameter)iter.next();
            // Yousun Ko
            examine(param.getName());
        }
        return super.visitFunction(func);
    }
    
    @Override
	public Object visitStmtVarDecl(StmtVarDecl stmt)
    {
        for (int i = 0; i < stmt.getNumVars(); i++){
            // Yousun Ko
            examine(stmt.getName(i));
        }
        return super.visitStmtVarDecl(stmt);
    }

    @Override
	public Object visitStreamSpec(StreamSpec spec)
    {
        // register parameters
        for (Parameter param : spec.getParams())
        {
            // Yousun Ko
            examine(param.getName());
        }
        // register functions
        for (Function func : spec.getFuncs())
        {
            // Yousun Ko
            examine(func.getName());
        }
        return super.visitStreamSpec(spec);
    }
}
