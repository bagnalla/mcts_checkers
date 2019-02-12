#ifndef MINIMAX_H
#define MINIMAX_H

#include <algorithm>
#include <functional>
#include <vector>
#include "state.h"

namespace checkers
{
  // Iterative deepening alpha-beta minimax search.
  std::pair<Action, double>
    ABS_deepening(const State &state, int time_limit_ms);

  // Depth-limited alpha-beta minimax search.
  std::pair<Action, double>
    ABS(const State &state, int d);
}

#endif
