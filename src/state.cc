#include <cstring>
#include "state.h"
#include "util.h"

using namespace std;
using namespace util;

namespace checkers
{
  State::State()
  {
    this->cur_player = P1;
  }

  State::State(Player cur_player, const Board &board)
  {
    this->cur_player = cur_player;
    this->board = board;
  }

  Player State::next()
  {
    this->cur_player = OTHER_PLAYER(this->cur_player);
    return this->cur_player;
  }

  Player State::get_cur_player() const
  {
    return this->cur_player;
  }

  double State::evaluate(Player p) const
  {
    return this->board.evaluate(p);
  }

  void State::apply_action(const Action &a)
  {
    this->board.apply_action(a);
    next();
  }

  ostream& operator<<(ostream& os, const State& s)
  {
    os << s.board << "Player " <<
      (s.cur_player == Player::P1 ? 1 : 2) << "'s turn.";
    return os;
  }

  void State::print() const
  {
    cout << *this << endl;
  }

  bool State::operator==(const State &other) const
  {
    return this->board == other.board && this->cur_player == other.cur_player;
  }

  bool State::operator<(const State &other) const
  {
    return (this->cur_player < other.cur_player) ||
      (this->cur_player == other.cur_player && this->board < other.board);
  }
}
