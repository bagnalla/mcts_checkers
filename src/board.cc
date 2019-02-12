#include <cstring>
#include <functional>
#include <iostream>
#include <iterator>
#include "board.h"
#include "state.h"
#include "util.h"

using namespace std;
using namespace util;

#define IN_BOUNDS(i) (i >= 0 && i < BOARD_SIZE)

#define IS_EMPTY(square) (square == Square::empty)

#define IS_KING(piece) (piece == P1_king || piece == P2_king)

// Assumes it's a valid piece
#define PLAYER_OF(piece) (piece == P1_piece || piece == P1_king ? P1 : P2)

namespace checkers
{
  Board::Board()
  {
    this->init();
  }

  Board::Board(Square board[BOARD_SIZE][BOARD_SIZE])
  {
    memcpy(this->board, board, BOARD_SIZE * BOARD_SIZE * sizeof(Square));
  }

  void Board::init()
  {
    bool j_offset = true;
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
	this->board[i][j] = Square::empty;
      }
    }
    for (int i = 0; i < 3; ++i) {
      int j = j_offset ? 1 : 0;
      while (j < BOARD_SIZE) {
	this->board[i][j] = P1_piece;
	j += 2;
      }
      j_offset = !j_offset;
    }
    int i_offset = BOARD_SIZE - 3;
    j_offset = false;
    for (int i = 0; i < 3; ++i) {
      int j = j_offset ? 1 : 0;
      while (j < BOARD_SIZE) {
	this->board[i + i_offset][j] = P2_piece;
	j += 2;
      }
      j_offset = !j_offset;
    }
  }

  double Board::evaluate(Player p) const
  {
    double score = 0.0;
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
	auto piece = this->board[i][j];
	if (IS_PLAYER(piece, Player::P1)) {
	  score += IS_KING(piece) ? 1.5 : 1.0;
	}
	else if (IS_PLAYER(piece, Player::P2)) {
	  score -= IS_KING(piece) ? 1.5 : 1.0;
	}
      }
    }
    return p == Player::P1 ? score : -score;
    // return p == Player::P1 ? -score : score;
    // return score;
  }

  namespace
  {
    vector<Move> convert_take_chain(const vector<pair<int, int>> &l,
				    int init_i, int init_j, Player player)
    {
      auto prev_i = init_i, prev_j = init_j;
      vector<Move> takes;
      for (auto it = l.begin(); it != l.end(); ++it) {
	auto i = it->first, j = it->second;
	takes.push_back(Move(prev_i, prev_j, i, j, MoveKind::take, player));
	prev_i = i;
	prev_j = j;
      }
      return takes;
    }
  }

  vector<Action>
    Board::legal_takes_for_piece(int i, int j, bool is_king,
				 Player player) const
  {
    auto take_coords =
      this->legal_takes_for_piece_rec(i, j, is_king, player,
    				      vector<pair<int, int>>());
    vector<Action> takes;
    for (auto it = take_coords.begin(); it != take_coords.end(); ++it) {
      takes.push_back(convert_take_chain(*it, i, j, player));
    }
    return takes;
  }

  // prev_taken is passed by value because we modify it but don't want
  // the changes to be seen by the parent call. It's like a reader
  // context.
  vector<vector<pair<int, int>>>
    Board::legal_takes_for_piece_rec(int i, int j, bool is_king, Player player,
				     vector<pair<int, int>> prev_taken) const
  {
    Player other = OTHER_PLAYER(player);
    int forward = player == P1 ? 1 : -1;
    vector<vector<pair<int, int>>> takes;
    if (j > 1) {
      // Take forward and west
      pair<int, int> take = make_pair(i+2*forward, j-2);
      if (IN_BOUNDS(i+2*forward) &&
	  board[take.first][take.second] == Square::empty &&
	  IS_PLAYER(board[i+forward][j-1], other) &&
	  !elem(make_pair(i+forward, j-1), prev_taken)) {
	auto taken = make_pair(i+forward, j-1);
	prev_taken.push_back(taken);
	auto rest_takes =
	  this->legal_takes_for_piece_rec(i+2*forward, j-2, is_king,
					  player, prev_taken);
	if (rest_takes.empty()) {
	  takes.push_back(singleton(take));
	}
	else {
	  auto l = prefix_vectors(take, rest_takes);
	  takes.insert(takes.end(), std::make_move_iterator(l.begin()),
	  	       std::make_move_iterator(l.end()));
	}
      }
      // Take backward and west
      take = make_pair(i-2*forward, j-2);
      if (is_king && IN_BOUNDS(i-2*forward) &&
	  IS_EMPTY(this->board[take.first][take.second]) &&
	  IS_PLAYER(this->board[i-forward][j-1], other) &&
	  !elem(make_pair(i-forward, j-1), prev_taken)) {
	auto taken = make_pair(i-forward, j-1);
	prev_taken.push_back(taken);
	auto rest_takes =
	  this->legal_takes_for_piece_rec(i-2*forward, j-2, is_king,
					  player, prev_taken);
	if (rest_takes.empty()) {
	  takes.push_back(singleton(take));
	}
	else {
	  auto l = prefix_vectors(take, rest_takes);
	  takes.insert(takes.end(), std::make_move_iterator(l.begin()),
	  	       std::make_move_iterator(l.end()));
	}
      }
    }
    if (j < BOARD_SIZE-2) {
      // Take forward and east
      auto take = make_pair(i+2*forward, j+2);
      if (IN_BOUNDS(i+2*forward) &&
	  IS_EMPTY(this->board[take.first][take.second]) &&
	  IS_PLAYER(this->board[i+forward][j+1], other) &&
	  !elem(make_pair(i+forward, j+1), prev_taken)) {
	auto taken = make_pair(i+forward, j+1);
	prev_taken.push_back(taken);
	auto rest_takes =
	  this->legal_takes_for_piece_rec(i+2*forward, j+2, is_king,
					  player, prev_taken);
	if (rest_takes.empty()) {
	  takes.push_back(singleton(take));
	}
	else {
	  auto l = prefix_vectors(take, rest_takes);
	  takes.insert(takes.end(), std::make_move_iterator(l.begin()),
	  	       std::make_move_iterator(l.end()));
	}
      }
      // Take backward and east
      take = make_pair(i-2*forward, j+2);
      if (is_king && IN_BOUNDS(i-2*forward) &&
	  IS_EMPTY(this->board[take.first][take.second]) &&
	  IS_PLAYER(this->board[i-forward][j+1], other) &&
	  !elem(make_pair(i-forward, j+1), prev_taken)) {
	auto taken = make_pair(i-forward, j+1);
	prev_taken.push_back(taken);
	auto rest_takes =
	  this->legal_takes_for_piece_rec(i-2*forward, j+2, is_king,
					  player, prev_taken);
	if (rest_takes.empty()) {
	  takes.push_back(singleton(take));
	}
	else {
	  auto l = prefix_vectors(take, rest_takes);
	  takes.insert(takes.end(), make_move_iterator(l.begin()),
	  	       make_move_iterator(l.end()));
	}
      }
    }
    return takes;
  }

  vector<Move>
  Board::legal_moves_for_piece(int i, int j, bool is_king, Player player) const
  {
    auto forward = player == P1 ? 1 : -1;
    vector<Move> moves;
    if (j > 0) {
      if (IN_BOUNDS(i+forward) && IS_EMPTY(this->board[i+forward][j-1])) {
	// Move forward and west
	moves.push_back(Move(i, j, i+forward, j-1, MoveKind::move, player));
      }
      if (is_king && IN_BOUNDS(i-forward) &&
	  IS_EMPTY(this->board[i-forward][j-1])) {
	// Move backward and west
	moves.push_back(Move(i, j, i-forward, j-1, MoveKind::move, player));
      }
    }
    if (j < BOARD_SIZE-1) {
      if (IN_BOUNDS(i+forward) && IS_EMPTY(this->board[i+forward][j+1])) {
	// Move forward and east
	moves.push_back(Move(i, j, i+forward, j+1, MoveKind::move, player));
      }
      if (is_king && IN_BOUNDS(i-forward) &&
	  IS_EMPTY(this->board[i-forward][j+1])) {
	// Move backward and east
	moves.push_back(Move(i, j, i-forward, j+1, MoveKind::move, player));
      }
    }
    return moves;
  }

  vector<Action> Board::legal_actions(Player player) const
  {
    bool found_take = false;
    vector<Action> all_actions;
    // Look for takes
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
	auto piece = this->board[i][j];
	if (!IS_EMPTY(piece) && PLAYER_OF(piece) == player) {
	  auto takes =
	    this->legal_takes_for_piece(i, j, IS_KING(piece), player);
	  if (!takes.empty()) {
	    found_take = true;
	    all_actions.insert(all_actions.end(),
			       make_move_iterator(takes.begin()),
			       make_move_iterator(takes.end()));
	  }
	}
      }
    }
    // If no takes found, look for moves
    if (!found_take) {
      for (int i = 0; i < BOARD_SIZE; ++i) {
	for (int j = 0; j < BOARD_SIZE; ++j) {
	  auto piece = this->board[i][j];
	  if (!IS_EMPTY(piece) && PLAYER_OF(piece) == player) {
	    auto moves =
	      this->legal_moves_for_piece(i, j, IS_KING(piece), player);
	    vector<Action> actions;
	    for (auto it = moves.begin(); it != moves.end(); ++it) {
	      actions.push_back(Action(singleton(*it)));
	    }
	    all_actions.insert(all_actions.begin(),
			       make_move_iterator(actions.begin()),
			       make_move_iterator(actions.end()));
	  }
	}
      }
    }
    return all_actions;
  }

  void Board::apply_action(const Action &action)
  {
    for (auto it = action.moves.begin(); it != action.moves.end(); ++it) {
      auto m = *it;
      this->board[m.i2][m.j2] = this->board[m.i1][m.j1];
      this->board[m.i1][m.j1] = Square::empty;
      if (m.kind == MoveKind::take) {
	auto i = (m.i1 + m.i2) / 2, j = (m.j1 + m.j2) / 2;
	this->board[i][j] = Square::empty;
      }
      // Promote to king if able
      if (m.player == Player::P1 && m.i2 == BOARD_SIZE-1) {
	board[m.i2][m.j2] = Square::P1_king;
      }
      else if (m.player == Player::P2 && m.i2 == 0) {
	board[m.i2][m.j2] = Square::P2_king;
      }
    }
  }

  // void Board::set_eval_function(const std::function<double(const State&)>
  // 				&eval_function)
  // {
  //   this->eval_function = eval_function;
  // }
  
  namespace
  {
    inline char square_char(Square sq)
    {
      switch (sq) {
      case empty:
	return ' ';
      case P1_piece:
	return 'x';
      case P1_king:
	return 'X';
      case P2_piece:
	return 'o';
      case P2_king:
	return 'O';
      default:
	return ' ';
      }
    }
  }

  ostream& operator<<(ostream& os, const Board& b)
  {
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
	os << square_char(b.board[i][j]) << " ";
      }
      os << endl;
    }
    return os;
  }

  // void Board::print() const
  // {
  //   for (int i = 0; i < BOARD_SIZE; ++i) {
  //     for (int j = 0; j < BOARD_SIZE; ++j) {
  // 	cout << square_char(this->board[i][j]) << " ";
  //     }
  //     cout << endl;
  //   }
  // }

  void Board::print() const
  {
    cout << *this << endl;
  }

  // void Move::print() const
  // {
  //   cout << '(' << this->i1 << ", " << this->j1 << ") (" << this->i2 <<
  //     ", " << this->j2 << ") " << this->kind << ' ' << this->player;
  // }

  ostream& operator<<(ostream& os, const Move& m)
  {
    os << '(' << m.i1 << ", " << m.j1 << ") (" << m.i2 <<
      ", " << m.j2 << ") " << m.kind << ' ' << m.player;
    return os;
  }

  ostream& operator<<(ostream &os, const vector<Move> &ms)
  {
    os << "[";
    for (size_t i = 0; i < ms.size(); ++i) {
      os << ms[i];
      if (i < ms.size() - 1) {
	os << ", ";
      }
    }
    os << "]";
    return os;
  }

  std::ostream& operator<<(std::ostream &os, const Action &a)
  {
    os << a.moves;
    return os;
  }

  bool Board::operator==(const Board &other) const
  {
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
	if (this->board[i][j] != other.board[i][j]) {
	  return false;
	}
      }
    }
    return true;
  }

  bool Board::operator<(const Board &other) const
  {
    for (int i = 0; i < BOARD_SIZE; ++i) {
      for (int j = 0; j < BOARD_SIZE; ++j) {
	if (this->board[i][j] < other.board[i][j]) {
	  return true;
	}
	else if (this->board[i][j] > other.board[i][j]) {
	  return false;
	}
      }
    }
    return false;
  }
}
