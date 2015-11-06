#ifndef TOKEN_H
#define TOKEN_H

#include <iostream>
using namespace std;

// Set up enumeration of all symbols and command types 
// in the DML 
typedef enum { 
  T_LPAR ,T_PLUS,T_MINUS, T_MULTIPLY, T_DIVIDE, T_MOD, T_RPAR, T_SELECT,
  /* "("  "+"     "-"         "*"         "/"     "%"    ")"      "SELECT" */
  T_ASSIGN, T_SEMICOLON, T_QUOTE, T_COMMA, T_TWOEQUALS, T_EQUAL, T_STRING,
  /*  "<-"       ";"        '"'     ","        "=="       "="       "STRING" */  
  T_NOTEQUAL, T_LESS, T_LESSEQUAL, T_GREATER, T_GREATEREQUAL, T_NULL, T_SAVE,
  /*  "!="      "<"       "<="        ">"         ">="      "NULL"      "SAVE" */
  T_ONEAND, T_TWOAND, T_ONEOR, T_TWOOR, T_CREATE, T_INSERT, T_PROJECT,
  /* "&"       "&&"      "|"      "||"   "CREATE"  "INSERT"  "PROJECT" */
  T_SHOW, T_RENAME, T_INTO, T_TABLE, T_VALUE, T_FROM, T_PRIMARYKEY, T_KEY,
  /*"SHOW" "RENAME" "INTO"  "TABLE"   "VALUE" "FROM"  "PRIMARY KEY" "KEY" */
  T_RELATION, T_WRITE, T_OPEN, T_CLOSE, T_DELETE, T_UPDATE, T_SET, T_WHERE,
  /*"RELATION" "WRITE" "OPEN"   "CLOSE"  "DELETE"  "UPDATE"  "SET"  "WHERE"*/
  T_INTTYPE, T_INTVALUE, T_DOUBLEVALUE, T_VARCHAR, T_IDENTIFIER, T_CHAR,
  /*"INT"   "INT=#"        "DOUBLE=#"   "VARCHAR"     "NAME"     "CHAR"*/
  T_EXIT
} TokenType;

string TokenType_Name[] = {
  "T_LPAR" ,"T_PLUS","T_MINUS", "T_MULTIPLY", "T_DIVIDE", "T_MOD", "T_RPAR", "T_SELECT",
  /* "("       "+"       "-"         "*"         "/"        "%"       ")"      "SELECT" */
  "T_ASSIGN", "T_SEMICOLON", "T_QUOTE", "T_COMMA", "T_TWOEQUALS", "T_EQUAL", "T_STRING",
  /*  "<-"          ";"        '"'          ","        "=="       "="       "STRING" */  
  "T_NOTEQUAL", "T_LESS", "T_LESSEQUAL", "T_GREATER", "T_GREATEREQUAL", "T_NULL", "T_SAVE",
  /*  "!="          "<"       "<="           ">"           ">="          "NULL"  "SAVE" */
  "T_ONEAND", "T_TWOAND", "T_ONEOR", "T_TWOOR", "T_CREATE", "T_INSERT", "T_PROJECT",
  /* "&"         "&&"         "|"      "||"       "CREATE"    "INSERT"    "PROJECT" */
  "T_SHOW", "T_RENAME", "T_INTO", "T_TABLE", "T_VALUE", "T_FROM", "T_PRIMARYKEY", "T_KEY",
  /*"SHOW"   "RENAME"    "INTO"    "TABLE"   "VALUE"     "FROM"     "PRIMARY KEY"  "KEY" */
  "T_RELATION", "T_WRITE", "T_OPEN", "T_CLOSE", "T_DELETE", "T_UPDATE", "T_SET", "T_WHERE",
  /*"RELATION"   "WRITE"     "OPEN"   "CLOSE"     "DELETE"    "UPDATE"   "SET"    "WHERE"*/
  "T_INTTYPE", "T_INTVALUE", "T_DOUBLEVALUE", "T_VARCHAR", "T_IDENTIFIER", "T_CHAR",
  /*"INT"       "INT=#"        "DOUBLE=#"       "VARCHAR"     "NAME"     "CHAR"*/
  "T_EXIT", "T_NEWLINE"
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
