#include <chrono>
#include <iostream>
#include <limits>
#include "minimax.h"
#include "state.h"
#include "util.h"

using namespace std;
using namespace util;

#define TERMINAL_SCORE 15

namespace checkers
{
  // Iterative deepening.
  pair<Action, double> ABS_deepening(const State &state, int time_limit_ms)
  {
    auto start_time = chrono::steady_clock::now();
    int d = 1;
    auto move_score = ABS(state, d);
    while (abs(move_score.second) != TERMINAL_SCORE &&
	   chrono::duration_cast<chrono::milliseconds>
	   (chrono::steady_clock::now() - start_time).count() <
	   time_limit_ms) {
      d += 1;
      move_score = ABS(state, d);
    }
    cout << "reached depth " << d << endl;
    return move_score;
  }

  // Forward declares
  pair<Action, double> ABS_max(const State&, double, double, int);
  double ABS_min(const State&, double, double, int);

  // Depth-limited search.
  pair<Action, double> ABS(const State &state, int d)
  {
    return ABS_max(state, numeric_limits<double>::lowest(),
		   numeric_limits<double>::max(), d);
  }

  pair<Action, double>
  ABS_max(const State &state, double alpha, double beta, int d)
  {
    auto actions = state.board.legal_actions(state.get_cur_player());
    if (actions.empty()) {
      return make_pair(Action::nil(), -TERMINAL_SCORE);
    }
    else if (d <= 0 && actions[0].moves[0].kind != MoveKind::take) {
      return make_pair(Action::nil(), state.evaluate(P2));
    }
    else {
      double v = numeric_limits<double>::lowest();
      int best_i = -1;
      for (size_t i = 0; i < actions.size(); ++i) {
	auto action = actions[i];
	State s(state);
	s.apply_action(action);
	double x = ABS_min(s, alpha, beta, d-1);
	if (x > v) {
	  v = x;
	  best_i = i;
	}
	if (v >= beta) {
	  return make_pair(actions[i], v);
	}
	alpha = max(alpha, v);
      }
      return make_pair(actions[best_i], v);
    }
  }

  double
  ABS_min(const State &state, double alpha, double beta, int d)
  {
    auto actions = state.board.legal_actions(state.get_cur_player());
    if (actions.empty()) {
      return TERMINAL_SCORE;
    }
    else if (d <= 0 && actions[0].moves[0].kind != MoveKind::take) {
      return state.evaluate(P2);
    }
    else {
      double v = numeric_limits<double>::max();
      for (size_t i = 0; i < actions.size(); ++i) {
	auto action = actions[i];
	State s(state);
	s.apply_action(action);
	auto p = ABS_max(s, alpha, beta, d-1);
	if (p.second < v) {
	  v = p.second;
	}
	if (v <= alpha) {
	  return v;
	}
	beta = min(beta, v);
      }
      return v;
    }
  }
}
