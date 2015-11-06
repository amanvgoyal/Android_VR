#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include <iostream>
#include <vector>
#include "Token.h"
#include "Board.h"


Board Game;
int counter =-1;

class Parser
{
 private:
  std::stack<Token> ordered;
  std::string password = "password";
 public:
  /*------------MAINTENANCE------------------------*/
  bool reverse_order(std::stack<Token>& theStack);
  std::string par_line();
  bool par_empty();
  int  par_stacksize();
  bool check_pass(std::string s);
  void set_pass(std::string s);
  std::string parse(std::string S);
  /*------------PROGRAM----------------------------*/
  std::string par_program(std::stack<Token>& theStack, int error_count);
  std::string par_statment();
  std::string par_display();
  /*------------Difficulty----------------------------*/
  void random();
  void mini_max();
  void alpha_beta();
};
#endif 