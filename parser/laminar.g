grammar laminar;
options {
  language=Python;
}

// Import python scripts
@header {
  import re
  from graph.sdf import sdfgraph
  from graph.ssdf import ssdfgraph
}

// A program which can be
//   a synchronous dataflow graph
//   a structured synchronous dataflow graph
graph returns [graph]:
  { pre = ''
  }
  (PREAMBLE pre=CODE 
    {  pre = $pre.text
    } )? 

  (
    sdf[pre]
    { graph = $sdf.graph}
  | ssdf[pre]
    {
      graph = $ssdf.graph
    }
  )

  (POSTAMBLE post=CODE 
    {  graph.set_postamble($post.text)
    } )? 
;

// The SDF graph
sdf [pre] returns [graph]:
  sdf_name=GRAPH 
    { 
        graph = sdfgraph($sdf_name.text,preamble=pre)
        graph.postamble = ''
    }
    sdf_body[graph]
  END_BRACE
;

// SDF graph definition
sdf_body [graph]:
  ( ((FOR LPARAN param_type=ID param_name=ID '=' init_num=INT ':' max_num=INT RPARAN
       BEGIN(
        WS*(loop_edge[graph, param_type.text, param_name.text, int(init_num.text),int(max_num.text)])+)
      END) | edge[graph])* )  
  (actor[graph]*)
;

// The SSDF graph
ssdf [pre] returns [graph]:
  ssdf_name=SSDF_GRAPH 
    { 
        graph = ssdfgraph($ssdf_name.text,preamble=pre)
        graph.postamble = ''
    }
    ssdf_body[graph]
  END_BRACE
;

// SSDF graph definition
ssdf_body [graph]:
  ( 
    structure[graph]*
    { graph.link_structures() }
  )
  (edge[graph]*)  (actor[graph]*)
;

// An SDF edge
edge [graph]:
  {
     p = 0
     c = 0
     delays = []
  } 
  type=ID 
    producer=ID ( '(' pnum=INT ')' { p = int(pnum.text) } )? 
      EDGE 
        consumer=ID ( '(' cnum=INT ')' { c = int(cnum.text) } )?
          ( EDGE_DELAYS  delay_tokens { delays = $delay_tokens.tokens } END_BRACE )  ? 
          EOL
  {
     graph.add_node(producer.text,  nt='distributor')
     graph.add_node(consumer.text,  nt='distributor')
     graph.add_edge(producer.text, consumer.text ,p_rate=p, c_rate=c,ttype=type.text, dt = delays) 
  }
;

loop_edge [graph, p_type, p_name, init, max]:
  {
     p = 0
     c = 0
     delays = []
     loop_cons=False
     loop_prod=False
  } 
  type=ID 
    producer=ID ( '[' arg_prod=ID (op_prod=OPERATOR rvalue_prod=INT)? ']' { loop_prod=True } )?
                ( '(' pnum=INT ')' { p = int(pnum.text) } )? 
      EDGE 
        consumer=ID ( '[' arg_cons=ID (op_cons=OPERATOR rvalue_cons=INT)?']' { loop_cons=True } )?
                    ( '(' cnum=INT ')' { c = int(cnum.text) } )?
          ( EDGE_DELAYS  delay_tokens { delays = $delay_tokens.tokens } END_BRACE )  ? 
          EOL
  {
     if loop_prod and not loop_cons:
        graph.add_node(consumer.text,  nt='distributor')

        for iter in range(init, max+1):
           if op_prod and op_prod.text == '-':
              index = iter - int(rvalue_prod.text)
           elif op_prod and op_prod.text == '+':
              index = iter + int(rvalue_prod.text)
           else:
              index = iter
           graph.add_node(producer.text+'__'+str(index),  nt='distributor')
           graph.add_edge(producer.text+'__'+str(index), consumer.text ,p_rate=p, c_rate=c,ttype=type.text, dt = delays)
           
           graph.add_param(producer.text+'__'+str(index), p_type, p_name,str(index))

     elif loop_cons and not loop_prod:
        graph.add_node(producer.text,  nt='distributor')
        for iter in range(init, max+1):
           if op_cons and op_cons.text == '-':
              index = iter - int(rvalue_cons.text)
           elif op_cons and op_cons.text == '+':
              index = iter + int(rvalue_cons.text)
           else:
              index = iter
           graph.add_node(consumer.text+'__'+str(index),  nt='distributor')
           graph.add_edge(producer.text, consumer.text+'__'+str(index)
                   ,p_rate=p, c_rate=c,ttype=type.text, dt = delays)
           
           graph.add_param(consumer.text+'__'+str(index), p_type, p_name, str(index))

     elif loop_cons and loop_prod:
        for iter in range(init, max+1):
           if op_cons and op_cons.text == '-':
              index_cons = iter - int(rvalue_cons.text)
           elif op_cons and op_cons.text == '+':
              index_cons = iter + int(rvalue_cons.text)
           else:
              index_cons = iter

           if op_prod and op_prod.text == '-':
              index_prod = iter - int(rvalue_prod.text)
           elif op_prod and op_prod.text == '+':
              index_prod = iter + int(rvalue_prod.text)
           else:
              index_prod = iter
           
           graph.add_node(consumer.text+'__'+str(index_cons),  nt='distributor')
           graph.add_node(producer.text+'__'+str(index_prod),  nt='distributor')
           graph.add_edge(producer.text+'__'+str(index_prod), consumer.text+'__'+str(index_cons) ,p_rate=p, c_rate=c,ttype=type.text, dt = delays)
           graph.add_param(consumer.text+'__'+str(index_cons), p_type, p_name, str(index_cons))
           graph.add_param(producer.text+'__'+str(index_prod), p_type, p_name,str(index_prod))
       
  }
;


delay_tokens returns [tokens]: 
    c1=CODE {tokens = [$c1.text]} 
       ( ',' c2=CODE {tokens.append($c2.text)})*  
;


// An SSDF structure
structure [graph] returns [name]:
  struct=STRUCTURE
    (
      {
        structures = []
      }
      (st=structure[graph]
        { structures.append(st.name) }
      | st_id=ID
        { structures.append(st_id.text) }
      )

      (LIST_SEP
        (st=structure[graph]
          { structures.append(st.name) }
        | st_id=ID
          { structures.append(st_id.text) }
        )
      )*
    )
  END_BRACE
  {
    struct_name, struct_type = struct.text.split(',')
    structure = graph.add_structure(struct_type, struct_name, structures)
    name = structure.name
  }
;


// An SDF actor
actor [graph]:
  actor_id=ACTOR
   state_defs[graph,actor_id.text]
   init_defs[graph,actor_id.text]
   token_defs[graph,actor_id.text] 
   peek_token_defs[graph,actor_id.text]
   actor_firing
   token_defs[graph,actor_id.text]
  END_BRACE
  {
    import re
    for node in graph.nodes():
       plane_node = node.split('__')[0]
       states=[]        # append index to the states
       if len(node.split('__')) > 1:
          state_stmts=$state_defs.code.replace('volatile ','').replace('static ', '').replace('const ','')
          state_stmts=state_stmts.replace('int ','').replace('float ','')
          state_stmts=state_stmts.strip().replace('\n','').split(';')
          for state_index in range(0,len(state_stmts)):
             state_stmt=state_stmts[state_index].strip()
             if state_stmt == '':
                continue
             tokens = state_stmt.split(' ')[0].split('=')[0].split('[')
             state = tokens[0]
             states.append(state)
          node_index = '__'+node.split('__')[1]
       else:
          node_index = ''

       updated_firing = $actor_firing.code
       updated_state_defs = $state_defs.code
       updated_init_defs = $init_defs.code
             
       # current node is an instance of an actor. 
       # rename global variables (e.g., state variables)
       if actor_id.text == plane_node:
          for state in states:
             updated_firing = re.sub(r"(?<=([^0-9A-Za-z_]))"+state+"(?=[^0-9A-Za-z_])", state+node_index, updated_firing)

             updated_state_defs = re.sub(r"(?<=([^0-9A-Za-z_]))"+state+"(?=[^0-9A-Za-z_])", state+node_index, updated_state_defs)

             updated_init_defs = re.sub(r"(?<=([^0-9A-Za-z_]))"+state+"(?=[^0-9A-Za-z_])", state+node_index, updated_init_defs)

       # add an instance (former cond.) or an actor (later cond.)
       # otherwise do nothing and iterate
       if actor_id.text == plane_node or actor_id.text == node:
          if graph.__class__ == ssdfgraph:
             graph.add_actor(node, nt='generic', code=updated_firing, state=updated_state_defs, init=updated_init_defs)
          else:
             graph.set_node(node, nt='generic', code=updated_firing, state=updated_state_defs, init=updated_init_defs)
        
  }
;

token_defs [graph, actor]: 
  ( actor_input[graph, actor] | actor_output[graph, actor] ) *  
;

peek_token_defs [graph, actor]:
( actor_peek[graph, actor] )*
;

state_defs [graph, actor] returns [code]:
  { code = '' }
  (ACTOR_STATE  LABEL_SEP  CODE 
      { code = $CODE.text } 
  )? 
;

init_defs [graph, actor] returns [code]:
  { code = '' }
  ( ACTOR_INIT  LABEL_SEP  CODE
     { code = $CODE.text } 
  )? 
;

actor_input [graph, actor]:
  ACTOR_INPUT  LABEL_SEP
  (pred_id=ID LABEL_SEP token_names
  {
     edge_found=False

     for pred in graph.nodes():
        if pred_id.text in pred:
           for post in graph.out_nodes(pred):
              if actor in post:
                 graph.set_targettokens(pred,post,$token_names.token_list)
                 edge_found=True
     
     if not edge_found:
        graph.error('Edge '+pred_id.text+'->'+actor+' undefined in input section of actor '+actor)
  }
  EOL)*
;

actor_peek [graph, actor]:
  ACTOR_PEEK  LABEL_SEP
  (pred_id=ID LABEL_SEP token_names
  {
     edge_found=False

     for pred in graph.nodes():
        if pred_id.text in pred:
           for post in graph.out_nodes(pred):
              if actor in post:
                 graph.set_peektokens(pred,post,$token_names.token_list)
                 edge_found=True
     
     if not edge_found:
        graph.error('Edge '+pred_id.text+'->'+actor+' undefined in input section of actor '+actor)
  }
  EOL)*
;


actor_firing returns [code]:
  ACTOR_FIRING LABEL_SEP CODE
  { code = $CODE.text }
;

actor_output [graph,actor]:
  ACTOR_OUTPUT LABEL_SEP
  (succ_id=ID LABEL_SEP token_names
  {
     edge_found=False

     for pred in graph.nodes():
        if actor in pred:
           for post in graph.out_nodes(pred):
              if succ_id.text in post:
                 graph.set_sourcetokens(pred,post,$token_names.token_list)
                 edge_found=True
     
     if not edge_found:
         graph.error('Edge '+actor+'->'+succ_id.text+' undefined in output section of actor '+actor)
 
  }
  EOL)*
;


// Edge token names for SDF actors
token_names returns [token_list]:
    (token_id=ID '[' from_index=INT '-' to_index=INT ']' 
      {token_list = [token_id.text+from_index.text]
       for i in range(int(from_index.text)+1, int(to_index.text)+1):
         token_list.append(token_id.text+str(i))
      })|(token_id=ID {token_list = [token_id.text] })
    ((token_id=ID '[' from_index=INT '-' to_index=INT ']' 
      {
       for i in range(int(from_index.text), int(to_index.text)+1):
         token_list.append(token_id.text+str(i))
      })|
  (LIST_SEP token_id=ID { token_list.append(token_id.text) }))*
;


// Lexical rules

GRAPH:          'sdf' START_NAMED_SECTION;
SSDF_GRAPH:     'ssdf' START_NAMED_SECTION;

EDGE:           '->';
EDGE_DELAYS:     '=' WS* '{';
ACTOR:          'actor' START_NAMED_SECTION;
ACTOR_INPUT:    'input';
ACTOR_PEEK:     'peek';
ACTOR_PARAM:    'param';
ACTOR_FIRING:   'firing';
ACTOR_OUTPUT:   'output';
ACTOR_STATE:    'state';
ACTOR_INIT:     'init';
PREAMBLE:       'preamble';
POSTAMBLE:      'postamble';
FOR:            'for';

fragment STRUCTURE_TYPE:
                ( 'pipeline'
                | 'splitjoin'
                | 'feedback'
                )
;
STRUCTURE:      STRUCTURE_TYPE
                { struct_type = self.text }
                (START_NAMED_SECTION|START_SECTION)
                { struct_name = self.text }
                { self.text = struct_name + "," + struct_type }
;


fragment START_BRACE:    '{';
END_BRACE:      '}';
LIST_SEP:       ',';
LABEL_SEP:      ':';
EOL:            ';';
LPARAN:         '(';
RPARAN:         ')';
BEGIN:          'begin';
END:            'end';

fragment START_SECTION:
  WS* START_BRACE
  {self.text = ''} // no name
;
fragment START_NAMED_SECTION:
  WS+ ID WS* START_BRACE
  {self.text = $text.split()[1].rstrip('}').strip()} // Extract name
;

fragment CODE_START:       '{';
fragment CODE_END:         '}';
fragment NESTED_CODE:
  CODE_START
    (NESTED_CODE | ~('{'|'}'))*
  CODE_END
;
CODE:
  NESTED_CODE
  { self.text = $text.lstrip('{').rstrip('}') }
;

LINE_COMMENT:
  '//' ~( '\r' | '\n' )* { self.skip() }
;
MULTILINE_COMMENT:
 '/*' (.*) '*/' { self.skip() }
;

ID: ('a'..'z'|'A'..'Z'|'_') ('a'..'z'|'A'..'Z'|'_'|'0'..'9')*;
INT: ('0'..'9')+;
OPERATOR: ('-'|'+');

WS: (' '|'\t'|'\r'? '\n')+ {$channel=HIDDEN;};


