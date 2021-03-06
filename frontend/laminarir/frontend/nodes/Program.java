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

package laminarir.frontend.nodes;

import java.util.List;

/**
 * An entire StreamIt program.  This includes all of the program's
 * declared streams and structure types.  It consequently has Lists of
 * streams (as {@link streamit.frontend.nodes.StreamSpec} objects) and
 * of structures (as {@link streamit.frontend.nodes.TypeStruct} objects).
 *
 * @author  David Maze &lt;dmaze@cag.lcs.mit.edu&gt;
 * @version $Id: Program.java,v 1.5 2006-09-25 13:54:54 dimock Exp $
 */
public class Program extends FENode
{
    private List<StreamSpec> streams;
    private List<TypeStruct> structs;
    private List<TypeHelper> helpers;

    // Yousun Ko: variable name for the sum and seed
    private String KEY_SUM;
    private String KEY_SEED;

    /** Creates a new StreamIt program, given lists of streams and
     * structures. */
    public Program(FEContext context, List<StreamSpec> streams, List<TypeStruct> structs, List<TypeHelper> helpers)
    {
        super(context);
        this.streams = streams;
        this.structs = structs;
        this.helpers = helpers;

        this.KEY_SUM = "sum";
        this.KEY_SEED = "seed";
    }
    
    /** Returns the list of streams declared in this. */
    public List<StreamSpec> getStreams()
    {
        return streams;
    }
    
    /** Returns the list of structures declared in this. */
    public List<TypeStruct> getStructs()
    {
        return structs;
    }

    /** Returns the list of helpers declared in this. */
    public List<TypeHelper> getHelpers()
    {
        return helpers;
    }
    
    // Yousun Ko: interfaces
    public String getKeySum(){
        return KEY_SUM;
    }

    public String getKeySeed(){
        return KEY_SEED;
    }

    public void setKeySum(String KEY_SUM){
        this.KEY_SUM=KEY_SUM;
    }

    public void setKeySeed(String KEY_SEED){
        this.KEY_SEED=KEY_SEED;
    }

    /** Accepts a front-end visitor. */
	public Object accept(FEVisitor v)
    {
        return v.visitProgram(this);
    }
}

