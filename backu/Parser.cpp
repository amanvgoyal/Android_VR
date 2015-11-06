#include <iostream>
#include <map>

//Intialized Program
bool Parser::par_program(stack<Token>& theStack, int error_count){
  //  cout<<"Stack SIZE:"<<theStack.size()<<endl;
  reverse_order(theStack); // Swap stack from abc to cba
  if(error_count==0){ //If tokenizer caught errors it i 
    //cout<<"--------------------------------------------------------------"<<endl;
    while(!ordered.empty()){ //Keep popping until nothing is left 
      if(!par_line()){ //call Par_line which calls query or commands
	//	cout<<"INVALID HERE:"<<endl;
	par_empty();//empty the stack
	//cout<<"SIZE OF STACK NOW"<<ordered.size()<<endl;
	list.push_back(false);//Push False value into 
      }
      else {
	//cout<<"VALID HERE1:"<<endl;
	par_empty();//empty stack
	list.push_back(true);//Push True value into 
      }
    }
  }
  if(error_count>0){// if there were errors found ABORT
    //cout<<"INVALID"<<endl;
    list.push_back(false);//Push False value into 
    par_empty();//empty the stack
  }
}

// This prints the vector into terminal and textfile
bool Parser::print_result(string &s){
  int count =0;
  ofstream myfile;
  myfile.open(s.c_str());
  for(int i =0;i<list.size();++i){
    ++count;
    if(list[i]==false) {
      cout<<count<<".)"<<"FALSE"<<endl;
      myfile<<count<<".)"<<"FALSE"<<endl;
    }
    else {
      cout<<count<<".)"<<"TRUE"<<endl;
      myfile<<count<<".)"<<"TRUE"<<endl;
    }	
  }
  myfile.close();	
}
//empty the stack
bool Parser::par_empty(){
  if(!ordered.empty()){
    while(!ordered.empty()){
      ordered.pop();
    }
    return true;
  }
  return false;
}
// return stack size
int Parser::par_stacksize(){
  return ordered.size();
}
//Reverse stack from abc to cba
bool Parser::reverse_order(stack<Token>& theStack){
  size_t temp = theStack.size();
  while(!theStack.empty()){
    ordered.push(theStack.top());
    theStack.pop();
  }
  //cout<<"Parser Stack Size:"<<ordered.size()<<endl;
  if(temp==ordered.size()) return true;
  return false;
}
//checks the first token and calls a function depending on which one is called
bool Parser::par_line(){
  Token temp = ordered.top();
  if(temp.get_type()==T_CREATE||temp.get_type()==T_INSERT||temp.get_type()==T_OPEN||
     temp.get_type()==T_CLOSE||temp.get_type()==T_SAVE||temp.get_type()==T_EXIT||temp.get_type()==T_SHOW||
     temp.get_type()==T_UPDATE||temp.get_type()==T_DELETE){
    //cout<<"PARSER::COMMAND"<<endl;
    if(par_command()){
      if(ordered.empty()){
	//cout<<"INVALID 1:"<<endl;
	return false;
      }
      temp = ordered.top();
      if(temp.get_type()==T_SEMICOLON){ // if it ends in a semicolon, exit the program 
	ordered.pop();
	if(ordered.empty()){
	  //cout<<"VALID"<<endl;
	  return true;
	}
	temp = ordered.top();
	if(temp.get_type()==T_SEMICOLON)// if there are two semicolons it prints false
	  {
	    //cout<<"INVALID 2:"<<endl;
	    return false;
	  }
 	//cout<<"VALID"<<endl;
	// RESET VARS HERE!!!!!!!!!!!!!!!!!!!!!!
	comp_ops.clear();
	return_types.clear();
	recur_depth = 0;
	return true;
      }
      //else cout<<"INVALID 3:"<<endl;
      //cout<<"STACK SIZE:"<<ordered.size()<<endl;
    }
    //else cout<<"INVALID"<<endl; 
  }
  //If it matchs SELECT, PROJEC, RENAME, IDENTIFIER its a query
  if(temp.get_type()==T_SELECT||temp.get_type()==T_PROJECT||temp.get_type()==T_RENAME||temp.get_type()==T_IDENTIFIER){
    //cout<<"PARSER::QUERY"<<endl;
    if(par_query()){
      if(ordered.empty()){
	//cout<<"INVALID 1:"<<endl;
	return false;
      }
      temp = ordered.top();
      if(temp.get_type()==T_SEMICOLON){
	ordered.pop();
	if(ordered.empty()){
	  //cout<<"VALID"<<endl;
	  return true;
	}
	//depending on the token call the correct function
	temp = ordered.top();
	if(temp.get_type()==T_SEMICOLON)
	  {
	    //cout<<"INVALID 2:"<<endl;
	    return false;
	  }
	temp = ordered.top();
	//cout<<"VALID"<<endl;
	return true;
      }
      //else cout<<"INVALID: DOES NOT END IN T_SEMICOLON"<<endl;
      //cout<<"STACK SIZE:"<<ordered.size()<<endl;

    }
  }
  return false; //if none of the above conditions match its not part of our language
}
// If its a command it calls the correlating function to that token
bool Parser::par_command(){
  Token temp = ordered.top();
  if(temp.get_type()==T_CREATE){
    ordered.pop();
    if(par_create()){
      //cout<<"Returned TRUE(CREATE)"<<endl;
      return true;	
    }
  }
  if(temp.get_type()==T_INSERT){
    //cout<<"INSERT COMMAND CALLED"<<endl;
    ordered.pop();
    if(par_insert()){
      //cout<<"Returned TRUE(INSERT)"<<endl;
      return true;
    }
    return false;
  }
  if(temp.get_type()==T_OPEN){
    //ordered.pop();
    if(par_open()){
      return true;
    }
  }
  if(temp.get_type()==T_CLOSE){
    //ordered.pop();
    if(par_close()){
      //cout<<"RETURNED TRUE(CLOSE)"<<endl;
      
      return true;
    }
  }
  if(temp.get_type()==T_SAVE){
    //ordered.pop();
    if(par_save()){
      //cout<<"RETURNED TRUE(SAVE)"<<endl;
      return true;
    }
  }
  if(temp.get_type()==T_EXIT){
    //ordered.pop();
    if(par_exit()){
      //cout<<"RETURNED TRUE(EXIT)"<<endl;
      return true;
    }
  }
  if(temp.get_type()==T_SHOW){
    //ordered.pop();
    if(par_show()){
      //cout<<"RETURNED TRUE(SHOW)"<<endl;
      return true;
    }
  }
  if(temp.get_type()==T_UPDATE){
    //ordered.pop();
    if(par_update()){
      //cout<<"RETURNED TRUE(UPDATE)"<<endl;
      return true;
    }
  }
  if(temp.get_type()==T_DELETE){
    //ordered.pop();
    if(par_delete()){
      //cout<<"RETURNED TRUE(DELETE)"<<endl;
      return true;				
    }
  }
  return false; //if its not any of these its not one of our commands
}
//Calls the correlating query function
bool Parser::par_query(){
  Token temp = ordered.top();
  if(temp.get_type()==T_SELECT){
    if(par_selection()){
      //cout<<"RETURNED TRUE(SELECT)"<<endl;
      return true;
    }
  }
  if(temp.get_type()==T_PROJECT){
    if(par_projection()){
      //cout<<"RETURNED TRUE(PROJECT)"<<endl;
      return true;
    }
  }
  if(temp.get_type()==T_RENAME){
    if(par_renaming()){
      //cout<<"RETURNED TRUE(RENAME)"<<endl;
      return true;
    }
  }
  if(temp.get_type()==T_IDENTIFIER){
    if(par_execute_query()){
      //cout<<"RETURNED TRUE(EXECUTE QUERY)"<<endl;
      return true;
    }
  }
  return false; //if none of these functions exit
}
//If it starts with an identifier it comes here first
bool Parser::par_execute_query(){
  Token temp = ordered.top();
  if(par_relationName()){ 
    temp=ordered.top();
    if(temp.get_type()==T_ASSIGN){// Looks for an arrow
      //cout<<"FOUND LEFT ARROW"<<endl;
      ordered.pop();
      if(par_expression()){
	return true;
      }
      //cout<<"Returning False par_execute_query"<<endl;
      return false;
    }
  }
  else return false;
}
//If create was the first token it comes here.
//CREATE TABLE relation-name ( typed-attribute-list ) PRIMARY KEY ( attribute-list )
bool Parser::par_create(){
  Token temp = ordered.top();
  int count_commas =0;
  int count_types = 0;
  table_list new_tbl;
  vector<string> attrs;
  vector<string> prim_keys;
  create_attrs = {}; 
  if(temp.get_type()==T_TABLE)
    {
      ordered.pop();
      string tbl = ordered.top().get_string_value();
      if(par_relationName()){
	temp=ordered.top();
	if(temp.get_type()==T_LPAR){
	  ordered.pop();
	  //cout<<"IN CREATE LOOKING FOR T_RPAR"<<endl;
	  while(temp.get_type()!=T_RPAR){
	    if(ordered.empty()){
	      //cout<<"INVALID(CREATE): STACK EMPTY"<<endl;
	      return false;
	    }
	    temp = ordered.top();
	    // CAPTURE ATTRIBUTES
	    create_attrs.push_back(temp.get_string_value());
	    attrs.push_back(ordered.top().get_string_value());
	    if(par_relationName()){
	      temp = ordered.top();
	      if(temp.get_type()==T_VARCHAR||T_INTTYPE){
		ordered.pop();
		//cout<<"FOUND TYPE T_VARCHAR||T_INTTYPE"<<endl;
		++count_types;
		temp =ordered.top();
		if(temp.get_type()==T_COMMA){
		  ordered.pop();
		  temp = ordered.top();
		  //cout<<"Popping COMMA(CREATE)"<<endl;
		  ++count_commas;
		  if(ordered.empty()){
		    //cout<<"INVALID(CREATE): STACK EMPTY"<<endl;
		    return false;
		  }

		}
							
	      }

	    }
	    temp = ordered.top();
	    //cout<<"\n"<<"THIS IS A "<<temp.get_type_string()<<endl;
	    if((temp.get_type()!=T_COMMA) && temp.get_type()!=T_IDENTIFIER && temp.get_type()!=T_RPAR && temp.get_type()!=T_VARCHAR && T_INTTYPE){
	      //cout<<"RETURN FLASE(CREATE)2"<<endl;
	      return false;
	    }

	  }
	  //cout<<"Count_Commas:"<<count_commas<<"count_types"<<count_types<<endl;
	  if(count_commas==count_types-1){}//{cout<<"IN HERE"<<endl;}
	  else {//cout<<"COMMAS DONT MATCH # of TYPES"<<endl;return false;
	  }
	  count_commas=0;
	  count_types=0; 
	  temp = ordered.top();
	  //cout<<"FOUND T_RPAR"<<endl;
	  if(temp.get_type()==T_RPAR){
	    ordered.pop();
	    temp=ordered.top();
	    if(temp.get_type()==T_PRIMARYKEY){
	      //cout<<"FOUND PRIMARY KEY(CREATE):"<<endl;
	      ordered.pop();
	      temp=ordered.top();
	      if(temp.get_type()==T_LPAR){
		ordered.pop();
		temp=ordered.top();
		while(temp.get_type()!=T_RPAR){
		  if(ordered.empty()){
		    //cout<<"INVALID(CREATE): STACK EMPTY"<<endl;
		    return false;
		  }
		  // GET PRIMARY KEYS HERE
		  prim_keys.push_back(ordered.top().get_string_value());
		  if(par_relationName()){
		    ++count_types;
		    temp = ordered.top();
		    if(ordered.empty()){
		      //cout<<"INVALID(CREATE): STACK EMPTY"<<endl;
		      return false;
		    }
		    if(temp.get_type()==T_COMMA){
		      //cout<<"POPPING COMMA"<<endl;
		      ++count_commas;
		      if(ordered.empty()){
			//cout<<"INVALID(CREATE): STACK EMPTY"<<endl;
			return false;
		      }
		      ordered.pop();
		      temp = ordered.top();
		    }
		  }
		  else return false;
		}
		//cout<<"Count_Commas:"<<count_commas<<"count_types"<<count_types<<endl;
		if(count_commas==count_types-1){}//{cout<<"IN HERE"<<endl;}
		else {
		  //cout<<"COMMAS DONT MATCH # of TYPES"<<endl;return false;
		}
		temp=ordered.top();
		if(temp.get_type()==T_RPAR){
		  //cout<<"FOUND T_RPAR"<<endl;
		  if(ordered.empty()){
		    //cout<<e"INVALID(CREATE): STACK EMPTY"<<endl;
		    return false;
		  }
		  ordered.pop();

		  // MAKE TABLE HERE...
		  table t = db.create(attrs, prim_keys);
		  //	  db.show(t);
		  db.save(t, tbl + ".db");
		  db.print_db();
		  return true;
		}
	      }
	      return false;
	    }
	    return false;

	  }
	  return false;
	}
	return false;
      }
    }
  return false;
}
//INSERT INTO relation-name VALUES FROM ( literal { , literal } )
//INSERT INTO relation-name VALUES FROM RELATION expr
bool Parser::par_insert(){
  Token temp = ordered.top();
  vector<string> vals;
  table_list t = db.return_DB();
  string s;

  if(temp.get_type()==T_INTO){
    ordered.pop();
    //cout<<"MADE IT HERE INTO"<<endl;
    Token temp = ordered.top();
    string tbl =  temp.get_string_value();
    if(par_relationName())
      {	
	//cout<<"MADE IT HERE INTO par_relationName"<<endl;
	Token temp = ordered.top();
	if(temp.get_type()==T_VALUE){
	  ordered.pop();
	  //cout<<"MADE IT HERE INTO T_VALUE"<<endl;
	  Token temp = ordered.top();
	  if(temp.get_type()==T_FROM){
	    ordered.pop();
	    //cout<<"MADE IT HERE INTO T_FROM"<<endl;
	    Token temp = ordered.top();
	    if(temp.get_type()==T_LPAR){
	      //cout<<"Popping T_LPAR INSERT"<<endl;
	      ordered.pop();
	      //cout<<"MADE IT HERE INTO T_LPAR"<<endl;
	      Token temp = ordered.top();
	      //cout<<"Searching for T_RPAR"<<endl;
	      while(temp.get_type()!=T_RPAR){
		if(ordered.empty()){
		  //cout<<"INVALID: STACK EMPTY"<<endl;
		  return false;
		}
		//cout << ordered.top().get_string_value() << endl;
		//		vals.push_back(temp.get_string_value());
		if (ordered.top().get_type() == T_STRING &&
		    ordered.top().get_string_value().find_first_not_of(' ') !=
		    string::npos) {
		  s = ordered.top().get_string_value();
		  vals.push_back(s.substr(1, s.size() - 2));
		}
		else if (ordered.top().get_type() == T_INTVALUE ||
			 ordered.top().get_type() == T_DOUBLEVALUE) {
		  vals.push_back(to_string(ordered.top().get_num()));
		}
		else{}

		if(par_literals()){
		  if(ordered.empty()){
		    //cout<<"INVALID: STACK EMPTY"<<endl;
		    return false;
		  }
		  temp = ordered.top();
		}
		if(temp.get_type()==T_COMMA){
		  //cout<<"FOUND COMMA"<<endl;
		  ordered.pop();
		  // EXPAND OPTIONS?
		  if (ordered.top().get_type() == T_STRING &&
		      ordered.top().get_string_value().find_first_not_of(' ') !=
		      string::npos) {
		    s = ordered.top().get_string_value();
		    vals.push_back(s.substr(1, s.size() - 2));
		  }
		  else if (ordered.top().get_type() == T_INTVALUE ||
			   ordered.top().get_type() == T_DOUBLEVALUE) {
		    vals.push_back(to_string(ordered.top().get_num()));
		  }
		  else{}

		  if(par_literals()){
		    if(ordered.empty()){
		      //cout<<"INVALID: STACK EMPTY"<<endl;
		      return false;
		    }
		    temp = ordered.top();
		  }
		  else return false;
									
		}
		else return false;
	      }
	      temp = ordered.top();
	      if(temp.get_type()==T_RPAR){
		//cout<<"Found T_RPAR"<<endl;
		ordered.pop();
		//Token temp = ordered.top();

		// Do insertion
		int rowCt = 0;
		
		for (auto it : t[tbl]) {
		  if (it.first == "$primary_key") {
		    insert_vals[it.first] = {it.second[0]};
		  }
		  else {
		    insert_vals[it.first] = {vals[vals.size() - 1 - rowCt]};
		    ++rowCt;
		  }
		 
		}
		
		table new_table = db.insert_tuple(tbl, insert_vals);
		db.show(insert_vals);
		return true;
	      }	
	    }
	    //INSERT INTO relation-name VALUES FROM RELATION expr
	    temp = ordered.top();
	    //cout<<"INSERTING INTO FROM RELATION:"<<endl;
	    if(temp.get_type()==T_RELATION){
	      ordered.pop();
	      
	      // LoGIC TO CREATE TABLE FROM WHATEVER FUNCTION
	      	
	      Token t = ordered.top();
	      if(par_expression()){
		switch(t.get_type()) {
		case T_PROJECT:
		  db.open(tbl);
		  table new_table = db.insert_tuple(tbl, project_vals);
		  //db.show(project_vals);
		  break;
		}

		return true;
	      }

	    }

	  }
	}
      }
    return false;
  }	
  return false;
}
/*expr ::= atomic-expr
  | selection
  | projection
  | renaming
  | union
  | difference
  | product*/
bool Parser::par_expression(){
  Token temp = ordered.top();
  if(par_selection()){
    //cout<<"SELECT RETURNED TRUE(Expression)"<<endl;
    return true;
  }
  if(par_projection()){
    //cout<<"PROJECT RETURNED TRUE(Expression)"<<endl;
    return_types.push_back(T_PROJECT);
    ++recur_depth;
    return true;
  }
  if(par_renaming()){
    //cout<<"RENAMING RETURNED TRUE(Expression)"<<endl;
    return true;
  }
  if(par_atomicexpression()){
    Token temp = ordered.top();
    //cout<<"MADE IT HERE(par_atomicexpression"<<endl;
    //SET UNION
    if(temp.get_type()==T_PLUS){
      ordered.pop();
      //cout<<"Popping PLUS(Atomic_Expression)"<<endl;
      temp = ordered.top();
      if(par_atomicexpression()){
	temp =ordered.top();
	//if(temp.get_type()==T_RPAR) ordered.pop();
	//	cout<<"Returning True Atomic_Expression"<<endl;
	return true;
      }
      return false;
    }
    //SET DIFFERENCE
    if(temp.get_type()==T_MINUS){
      ordered.pop();
      temp = ordered.top();
      if(par_atomicexpression()){
	return true;
      }
      return false;
    }
    if(temp.get_type()==T_MULTIPLY){
      ordered.pop();
      temp = ordered.top();
      if(par_atomicexpression()){
	return true;
      }
      return false;
    }
    //cout<<"Atomic RETURNED TRUE(Expression)"<<endl;
    return true;
  }
  return false;
}
// atomic-expr ::= relation-name | ( expr )
bool Parser::par_atomicexpression(){
  Token temp = ordered.top();
  if(temp.get_type()==T_LPAR){
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    ordered.pop();
    //cout<<"Popping T_LPAR(par_atomicexpression)"<<endl;
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    if(par_expression()){
      temp = ordered.top();
      if(temp.get_type()==T_RPAR){
	ordered.pop();
	if(ordered.empty()){
	  //cout<<"INVALID: STACK EMPTY"<<endl;
	  return false;
	}
	return true;
      }
      else return false;
    }
  }
  else if(par_relationName()) return true;
  return false; 
}
//relation-name ::= identifier
bool Parser::par_relationName(){
  Token temp = ordered.top();
  if(temp.get_type()==T_IDENTIFIER){
    //cout<<"FOUND T_IDENTIFIER (par_relationName) "<<temp.get_string_value()<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID(RELATION NAME): STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  else return false;
}
// relation-name ::= identifier
bool Parser::par_literals(){
  if(ordered.empty()){
    //cout<<"INVALID(LITERAL): STACK EMPTY"<<endl;
    return false;
  }
  Token temp = ordered.top();
  if(temp.get_type()==T_STRING){
    //cout<<"FOUND LITERAL:"<<temp.get_string_value()<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID(LITERAL): STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  else if(temp.get_type()==T_INTVALUE){
    //cout<<"FOUND LITERAL:"<<temp.get_num()<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID(LITERAL): STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  else if(temp.get_type()==T_DOUBLEVALUE){
    //cout<<"FOUND LITERAL:"<<temp.get_num()<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID(LITERAL): STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }

  return false;
}
//select ( condition ) atomic-expr
bool Parser::par_selection(){
  Token temp = ordered.top();
  string tbl;
  table new_table;
  table_list t = db.return_DB();
  if(temp.get_type()==T_SELECT){
    //cout<<"SELECT COMMAND CALLED"<<endl;
    //cout<<"Popped Select"<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID(SELECT): STACK EMPTY"<<endl;
      return false;
    }
    temp = ordered.top();
    if(temp.get_type()==T_LPAR){
      ordered.pop();
      if(ordered.empty()){
	//cout<<"INVALID(SELECT): STACK EMPTY"<<endl;
	return false;
      }
      //cout<<"POPPED T_LPAR"<<endl;
      if(par_condition()){
	//cout<<"par_condition returned true:"<<endl;
	temp = ordered.top();
	if(temp.get_type()==T_RPAR){
	  ordered.pop();
	  //cout<<"POPPED T_RPAR"<<endl;
	  temp=ordered.top();
	  tbl = temp.get_string_value();
	  if(par_atomicexpression()) {
	    new_table = t[tbl];
	    vector<string> cmp;
	    for (int i = 0; i < comp_ops.size(); ++i) {
	      cmp = comp_ops[i];
	      new_table = db.selection(new_table, cmp[0], cmp[1], cmp[2]);
	    }			  
	    return true;
	  }
	}
      }
      //cout<<"Selection Returning False:"<<endl;
      return false;	
    }
  }
  return false;
}
// condition ::= conjunction { || conjunction }
bool Parser::par_condition(){
  //cout<<"Conditon Called"<<endl;
  Token temp = ordered.top();
  if(par_conjuction()){
    temp = ordered.top();
    if(temp.get_type()==T_TWOOR){
      temp = ordered.top();
      while(temp.get_type()==T_TWOOR){
	ordered.pop();
	//cout<<"Popping TWOOR"<<endl;
	if(par_conjuction()){
	  //cout<<"IN HERE BOI(Conjuction)"<<endl;
	}
	temp = ordered.top();
      }
    }
    //cout<<"Returning TRUE(Conditon):"<<endl;
    return true;
  }
  //cout<<"Returning false(Conditon):"<<endl;
  return false;
}
// conjunction ::= comparison { && comparison }
bool Parser::par_conjuction(){
  //cout<<"Conjuction Called"<<endl;
  Token temp = ordered.top();
  if(par_comparison()){
    temp = ordered.top();
    //cout<<"CHECK THIS JOINT OUT:" <<temp.get_type_string()<<endl;
    if(temp.get_type()==T_TWOAND){
      temp = ordered.top();
      //cout<<"Entering While Loop(Conjuction)"<<temp.get_type_string()<<endl;
      while(temp.get_type()==T_TWOAND){
	//cout<<"Popping TWOAND(Conjuction)"<<endl;
	ordered.pop();
	if(par_comparison()){
	  //cout<<"IN HERE BOI(Conjuction)"<<endl;
	}
	temp = ordered.top();
      }
    }
    //cout<<"Returning TRUE(CONJUCTION)"<<endl;
    return true;
  }
  //cout<<"Returning False(CONJUCTION)"<<endl;
  return false;
}
//comparison ::= operand op operand | ( condition )
bool Parser::par_comparison(){
  string opr1, op, opr2;
  //cout<<"Comparison Called"<<endl;
  Token temp = ordered.top();
  opr1 = ordered.top().get_string_value();
  if(par_operand()){
    op = ordered.top().get_string_value();
    if(par_op()){
      opr2 = ordered.top().get_string_value();
      if(par_operand()){
	vector<string> opset = {opr1, op, opr2};
	cout << "OPS: " + opr1 + ' ' + op + ' ' + opr2 << endl;
	comp_ops.push_back(opset);
	//cout<<"Return True(Comparison)"<<endl;
	return true;
      }
      return false;
    }
    return false;
  }
  if(ordered.empty()){
    //cout<<"INVALID(Comparison): STACK EMPTY"<<endl;
    return false;
  }
  temp =ordered.top();
  if(temp.get_type()==T_LPAR){
    //cout<<"Popping T_LPAR Comparison"<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID(Comparison): STACK EMPTY"<<endl;
      return false;
    }
    temp = ordered.top();
    while(temp.get_type()!=T_RPAR){
      if(par_condition()){
	temp = ordered.top();
      }
      else return false;
    }
    temp = ordered.top();
    if(temp.get_type()==T_RPAR){
      //cout<<"Popping T_RPAR Comparison"<<endl;
      ordered.pop();
      if(ordered.empty()){
	//cout<<"INVALID(Comparison): STACK EMPTY"<<endl;
	return false;
      }
      cout << "sdss" << endl;
      return true;
    }
  }
  return false;
}
//operand ::= attribute-name | literal
bool Parser::par_operand(){
  //cout<<"Operand Called"<<endl;
  Token temp = ordered.top();
  if(par_relationName()){
    return true;
  }
  if(par_literals()){
    return true;
  }
  //cout<<"Returning FALSE(OPERAND)"<<endl;
  return false;
}
//op ::= == | != | < | > | <= | >=
bool Parser::par_op(){
  //cout<<"OP Called"<<endl;
  Token temp = ordered.top();
  if(temp.get_type()==T_TWOEQUALS){
    //cout<<"Popping == "<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  if(temp.get_type()==T_NOTEQUAL){
    //cout<<"Popping != "<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  if(temp.get_type()==T_LESS){
    //cout<<"Popping < "<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  if(temp.get_type()==T_LESSEQUAL){
    //cout<<"Popping <= "<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  if(temp.get_type()==T_GREATER){
    //cout<<"Popping > "<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  if(temp.get_type()==T_GREATEREQUAL){
    //cout<<"Popping >= "<<endl;
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    return true;
  }
  //cout<<"Returning False: OP"<<endl;
  return false;
}
//projection ::= project ( attribute-list ) atomic-expr
bool Parser::par_projection(){
  Token temp = ordered.top();
  //table project_vals;
  vector<string> attrs;
  if(temp.get_type()==T_PROJECT){
    //cout<<"Popping Project(PROJECT)"<<endl;
    ordered.pop();
    temp = ordered.top();
    if(temp.get_type()==T_LPAR){
      if(ordered.empty()){
	//cout<<"INVALID: STACK EMPTY"<<endl;
	return false;
      }
      ordered.pop();
      //cout<<"Popped T_LPAR"<<endl;
      if(ordered.empty()){
	//cout<<"INVALID: STACK EMPTY"<<endl;
	return false;
      }
      temp = ordered.top();
      //cout<<"IN PROJECTION LOOKING FOR T_RPAR"<<endl;
      while(temp.get_type()!=T_RPAR){
	if(ordered.empty()){
	  //cout<<"INVALID(PROJECTION): STACK EMPTY"<<endl;
	  return false;
	}
	
	attrs.push_back(ordered.top().get_string_value());
	if(par_relationName()){
	  temp = ordered.top();
	  if(ordered.empty()){
	    //cout<<"INVALID(PROJECTION): STACK EMPTY"<<endl;
	    return false;

	  }
	  if(temp.get_type()==T_COMMA){
	    if(ordered.empty()){
	      //cout<<"INVALID: STACK EMPTY"<<endl;
	      return false;
	    }
	    ordered.pop();
	    //cout<<"Popping Comma (PROJECTION):"<<endl;
	    temp = ordered.top();
	  }
	}
	else return false;

      }
      //cout<<"FOUND T_RPAR(PROJECTION)"<<endl;
      ordered.pop();
      if(ordered.empty()){
	//cout<<"INVALID(PROJECTION): STACK EMPTY"<<endl;
	return false;
      }
      string tbl = ordered.top().get_string_value();
      if(par_atomicexpression()){
	//cout << "db.projection(" + tbl + ", " + "attrs);" << endl;
	project_vals = db.projection(tbl, attrs);
	//db.show(project_vals);
	return true;
      }
      else return false;
    }
  }

  return false;
}
//open-cmd ::== OPEN relation-name
bool Parser::par_open(){
  //cout<<"PARSER(OPEN)"<<endl;
  Token temp = ordered.top();
  if(temp.get_type()==T_OPEN){
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    string tbl = ordered.top().get_string_value();
    if(par_relationName()){
      cout << "db.open(\"" + tbl + "\");" << endl;
      db.open(tbl);
      //db.print_db();
      return true;
    }
    return false;
  }
  else return false;
}
// close-cmd ::== CLOSE relation-name 
bool Parser::par_close(){
  Token temp = ordered.top();
  //cout<<"CLOSE COMMAND REQUESTED"<<endl;
  if(temp.get_type()==T_CLOSE){
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    string tbl = (ordered.top()).get_string_value();
    if(par_relationName()){
      db.close(tbl);
      return true;
    }
    else return false;

  }
  return false;


}
// save-cmd ::== SAVE relation-name 
bool Parser::par_save(){
  Token temp = ordered.top();
  //cout<<"SAVE COMMAND REQUESTED"<<endl;
  if(temp.get_type()==T_SAVE){
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    string tbl = (ordered.top()).get_string_value() + ".db";
    if(par_relationName()){
      table_list t = db.return_DB();
      db.save(t[tbl], tbl);
      return true;
    }
    else return false;

  }
  return false;

}
// exit-cmd ::== EXIT 
bool Parser::par_exit(){
  Token temp = ordered.top();
  //cout<<"EXIT COMMAND REQUESTED"<<endl;
  if(temp.get_type()==T_EXIT){
    ordered.pop();
    if(ordered.empty()){
      //  cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    cout << "db.exit()" << endl;
    db.exit();
    return true;
  }
  return false;
}
// show-cmd ::== SHOW atomic-expr 
bool Parser::par_show(){
  Token temp = ordered.top();
  //cout<<"SHOW COMMAND REQUESTED"<<endl;
  if(temp.get_type()==T_SHOW){
    ordered.pop();
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    string tbl = (ordered.top()).get_string_value();
    if(par_atomicexpression()){
      cout << "db.show(\"" + tbl + "\");" << endl;
      db.show(tbl);
      return true;
    }
    else return false;

  }
  return false;
}
// update-cmd ::= UPDATE relation-name SET attribute-name = literal { , attribute-name = literal } WHERE condition 
bool Parser::par_update(){
  vector<string> attrs;
  vector<string> vals;
  string s;
  Token temp = ordered.top();
  if(temp.get_type()==T_UPDATE){
    //cout<<"In UPDATE"<<endl;
    //cout<<"Popping UPDATE"<<endl;
    ordered.pop();
    attrs.push_back(ordered.top().get_string_value());
    if(par_relationName()){
      temp = ordered.top();
      if(temp.get_type()==T_SET){
	ordered.pop();
	//cout<<"Popping SET(UPDATE)"<<endl;
	temp=ordered.top();
	attrs.push_back(ordered.top().get_string_value());
	if(par_relationName()){
	  temp = ordered.top();
	  if(temp.get_type()==T_EQUAL){
	    ordered.pop();
	    //cout<<"Popping SET(UPDATE)"<<endl;
	    temp = ordered.top();
	    s = temp.get_string_value();	    
	    vals.push_back(s.substr(1, s.size() - 2));
	    if(par_literals()){
	      temp = ordered.top();
	      //cout<<"BEFORE HITTING WHILE LOOP:"<<temp.get_type_string()<<endl;
	      while(temp.get_type()!=T_WHERE){
		//cout<<"LOOKING FOR WHERE(UPDATE)"<<endl;
		temp = ordered.top();
		if(temp.get_type()==T_COMMA){
		  ordered.pop();
		  //cout<<"Popping COMMA(UPDATE)"<<endl;
		  attrs.push_back(ordered.top().get_string_value());
		  if(par_relationName()){
		    temp = ordered.top();
		    if(temp.get_type()==T_EQUAL){
		      ordered.pop();
		      //cout<<"Popping EQUAL(UPDATE)"<<endl;
		      s = ordered.top().get_string_value();
		      vals.push_back(s.substr(1, s.size() - 2));
		      if(par_literals()){
			temp = ordered.top();
			
			// Do something with these literals!
		      }
		      else return false;
		    }
		    else return false;
		  }
		}
		else return false;
	      }
	      temp = ordered.top();
	      cout<<"Found WHERE (UPDATE)"<<endl;
	      ordered.pop();
	      if(par_condition()){ 
		for (int i = 0; i < comp_ops.size(); ++i) {
		  for (int j = 0; j < comp_ops[i].size(); ++j) {
		    cout << comp_ops[i][j] << ' ';
		  }
		  cout << endl;
		}
		return true;
	      }
	      return false;
	    }
	  }
	}
      }
    }
  }
  return false;
}
// delete-cmd ::= DELETE FROM relation-name WHERE condition
bool Parser::par_delete(){
  Token temp = ordered.top();
  string del_tbl, cond_attr, op, cond_val;
  if(temp.get_type()==T_DELETE){
    ordered.pop();
    //cout<<"IN DELETE"<<endl;
    if(ordered.empty()){
      //cout<<"INVALID: STACK EMPTY"<<endl;
      return false;
    }
    temp = ordered.top();
    if(temp.get_type()==T_FROM){
      ordered.pop();
      if(ordered.empty()){
	//cout<<"INVALID: STACK EMPTY"<<endl;
	return false;
      }
      temp = ordered.top();
      del_tbl = temp.get_string_value();
      if(par_relationName()){
	temp = ordered.top();
	if(temp.get_type()==T_WHERE){
	  ordered.pop();
	  if(ordered.empty()){
	    //cout<<"INVALID: STACK EMPTY"<<endl;
	    return false;
	  }
	  temp = ordered.top();
	  if(par_conjuction()){
	    for (int i = 0; i < comp_ops.size(); ++i) {
	      vector<string> ops = comp_ops[i];
	      db.delete_tuple(del_tbl, ops[0], ops[1], ops[2]);
	      cout << ops[0] << ' '<< ops[1] << ' ' << ops[2] << endl;
	    }
	    //db.delete(del_tbl, cond_attr, op, cond_val);
	    return true;
	  }
	}
      }
    }
  }
  return false;
}
// renaming ::= rename ( attribute-list ) atomic-expr
bool Parser::par_renaming(){
  Token temp = ordered.top();
  vector<string> new_names;
  table_list t = db.return_DB();
  if(temp.get_type()==T_RENAME){
    //cout<<"RENAMING CALLED"<<endl;
    //cout<<"Popping Renaming (Renaming)"<<endl;
    ordered.pop();
    temp = ordered.top();
    if(temp.get_type()==T_LPAR){
      if(ordered.empty()){
	//cout<<"INVALID: STACK EMPTY"<<endl;
	return false;
      }
      ordered.pop();
      //cout<<"Popped T_LPAR"<<endl;
      if(ordered.empty()){
	//cout<<"INVALID: STACK EMPTY"<<endl;
	return false;
      }
      temp = ordered.top();
      //cout<<"IN Renaming LOOKING FOR T_RPAR"<<endl;
      while(temp.get_type()!=T_RPAR){
	if(ordered.empty()){
	  //cout<<"INVALID(Renaming): STACK EMPTY"<<endl;
	  return false;
	}
	new_names.push_back(ordered.top().get_string_value());
	if(par_relationName()){
	  temp = ordered.top();
	  if(ordered.empty()){
	    //cout<<"INVALID(Renaming): STACK EMPTY"<<endl;
	    return false;

	  }
	  if(temp.get_type()==T_COMMA){
	    if(ordered.empty()){
	      //cout<<"INVALID: STACK EMPTY"<<endl;
	      return false;
	    }
	    ordered.pop();
 	    //cout<<"Popping Comma (PROJECTION):"<<endl;
	    temp = ordered.top();
	  }
	}
	else return false;

      }
      //cout<<"FOUND T_RPAR(Renming)"<<endl;
      ordered.pop();
      if(ordered.empty()){
	//cout<<"INVALID(Renaming): STACK EMPTY"<<endl;
	return false;
      }
      if(par_atomicexpression()){
	for (int i = 0; i < recur_depth; ++i) {
	  switch (return_types[i]) {
	  case T_PROJECT:
	    cout << "proj type" << endl;
	    break;
	  default:
	    break;
	  }
	}
	return true;
      }
      else return false;
    }
  }
  return false;
}
