%option caseless
%{
#include <iostream>
#include "Token.h"
#include <string>
#include <stack>
#include <vector>
#include <fstream>
#include <sstream>
#include <stdlib.h>
#include <stdio.h>
#include <cctype>


stack<Token> tokens;
int errors= 0;
%}
MOVE [a-h]{1}[1-8]{1}
Newline "\n"
ID [a-z]|[a-z0-9.]*
DIGIT [0-9]
%%
{DIGIT}+ {  
            cout<<"Integer VALUE:"<<yytext<<endl;
            Token temp(T_INTVALUE,atoi(yytext));
            tokens.push(temp);
        }
{Newline}  { 
}
" "  { 
}
EXIT|DISPLAY|UNDO|HUMAN-AI|AI-AI  {
            // Push Command to Stack
            cout<<"COMMAND "<<yytext<<endl;
            string s(yytext);
            for(int i =0;i<s.length();++i)
                s[i] = toupper(s[i]);
            if(s=="EXIT"){
                Token temp(T_EXIT);
                tokens.push(temp);
            }
            if(s=="DISPLAY"){
                Token temp(T_DISPLAY);
                tokens.push(temp);
            }
            if(s=="UNDO"){
                Token temp(T_UNDO);
                tokens.push(temp);
            }
            if(s=="HUMAN-AI"){
                Token temp(T_HUMANAI);
                tokens.push(temp);
            }
            if(s=="AI-AI"){
                Token temp(T_AIAI);
                tokens.push(temp);
            }
            cout<<s<<endl;
}
FWD|LEFT|RIGHT  {
            // Push string identifier type token to stack 
            cout<<"DIRECTION: "<<yytext<<endl;
            string s(yytext);
            for(int i =0;i<s.length();++i)
                s[i] = toupper(s[i]);
            if(s=="FWD"){
                Token temp(T_FWD);
                tokens.push(temp);
            }
            if(s=="LEFT"){
                Token temp(T_LEFT);
                tokens.push(temp);
            }
            if(s=="RIGHT"){
                Token temp(T_RIGHT);
                tokens.push(temp);
            }
}
EASY|MEDIUM|HARD    {
            cout<<"Difficulty: "<<yytext<<endl;
            string s(yytext);
            Token temp;
            for(int i =0;i<s.length();++i)
                s[i] = toupper(s[i]);
            if(s=="EASY"){
                Token temp(T_EASY);
                tokens.push(temp);
            }
            if(s=="MEDIUM"){
                Token temp(T_MEDIUM);
                tokens.push(temp);
            }
            if(s=="HARD"){
                Token temp(T_HARD);
                tokens.push(temp);
            }
}
IP+" "[{0-9}+"."{0-9}+]+    {
            cout<<"IP ADDRESS:"<<yytext<<endl;
            string s(yytext);
            s.erase(0,3);
            Token temp(T_IP, s);
            tokens.push(temp);

}
IP+" "{ID}    {
            cout<<"IP ADDRESS:"<<yytext<<endl;
            string s(yytext);
            s.erase(0,3);
            Token temp(T_IP, s);
            tokens.push(temp);
            
}
";".*\n  { 
        cout<<"Comment"<<yytext;
        string s(yytext);
        Token temp(T_COMMENT,s);
        tokens.push(temp);
}
{MOVE}  { 
        cout<<"Move:";
        string s(yytext);
        Token temp(T_MOVE,s);
        cout<<s<<endl;
        tokens.push(temp);
}
{ID} {
            cout<<"An IDENTIFIER: " <<yytext<<endl;
            string s(yytext);
            Token temp(T_IDENTIFIER,s);
            tokens.push(temp);
}
.   {
    cout<<"INPUT ERROR"<<endl;
    ++errors;
}
%%
