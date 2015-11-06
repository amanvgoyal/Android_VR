#include <iostream>
#include <map>
#include <algorithm>
#include "Parser.h"

bool Parser::reverse_order(stack<Token>& theStack){
  size_t temp = theStack.size();
  while(!theStack.empty()){
    ordered.push(theStack.top());
    theStack.pop();
  }
  if(temp == ordered.size()) return true;
  return false;
}
bool Parser::check_pass(string s){
  if(s==password)return true;
  else return false;
}
string Parser::parse(string S){
  Parser dp;
  const char * c = S.c_str();
    YY_BUFFER_STATE bp = yy_scan_string(c);
    yy_switch_to_buffer(bp);
    yylex();

    string temp = dp.par_program(tokens,errors);
    yy_delete_buffer(bp);
    errors =0;
    while(!tokens.empty()){
      tokens.pop();
    }
    dp.par_empty();
    return temp;

}
void Parser::set_pass(string s){
  password = s;
}
string Parser::par_program(stack<Token>& theStack, int error_counter){
  reverse_order(theStack); // Swap stack from abc to cba
  if(error_counter==0){ //If tokenizer caught errors it i 
    while(!ordered.empty()){ //Keep popping until nothing is left 
      string temp;
      temp = par_line();
      if(temp=="FALSE"){ //call Par_line which calls query or commands
		par_empty();//empty the stack becase statement is invalid
    return "FALSE";
      }
    else {
      cout<<"Done"<<endl;
      return temp;
    }
		par_empty();
    }
  }
  if(error_counter>0){// if there were errors found ABORT
    par_empty();//empty the stack
    return "FALSE";
  }
}
bool Parser::par_empty(){
  if(!ordered.empty()){
    while(!ordered.empty()){
      ordered.pop();
    }
    return true;
  }
  return false;
}
int Parser::par_stacksize(){
  return ordered.size();
}
string Parser::par_display(){
  if(counter!=-1){
  string temp = Game.output_board();
  return temp;
  }
  string temp = "NO GAME HAS STARTED";
  return temp;
}
string Parser::par_line(){
  Token temp = ordered.top();
  if(temp.get_type()==T_IDENTIFIER){
        return "FALSE";
    }
  if(counter!=-1){
    string move;
    if(temp.get_type()==T_EASY||temp.get_type()==T_MEDIUM||temp.get_type()==T_HARD){
        return "TRUE";
    }
    if(temp.get_type()==T_MOVE){
      move = temp.get_string_value();
      cout<<"Move"<<move<<endl;
      ordered.pop();
      temp = ordered.top();
      //bool move_piece(int rows, char cols, Direction direction);
        if(temp.get_type()==T_FWD){
          ordered.pop();
          cout<<"MOVE[0] "<<move[0]<<" MOVE[1]"<<move[1]<<endl;
          string row;
          row.insert(0,1,move[1]);
          cout<<"This is what row has:"<<row<<endl;
          if(Game.move_piece(atoi(row.c_str()),move[0],FORWARD)){
            Game.end_turn();
            return par_display();
          }
          else return "FALSE";
        }
        else if(temp.get_type()==T_LEFT){
          ordered.pop();
          cout<<"MOVE[0] "<<move[0]<<" MOVE[1]"<<move[1]<<endl;
          string row;
          row.insert(0,1,move[1]);
          if(Game.move_piece(atoi(row.c_str()),move[0],LEFT)){
            Game.end_turn();
            return par_display();
          }
          else return "FALSE";
        }
        else if(temp.get_type()==T_RIGHT){
          ordered.pop();
          cout<<"MOVE[0] "<<move[0]<<" MOVE[1]"<<move[1]<<endl;
          string row;
          row.insert(0,1,move[1]);
          if(Game.move_piece(atoi(row.c_str()),move[0],RIGHT)){
            Game.end_turn();
            return par_display();
          }
          else return "FALSE";
        }
    }
    if(temp.get_type()==T_EXIT){
        ordered.pop();
        return "EXIT";
    }
    if(temp.get_type()==T_DISPLAY){
        ordered.pop();
        string temp = par_display();
        return temp;
    }
    if(temp.get_type()==T_UNDO){
        ordered.pop();
        if(Game.undo_move()){
          return par_display();
        }
        return "FALSE";
    }
  }
  if(temp.get_type()==T_HUMANAI){
      ordered.pop();
  		temp = ordered.top();
  		if(temp.get_type()==T_EASY||temp.get_type()==T_MEDIUM||temp.get_type()==T_HARD){
  			switch(temp.get_type()){
  				case T_EASY:{
  					ordered.pop();
            ++counter;
  					random();
  					return "TRUE";
  					break;
  				}
  				case T_MEDIUM:{
  					ordered.pop();
            ++counter;
  					mini_max();
  					return "TRUE";
  					break;
  				}
  				case T_HARD:{
  					ordered.pop();
            ++counter;
  					alpha_beta();
  					return "TRUE";
  					break;
  				}
  				default : return "FALSE";
  			}
  		}
  		else return "FALSE";
  }
  temp =ordered.top();
   if(temp.get_type()==T_AIAI){
  		ordered.pop();
      if(ordered.empty()) return "FALSE";
  		temp = ordered.top();
  		if(temp.get_type()==T_IP){
  			string  server = temp.get_string_value(); 
  			ordered.pop();
  			temp = ordered.top();
  			if(temp.get_type()==T_INTVALUE){
  				int port = temp.get_num();
  				ordered.pop();
  				temp = ordered.top();
  				if(temp.get_type()==T_IDENTIFIER){
  					string password = temp.get_string_value();
  					ordered.pop();
  					temp = ordered.top();
  					if(temp.get_type()==T_EASY||temp.get_type()==T_MEDIUM||temp.get_type()==T_HARD){
  						string my_difficulty;
  						switch(temp.get_type()){
			  				case T_EASY:{
			  					ordered.pop();
			  					my_difficulty = "EASY";
			  					break;
			  				}
			  				case T_MEDIUM:{
			  					ordered.pop();
			  					my_difficulty = "MEDIUM";
			  					break;
			  				}
			  				case T_HARD:{
			  					ordered.pop();
			  					my_difficulty = "Hard";
			  					break;
			  				}
			  				default : return "FALSE";
			  			}
  						temp = ordered.top();
  						string opp_difficulty;
  						if(temp.get_type()==T_EASY||temp.get_type()==T_MEDIUM||temp.get_type()==T_HARD){
	  								switch(temp.get_type()){
				  				case T_EASY:{
				  					ordered.pop();
                    ++counter;
				  					opp_difficulty = "EASY";
				  					break;
				  				}
				  				case T_MEDIUM:{
				  					ordered.pop();
                    ++counter;
				  					opp_difficulty = "MEDIUM";
				  					break;
				  				}
				  				case T_HARD:{
				  					ordered.pop();
                    ++counter;
				  					opp_difficulty = "Hard";
				  					break;
				  				}
				  				default : return "FALSE";
				  			}
				  			//server(server name, port number, password, my_difficulty, opponenet difficulty)
				  			return "TRUE";
  						}
  					}
  				}

  			}
  		}
  }
  else if(counter==-1) return par_display();
  return "FALSE"; //if none of the above conditions match its not part of our language
}
void Parser::random(){

}
void Parser::mini_max(){
	
}
void Parser::alpha_beta(){
	
}