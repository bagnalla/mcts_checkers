#ifndef STATE_H
#define STATE_H

#include <vector>
#include "board.h"

#define BOARD_SIZE 8

#define OTHER_PLAYER(player) (player == P1 ? P2 : P1)

#define IS_PLAYER(square, player) (player == P1 ? \
    (square == P1_piece || square == P1_king) : \
    (square == P2_piece || square == P2_king))

namespace checkers
{
  enum Player : unsigned char { P1, P2 };

  struct State
  {
  public:
    State();
    State(Player cur_player, const Board &board);
    Player next(); // Change current player
    void print() const;
    Player get_cur_player() const;
    double evaluate(Player p) const;
    void apply_action(const Action &a); // Also calls next()
    Board board;
    bool operator==(const State &other) const;
    bool operator<(const State &other) const;
  private:
    Player cur_player;
    friend std::ostream& operator<<(std::ostream& out, const State& s);
  };

  std::ostream& operator<<(std::ostream &os, const State &s);
}

#endif
