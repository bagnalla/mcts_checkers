#ifndef MCTS_H
#define MCTS_H

#include "tree.h"

namespace checkers
{
  namespace MCTS
  {
    // Monte carlo tree search with UCB
    Action UCTSearch(const State &state, // root state
		     int time_limit_ms); // time budget in milliseconds
  }
}

#endif
