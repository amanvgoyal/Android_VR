/*
 *********************************************************
**    Game_State.cpp    ***********************************
**  Description: This class will decode strings          **
**  that contain the game state and encode the game      **
**  state into a string to be sent over the network.     **
 *********************************************************
*/
/*
 *********************************************************
**    HEADER FILES    *************************************
 *********************************************************
*/
#include <iostream>
#include <string>
#include <vector>
#include "Game_State.h"

using namespace std;
/*
 *********************************************************
**    CLASS DECLARATION    ********************************
 *********************************************************
*/
/*    CONSTRUCTOR    */
Game_State::Game_State()
{
	string function = "Game_State";
	game_state_log.open("game_state_log.txt", fstream::out | fstream::trunc);
	if (!game_state_log.is_open())
	{	// do not output to log if file cannot be opened
		cerr << "Error creating game_state_log.txt. Game activity will be recorded.\n";
	}
	else
	{
		game_state_log.print(INFORMATION, function, string("Log file created successfully."));
	}
}

/*    MEMBER FUNCTIONS    */
// Code based off of stackoverflow question
// http://stackoverflow.com/questions/14265581/parse-split-a-string-in-c-using-string-delimiter-standard-c
vector<vector<Piece*>> Game_State::decode_string(string message)
{
	vector<vector<Piece*>> output(8, vector<Piece*>(8, nullptr));
	string function = "decode_string";
	string delimiter = ",";
	string id_delimiter = "-";
	string token;
	string id_str;
	size_t pos = 0;
	int number;
	int column;
	int row;
	Color color;
	Position position;

	game_state_log.print(INFORMATION, function, string("Decoding string. . ."));
	while ((pos = message.find(delimiter)) != string::npos)
	{	// grab the pieces on the board and their location information
		token = message.substr(0, pos);
		game_state_log.print(INFORMATION, function, string("Added token: ") + token);
		// add the piece to the board
		id_str = token.substr(0, token.find(id_delimiter));
		token.erase(0, token.find(id_delimiter) + id_delimiter.length());
		number = stoi(id_str);
		row = token[0] - '1';
		column = token[1] - 'A';
		if (token[2] == 'w') color == Color::WHITE;
		else if (token[2] == 'b') color == Color::BLACK;
		position.row = row;
		position.col = column;
		output[row][column] = new Piece(number, color, position, true);
		message.erase(0, pos + delimiter.length());
	}
}

string Game_State::encode_vector(vector<vector<Piece*>> board_data)
{
	string function = "encode_vector";
	string output = "";
	char column = 'A';
	char color;
	int id;
	int row = 1;

	game_state_log.print(INFORMATION, function, string("Encoding vector. . ."));
	for (int i = 0; i < board_data.size(); ++i)
	{	// iterate through the rows
		for (int j = 0; j < board_data[i].size(); ++j)
		{	// iterate through the columns
			if (board_data[i][j] != nullptr)
			{	// if a piece exists on that spots
				// output to the string
				id = board_data[i][j]->number();
				color = board_data[i][j]->color == Color::WHITE ? 'w' : 'b';
				output += id + '-' + row + column + color + ',';
			}
			++column;
		}
		++row;
	}
}