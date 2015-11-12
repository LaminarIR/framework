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

package laminarir.frontend;

import java.io.*;
import java.util.List;
import java.util.Iterator;
import laminarir.frontend.nodes.*;
import laminarir.frontend.passes.*;
import laminarir.frontend.tolaminar.*;

/**
 * Convert StreamIt programs to LaminarIR code.  
 * This file is derived from streamit.frontend.ToJava.java and 
 * modfied for the LaminarIR Framework. This is the main
 * entry point for the StreamIt syntax converter.  Running it as
 * a standalone program reads the list of files provided on the
 * command line and produces equivalent LaminarIR code on standard
 * output or the file named in the <tt>--output</tt> command-line
 * parameter.
 *
 * Information of the original code, streamit.frontend.ToJava.java
 * @author  David Maze &lt;dmaze@cag.lcs.mit.edu&gt;
 * @version $Id: ToJava.java,v 1.79 2007-06-27 01:31:21 thies Exp $
 */
public class ToLaminarIR
{
    public void printUsage()
    {
        System.err.println(
           "laminarir.frontend.ToLaminarIR: StreamIt syntax translator to LaminarIR\n" +
           "Usage: java laminarir.frontend.ToLaminarIR [--output out.java] in.str ...\n" +
           "\n" +
           "Options:\n" +
           "  --help         Print this message\n" +
           "  --output file  Write output to file, not stdout\n" +
           // Yousun Ko
           "  --laminar <type>    Genenerate LaminarIR with <type>.\n" +
           "                      Options for <type>: direct, fifo\n" +
           "  --graph_file <file> Read <file> for stream graph of the program\n" +
           "  --const_file <file> Read <file> for constant values of actors\n" +
           "  --path_file <file>  Read <file> for list of I/O files required to run program\n" +
           "  --randomize         Randomize input data\n" +
           "  --sanity-check       Generate code that prints each item token reached the sink node" +
           "\n");
    }

    private boolean printHelp = false;
    private String outputFile = null;
    // Yousun Ko: store path to required files
    private String graphFile = null;
    private String constFile = null;
    private String pathFile = null;
    // Yousun Ko: store communication type, direct or fifo
    private String commType = null;

    private boolean randomize = false;
    private boolean sanitycheck = false;
    private List<String> inputFiles = new java.util.ArrayList<String>();

    public void doOptions(String[] args)
    {
        for (int i = 0; i < args.length; i++)
            {
                if (args[i].equals("--full"))
                    ; // Accept but ignore for compatibility
                else if (args[i].equals("--help"))
                    printHelp = true;
                else if (args[i].equals("--"))
                    {
                        // Add all of the remaining args as input files.
                        for (i++; i < args.length; i++)
                            inputFiles.add(args[i]);
                    }
                else if (args[i].equals("--laminar"))
                    commType = args[++i];
                else if (args[i].equals("--output"))
                    outputFile = args[++i];
                else if (args[i].equals("--graph_file"))
                    graphFile = args[++i];
                else if (args[i].equals("--const_file"))
                    constFile = args[++i];
                else if (args[i].equals("--path_file"))
                    pathFile = args[++i];
                else if (args[i].equals("--randomize"))
                    randomize = true;
                else if (args[i].equals("--sanity-check"))
                    sanitycheck = true;
                else
                    // Maybe check for unrecognized options.
                    inputFiles.add(args[i]);
            }
        if (!(commType.equals("direct") || commType.equals("fifo"))){
            System.err.println("Error: wrong communication type given, "+commType);
        }
    }

    /**
     * Generate a Program object that includes built-in structures
     * and streams with code, but no user code.
     *
     * @return a StreamIt program containing only built-in code
     */
    public static Program emptyProgram()
    {
        List streams = new java.util.ArrayList();
        List structs = new java.util.ArrayList();
        List helpers = new java.util.ArrayList();
        
        // Yousun Ko: no struct nor complex is supported

        return new Program(null, streams, structs, helpers);
    }

    /**
     * Read, parse, and combine all of the StreamIt code in a list of
     * files.  Reads each of the files in <code>inputFiles</code> in
     * turn and runs <code>laminarir.frontend.StreamItParserFE</code>
     * over it.  This produces a
     * <code>laminarir.frontend.nodes.Program</code> containing lists
     * of structures and streams; combine these into a single
     * <code>laminarir.frontend.nodes.Program</code> with all of the
     * structures and streams.
     *
     * @param inputFiles  list of strings naming the files to be read
     * @return a representation of the entire program, composed of the
     *          code in all of the input files
     * @throws java.io.IOException if an error occurs reading the input
     *         files
     * @throws antlr.RecognitionException if an error occurs parsing
     *         the input files; that is, if the code is syntactically
     *         incorrect
     * @throws antlr.TokenStreamException if an error occurs producing
     *         the input token stream
     */
    public static Program parseFiles(List<String> inputFiles)
        throws java.io.IOException,
               antlr.RecognitionException, 
               antlr.TokenStreamException
    {
        Program prog = emptyProgram();
        for (Iterator<String> iter = inputFiles.iterator(); iter.hasNext(); )
            {
                String fileName = iter.next();
                InputStream inStream = new FileInputStream(fileName);
                DataInputStream dis = new DataInputStream(inStream);
                StreamItLex lexer = new StreamItLex(dis);
                StreamItParserFE parser = new StreamItParserFE(lexer);
                parser.setFilename(fileName);
                Program pprog = parser.program();
                if (pprog != null)
                    {
                        List newStreams, newStructs, newHelpers;
                        newStreams = new java.util.ArrayList();
                        newStreams.addAll(prog.getStreams());
                        newStreams.addAll(pprog.getStreams());
                        newStructs = new java.util.ArrayList();
                        newStructs.addAll(prog.getStructs());
                        newStructs.addAll(pprog.getStructs());
                        newHelpers = new java.util.ArrayList();
                        newHelpers.addAll(prog.getHelpers());
                        newHelpers.addAll(pprog.getHelpers());
                        prog = new Program(null, newStreams, newStructs, newHelpers);
                    }
            }
        return prog;
    }

    /**
     * Transform front-end code to have the Java syntax.  Goes through
     * a series of lowering passes to convert an IR tree from the
     * "new" syntax to the "old" Java syntax understood by the main
     * StreamIt compiler.  Conversion directed towards the StreamIt
     * Java library, as opposed to the compiler, has slightly
     * different output.
     *
     * @param prog  the complete IR tree to lower
     * @param libraryFormat  true if the program is being converted
     *        to run under the StreamIt Java library
     * @param varGen  object to generate unique temporary variable names
     * @return the converted IR tree
     */
    public static Program lowerIRToLaminarIR(Program prog)
    {

        /* What's the right order for these?  Clearly generic
         * things like MakeBodiesBlocks need to happen first.
         * I don't think there's actually a problem running
         * MoveStreamParameters after DoComplexProp, since
         * this introduces only straight assignments which the
         * Java front-end can handle.  OTOH,
         * MoveStreamParameters introduces references to
         * "this", which doesn't exist. */

        // Yousun Ko: Why the two following passes were out of the lowerIRToJAVA wrapper?
        prog = (Program)prog.accept(new RenameBitVars());
        prog = (Program)prog.accept(new AssignLoopTypes());
        if (prog == null)
            return null;

        TempVarGen varGen = new TempVarGen(prog);

        prog = (Program)prog.accept(new MakeBodiesBlocks());
        prog = (Program)prog.accept(new DisambiguateUnaries(varGen));
        prog = (Program)prog.accept(new NameAnonymousStreams(varGen));

        prog = (Program)prog.accept(new NameAnonymousFunctions());
        prog = (Program)prog.accept(new AssembleInitializers());

        // Yousun Ko: mark propagatable ExprVars
        prog = (Program)prog.accept(new MarkPropagatableExprVars());
        // Yousun Ko: mark true state FieldDecls
        prog = (Program)prog.accept(new MarkTrueStateVars());
        // Yousun Ko: rename Program/StreamSpec keywords if needed
        prog = (Program)prog.accept(new RenameKeywords());

        return prog;
    }

    public int run(String[] args)
    {
        doOptions(args);
        if (printHelp)
            {
                printUsage();
                return 0;
            }
        
        Program prog = null;
        Writer outWriter;

        try
            {
                prog = parseFiles(inputFiles);
            }
        catch (java.io.IOException e)
            {
                e.printStackTrace(System.err);
                return 1;
            }
        catch (antlr.RecognitionException e)
            {
                e.printStackTrace(System.err);
                return 1;
            }
        catch (antlr.TokenStreamException e)
            {
                e.printStackTrace(System.err);
                return 1;
            }

        if (prog == null) {
            System.err.println("Compilation didn't generate a parse tree.");
            return 1;
        }

        prog = lowerIRToLaminarIR(prog);

        if (prog == null) {
            System.err.println("Compilation didn't lowerIR to LaminarIR.");
            return 1;
        }

        try
            {
                if (outputFile != null)
                    outWriter = new FileWriter(outputFile);
                else
                    outWriter = new OutputStreamWriter(System.out);

                String javaOut =
                    (String)prog.accept(new NodesToLaminarIR(commType, randomize, sanitycheck, 
                                graphFile, constFile, pathFile));
                outWriter.write(javaOut);
                outWriter.flush();
            }
        catch (java.io.IOException e)
            {
                e.printStackTrace(System.err);
                return 1;
            }
        return 0;
    }
    
    public static void main(String[] args)
    {
        try {
            int result = new ToLaminarIR().run(args);
            System.exit(result); }
        catch (Throwable e) { 
            e.printStackTrace();
            System.exit(1);
        }
    }
}

