#include <iostream>
#include <string>
#include "board.h"
#include "mcts.h"
#include "minimax.h"
#include "state.h"

using namespace std;
using namespace checkers;
using namespace MCTS;

// The minimax agent only checks the time between each search
// iteration (iterative deepening), so it may exceed the time limit
// set here. That's why we give it less time than MCTS.
#define MINIMAX_TIME_LIMIT 1000

// MCTS doesn't really exceed its time limit (maybe by a few ms in the
// worst case).
#define MCTS_TIME_LIMIT 5000

int main()
{
  // Initial state
  State s;
  s.print();

  // Run a game
  for (int i = 0; ; ++i) {
    auto actions = s.board.legal_actions(s.get_cur_player());
    if (actions.size() == 1) {
      s.apply_action(actions[0]);
    }
    else if (!actions.size()) {
      // The game is over if the current player can't make a move.
      break;
    }
    else {
      if (i % 2) {
	// Player 2 uses minimax.
	auto action_score = ABS_deepening(s, MINIMAX_TIME_LIMIT);
	auto action = action_score.first;
	auto score = action_score.second;
	cout << "minimax: " << action << " " << score << endl;
	s.apply_action(action);
      }
      else {
	// Player 1 uses MCTS.
	auto action = UCTSearch(s, MCTS_TIME_LIMIT);
	cout << "mcts: " << action << endl;
	s.apply_action(action);

	// Some code for moving uniformly at random.
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

  return 0;
}
