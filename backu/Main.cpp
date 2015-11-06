#include <iostream>
#include "Database.h"
#include "Parser.h"
#include "Token.h"

using namespace std;

int main() {
  // Database db;
  //db.open("animals");
  //db.show("animals");
  ifstream infile;
  string S;
  Parser dp;
  string input_file = "Input.txt";
  string output_file = "Output.txt";
  infile.open(input_file.c_str());

  // Read each line, tokenize, pass token stack to parser
  // Parser then verifies/refutes strings
  while(!infile.eof()){
    //    cout<<"Reading From File"<<endl;
    getline(infile,S);
    const char * c = S.c_str();
    YY_BUFFER_STATE bp = yy_scan_string(c);
    yy_switch_to_buffer(bp);
    yylex();

    dp.par_program(tokens,errors);
    yy_delete_buffer(bp);
    
    while(!tokens.empty()){
      tokens.pop();
    }
    dp.par_empty();
  }
  infile.close();
  //cout<<"-------------------------------------------------------------"<<endl;
  dp.print_result(output_file);

  return 0;
}


