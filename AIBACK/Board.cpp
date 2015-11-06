/*
 *********************************************************
**    Board.cpp    ****************************************
**  Description: This class will represent the state of  **
**  the board. It will store the the pieces of each      **
**  player in a vector of pointers to Pieces.            **
 *********************************************************
*/
/*
 *********************************************************
**    HEADER FILES    *************************************
 *********************************************************
*/
#include <iostream>
#include <iomanip>
#include <exception>
#include <vector>
#include <string>
#include "Board.h"
#include "Piece.h"

using namespace std;
/*
 *********************************************************
**    CLASS DEFINITIONS    ********************************
 *********************************************************
*/
/*    History    */
/*    MEMBER FUNCTIONS    */
void History::move(Move move)
{
	move_history.push_back(move);
}

Move History::move(int turn)
{
	return move_history[turn - 1];
}

void History::capture(Piece* piece)
{
	capture_history.push_back(piece);
}

Piece* History::capture(int turn)
{
	return capture_history[turn - 1];
}

void History::board_state(vector<vector<Piece*>> board)
{
	board_state_history.push_back(board);
}

vector<vector<Piece*>> History::board_state(int turn)
{
	return board_state_history[turn - 1];
}

void History::clear_history()
{
	move_history.clear();
	capture_history.clear();
	board_state_history.clear();
}
/*    Board    */
/*    CONSTRUCTOR    */
Board::Board()
{// initialize the board
	// open log file for output and set to clear previous run
	string function = "Board";

	// initialize private members
	turn = Color::WHITE;
	winner = Color::WHITE;
	row = 8;
	col = 8;
	num_white_pieces = 0;
	num_black_pieces = 0;
	game_end = false;
	game_abort = false;
	turn_num = 1;
	game_log.open("board_log.txt", fstream::out | fstream::trunc);
	if (!game_log.is_open())
	{	// do not output to log if file cannot be opened
		cerr << "Error creating board_log.txt. Game activity will not be recorded.\n";
	}
	else
	{
		game_log.print(INFORMATION, function, string("Log file created successfully."));
	}
	init();
}
Board::Board(int rows, int columns)
{	// initialize the board
	// open log file for output and set to clear previous run
	string function = "Board";

	/* These variables should be reset to this state when clearing the board */
	turn = Color::WHITE;
	winner = Color::WHITE;
	row = rows;
	col = columns;
	num_white_pieces = 0;
	num_black_pieces = 0;
	game_end = false;
	game_abort = false;
	turn_num = 1;
	/* The top variables should be set to this when clearing the board */
	game_log.open("board_log.txt", fstream::out | fstream::trunc);
	
	if(!game_log.is_open())
	{	// do not output to log if file cannot be opened
		// this must be checked in each section that outputs to this file
		cerr << "Error creating board_log.txt. Game activity will not be recorded.\n";
	}
	else
	{
		game_log.print(INFORMATION, function, string("Log file created successfully."));
	}
	init();
}

/*    PRIVATE MEMBER FUNCTIONS    */
bool Board::check_move(Move move)
{	// returns true if the move is valid or
	// false if a move is invalid
	string function = "check_move";

	bool check_up = (turn == Color::WHITE) ? true : false;
	int r = check_up ? move.piece->position().row + 1 : move.piece->position().row - 1;
	game_log.print(INFORMATION, function, string("Checking move. . ."));
	if (move.piece->color() == turn)
	{	// the player is not trying to move a piece that is not theirs
		switch(move.direction)
		{
			case FORWARD:
			{
				if (board_spaces[r][move.piece->position().col] != nullptr)
				{
					game_log.print(WARNING, function, string("Forward move is invalid; piece in the way."));
					return false;
				}
				else
				{
					game_log.print(INFORMATION, function, string("Forward move is valid."));
					return true;
				}
				break;
			}
			case RIGHT:
			{
				int c = (turn == Color::WHITE) ? move.piece->position().col + 1 : move.piece->position().col - 1;
				if (board_spaces[r][c] != nullptr)
				{
					if (board_spaces[r][c]->color() == turn)
					{
						game_log.print(WARNING, function, string("Diagonally right move is invalid; friendly piece in the way."));
						return false;
					}
					else
					{
						game_log.print(INFORMATION, function, string("Diagonally right move is valid; enemy piece will be captured."));
						return true;
					}
				}
				else
				{
					game_log.print(INFORMATION, function, string("Diagonally right move is valid."));
					return true;
				}
				break;
			}
			case LEFT:
			{
				int c = (turn == Color::WHITE) ? move.piece->position().col - 1 : move.piece->position().col + 1;
				if (board_spaces[r][c] != nullptr)
				{
					if (board_spaces[r][c]->color() == turn)
					{
						game_log.print(WARNING, function, string("Diagonally left move is invalid; friendly piece in the way."));
						return false;
					}
					else
					{
						game_log.print(INFORMATION, function, string("Diagonally left move is valid. enemy piece will be captured."));
						return true;
					}
				}
				else
				{
					game_log.print(INFORMATION, function, string("Diagonally left move is valid."));
					return true;
				}
				break;
			}
			default:		// the code should not reach this point
			{
				game_log.print(ERROR, function, string("The default case was reached when checking move direction.\tmove.direction = " + move.direction));
				return false;
				break;
			}
		}
	}
	else
	{
		game_log.print(WARNING, function, string("The player attempted to move a piece that is not theirs."));
		return false;
	}
}

bool Board::check_game()
{	// check for victory conditions
	// returns true if victory conditions are met
	string function = "check_game";

	game_log.print(INFORMATION, function, string("Checking victory conditions . . ."));
	if (num_white_pieces == 0 && num_black_pieces == 0)
	{	// this condition should never be hit
		game_log.print(ERROR, function, string("Victory condition hit; both players have no more pieces on board."));
		return true;
	}
	else if (num_white_pieces == 0)
	{
		game_log.print(INFORMATION, function, string("Victory condition hit; white player has no more pieces on board."));
		winner = Color::BLACK;
		return true;
	}
	else if (num_black_pieces == 0)
	{
		game_log.print(INFORMATION, function, string("Victory condition hit; black player has no more pieces on board."));
		winner = Color::WHITE;
		return true;
	}
	else
	{	// check if either player has reached the last row for them
		for (int i = 0; i < col; ++i)
		{	// both players should not have pieces on the enemy home row at the same time
			// since this function is called at the end of each player's turn
			// iterate through the white home row
			if (board_spaces[0][i]->color() == Color::BLACK)
			{
				game_log.print(INFORMATION, function, string("Victory condition hit; black player has reached the white home row."));
				winner = Color::BLACK;
				return true;
			}
			// iterate through the black home row
			if (board_spaces[row - 1][i]->color() == Color::WHITE)
			{
				game_log.print(INFORMATION, function, string("Victory condition hit; white player has reached the black home row."));
				winner = Color::WHITE;
				return true;
			}
		}
		game_log.print(INFORMATION, function, string("No victory conditions were met."));
		return false;
	}
}

int Board::column_inx(char index)
{	// converts the letter index of the column to an integer
	// for the vector
	index = toupper(index);
	if (index < 'A' || index > 'Z') return -1;
	else return index - 65;
}

char Board::column_inx(int index)
{	// converts the integer index of the column to a character
	if (index < 0 || index > 26) return '\0';
	else return index + 65;
}

/*    MEMBER FUNCTIONS    */
const vector<vector<Piece*>> Board::board()
{
	return board_spaces;
}

Piece* Board::piece_at(int rows, char cols)
{
	int r = rows - 1;
	int c = toupper(cols) - 'A';
	return board_spaces[r][c];
}

const Color Board::current_turn()
{
	return turn;
}

const int Board::row_size()
{
	return row;
}

const int Board::column_size()
{
	return col;
}

const int Board::current_turn_number()
{
	return turn_num;
}

const int Board::total_white_pieces()
{
	return num_white_pieces;
}

const int Board::total_black_pieces()
{
	return num_black_pieces;
}

const int Board::total_undoes()
{
	return num_undoes;
}

const bool Board::game_ended()
{
	return game_end;
}

const bool Board::game_aborted()
{
	return game_abort;
}

void Board::game_aborted(bool aborted)
{
	game_abort = aborted;
}

bool Board::init()
{	// initialize the board for a new game
	// returns true if no errors were found
	// set the size of the board
	string function = "init";
	int piece_id = 1;
	Position cur_space;

	num_black_pieces = 0;
	num_white_pieces = 0;
	game_log.print(INFORMATION, function, string("Adding the pieces to the board."));
	vector<vector<Piece*>> new_board(row, vector<Piece*>(col, nullptr));
	board_spaces = new_board;
	// add the black pieces to the board
	for (int i = row - 2; i < row; ++i)
	{	// iterate through the top two rows of the board
		for (int j = 0; j < col; ++j)
		{	// iterate through the columns
			// add pieces to the board
			cur_space.row = i;
			cur_space.col = j;
			board_spaces[i][j] = new Piece(piece_id, Color::BLACK, cur_space, true);
			++piece_id;
			++num_black_pieces;
		}
	}
	// add the white pieces to the board
	for (int i = 0; i < 2; ++i)
	{	// iterate through the bottom two rows of the board
		for (int j = 0; j < col; ++j)
		{	// iterate through the columns
			// add pieces to the board
			cur_space.row = i;
			cur_space.col = j;
			board_spaces[i][j] = new Piece(piece_id, Color::WHITE, cur_space, true);
			++piece_id;
			++num_white_pieces;
		}
	}
	history.board_state(board_spaces);
}

bool Board::clear()
{	// clear the board in preparation for exit or a new game
	// reset variables for a new game
	// returns true if no errors were found
	
	// set all the spaces to point to nullptr

	// set all the variables that were modified during the game to the default value
	
	num_black_pieces = 0;
	num_white_pieces = 0;
	game_end = false;
	game_abort = false;
	turn_num = 1;
	string function = "clear";
	int piece_id = 1;
		
	game_log.print(INFORMATION, function, string("Resetting for new game."));
	for (int i = 0; i < board_spaces.size(); ++i)
	{
		for (int j = 0; j < board_spaces[i].size(); ++j)
		{
			if (board_spaces[i][j] != nullptr)
			{
				delete board_spaces[i][j];
			}
		}
	}
	history.clear_history();

	return true;
}

bool Board::set_size(int rows, int cols)
{	// set the size of the board before starting a new game
	string function = "set_size";

	game_log.print(INFORMATION, function, string("Attempting to set size to ") + to_string(rows) + string("x") + to_string(cols));
	if (rows > 1 && rows < 10)
	{
		row = rows;
	}
	else
	{
		game_log.print(ERROR, function, string("The size for rows is invalid"));
		return false;
	}
	if (cols > 1 && cols < 10)
	{
		col = cols;
	}
	else
	{
		game_log.print(ERROR, function, string("The size for columns is invalid"));
		return false;
	}
	return true;
}

bool Board::move_piece(int piece_row, char piece_col, int move_row, char move_col)
{
	// figure out direction that piece is moving
	int fwd_r = (turn == Color::WHITE) ? piece_row + 1 : piece_row - 1;
	if (piece_col == move_col)
	{	// piece is moving forward
		cout << "Piece at " << piece_row << ',' << piece_col << "moves forward to " << move_row << ',' << move_col << ".\n";
		return move_piece(piece_row, piece_col, FORWARD);
	}
	else
	{	// piece is moving left or right
		int right_c = (turn == Color::WHITE) ? piece_col + 1 : piece_col - 1;
		int left_c = (turn == Color::WHITE) ? piece_col - 1 : piece_col + 1;
		if (right_c == move_col) 
		{
			cout << "Piece at " << piece_row << ',' << piece_col << "moves fwd-right to " << move_row << ',' << move_col << ".\n";
			return move_piece(piece_row, piece_col, RIGHT);
		}
		else if(left_c == move_col)
		{
			cout << "Piece at " << piece_row << ',' << piece_col << "moves fwd-left to " << move_row << ',' << move_col << ".\n";
			return move_piece(piece_row, piece_col, LEFT);
		}
		else 
		{
			cout << "Invalid move: Piece at " << piece_row << ',' << piece_col << " to " << move_row << ',' << move_col << ".\n";
			return false;
		}
	}
}

bool Board::move_piece(int rows, char cols, Direction direction)
{	// assumes the rows starts at 1 and cols start at A
	Move move;
	int rows_inx = rows - 1;
	int cols_inx = column_inx(cols);
	Piece* piece = board_spaces[rows_inx][cols_inx];
	string function = "move_piece";

	if (piece == nullptr)
	{
		game_log.print(WARNING, function, string("There is no piece at") + to_string(rows) + cols);
		return false;
	}
	move.direction = direction;
	move.piece = piece;
	return move_piece(move);
}

bool Board::move_piece(Move move)
{	// moves the specified piece after checking the validity of the move
	string function = "move_piece";

	game_log.print(INFORMATION, function, string("Moving piece. . ."));
	if (check_move(move))
	{
		Piece* piece_to_move = move.piece;
		bool check_up = (turn == Color::WHITE) ? true : false;
		int r = check_up ? move.piece->position().row + 1 : move.piece->position().row - 1;
		int c = 0;
		int orig_r = piece_to_move->position().row;
		int orig_c = piece_to_move->position().col;
		// add the move to the move history
		history.move(move);
		switch (move.direction)
		{
		case FORWARD:
		{
			c = orig_c;
			break;
		}
		case LEFT:
		{
			c = (turn == Color::WHITE) ? orig_c - 1 : orig_c + 1;
			break;
		}
		case RIGHT:
		{
			c = (turn == Color::WHITE) ? orig_c + 1 : orig_c - 1;
			break;
		}
		default:
		{
			game_log.print(ERROR, function, string("The default case was reached when checking move direction.\tmove.direction = " + move.direction));
			return false;
			break;
		}
		}
		// remove the piece from its original spot
		board_spaces[orig_r][orig_c] = nullptr;
		// modify piece position
		piece_to_move->position(r, c);
		if (board_spaces[r][c] != nullptr)
		{	// enemy piece to be captured; store in history
			board_spaces[r][c]->on_board(false);
			history.capture(board_spaces[r][c]);
			if (board_spaces[r][c]->color() == Color::WHITE) --num_white_pieces;
			else --num_black_pieces;
		}
		else
		{	// enemy piece is not captured; store nullptr in history
			history.capture(nullptr);
		}
		board_spaces[r][c] = piece_to_move;
		return true;
	}
	return false;
}

void Board::end_turn()
{	// checks victory conditions and displays the results if they are met
	// or switches to the other player's turn
	string function = "end_turn";
	// add board state to history and increase turn number
	history.board_state(board_spaces);
	++turn_num;
	if (check_game())
	{	// victory conditions were found
		game_end = true;
		//show_results();
	}
	else
	{	// switch turns
		turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
		string cur_player = turn == Color::WHITE ? "white" : "black";
		game_log.print(INFORMATION, function, string("Switching turns; it is the " + cur_player + " player's turn now."));
	}
			string s = smart.move(board_spaces,"EASY","BLACK");
			//bool move_piece(int piece_row, char piece_col, int move_row, char move_col);
			cout<<"THIS IS S:"<<s<<endl;
			for(int i=0;i<s.length();++i)
				cout<<"S["<<i<<"]:"<<s[i]<<endl;
			string a,b;
		    a.insert(0,1,s[4]);
		    b.insert(0,1,s[11]);
		    cout<<"This is what row  a has:"<<a<<endl;
		    cout<<"This is what row  b has:"<<b<<endl;
			if(move_piece(8-atoi(a.c_str()),(s[1]-'0')+'A',8-atoi(b.c_str()),(s[8]-'0')+'A'))
					cout<<"BLACK PIECE MOVED CORRECTLY"<<endl;

			history.board_state(board_spaces);
			++turn_num;
	if (check_game())
	{	// victory conditions were found
		game_end = true;
		//show_results();
	}
	else
	{	// switch turns
		turn = turn == Color::WHITE ? Color::BLACK : Color::WHITE;
		string cur_player = turn == Color::WHITE ? "white" : "black";
		game_log.print(INFORMATION, function, string("Switching turns; it is the " + cur_player + " player's turn now."));
	}
}

Direction Board::show_moves(int rows, char cols)
{
	int rows_inx = rows - 1;
	int cols_inx = column_inx(cols);
	Piece* piece = board_spaces[rows_inx][cols_inx];
	string function = "show_moves";

	if (piece == nullptr)
	{
		game_log.print(WARNING, function, string("There is no piece at") + to_string(rows) + cols);
		return NONE;
	}
	return show_moves(piece);
}

Direction Board::show_moves(Piece* piece)
{	// show the valid moves for the selected piece regardless of color
	string function = "show_moves";
	Direction dir = NONE;
	
	if (piece != nullptr)
	{
		bool can_move_fwd = false;
		bool can_move_right = false;
		bool can_move_left = false;
		bool check_up = (piece->color() == Color::WHITE) ? true : false;
		int r = check_up ? piece->position().row + 1 : piece->position().row - 1;
		int c = piece->position().col;

		game_log.print(INFORMATION, function, string("Showing moves. . ."));
		// check forward
		if (board_spaces[r][c] == nullptr)
		{
			can_move_fwd = true;
			dir = dir | FORWARD;
		}
		c += 1;
		// check right
		if (board_spaces[r][c] == nullptr)
		{
			if (turn == Color::WHITE)
			{
				can_move_right = true;
				dir = dir | RIGHT;
			}
			else
			{
				can_move_left = true;
				dir = dir | LEFT;
			}
		}
		c -= 2;
		if (board_spaces[r][c] == nullptr)
		{
			if (turn == Color::WHITE)
			{
				can_move_left = true;
				dir = dir | LEFT;
			}
			else 
			{
				can_move_right = true;
				dir = dir | RIGHT;
			}
		}
		// display board
		string cur_turn = turn == Color::WHITE ? "White" : "Black";
		int cur_player_pieces = turn == Color::WHITE ? num_white_pieces : num_black_pieces;
		int en_player_pieces = turn == Color::WHITE ? num_black_pieces : num_white_pieces;
		char col_label = 'A';
		int row_label = 8;

		game_log.print(INFORMATION, function, string("Displaying board. . ."));
		cout << "Current turn number: " << turn_num << "\t\t" << cur_turn << " player's turn\n\n\n";
		for (int i = row + 1; i >= 0; --i)
		{
			if (i < row)
			{	// add the row numbers
				cout << setw(4) << right;
				cout << row_label << ' ' << left;
				--row_label;
			}
			else
			{
				cout << "     ";
			}
			for (int j = 0; j < col; ++j)
			{	// draw the board
				if (i == row + 1)
				{
					cout << ' ' << col_label;
					++col_label;
				}
				else if (i == row)
				{
					cout << " _";
				}
				else
				{
					Piece* cur_space = board_spaces[i][j];
					cout << '|';
					if (cur_space == nullptr)
					{	// empty space
						cout << '_';
					}
					else if (cur_space->color() == Color::WHITE)
					{	// white piece (indicated by O)
						cout << 'O';
					}
					else if (cur_space->color() == Color::BLACK)
					{	// black piece (indicated by X)
						cout << 'X';
					}
					if (j == col - 1)
					{
						cout << '|';
					}
				}

			}
			if (i == 5)
			{
				cout << "\t\tYour total number of pieces:  " << cur_player_pieces;
			}
			else if (i == 4)
			{
				cout << "\t\tEnemy total number of pieces: " << en_player_pieces;
			}
			else if (i == 3)
			{
				int rr = piece->position().row + 1;
				char cc = column_inx(piece->position().col);
				cout << "\t\tPiece located at row " << rr << ", " << cc << " can move:";
			}
			else if (i == 2)
			{
				cout << "\t\t";
				//if (can_move_fwd)
				if (dir & FORWARD == FORWARD)
				{
					cout << "FORWARD ";
				}
				if (dir & RIGHT == RIGHT)
				//if (can_move_right)
				{
					cout << "RIGHT ";
				}
				//if (can_move_left)
				if (dir & LEFT == LEFT)
				{
					cout << "LEFT";
				}
			}
			cout << '\n';
		}
		cout << '\n';
		return dir;
	}
	else
	{
		game_log.print(WARNING, function, string("No piece was selected."));
		return dir;
	}
}

bool Board::undo_move()
{	// undoes the previous two turns
	string function = "undo_move";
	
	game_log.print(INFORMATION, function, string("Undoing the last two moves. . ."));
	if (turn_num < 3)
	{
		game_log.print(WARNING, function, string("Player attempted to undo on the first round."));
		return false;
	}
	else if (num_undoes > 10)
	{
		game_log.print(WARNING, function, string("Player has exceeded the allowed number of undoes."));
		return false;
	}
	board_spaces = history.board_state(turn_num - 2);
	// update the position of each piece
	for (int i = 0; i < board_spaces.size(); ++i)
	{
		for (int j = 0; j < board_spaces[i].size(); ++j)
		{
			if (board_spaces[i][j] != nullptr)
			{
				board_spaces[i][j]->position(i, j);
			}
		}
	}
	turn_num -= 2;
	++num_undoes;
	return true;
}

string Board::show_results()
{	// shows the results of the game
	string function = "show_results";
	string win = (winner == Color::WHITE) ? "White" : "Black";
	string reason;
	string output;
	int enemy_pieces = (winner == Color::WHITE) ? num_black_pieces : num_white_pieces;
	
	game_log.print(INFORMATION, function, string("Displaying game results. . ."));
	// find win reason
	if (enemy_pieces == 0) reason = "captured all of opponent's pieces.";
	else reason = "reached the opponent's home row.";
	
	cout << "The " << win << " player " << reason <<'\n';
	output += "The " + win + " player " + reason + "\n";
	cout << "This game's winner is the " << win << " player!\n";
	output += "This game's winner is the " + win + " player!\n";
}

void Board::display_board()
{	// output the board on the command line and in a file called recent_game.txt
	// Expected output:
	// Current turn number: 1		White player's turn
	//
	//
	//      A B C D E F G H
	//      _ _ _ _ _ _ _ _
	//   8 |X|X|X|X|X|X|X|X|
	//   7 |X|X|X|X|X|X|X|X|
	//   6 |_|_|_|_|_|_|_|_|		Your total number of pieces:  16
	//   5 |_|_|_|_|_|_|_|_|		Enemy total number of pieces: 16
	//   4 |_|_|_|_|_|_|_|_|
	//   3 |_|_|_|_|_|_|_|_|
	//   2 |O|O|O|O|O|O|O|O|
	//   1 |O|O|O|O|O|O|O|O|
	//
	string function = "display_board";
	string cur_turn = turn == Color::WHITE ? "White" : "Black";
	int cur_player_pieces = turn == Color::WHITE ? num_white_pieces : num_black_pieces;
	int en_player_pieces = turn == Color::WHITE ? num_black_pieces : num_white_pieces;
	char col_label = 'A';
	int row_label = 8;

	game_log.print(INFORMATION, function, string("Displaying board. . ."));
	cout << "Current turn number: " << turn_num << "\t\t" << cur_turn << " player's turn\n\n\n";
	for (int i = row + 1; i >= 0; --i)
	{
		if (i < row)
		{	// add the row numbers
			cout << setw(4) << right;
			cout << row_label << ' ' << left;
			--row_label;
		}
		else
		{
			cout << "     ";
		}
		for (int j = 0; j < col; ++j)
		{	// draw the board
			if (i == row + 1)
			{
				cout << ' ' << col_label;
				++col_label;
			}
			else if (i == row)
			{
				cout << " _";
			}
			else
			{
				Piece* cur_space = board_spaces[i][j];
				cout << '|';
				if (cur_space == nullptr)
				{	// empty space
					cout << '_';
				}
				else if (cur_space->color() == Color::WHITE)
				{	// white piece (indicated by O)
					cout << 'O';
				}
				else if (cur_space->color() == Color::BLACK)
				{	// black piece (indicated by X)
					cout << 'X';
				}
				if (j == col - 1)
				{
					cout << '|';
				}
			}

		}
		if (i == 5)
		{
			cout << "\t\tYour total number of pieces:  " << cur_player_pieces;
		}
		else if (i == 4)
		{
			cout << "\t\tEnemy total number of pieces: " << en_player_pieces;
		}
		cout << '\n';
	}
	cout << '\n';
}

string Board::output_history()
{	// outputs the history of the game to the specified medium
	string function = "output_history";
}

string Board::output_board()
{
	return output_board(board_spaces);
}

string Board::output_board(vector<vector<Piece*>> game_board)
{
	// Expected output:
	// Current turn number: 1		White player's turn
	//
	//
	//      A B C D E F G H
	//      _ _ _ _ _ _ _ _
	//   8 |X|X|X|X|X|X|X|X|
	//   7 |X|X|X|X|X|X|X|X|
	//   6 |_|_|_|_|_|_|_|_|		Your total number of pieces:  16
	//   5 |_|_|_|_|_|_|_|_|		Enemy total number of pieces: 16
	//   4 |_|_|_|_|_|_|_|_|
	//   3 |_|_|_|_|_|_|_|_|
	//   2 |O|O|O|O|O|O|O|O|
	//   1 |O|O|O|O|O|O|O|O|
	//
	string function = "output_board";
	string cur_turn = turn == Color::WHITE ? "White" : "Black";
	string output;
	int cur_player_pieces = turn == Color::WHITE ? num_white_pieces : num_black_pieces;
	int en_player_pieces = turn == Color::WHITE ? num_black_pieces : num_white_pieces;
	char col_label = 'A';
	int row_label = 8;
	
	if (game_end)
	{	// Show results
		return show_results();
	}
	else
	{
		game_log.print(INFORMATION, function, string("Outputting board. . ."));
		output = "Current turn number: " + to_string(turn_num) + "\t\t" + cur_turn + " player's turn\n\n\n";
		for (int i = row + 1; i >= 0; --i)
		{
			output += "  ";
			if (i < row)
			{	// add the row numbers
				output += to_string(row_label) + " ";
				--row_label;
			}
			else
			{
				output += "  ";
			}
			for (int j = 0; j < col; ++j)
			{	// draw the board
				if (i == row + 1)
				{
					output += " ";
					output += col_label;
					++col_label;
				}
				else if (i == row)
				{
					output += " _";
				}
				else
				{
					Piece* cur_space = board_spaces[i][j];
					output += '|';
					if (cur_space == nullptr)
					{	// empty space
						output += "_";
					}
					else if (cur_space->color() == Color::WHITE)
					{	// white piece (indicated by O)
						output += "O";
					}
					else if (cur_space->color() == Color::BLACK)
					{	// black piece (indicated by X)
						output += "X";
					}
					if (j == col - 1)
					{
						output += "|";
					}
				}

			}
			if (i == 5)
			{
				output += "\t\tYour total number of pieces:  " + to_string(cur_player_pieces);
			}
			else if (i == 4)
			{
				output += "\t\tEnemy total number of pieces: " + to_string(en_player_pieces);
			}
			output += "\n";
		}
		output += "\n";
		return output;
	}
}
