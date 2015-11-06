/*
 *********************************************************
**    Log.cpp    ******************************************
**  Description: This class will be used to keep track   **
**  of the program's activity and errors.                **
 *********************************************************
*/
/*
 *********************************************************
**    HEADER FILES    *************************************
 *********************************************************
*/
#include <string>
#include <ctime>
#include <iostream>
#include "Log.h"

using namespace std;
/*
 *********************************************************
**    CLASS DEFINITIONS    ********************************
 *********************************************************
*/
/*    CONSTRUCTOR    */
Log::Log()
{	// no file has been opened
	file_open = false;
}

Log::Log(const char* filename, ios_base::openmode mode)
{
	log.open(filename, mode);
	file_open = log.is_open();
}

Log::Log(string filename, ios_base::openmode mode)
{
	log.open(filename, mode);
	file_open = log.is_open();
}

/*    MEMBER FUNCTIONS    */
void Log::open(const char* filename, ios_base::openmode mode)
{
	string fn(filename);
	open(fn, mode);
}

void Log::open(string filename, ios_base::openmode mode)
{
	log.open(filename, mode);
}

void Log::print(Message_Type type, const char* function, const char* message)
{
	string func(function);
	string msg(message);
	print(type, func, msg);
}

void Log::print(Message_Type type, string function, string message)
{
	string msg_type;
	switch(type)
	{
		case ERROR:
			msg_type = "ERROR:   ";
			break;
		case WARNING:
			msg_type = "WARNING: ";
			break;
		case INFORMATION:
			msg_type = "INFO:    ";
			break;
		default:
			break;
	}
	log << '[' << timestamp() << "]\t" << msg_type << ' ' << function << ": " << message << '\n';
}

bool Log::is_open()
{
	return file_open;
}

// Borrowed from http://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
string Log::timestamp() 
{
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%d.%X", &tstruct);
    return buf;
}