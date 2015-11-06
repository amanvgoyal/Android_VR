#ifndef PARSER_H
#define PARSER_H

#include <stack>
#include <iostream>
#include "Database.h"
#include <vector>

typedef std::map<std::string, std::vector<std::string> > table;
typedef std::map<std::string, table> table_list;

class Parser
{
 private:
  std::stack<Token> ordered;
  std::vector<bool> list;
  Database db;
 public:
  /*------------MAINTENANCE------------------------*/
  bool reverse_order(std::stack<Token>& theStack);
  bool par_line();
  bool par_empty();
  int  par_stacksize();
  bool print_result(std::string &s);
  /*------------PROGRAM----------------------------*/
  bool par_program(std::stack<Token>& theStack, int error_count);
  bool par_command();
  bool par_query();
  bool par_relationName();
  bool par_literals();
  bool par_condition();
  bool par_conjuction();
  bool par_atomicexpression();
  bool par_comparison(); //x
  bool par_expression();
  bool par_execute_query();
  bool par_operand();
  bool par_op();
  /*------------EXPRESSION-------------------------*/
  bool par_selection(); //-
  bool par_projection(); //x	
  bool par_renaming();
  bool par_union();
  bool par_difference();
  bool par_product();
  /*------------COMMANDS---------------------------*/
  bool par_open(); //-
  bool par_close(); //x
  bool par_save(); //x
  bool par_exit(); //x
  bool par_show(); //x
  bool par_create(); //x
  bool par_update(); //-
  bool par_insert(); //-
  bool par_delete(); //x
  
  std::vector<std::string> open_vals;
  std::vector<std::string> create_attrs;
  std::vector<std::vector<std::string> > comp_ops;
  table insert_vals;
  table project_vals; 

  std::vector<TokenType> return_types;
  int recur_depth = 0;
};
#endif 
