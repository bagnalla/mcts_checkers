#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>

#define BOARD_SIZE 8

namespace checkers
{
  typedef unsigned char byte;
  enum Player : byte;
  enum Square : byte { empty, P1_piece, P1_king, P2_piece, P2_king };
  enum MoveKind : byte { move, take };


  // A single move of a piece.
  struct Move
  {
    Move(int i1, int j1, int i2, int j2, MoveKind kind, Player player)
    : i1(i1), j1(j1), i2(i2), j2(j2), kind(kind), player(player) {}
    void print() const;
    int i1, j1, i2, j2;
    MoveKind kind;
    Player player;
  };

  std::ostream& operator<<(std::ostream &os, const Move &m);
  std::ostream& operator<<(std::ostream &os, const std::vector<Move> &ms);


  // A player actions is sequence of piece moves. In most cases it's a
  // singleton, but sometimes there are multiple jumps in a single
  // action.
  struct Action
  {
    Action() {};
    Action(const std::vector<Move> &moves) : moves(moves) {};
    static Action nil() { return Action(); }
    std::vector<Move> moves;
  };

  std::ostream& operator<<(std::ostream &os, const Action &a);


  // A board contains the board state and provides methods for
  // computing legal moves and evaluating the current position.
  class Board
  {
  public:
    Board();
    Board(Square board[BOARD_SIZE][BOARD_SIZE]);
    std::vector<Action>
      legal_takes_for_piece(int i, int j, bool is_king, Player player) const;
    std::vector<Move>
      legal_moves_for_piece(int i, int j, bool is_king, Player player) const;
    std::vector<Action> legal_actions(Player player) const;
    void apply_action(const Action &a);
    void print() const;
    double evaluate(Player p) const;
    bool operator==(const Board &other) const;
    bool operator<(const Board &other) const;
  private:
    Square board[BOARD_SIZE][BOARD_SIZE];
    void init();
    std::vector<std::vector<std::pair<int, int>>>
      legal_takes_for_piece_rec(int i, int j, bool is_king, Player player,
				std::vector<std::pair<int, int>>
				prev_taken) const;
    friend std::ostream& operator<<(std::ostream& out, const Board& b);
  };

  std::ostream& operator<<(std::ostream &os, const Board &m);
}

#endif
