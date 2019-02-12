#include <iostream>
#include <string>
#include "board.h"
#include "mcts.h"
#include "minimax.h"
#include "state.h"

using namespace std;
using namespace checkers;
using namespace MCTS;

// #define MINIMAX_TIME_LIMIT 1000
// #define MCTS_TIME_LIMIT 3000

#define MINIMAX_TIME_LIMIT 1000
#define MCTS_TIME_LIMIT 5000

int main()
{
  State s;
  s.print();

  for (int i = 0; ; ++i) {
    auto actions = s.board.legal_actions(s.get_cur_player());
    if (actions.size() == 1) {
      s.apply_action(actions[0]);
    }
    else if (!actions.size()) {
      break;
    }
    else {
      if (i % 2) {
	auto action_score = ABS_deepening(s, MINIMAX_TIME_LIMIT);
	auto action = action_score.first;
	auto score = action_score.second;
	cout << "minimax: " << action << " " << score << endl;
	s.apply_action(action);
      }
      else {
	auto action = UCTSearch(s, MCTS_TIME_LIMIT);
	cout << "mcts: " << action << endl;
	s.apply_action(action);
	// auto gen = ranlux48_base(random_device()());
	// auto dist = uniform_real_distribution<>(0.0, 1.0);
	// double x = dist(gen);
    	// int i = static_cast<int>(x * actions.size());
    	// Action a = actions[i];
	// cout << "mcts: " << a << endl;
	// s.apply_action(a);
      }
    }
    s.print();
    // cin.ignore();
  }

  // State s2(s);
  // s2.board.print();

  return 0;
}
