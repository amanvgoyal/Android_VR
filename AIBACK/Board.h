/*
 *********************************************************
**    Board.h    ******************************************
**  Description: This class will represent the state of  **
**  the board. It will store the the pieces of each      **
**  player in a vector of pointers to Pieces.            **
**  The board will be represented in the following way:  **
**     A B C D E F G H                                   **
**     _ _ _ _ _ _ _ _                                   **
**  8 |_|_|_|_|_|_|_|_|                                  **
**  7 |_|_|_|_|_|_|_|_|                                  **
**  6 |_|_|_|_|_|_|_|_|                                  **
**  5 |_|_|_|_|_|_|_|_|                                  **
**  4 |_|_|_|_|_|_|_|_|                                  **
**  3 |_|_|_|_|_|_|_|_|                                  **
**  2 |_|_|_|_|_|_|_|_|                                  **
**  1 |_|_|_|_|_|_|_|_|                                  **
 *********************************************************
*/
#ifndef BOARD
#define BOARD
/*
 *********************************************************
**    HEADER FILES    *************************************
 *********************************************************
*/
#include <vector>
#include <string>
#include <fstream>
#include "Log.h"
#include "Piece.h"
#include "AI.h"
/*
 *********************************************************
**    ENUMERATIONS & STRUCTS    ***************************
 *********************************************************
*/
// Movement directions, left and right move diagonally
enum Direction 
{
	NONE = 0,
	FORWARD = 1 << 0, 
	LEFT = 1 << 1, 
	RIGHT = 1 << 2 
};

inline Direction operator|(Direction a, Direction b)
{
	return static_cast<Direction>(static_cast<int>(a) | static_cast<int>(b));
}

inline Direction operator&(Direction a, Direction b)
{
	return static_cast<Direction>(static_cast<int>(a) & static_cast<int>(b));
}

// Contains the piece and direction the piece will move
struct Move
{
	Piece *piece;
	Direction direction;
};
/*
 *********************************************************
**    CLASS DECLARATION    ********************************
 *********************************************************
*/
class History
{
	std::vector<Move> move_history;
	std::vector<Piece*> capture_history;
	std::vector<std::vector<std::vector<Piece*>>> board_state_history;
public:
	void move(Move move);										// add the moves performed
	Move move(int turn);										// returns the move performed at the specified turn
	void capture(Piece* piece);									// add the captured pieces
	Piece* capture(int turn);									// returns the piece captured at the specified turn or NULL if it is empty
	void board_state(std::vector<std::vector<Piece*>> board);	// add the board state
	std::vector<std::vector<Piece*>> board_state(int turn);		// returns the board state at the specified turn
	void clear_history();										// clears all the vectors
};

class Board
{	
	AI smart;
	Log game_log;											// keeps a log of the game's activity and errors
	std::vector<std::vector<Piece*>> board_spaces;			// keeps track of the pieces on the board
	History history;										// will keep track of the move history
	Color turn;												// keeps track of who's turn it is
	Color winner;											// will be set to the player who wins
	int row;												// vertical size of board
	int col;												// horizontal size of board
	int turn_num;											// keeps track of turn number; one turn is one player's move
	int num_white_pieces;									// keeps track of the white team's pieces
	int num_black_pieces;									// keeps track of the black team's pieces
	int num_undoes;											// keeps track of the total number of undoes
	bool game_end;											// will be set to true when the game ends
	bool game_abort;										// will be set to true if the game did not finish
	
	/*    PRIVATE MEMBER FUNCTIONS    */
	bool check_move(Move move);								// check if the move is valid
	bool check_game();										// check game conditions
	int	 column_inx(char index);							// converts the letter index of the column to an int
	char column_inx(int index);								// converts the int index of the column to a letter
public:
	/*    CONSTRUCTOR    */
	Board();
	Board(int rows, int columns);
	
	/*    MEMBER FUNCTIONS    */
	const std::vector<std::vector<Piece*>> board();			// get the current board state
	Piece* piece_at(int rows, char cols);				// get the piece at the specified space
	const Color current_turn();								// get the current turn
	const int row_size();									// get the row size of the board
	const int column_size();								// get the column size of the board
	const int current_turn_number();						// get the current turn number
	const int total_white_pieces();							// get the total number of white pieces
	const int total_black_pieces();							// get the total number of black pieces
	const int total_undoes();								// get the total number of undoes
	const bool game_ended();								// true if the game has finished
	const bool game_aborted();								// true if the game has been aborted
	void game_aborted(bool aborted);						// set to true if the game has been aborted
	bool init();											// initialize the board
	bool clear();											// clear the board
	bool set_size(int rows, int cols);						// set the size of the board
	bool move_piece(int piece_row, char piece_col, int move_row, char move_col);
	bool move_piece(int rows, char cols, Direction direction);// move the specified piece
	bool move_piece(Move move);								// move the specified piece
	void end_turn();										// end the current turn and switch to the other player
	Direction show_moves(int rows, char cols);				// show the valid moves for the specified piece
	Direction show_moves(Piece* piece);						// show the valid moves for the specified piece
	bool undo_move();										// undo the previous move
	std::string show_results();									// show the results of the game
	void display_board();									// display the board in the command line window
	std::string output_history();							// output the game history
	std::string output_board();								// output the current board state
	std::string output_board(std::vector<std::vector<Piece*>> game_board);	// output the specified board
};
#endif