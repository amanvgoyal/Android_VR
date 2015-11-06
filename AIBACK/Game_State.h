/*
 *********************************************************
**    Game_State.h    *************************************
**  Description: This class will decode strings          **
**  that contain the game state and encode the game      **
**  state into a string to be sent over the network.     **
 *********************************************************
*/
#ifndef GAME_STATE
#define GAME_STATE
/*
 *********************************************************
**    HEADER FILES    *************************************
 *********************************************************
*/
#include <vector>
#include <string>
#include "Piece.h"
#include "Log.h"
/*
 *********************************************************
**    CLASS DECLARATION    ********************************
 *********************************************************
*/
class Game_State
{
	Log game_state_log;
	std::vector<std::vector<Piece*>> gv;
	std::string gs;
public:
	/*    CONSTRUCTOR    */
	Game_State();
	/*    MEMBER FUNCTIONS    */
	std::vector<std::vector<Piece*>> decode_string(std::string message);		// This will decode strings sent over the network
	std::string encode_vector(std::vector<std::vector<Piece*>> board_data);		// This will encode the board data structure into a string
};
#endif