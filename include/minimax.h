#ifndef MINIMAX_H
#define MINIMAX_H

#include <algorithm>
#include <functional>
#include <vector>
#include "state.h"

namespace checkers
{
  std::pair<Action, double>
    ABS_deepening(const State &state, int time_limit_ms);

  std::pair<Action, double>
    ABS(const State &state, int d);

  std::pair<Action, double>
    ABS_max(const State &state, double alpha, double beta, int d);

  double
    ABS_min(const State &state, double alpha, double beta, int d);
}

#endif
