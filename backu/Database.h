#ifndef DATABASE_H
#define DATABASE_H

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>

#define PRIMARY_KEY "$primary_key"

typedef std::map<std::string, std::vector<std::string> > table;
typedef std::map<std::string, table> table_list;

class Database {
 public:
  Database();
  table selection(table, std::string, std::string, std::string);			// returns the tuples specified by the condition
  table selection(std::string, std::string, std::string, std::string);	// overloaded to accept a string table name argument
  table projection(table, std::vector<std::string>);						// returns the specified attributes
  table projection(std::string, std::vector<std::string>);				// overloaded to accept a string table name argument
  table renaming(table, std::vector<std::string>, std::vector<std::string>);			// renames the table
  table renaming(std::string, std::vector<std::string>, std::vector<std::string>);	// renames the table
  table set_union(table, table);								// returns the combined data and removes duplicates
  table set_union(std::string, std::string);								// overloaded to accept strings of table names
  table set_diff(table, table);											// returns the set difference of the two tables
  table set_diff(std::string, std::string);								// overloaded to accept strings of table names
  table cross_product(table, table);										// returns a table that pairs every row of both tables
  table cross_product(std::string, std::string);							// overloaded to accept strings of table names

  void open(std::string);													// ......
  void close(std::string);												// removes the specified table from db_copy
  void exit();															// exits the program
  void delete_table(std::string);											// deletes the specified table
  void save(table, std::string);											// saves the table in a .db file
  void show(table);														// displays the table in the command prompt
  void show(std::string);													// overloaded to accept a string table name
  table update(table, std::string, std::string, std::string,				// updates rows that match the condition
	       std::vector<std::string>, std::vector<std::string>); 
  table update(std::string, std::string, std::string, std::string,		// overload of update that accepts the table name as a string
	       std::vector<std::string>, std::vector<std::string>);
  table create(std::vector<std::string>, std::vector<std::string>);									// creates a new table with the specified name
  table insert_tuple(table, table);										// inserts a new row into the table
  table insert_tuple(std::string, table);									// overload of insert_tuple that accepts string
  table delete_tuple(table, std::string, std::string,	std::string);		// deletes rows that match the condition
  table delete_tuple(std::string, std::string, std::string, std::string);	// overload of delete_tuple that accepts the table name as a string
  void update_mat(std::string); 
  void print_db();
  table_list return_DB();
 private:
  std::fstream fs;
  table_list db_copy;
  //table primary_keys;
  bool mat_updated;

  bool numerical_str(std::string&);
  bool file_exists(std::string);
};

#endif
