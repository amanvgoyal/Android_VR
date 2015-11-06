/*
 *********************************************************
**    Log.h    ********************************************
**  Description: This class will be used to keep track   **
**  of the program's activity and errors.                **
 *********************************************************
*/
#ifndef LOG
#define LOG
/*
 *********************************************************
**    HEADER FILES    *************************************
 *********************************************************
*/
#include <string>
#include <fstream>
#include <iostream>
/*
 *********************************************************
**    ENUMERATIONS & STRUCTS    ***************************
 *********************************************************
*/
enum Message_Type { ERROR, WARNING, INFORMATION };
/*
 *********************************************************
**    CLASS DECLARATION    ********************************
 *********************************************************
*/
class Log
{
	std::fstream log;
	bool file_open;
public:
	/*    CONSTRUCTOR    */
	Log();
	Log(const char* filename, std::ios_base::openmode mode = std::fstream::out | std::fstream::trunc);
	Log(std::string filename, std::ios_base::openmode mode = std::fstream::out | std::fstream::trunc);
	
	/*    MEMBER FUNCTIONS    */
	void open(const char* filename, std::ios_base::openmode mode = std::fstream::out | std::fstream::trunc);
	void open(std::string filename, std::ios_base::openmode mode = std::fstream::out | std::fstream::trunc);
	void print(Message_Type type, const char* function, const char* message);
	void print(Message_Type type, std::string function, std::string message);
	bool is_open();
	std::string timestamp();
};
#endif