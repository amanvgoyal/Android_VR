#include "Database.h"
#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

typedef map<string, vector<string> > table;
typedef map<string, table> table_list;

bool Database::numerical_str(string& s) {
  string::const_iterator it = s.begin();
  while (it != s.end()) {
    if (!isdigit(*it)) { return false; }
    ++it;
  }
  return true;
}

Database::Database() {
  //mat_updated = false;
}

bool Database::file_exists(string file_name) {
  ifstream ifs(file_name);
  if (ifs.good()) { return true; }
  return false;
}

// In relation 'table_name', if attribute 'lhs' exists,
// check entries 'x' satisfying x op rhs
table Database::selection(string table_name,
			  string lhs, string op, string rhs) {
  table_list::iterator it = db_copy.find(table_name);
  if (it != db_copy.end())
    {
      table t = db_copy[table_name];
      return selection(t, lhs, op, rhs);
    }
  else
    {
      throw invalid_argument("No such table open!\n");
      return table();
    }
}

// In relation 'table_name', if attribute 'lhs' exists,
// check entries 'x' satisfying x op rhs
table Database::selection(table t,
			  string lhs, string op, string rhs) {
  //table_list::iterator table_it = db_copy.find(table_name);
  table result;
  vector<string> selected;
  // add fitting selection from db_copy to result
  vector<string> v = t[lhs];
  // add the primary_key
  result[PRIMARY_KEY] = t[PRIMARY_KEY];

  for (int i = 0; i < v.size(); ++i) {
    if (op != "==") {
      if (!numerical_str(rhs)) {
	cout << "Non numerical input!" << endl;
	return result;
      }

      else {
	if ((op == "<" && stod(v[i]) < stod(rhs)) ||
	    (op == "<=" && stod(v[i]) <= stod(rhs)) ||
	    (op == ">" && stod(v[i]) > stod(rhs)) ||
	    (op == ">=" && stod(v[i]) >= stod(rhs))) {
	  for (auto it : t) {
	    if (it.first != PRIMARY_KEY)
	      {
		selected = t[it.first];
		result[it.first].push_back(selected[i]);
	      }
	  }
	}
      }
    }

    // Handles equality for both words and strings
    else if (op == "==") {
      if (v[i] == rhs) {
	for (auto it : t) {
	  if (it.first != PRIMARY_KEY)
	    {
	      selected = t[it.first];
	      result[it.first].push_back(selected[i]);
	    }
	}
      }
    }
    else if (op == "!=") {
      if (v[i] != rhs) {
	for (auto it : t) {
	  if (it.first != PRIMARY_KEY)
	    {
	      selected = t[it.first];
	      result[it.first].push_back(selected[i]);
	    }
	}
      }
    }
    else if (op == "*") {
      for (auto it : t) {
	if (it.first != PRIMARY_KEY)
	  {
	    selected = t[it.first];
	    result[it.first].push_back(selected[i]);
	  }
      }
    }
  }

  return result;
}



table Database::projection(table tbl, vector<string> attributes) {
  table result;
  table::iterator it;
  // add primary_key
  result[PRIMARY_KEY] = tbl[PRIMARY_KEY];
  for (int i = 0; i < attributes.size(); ++i)
    {	// find the columns to get
      it = tbl.find(attributes[i]);
      if (it != tbl.end())
	{	// grab the values
	  result[attributes[i]] = tbl[attributes[i]];
	}
    }
  return result;
}

table Database::projection(string tbl_name, vector<string> attributes)
{
  table_list::iterator it = db_copy.find(tbl_name);
  if (it != db_copy.end())
    {
      return projection(it->second, attributes);
    }
  else
    {
      throw invalid_argument("Table " + tbl_name + " not found!");
      return table();
    }
}

table Database::renaming(table tbl, vector<string> old_attr, vector<string> new_attr) {
  table new_table;

  table::iterator it = tbl.begin();
  while (it != tbl.end())
    {
      vector<string>::iterator vec_it = find(old_attr.begin(), old_attr.end(), it->first);
      if (vec_it != old_attr.end())
	{
			
	  int index = vec_it - old_attr.begin();
	  // Do not rename primary_key column
	  if(old_attr[index] != PRIMARY_KEY) new_table[new_attr[index]] = it->second;
	}
      else
	{
	  new_table[it->first] = it->second;
	}
      ++it;
    }

  return new_table;
}

table Database::renaming(string tbl_name, vector<string> old_attr, vector<string> new_attr) {
  if (db_copy.count(tbl_name) > 0)
    {
      return renaming(db_copy[tbl_name], old_attr, new_attr);
    }
  else
    {
      throw invalid_argument("Table " + tbl_name + " not found!");
      return table();
    }
}

table Database::set_union(table tbl1, table tbl2) {
  // check if tables exist in the database
  table result;
  table::iterator it1 = tbl1.begin();
  table::iterator it2 = tbl2.begin();

  // check if the two tables have the same columns
  bool same_attributes = true;
  for (it1; it1 != tbl1.end(); ++it1)
    {
      same_attributes = (it1->first != it2->first) ? false : true;
      ++it2;
    }
  if (same_attributes)
    {
      it1 = tbl1.begin();
      it2 = tbl2.begin();
      // insert tuples from first table
      for (it1; it1 != tbl1.end(); ++it1)
	{
	  result[it1->first] = it1->second;
	}
      // insert tuples from second table and check if duplicates exist
      table::iterator r_it = result.begin();
      bool duplicate_tuple = true;
      bool duplicate_primary_key = true;
      for (int i = 0; i < it2->second.size(); ++i)
	{	// start comparing the values of the first attribute in result
	  for (int j = 0; j < r_it->second.size(); ++j)
	    {	// to the values of the first attribute of table_2
	      // check if the tuple is a duplicate
	      if (duplicate_tuple)
		{
		  for (r_it; r_it != result.end(); ++r_it)
		    {	// iterate through all the columns and compare
		      // the values of the tuples
		      if (r_it != result.end() || it2 != tbl2.end())
			{
			  if (duplicate_tuple)
			    {
			      if (it2->second[i] != r_it->second[j])
				{
				  if (r_it->first == PRIMARY_KEY) duplicate_primary_key = false;
				  duplicate_tuple = false;
				}
			    }
			  else break;
			}
		      else break;
		      ++it2;
		    }
		  r_it = result.begin();
		  it2 = tbl2.begin();
		}
	      else break;
	    }

	  if (!duplicate_tuple && !duplicate_primary_key)
	    {	// add the row from table_2 to result if not a duplicate
	      for (r_it; r_it != result.end(); ++r_it)
		{	// iterate through all the columns and add 
		  // the value of the correct row
		  r_it->second.push_back(it2->second[i]);
		  ++it2;
		}
	      // reset variables
	      r_it = result.begin();
	      it2 = tbl2.begin();
	      duplicate_tuple = true;
	    }
	}
    }

  return result;
}

table Database::set_union(string tbl_name1, string tbl_name2)
{
  table_list::iterator it1 = db_copy.find(tbl_name1);
  table_list::iterator it2 = db_copy.find(tbl_name2);
  //table::iterator p_key = primary_keys.find(tbl_name1);
  if (it1 != db_copy.end())
    {
      if (it2 != db_copy.end())
	{
	  return set_union(it1->second, it2->second);
	}
      else
	{
	  throw invalid_argument("Table " + tbl_name1 + " not found!");
	}
    }
  else
    {
      throw invalid_argument("Table " + tbl_name1 + " not found!");
      return table();
    }
}

// A \ B = {a in A | a not in B}
table Database::set_diff(string table_name_1, string table_name_2) {
  table result;

  // check if tables exist in the database
  if (db_copy.count(table_name_1) > 0 && db_copy.count(table_name_2) > 0) {
    table table_1 = db_copy[table_name_1];
    table table_2 = db_copy[table_name_2];

    vector<string> t2_data;
    // For each attribute check if corresponding vector in tb1 is not in tb2
    // If, so add that attribute and its vector into result
    for (auto& it1 : table_1) {
      // Case where attributes match

      if (table_2.count(it1.first) > 0) {
	for (int i = 0; i < it1.second.size(); ++i) {
	  t2_data = table_2[it1.first];

	  // If elem not in attr vector 2, but in attr vector 1, put in result 
	  if (find(t2_data.begin(), t2_data.end(), it1.second[i])
	      == t2_data.end()) {
	    result[it1.first].push_back(it1.second[i]);
	  }
	}
      }

      // Case where attributes do not mach
      else {
	result[it1.first] = it1.second;
      }
    }
    // Add primary key
    result[PRIMARY_KEY] = table_1[PRIMARY_KEY];
  }
  return result;
}


// A \ B = {a in A | a not in b}
table Database::set_diff(table table_1, table table_2) {
  table result;

  vector<string> t2_data;

  // For each attribute check if corresponding vector in tb1 is not in tb2
  // If, so add that attribute and its vector into result
  for (auto& it1 : table_1) {
    // Case where attributes match

    if (table_2.count(it1.first) > 0) {
      for (int i = 0; i < it1.second.size(); ++i) {
	t2_data = table_2[it1.first];

	// If elem not in attr vector 2, but in attr vector 1, put in result 
	if (find(t2_data.begin(), t2_data.end(), it1.second[i])
	    == t2_data.end()) {
	  result[it1.first].push_back(it1.second[i]);
	}
      }
    }
    // Case where attributes do not mach
    else {
      result[it1.first] = it1.second;
    }
  }
  // Add primary key
  result[PRIMARY_KEY] = table_1[PRIMARY_KEY];
  return result;
}

table Database::cross_product(table tbl1, table tbl2) {
  // check if tables exist in the database
  table result;
  table::iterator it1 = tbl1.begin();
  table::iterator it2 = tbl2.begin();
  // add the attributes to the resulting table, even if they are duplicates
  for (it1; it1 != tbl1.end(); ++it1)
    {
      if (it1->first != PRIMARY_KEY) result[it1->first];
    }
  for (it2; it2 != tbl2.end(); ++it2)
    {	// check if there are duplicate attributes present
      if(it2->first == PRIMARY_KEY) ++it2;
      table::iterator dup = result.find(it2->first);
      if (dup == result.end())
	{
	  result[it2->first];
	}
      else
	{
	  char suffix = '1';
	  string dup_attribute_name = it2->first;
	  dup_attribute_name += suffix;
	  while (result.find(dup_attribute_name) != result.end())
	    {
	      dup_attribute_name = it2->first;
	      suffix += 1;
	    }
	  result[dup_attribute_name];
	}
    }
  // add the values to the attibutes
  table::iterator r_it = result.begin();
  it1 = tbl1.begin();
  it2 = tbl2.begin();
  if (it1->first == PRIMARY_KEY) ++it1;
  if (it2->first == PRIMARY_KEY) ++it2;
  for (int i = 0; i < it1->second.size(); ++i)
    {
      for (int j = 0; j < it2->second.size(); ++j)
	{
	  for (it1; it1 != tbl1.end(); ++it1)
	    {
	      if (it1->first != PRIMARY_KEY) r_it->second.push_back(it1->second[i]);
	      else ++it1;
	      ++r_it;
	    }
	  for (it2; it2 != tbl2.end(); ++it2)
	    {
	      if (it2->first != PRIMARY_KEY) r_it->second.push_back(it2->second[j]);
	      else ++it2;
	      ++r_it;
	    }
	  // reset iterators
	  it1 = tbl1.begin();
	  it2 = tbl2.begin();
	  if (it1->first == PRIMARY_KEY) ++it1;
	  if (it2->first == PRIMARY_KEY) ++it2;
	  r_it = result.begin();
	}
    }
  // add the primary key from table 1
  result[PRIMARY_KEY] = tbl1[PRIMARY_KEY];
  return result;
}

table Database::cross_product(string tbl_name1, string tbl_name2)
{
  table_list::iterator it1 = db_copy.find(tbl_name1);
  table_list::iterator it2 = db_copy.find(tbl_name2);
  if (it1 != db_copy.end())
    {
      if (it2 != db_copy.end())
	{
	  return cross_product(it1->second, it2->second);
	}
      else
	{
	  throw invalid_argument("Table " + tbl_name2 + " not found!");
	}
    }
  else
    {
      throw invalid_argument("Table " + tbl_name1 + " not found!");
      return table();
    }
}

void Database::open(string table_name) {
  update_mat(table_name);
}

//Removes a table from db_copy                                    
void Database::close(string table_name) {
  if (db_copy.count(table_name) > 0) {
    db_copy.erase(table_name);
  }
}


/* This performs the same function as close
   void Database::delete_table(string table_name) {
   table_list::iterator it;
   it = db_copy.find(table_name);
   if (it != db_copy.end())
   {
   db_copy.erase(it);
   }
   }*/

void Database::save(table t, string table_name) {
  if (file_exists(table_name)) { return; } // Throw exp

  fstream fs(table_name, fstream::in | fstream::out | fstream::trunc);
  // find primary key
  table::iterator key_it = t.find(PRIMARY_KEY);
  // write table_name minus ".db"
  // write table contents
  fs << table_name.substr(0, table_name.size() - 3) << ' ' << endl;
  for (auto it1 : t) {
    if (key_it->second[0] == it1.first) {
      // add the primary key to the file
      fs << "$        ";// << key_it->second[0] << endl;
      for (int i = 0; i < t["$primary_key"].size(); ++i) {
	fs << t["$primary_key"][i] << ' ';
      }
      fs << endl;
    }
    else {
      // add the attribute and its members to the file
      fs << "*        " << it1.first << ' ';
      for (auto it2 : t[it1.first]) {
	fs << it2 << ' ';
      }
      fs << endl;
    }
  }
  cout << "Done" << endl;
  fs.close();
}


// Halt execution
void Database::exit() {
  abort();
}

void Database::show(table t) {
  for (auto it1 : t) {
    cout << it1.first << " - ";
    for (auto it2 : t[it1.first]) {
      cout << it2 << ' ';
    }
    cout << endl;
  }
  cout << endl;
}

void Database::show(string tbl_name)
{
  table_list::iterator it = db_copy.find(tbl_name);
  if (it != db_copy.end())
    {
      show(it->second);
    }
  else
    {
      cerr << "The table does not exist!\n";
    }
}

table Database::update(table tbl, string cond_attr, string op, string cond_val, vector<string> attr_list, vector<string> val_list)
{
  // deletes the row specified by the condition
  table::iterator it = tbl.find(cond_attr);
  vector<int> row_numbers;
  // return the same table if primary key is going to be modified
  vector<string>::iterator v_it = find(attr_list.begin(), attr_list.end(), tbl[PRIMARY_KEY][0]);
  if (v_it != attr_list.end())
    {	// if the primary key is in the list of attributes to modify
      // check if the value that corresponds already exists
      int list_index = v_it - attr_list.begin();			// find the index that corresponds
      vector<string> key_values = tbl[tbl[PRIMARY_KEY][0]];
      key_values.push_back(val_list[list_index]);			// insert value to modify
      // check if the column is still unique if the value is inserted
      sort(key_values.begin(), key_values.end());
      if (unique(key_values.begin(), key_values.end()) != key_values.end())
	{	// duplicate primary key is trying to be put into the table
	  cerr << "The value " << val_list[list_index] << " already exists for key attribute " << attr_list[list_index] << ".\n";
	  cerr << "The table will not be modified.\n";
	}
    }
  // check the condition and select the appropriate comparison
  if (op == "==")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the rows to update
	  if (it->second[i] == cond_val) row_numbers.push_back(i);
	}
    }
  else if (op == "!=")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the rows to update
	  if (it->second[i] != cond_val) row_numbers.push_back(i);
	}
    }
  else if (op == "<")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the rows to update
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) < stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] < cond_val) row_numbers.push_back(i);
	}
    }
  else if (op == ">")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the rows to update
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) > stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] > cond_val) row_numbers.push_back(i);
	}
    }
  else if (op == "<=")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the rows to update
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) <= stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] <= cond_val) row_numbers.push_back(i);
	}
    }
  else if (op == ">=")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the rows to update
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) >= stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] >= cond_val) row_numbers.push_back(i);
	}
    }
  if (!row_numbers.empty())
    {	// if row_numbers is empty, the rows were not found
      for (int i = 0; i < attr_list.size(); ++i)
	{	// find the attribute in the table from the 
	  // attribute list
	  it = tbl.find(attr_list[i]);
	  for (int j = 0; j < row_numbers.size(); ++j)
	    {	// go through the row numbers to modify
	      // and modify it with the corresponding value
	      it->second[row_numbers[j]] = val_list[i];
	    }
	}

    }
  return tbl;
}

table Database::update(string tbl_name, string cond_attr, string op, string cond_val, vector<string> attr_list, vector<string> val_list)
{
  table_list::iterator it = db_copy.find(tbl_name);
  if (it != db_copy.end())
    {
      return update(it->second, cond_attr, op, cond_val, attr_list, val_list);
    }
  else
    {
      throw invalid_argument("Table " + tbl_name + " not found!");
      return db_copy.end()->second;
    }
}

table Database::create(vector<string> attributes, vector<string> prim_keys) {
  // create a new table with the specified attributes
  // insert_tuple should be used to insert data
  table new_table;
  new_table[PRIMARY_KEY] = prim_keys;//vector<string>(1, prim_keys);
  //new_table[primary_key];		// insert primary key if not specified in attributes vector
  for (int i = 0; i < attributes.size(); ++i)
    {
      new_table[attributes[i]];
    }
  return new_table;
}

table Database::insert_tuple(table dest_tbl, table tuples) {
  // inserts the tuples into the destination table
  table::iterator dest_it = dest_tbl.begin();
  table::iterator tup_it = tuples.begin();
  bool attributes_match = true;
  while (dest_it != dest_tbl.end() || tup_it != tuples.end())
    {	// check if the destination table and the tuples to insert
      // have the same attributes
      if (dest_it->first == PRIMARY_KEY) ++dest_it;
      if (dest_it->first != tup_it->first)
	{
	  attributes_match = false;
	  break;
	}
      ++dest_it;
      ++tup_it;
    }
  if (attributes_match)
    {	// check if a duplicate primary key will be inserted
      dest_it = dest_tbl.find(dest_tbl[PRIMARY_KEY][0]);
      tup_it = tuples.find(dest_tbl[PRIMARY_KEY][0]);
      vector<string> key_insertions = dest_it->second;
      key_insertions.insert(key_insertions.end(), tup_it->second.begin(), tup_it->second.end());
      sort(key_insertions.begin(), key_insertions.end());
      if (unique(key_insertions.begin(), key_insertions.end()) == key_insertions.end())
	{	// if the keys that are inserted are unique and
	  // if the attributes match, insert the tuples
	  dest_it = dest_tbl.begin();
	  tup_it = tuples.begin();
	  while (dest_it != dest_tbl.end() || tup_it != tuples.end())
	    {
	      if (dest_it->first == PRIMARY_KEY) ++dest_it;
	      for (int i = 0; i < tup_it->second.size(); ++i)
		{	// insert the tuple into the table
		  // but don't insert into primary_key column
		  dest_it->second.push_back(tup_it->second[i]);
		}
	      ++dest_it;
	      ++tup_it;
	    }
	}
    }
  return dest_tbl;
}

table Database::insert_tuple(string dest_tbl, table tuples)
{
  table_list::iterator it = db_copy.find(dest_tbl);
  if (it != db_copy.end())
    {
      return insert_tuple(it->second, tuples);
    }
  else
    {
      throw invalid_argument("Table " + dest_tbl + " not found!");
      return db_copy.end()->second;
    }
}

table Database::delete_tuple(table tbl, string cond_attr, string op, string cond_val) {
  // deletes the row specified by an attribute and a 
  if (cond_attr == PRIMARY_KEY) return tbl;		// do nothing if asked to delete the primary_key
  table::iterator it = tbl.find(cond_attr);
  vector<int> row_numbers;
  // check the condition and select the appropriate comparison
  if (op == "==")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the row to delete
	  if (it->second[i] == cond_val) row_numbers.insert(row_numbers.begin(), i);
	}
    }
  else if (op == "!=")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the row to delete
	  if (it->second[i] != cond_val) row_numbers.insert(row_numbers.begin(), i);
	}
    }
  else if (op == "<")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the row to delete
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) < stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] < cond_val) row_numbers.insert(row_numbers.begin(), i);
	}
    }
  else if (op == ">")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the row to delete
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) > stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] > cond_val) row_numbers.insert(row_numbers.begin(), i);
	}
    }
  else if (op == "<=")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the row to delete
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) <= stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] <= cond_val) row_numbers.insert(row_numbers.begin(), i);
	}
    }
  else if (op == ">=")
    {
      for (int i = 0; i < it->second.size(); ++i)
	{	// figure out the index of the row to delete
	  if (numerical_str(cond_val) && numerical_str(it->second[i]))
	    {
	      if (stod(it->second[i]) >= stod(cond_val)) row_numbers.insert(row_numbers.begin(), i);
	    }
	  else if (it->second[i] >= cond_val) row_numbers.insert(row_numbers.begin(), i);
	}
    }
  // delete the rows that matched, if any
  if (!row_numbers.empty())
    {	// if row_numbers is empty, the rows were not found
      it = tbl.begin();
      while (it != tbl.end())
	{	// iterate through and erase the tuple
	  if (it->first != PRIMARY_KEY)
	    {
	      for (int i = 0; i < row_numbers.size(); ++i)
		{
		  it->second.erase(it->second.begin() + row_numbers[i]);
		}
	    }
	  ++it;
	}
    }
  return tbl;
}

table Database::delete_tuple(string tbl_name, string cond_attr, string op, string cond_val)
{
  table_list::iterator it = db_copy.find(tbl_name);
  if (it != db_copy.end())
    {
      return delete_tuple(it->second, cond_attr, op, cond_val);
    }
  else
    {
      throw invalid_argument("Table " + tbl_name + " not found!");
      return db_copy.end()->second;
    }
}

// Add a table from a db file to map structure
void Database::update_mat(string table_name) {
  // No trying to re-use table names
  if (db_copy.count(table_name) > 0)
    {
      throw invalid_argument("Table name exists already!");
    }

  string line, tok, new_name, attr_name, primary_key;
  string delim = " ";
  vector<string> temp;
  table new_table;
  size_t pos = 0;

  fstream fs(table_name + ".db");
  if (fs.is_open())
    {
      // Tokenize 
      while (getline(fs, line)) {
	while ((pos = line.find(delim)) != string::npos) {
	  tok = line.substr(0, pos);
	  temp.push_back(tok);
	  line.erase(0, pos + delim.length());
	}

	// Grab table name if nontrivial line did not start with * or $. 
	// Grab table attribute name otherwise
	if (temp.size() != 0) {
	  if (temp[0] != "*" && temp[0] != "$") {
	    new_name = temp[0];
	  }

	  // Get table attribute name, put following data into vector
	  else if (temp[0] == "*") {
	    vector<string>::const_iterator beg = temp.begin() + 1;
	    vector<string>::const_iterator end = temp.end();
	    vector<string> data(beg, end);

	    attr_name = data[0];
	    data.erase(data.begin());
	    new_table[attr_name] = data;
	  }

	  // Get the primary key, put into table under PRIMARY_KEY attribute
	  else if (temp[0] == "$") {
	    vector<string>::const_iterator beg = temp.begin() + 1;
	    vector<string>::const_iterator end = temp.end();
	    vector<string> data(beg, end);

	    attr_name = data[0];
	    primary_key = data[0];
	    data.erase(data.begin());
	    //db_copy[new_name][PRIMARY_KEY] = vector<string> (1, primary_key);	// Add primary key to db
	    //db_copy[new_name][attr_name] = data; // Add entry to db
	    new_table[PRIMARY_KEY] = vector<string>(1, primary_key);
	    new_table[attr_name] = data;

	    // check for duplicate primary keys
	    sort(data.begin(), data.end());
	    if (unique(data.begin(), data.end()) != data.end())
	      {	// do not insert table because primary keys are not unique
		cerr << "The table " << table_name << " could not be inserted because the\nprimary keys are not unique.\n";
		fs.close();
		return;
	      }
	  }
	}
	temp = {};
      }

      // insert into table list
      db_copy[new_name] = new_table;
      mat_updated = true;
      fs.close();
    }
  else
    {
      cerr << "Error opening file " << table_name << ".db\n";
    }
}

void Database::print_db() {
  for (auto it1 : db_copy) {
    cout << it1.first << " - ";
    map<string, vector<string> > &inner1 = it1.second;
    cout << inner1.size() << endl;
    for (auto it2 : inner1) {
      cout << it2.first << ": ";
      vector<string> &innermost = it2.second;
      for (auto it3 : innermost) {
	cout << it3 << ' ';
      }
      cout << endl;
    }
    cout << endl;
  }
  cout << endl;
}

table_list Database::return_DB() {
  return db_copy;
}