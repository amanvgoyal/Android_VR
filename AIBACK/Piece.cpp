/*
 *********************************************************
**    Piece.cpp    ****************************************
**  Description: This class will represent the pieces    **
**  on the board and will contain information about the  **
**  piece.                                               **
 *********************************************************
*/
/*
 *********************************************************
**    HEADER FILES    *************************************
 *********************************************************
*/
#include "Piece.h"

using namespace std;
/*
 *********************************************************
**    CLASS DEFINITIONS    ********************************
 *********************************************************
*/
void Piece::position(Position space)
{	// the validity of the position is checked on the board before being set
	pos = space;
	return;
}

void Piece::position(int row, int col)
{
	pos.row = row;
	pos.col = col;
	return;
}

Position Piece::position()
{	// returns the position of the piece
	return pos;
}

void Piece::on_board(bool on)
{	// specifies whether the piece is on the board or not
	active = on;
}

bool Piece::on_board()
{	// returns true if the piece is on the board
	return active;
}

void Piece::number(int number)
{	// set the number of the piece
	// each piece from the same team should have different numbers
	id = number;
}

int Piece::number()
{	// returns the number of the piece
	return id;
}

bool Piece::color(Color col)
{	// set the color of the piece
	team = col;
}

Color Piece::color()
{	// returns the color of the piece
	return team;
}