/* 
    COMPILE COMMANDS:
    flex scanner.l
    g++ lex.yy.c -lfl
    ./a.out
*/
%{
#include <iostream>
#include "Token.h"
#include <string.h>
#include <stdlib.h>
#include <stack>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include "Parser.h"
 
using namespace std;

stack<Token> tokens;
int errors= 0;
%}

ALPHA [a-zA-Z]
DIGIT [0-9]
OPERATOR "+"|"-"|"*"|"/"|"="|"<"|">"
SPACE []+
WS " "
ID [a-zA-Z]|[a-zA-Z0-9_]*    
TYPE "VARCHAR("[0-9]*")"
Newline "\n"
DELETE "DELETE"
FROM "FROM"
CREATE "CREATE"
TABLE "TABLE"
LEFTPAR "("
RIGHTPAR ")"
COMMA ","
PRIMARY "PRIMARY"
KEY "KEY"
SEMICOLON ";"
INSERT "INSERT"
INTO "INTO"
VALUES "VALUES"
SET "SET"
WHERE "WHERE"
BEG_STRING (\"[^"\n]*)
STRING ({BEG_STRING}\")
SHOW "SHOW"
LEFTARROW "<-"
SELECT "select"
RELATION "RELATION"
PROJECT "project"
RENAME "rename"
DOUBLEAND "&&"
DOUBLEOR "||"
EXIT "EXIT"
OPEN "OPEN"
CLOSE "CLOSE"
SAVE "SAVE"
UPDATE "UPDATE"
ANDY "&"
ORY "|"
%%
{DIGIT}+ {  
            // Identifies and passes integers to stack 
            //cout<<"Integer VALUE:"<<yytext<<endl;
            Token temp(T_INTVALUE,atoi(yytext));
            tokens.push(temp);
        }
{DIGIT}+"."{DIGIT}* {
            // Identifies and passes Doubles to stack 
            //cout<<"Double VALUE:"<<yytext<<endl;
            Token temp(T_DOUBLEVALUE,atof(yytext));
            tokens.push(temp);
}
{OPERATOR}  { 
            // Check what operator the string is, pass token with type 
            // corresponding to that operator
            //cout<<"OPERATOR:"<<yytext<<endl;
            string s(yytext); 
            switch(yytext[0]){
                case '+':{  
                    Token temp(T_PLUS,s);
                    tokens.push(temp);
                    break;
                }
                case '-':{
                    Token temp(T_MINUS,s);
                    tokens.push(temp);
                    break;
                }
                case '*':{
                    Token temp(T_MULTIPLY,s);
                    tokens.push(temp);
                    break;
                }
                case '/':{
                    Token temp(T_DIVIDE,s);
                    tokens.push(temp);
                    break;
                }
                case '=':{
                    Token temp(T_EQUAL,s);
                    tokens.push(temp);
                    break;
                }
                case '<':{
                    Token temp(T_LESS,s);
                    tokens.push(temp);
                    break;
                }
                 case '>':{
                    Token temp(T_GREATER,s);
                    tokens.push(temp);
                    break;
                }
                default:{
                    break;
                }

            }
}
"=="  {             
            //cout<<"TWO EQUALS SIGN"<<endl;
            string s(yytext);
            Token temp(T_TWOEQUALS,s);
            tokens.push(temp);

}
"!="  {             
            //cout<<"NOT EQUALS SIGN"<<endl;
            string s(yytext);   
            Token temp(T_NOTEQUAL,s);
            tokens.push(temp);

}
"<="  {             
            //cout<<"LESS EQUALS SIGN"<<endl;
            string s(yytext); 
            Token temp(T_LESSEQUAL,s);
            tokens.push(temp);

}
">="  {             
            //cout<<"LESS EQUALS SIGN"<<endl;
            string s(yytext); 
            Token temp(T_GREATEREQUAL,s);
            tokens.push(temp);

}
{ID} {
        // push token with corresponding identifier type to stack
        //cout<<"An IDENTIFIER: " <<yytext<<endl;
            string s(yytext);
            Token temp(T_IDENTIFIER,s);
            tokens.push(temp);
}
{TYPE} {
        // push token with corresponding type and value to stack
        //cout<<"A TYPE:"<<yytext;
        std::string str(yytext);
        std::string temp;
        int number=0;
        for (unsigned int i=0; i < str.size(); i++)
        {
            if (isdigit(str[i]))
            {
                for (unsigned int a=i; a<str.size(); a++)
                {temp += str[a];
                }
              break;
            }    
        }
        istringstream stream(temp);
        stream >> number;
        //cout<<"With Value:"<<number<<endl; 
        Token temp1(T_VARCHAR,number);
        tokens.push(temp1);
}
" "+"INTEGER" {
            //cout<<"A TYPE: " <<yytext<<endl;
            Token temp(T_INTTYPE);
            tokens.push(temp);
}
{Newline}  { 
}
" "  { 
}
{DELETE}+" "  {
            // Push token with delete command type to stack 
            //cout<<"DELETE COMMAND"<<endl;
            Token temp(T_DELETE);
            tokens.push(temp);
}
{CREATE}+" "  {
            // Push token with create command type to stack
            //cout<<"CREATE COMMAND"<<endl;
            Token temp(T_CREATE);
            tokens.push(temp);
}
{WHERE}+" "  {
            // Push token with where command type to stack 
            //cout<<"WHERE COMMAND"<<endl;
            Token temp(T_WHERE);
            tokens.push(temp);
}
{TABLE}+" "  {
            // Push token with table type to stack 
            //cout<<"TABLE COMMAND"<<endl;
            Token temp(T_TABLE);
            tokens.push(temp);
}       
{LEFTPAR}  {
            // Push left parenthesis to stack
            //cout<<"LEFTPAR"<<endl;
            Token temp(T_LPAR);
            tokens.push(temp);
}
{SET}+" "  {
            // Push token with word type to stack 
            //cout<<"SET"<<endl;
            Token temp(T_SET);
            tokens.push(temp);
}
{RIGHTPAR}  {
            // Push right parenthesis token to stack 
            //cout<<"RIGHTPAR"<<endl;
            Token temp(T_RPAR);
            tokens.push(temp); 
}
{COMMA}  {
            // Push comma token to stack
            //cout<<"COMMA"<<endl;
            Token temp(T_COMMA);
            tokens.push(temp); 
}
{FROM}+" " {
            // Push from key word to stack
            //cout<<"COMMAND FROM"<<endl;
            Token temp(T_FROM);
            tokens.push(temp);  
}
{PRIMARY}+" "+{KEY}  {
            // Push primary key token to stack
            //cout<<"PRIMARY KEY RECIEVED"<<endl;
            Token temp(T_PRIMARYKEY);
            tokens.push(temp); 
}
{SEMICOLON}  {
            // Push semi colon to stack 
            //cout<<"ENDSENTENCE"<<endl;
            Token temp(T_SEMICOLON);
            tokens.push(temp); 
}
{INSERT}+" "  {
            // Push insert command type token to stack 
            //cout<<"INSERT COMMAND"<<endl;
            Token temp(T_INSERT);
            tokens.push(temp);
}
{INTO}+" "  {
            // Push into key word token to stack 
            //cout<<"INTO COMMAND"<<endl;
            Token temp(T_INTO);
            tokens.push(temp);
}
{STRING}  {
            // Push string identifier type token to stack 
            //cout<<"STRING"<<yytext<<endl;
            string s(yytext);
            Token temp(T_STRING, s);
            tokens.push(temp);
}
{VALUES}+" "  {
            // Push token with value type to stack 
            //cout<<"GET VALUES COMMAND"<<endl;
            Token temp(T_VALUE);
            tokens.push(temp);
}
{SHOW}+" "  {
            // Push token with show command type to stack 
            //cout<<"SHOW COMMAND"<<endl;
            Token temp(T_SHOW);
            tokens.push(temp);
}
{LEFTARROW}  {
            // Push token of type left arrow to stack 
            //cout<<"LEFTARROW"<<endl;
            Token temp(T_ASSIGN);
            tokens.push(temp); 
}
{SELECT}+" "  {
            // Push token with select command type to stack 
            //cout<<"SELECT COMMAND"<<endl;
            Token temp(T_SELECT);
            tokens.push(temp);
}
{RELATION}+" "  {
            // Push token with relation type to stack 
            //cout<<"GET VALUES FROM RELATION"<<endl;
            Token temp(T_RELATION);
            tokens.push(temp);
}
{PROJECT}+" "  {
            // Push token with project command type to stack 
            //cout<<"PROJECT COMMAND"<<endl;
            Token temp(T_PROJECT);
            tokens.push(temp); 
}
{RENAME}+" "  {
            // Push token with rename command type to stack 
            //cout<<"RENAME COMMAND"<<endl;
            Token temp(T_RENAME);
            tokens.push(temp);
}
{ANDY}  {
            // Push ampersand type token to stack   
            //cout<<"ONE AND"<<endl;
            Token temp(T_ONEAND);
            tokens.push(temp); 
}
{DOUBLEAND}  {
            // Push logical and type token to stack 
            //cout<<"COMPARISON(&&)"<<endl;
            Token temp(T_TWOAND);
            tokens.push(temp); 
}
{ORY}  {
            // Push or type token to stack 
            //cout<<"ONE OR"<<endl;
            Token temp(T_ONEOR);
            tokens.push(temp); 
}
{DOUBLEOR}  {
            // Push logical or type token to stack 
            //cout<<"COMPARISON(||)"<<endl;
            Token temp(T_TWOOR);
            tokens.push(temp);
}
{EXIT}+";"  {
            // Push exit command type token to stack 
            //cout<<"EXIT COMMAND"<<endl;
            Token temp(T_EXIT);
            Token temp1(T_SEMICOLON);
            tokens.push(temp); 
            tokens.push(temp1);
}
{OPEN}+" "  {
            // Push open command type token to stack 
            //cout<<"OPEN COMMAND"<<endl;
            Token temp(T_OPEN);
            tokens.push(temp); 
}
{CLOSE}+" "  {
            // Push close command type token to stack 
            //cout<<"CLOSE COMMAND"<<endl;
            Token temp(T_CLOSE);
            tokens.push(temp); 
}
{SAVE}+" "  {
            // Push save command type token to stack 
            //cout<<"SAVE COMMAND"<<endl;
            Token temp(T_SAVE);
            tokens.push(temp);
}
{UPDATE}+" "  {
            // Push update command type token to stack 
            //cout<<"UPDATE COMMAND"<<endl;
            Token temp(T_UPDATE);
            tokens.push(temp);
}
.   {//cout<<"INPUT ERROR"<<endl;
    ++errors;
}
%%