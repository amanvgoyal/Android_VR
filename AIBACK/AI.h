#ifndef AI_H
#define AI_H

#include <string>
#include <vector>
#include <array>
#include <map>
#include "Piece.h"

//typedef Color board [8][8];
typedef std::array<std::array<Color, 8>, 8> board;

class AI {
 private:
  std::vector<std::vector<Piece*>> game_state;
  
  //Color board[8][8];
  board brd;
  std::string ai_color;

  Color win(board);
  std::vector<board> generate_moves(board, Color);
  int board_val(board, Color);
  int threat_level(board, int, int);
  int mobility_level(board, int, int);
  int piece_val(board, int, int);

  void print_board(board);

public:
  std::string move(std::vector<std::vector<Piece*> >, std::string diff, std::string color);

  std::string random(std::string diff);
  std::string minimax(std::string diff);
  std::string alpha_beta(std::string diff);
    
  void update_state(std::vector<std::vector<Piece*>>);
};

#endif