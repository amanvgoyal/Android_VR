#include "AI.h"
#include "Piece.h"
#include <string>
#include <vector>
#include <stdlib.h>
#include <iostream>

using namespace std;

const int rows = 8;
const int cols = 8;

string AI::move(vector<vector<Piece*> > state, string diff, string color) {
  ai_color = color;
  update_state(state);
  /*
  if (diff == "EASY") {
    return random(color);
  }
   
  else if (diff == "MEDIUM") {
    return minimax(color);
  }
  else if (diff == "HARD") {
    return alpha_beta(color);
  }
  else {
    cerr << "Difficulty string not well formed!" << endl;
    }*/
  return random("BLACK");
}

void AI::update_state(vector<vector<Piece*>> state) {
  Position pos;
  for (int i = 0; i < 8; ++i) {
    for (int j = 0; j < 8; ++j) {
      brd[i][j] = Color::NONE;
    }
  }

  for (auto v : state) {
    for (auto p : v) {
      if (p != nullptr) {
	pos = p->position(); 
	brd[pos.col][pos.row] = p->color();
      }
    }
  }
  
  /*
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (i == 0 || i == 1) {
	brd[i][j] = Color::BLACK;
      }
      
      else if (i == 6 || i == 7) {
	brd[i][j] = Color::WHITE;
      }
      
      else {
	brd[i][j] = Color::NONE;
      }
    }
  }*/

  print_board(brd);
  //cout << endl;
  //vector<board> bs = generate_moves(brd, Color::BLACK);
  //cout << bs.size() << endl;

  //cout << random("BLACK") << endl;

  /*for (auto v : bs) {
    print_board(v);
    cout << endl;
    }*/
} 

Color AI::win(board b) {
  for (int i = 0; i < rows; ++i) {
    if (b[0][i] == Color::WHITE) {
      return Color::WHITE; // White has won
    }
    
    if (b[7][i] == Color::BLACK) {
      return Color::BLACK; // Black has won
    }
  }

  return Color::NONE; // No one has won
}

vector<board> AI::generate_moves(board b, Color player_color) {
  //board b_copy = b;
  vector<board> ret;
  Color original;
  int ct = 0;
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (b[i][j] == player_color) {
	
	if (player_color == Color::WHITE) {
	  // Can go up
	  if (i > 0) {
	    if (b[i-1][j] == Color::NONE) {
	      b[i-1][j] = player_color;
	      b[i][j] = Color::NONE;
	      ret.push_back(b);
	      b[i-1][j] = Color::NONE;
	      b[i][j] = player_color;
	    }
	  }

	  // Can go diag left
	  if (i > 0 && j > 0) {
	    if (b[i-1][j-1] != player_color) {
	      original = b[i-1][j-1];
	      b[i-1][j-1] = player_color;
	      b[i][j] = Color::NONE;
	      ret.push_back(b);
	      b[i][j] = player_color;
	      b[i-1][j-1] = original;
	    }
	  }
      

	  // Can go diag right
	  if (i > 0 && j < 7) {
	    if (b[i-1][j+1] != player_color) {
	      original = b[i-1][j+1];
	      b[i-1][j+1] = player_color;
	      b[i][j] = Color::NONE;
	      ret.push_back(b);
	      b[i-1][j+1] = original;
	      b[i][j] = player_color;
	    }
	  }
	}

	if (player_color == Color::BLACK) {
	  // Can go down 
	  if (i < 7) {
	    if (b[i+1][j] == Color::NONE) {
	      original = b[i+1][j];
	      b[i+1][j] = player_color;
	      b[i][j] = Color::NONE;
	      ret.push_back(b);
	      b[i+1][j] = original;
	      b[i][j] = player_color;
	    }
	  }

	  // Can go diag left
	  if (i < 7 && j > 0) {
	    if (b[i+1][j-1] != player_color) {
	      original = b[i+1][j-1];
	      b[i+1][j-1] = player_color;
	      b[i][j] = Color::NONE;
	      ret.push_back(b);
	      b[i+1][j-1] = original;
	      b[i][j] = player_color;	    
	    }
	  }

	  // Can go diag right
	  if (i < 7 && j < 7) {
	    if (b[i+1][j+1] != player_color) {
	      original = b[i+1][j+1];
	      b[i+1][j+1] = player_color;
	      b[i][j] = Color::NONE;
	      ret.push_back(b);
	      b[i+1][j+1] = original;
	      b[i][j] = player_color;
	    }
	  }
	}
      }
    }
  }
  return ret;
} 

// IMPROVE THIS
int AI::threat_level(board b, int x, int y) {
  int val = 0;

  if (b[x][y] == Color::WHITE) {
    // Check if protected
    if (x < 7 && y > 0) {
      if (b[x+1][y-1] == Color::WHITE) {
	val += 1;
      }
    }
    
    if (x < 7 && y < 7) {
      if (b[x+1][y+1] == Color::WHITE) {
	val += 1;
      }
    }
    
    // Check for Attackers
    if (x < 7 && y > 0) {
      if (b[x+1][y-1] == Color::BLACK) {
	val -= 1;
      }
    }
    if (x < 7 && y < 7) {
      if (b[x+1][y+1] == Color::BLACK) {
	val -= 1;
      }
    }
  }
  
  else if (b[x][y] == Color::BLACK) {
    // Check if protected
    if (x > 0 && y > 0) {
      if (b[x-1][y-1] == Color::BLACK) {
	val += 1;
      }
    }
    if (x > 0 && y < 7) {
      if (b[x-1][y+1] == Color::BLACK) {
	val += 1;
      }
    }
    
    // Check for Attackers
    if (x > 0 && y > 0) {
      if (b[x-1][y-1] == Color::WHITE) {
	val -= 1;
      }
    }
    if (x > 0 && y < 7) {
      if (b[x-1][y+1] == Color::WHITE) {
	val -= 1;
      }
    }
  }

  else {cerr << "no piece there!" << endl;}
}

// MAYBE CHECK IF X, Y, IN RANGE
// SUPPOSED TO STOP GAME IF WE GET TO THE ENDS
int AI::mobility_level(board b, int x, int y) {
  int mobi_val = 0;
  
  if (b[x][y] == Color::WHITE && b[x-1][y] == Color::NONE) {
    return 2; 
  }
  else {return -2;}
  
  if (b[x][y] == Color::BLACK && b[x+1][y] == Color::NONE) {
    return 2;
  }
  else {return -2;}
}

int AI::piece_val(board b, int x, int y) {
  int piece_val = 0;
  piece_val += threat_level(b, x, y);
  piece_val += mobility_level(b, x, y);

  return piece_val;
}

int AI::board_val(board b, Color player_color) {
  int board_val = 0;
  
  // First check for wins??
  if (win(b) == player_color) return 99999999;

  // Now check features
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (b[i][j] == Color::NONE) {
	continue; // No Piece on this square
      }

      else if (b[i][j] == player_color) {
	board_val += piece_val(b, i, j); //???
      }

      else if (b[i][j] != player_color) {
	board_val -= piece_val(b, i, j); //???
      }
    }
  }
}

void AI::print_board(board b) {
  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {

      if (b[i][j] == Color::WHITE) {
        cout << "w ";
      }

      else if (b[i][j] == Color::BLACK) {
        cout << "b ";
      }

      else {
        cout << "n ";
      }
    }
    cout << endl;
  }
}
 
string AI::random(string color) {
  srand(time(NULL));

  vector<string> possible_moves;
  vector<string> eat_moves;

  Color player_color;
  Color enemy_color;
  if (color == "BLACK") {
    player_color = Color::BLACK;
    enemy_color = Color::WHITE;
  }
  else {
    player_color = Color::WHITE;
    enemy_color = Color::BLACK;
  }

  for (int i = 0; i < rows; ++i) {
    for (int j = 0; j < cols; ++j) {
      if (brd[i][j] == player_color) {
	if (player_color == Color::WHITE) {

	  // Can go up
	  if (i > 0) {
	    if (brd[i-1][j] != Color::NONE) {
	      possible_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j)+", "+to_string(i-1)+")");
	    }
	  }

	  // Can go diag left
	  if (i > 0 && j > 0) {
	    if (brd[i-1][j-1] != player_color) {
	      if (brd[i-1][j-1] == enemy_color) {
		eat_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j-1)+", "+to_string(i-1)+")");
	      }
	      else {
		possible_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j-1)+", "+to_string(i-1)+")");
	      }
	    }
	  }
      
	  // Can go diag right
	  if (i > 0 && j < 7) {
	    if (brd[i-1][j+1] != player_color) {
	      if (brd[i-1][j+1] == enemy_color) {
		eat_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j+1)+", "+to_string(i-1)+")");
	      }
	      else {possible_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j+1)+", "+to_string(i-1)+")");}
	    }
	  }
	}

	if (player_color == Color::BLACK) {
	  // Can go down 
	  if (i < 7) {
	    if (brd[i+1][j] == Color::NONE) {
	      possible_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j)+", "+to_string(i+1)+")");
	    }
	  }

	  // Can go diag left
	  if (i < 7 && j > 0) {
	    if (brd[i+1][j-1] != player_color) {
	      if (brd[i+1][j-1] == enemy_color) {
		cout << "EATING!!!!!!!!!!!!!!!!!" << endl;
		eat_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j-1)+", "+to_string(i+1)+")");
	      }
	      else {possible_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j-1)+", "+to_string(i+1)+")");}
	    }
	  }

	  // Can go diag right
	  if (i < 7 && j < 7) {
	    if (brd[i+1][j+1] != player_color) {
	      if (brd[i+1][j+1] == enemy_color) {
		cout << "EATING!!!!!!!!!!!!!!!!!" << endl;
		eat_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j+1)+", "+to_string(i+1)+")");
	      }
	      else {possible_moves.push_back("("+to_string(j)+", "+to_string(i)+")*("+to_string(j+1)+", "+to_string(i+1)+")");}
	    }
	  }
	}
      }
    }
  }
  
  cout << possible_moves.size() << endl;
  cout << eat_moves.size() << endl;

  int index = 0;
  if (eat_moves.size() > 0) {
    index = rand() % eat_moves.size();
    return eat_moves[index];
  }
  else {
    index = rand() % possible_moves.size();
    return possible_moves[index];
  }
}

string AI::minimax(string color) {

}

string AI::alpha_beta(string color) {

}
