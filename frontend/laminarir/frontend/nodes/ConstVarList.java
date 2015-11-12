/* ---------------------------------------------------------------------------
 * LaminarIR 
 *
 * This file is distributed under the MIT Open Source License.
 * See LICENSE.LaminarIR for details. 
 * ---------------------------------------------------------------------------
 */

package laminarir.frontend.nodes;

import java.util.ArrayList;
import java.util.List;
import java.util.regex.Matcher;
import java.util.regex.Pattern;

/**
 * Container class containing constant variables.
 */
public class ConstVarList
{
    // Yousun Ko: following fields are containers for instances
    private List<Type> types;
    private List<String> names;
    private List<String> vals;
    
    public ConstVarList()
    {
        this.types = new ArrayList<Type>();
        this.names = new ArrayList<String>();
        this.vals = new ArrayList<String>();
    }

    public ConstVarList(ConstVarList cv){
        this(cv.getTypes(), cv.getNames(), cv.getVals());
    }
    
    public ConstVarList(List<Type> initTypes, List<String> initNames, List<String> initVals) {
        if (!(initTypes.size() == initNames.size() 
              && initTypes.size() == initVals.size())){
            System.err.println("Error: given list are not same in length."); 
        }

        this.types = new ArrayList<Type>(initTypes);
        this.names = new ArrayList<String>(initNames);
        this.vals = new ArrayList<String>(initVals);
    }

    public void add(Type type, String name, String val) {
        boolean found = false;
        int numVars = size();

        for(int i = 0; i < numVars; i++){
            if (names.get(i).equals(name)){
                found = true;
                types.set(i,type);
                names.set(i,name);
                vals.set(i,val);
            }
        }

        if(!found){
            types.add(type);
            names.add(name);
            vals.add(val);
        }
    }

    public int size(){
        if (!(types.size() == names.size() 
              && types.size() == vals.size())){
            System.err.println("Error: structure of the constant var list is spoiled."); 
        }
        return types.size();
    }
    
    public List<Type> getTypes() {
        return types;
    }

    public Type getType(int idx) {
        assert idx < size(): "Index out of bound: "+idx;
        return types.get(idx);
    }

    public List<String> getNames() {
        return names;
    }

    public String getName(int idx) {
        assert idx < size(): "Index out of bound: "+idx;
        return names.get(idx);
    }
    
    public List<String> getVals(){
        return vals;
    }

    public String getVal(int idx) {
        assert idx < size(): "Index out of bound: "+idx;
        return vals.get(idx);
    }

    public String getVal(String name){
        String dims = null;
        String result = null;
        
        name = name.replaceAll("\\s","");   // no white space

        int dimPos = name.indexOf("[");
        int numVars = size();
        if (dimPos != -1){
            dims = name.substring(dimPos, name.length());
            name = name.substring(0, dimPos);
        }
        for (int i = 0; i < numVars; i++){
            if (name.equals(names.get(i))){
                result = vals.get(i);
            }
        }

        if (dims == null || result == null ){
            return result;
        }

        // if reaches here, the var to find is an array element,
        // and the array is found.

        // return null when non-numeric offsets are detected
        if (dims.matches("[A-Za-z_]")){
            return null;
        }

        Pattern patDim = Pattern.compile("\\d+");
        Matcher matDim = patDim.matcher(dims);

        Pattern patElem = Pattern.compile("[^,\\{\\}]+");

        String elem = null;
        int strIdx = 0;
        int dimLv = 0;
        int curOffset = 0;
        int curDimLv = -1;
        while(matDim.find()){
            int offset = Integer.parseInt(matDim.group());
            for(;; strIdx++){
                switch (result.charAt(strIdx)){
                    case '{':
                        curDimLv++;
                        break;
                    case '}':
                        curDimLv--;
                        break;
                    case ',':
                        if (curDimLv == dimLv)
                            curOffset++;
                        break;
                }
                
                if (curDimLv == dimLv
                    && curOffset == offset){
                    Matcher matNum = 
                        patElem.matcher(result.substring(strIdx+1,result.length()));
                    matNum.find();
                    elem = matNum.group();
                    elem = elem.replaceAll("\\s","");   // no white space
                    break;
                }
            }
            dimLv++;
            curOffset = 0;
        }
        return elem;
    }

    public void remove(String name){
        int numVars = size();

        for (int i = 0; i < numVars; i++){
            if (name.equals(names.get(i))){
                types.remove(i);
                names.remove(i);
                vals.remove(i);
                break;
            }
        }
    }

    public void dump(){
        int numVars = size();

        for(int i = 0; i < numVars; i++) {
            System.out.println(names.get(i)+"="+vals.get(i));
        }
    }
}
