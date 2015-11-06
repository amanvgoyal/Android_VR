#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
using namespace std;

// Set up enumeration of all symbols and command types 
// in the DML 
typedef enum { 

   T_IDENTIFIER , T_EXIT , T_DISPLAY , T_UNDO , T_HUMANAI , T_AIAI , T_NULL,

   T_FWD , T_LEFT, T_RIGHT, T_EASY, T_MEDIUM , T_HARD , T_IP, T_COMMENT , T_COLUMN ,

   T_ROW , T_INTVALUE , T_MOVE
} TokenType;

string TokenType_Name[] = {
   "T_IDENTIFIER", "T_EXIT" , "T_DISPLAY" , "T_UNDO" , "T_HUMANAI" , "T_AIAI" ,

   "T_FWD" , "T_LEFT", "T_RIGHT", "T_EASY", "T_MEDIUM" , "T_HARD" , "T_IP", "T_COMMENT" , "T_COLUMN" ,

   "T_ROW" , "T_NULL" ,"T_INTVALUE" , "T_MOVE"
};

class Token
{
 private:
  TokenType type; // Type corresponding to value in above enum
  string s; // Used to check if contained enitity is a string or number
  double number; // Represents numerical value passed in command 
 public:
 Token():type(T_NULL){}
 Token(const Token&t):type(t.type),s(t.s),number(t.number){}
  Token& operator =(const Token&t){
    type = t.type;
    s = t.s;
    number = t.number;
    return *this;
  }
  
  // Used to check what symbol a token fits into in the above enumeration
  TokenType get_type(){
    return type;
  }

 Token(TokenType type):type(type){}

 Token(TokenType type, double a):type(type),number(a){}

 Token(TokenType type, string s):type(type),s(s){}
  string get_type_string(){return TokenType_Name[type];}
  string get_string_value(){
    return s;
  }

  // Used to get value of numbers passed as values to Insert
  int get_num(){
    return number;
  } 

  ~Token(){}
    
};

#endif 
