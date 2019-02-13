#include <limits>
#include "tree.h"

using namespace std;

namespace checkers
{
  static int node_id_counter = 0;

  Node::Node(Node *parent, const State &state, const Action &action)
  {
    this->init(parent, state, action);
    this->unvisited_actions =
      state.board.legal_actions(state.get_cur_player());
  }

  void Node::init(Node *parent, const State &state, const Action &action)
  {
    this->id = node_id_counter++;
    this->parent = parent;
    this->state = state;
    this->action = action;
    this->total_reward = 0.0;
    this->avg_reward = numeric_limits<double>::max();
    this->visit_count = 0;
  }

  Node::~Node()
  {
    for (auto it = this->children.begin();
	 it != this->children.end(); ++it) {
      delete *it;
    }
  }

  int tree_size(const Node *tree)
  {
    int sum = 1;
    for (auto it = tree->children.begin(); it != tree->children.end(); ++it) {
      sum += tree_size(*it);
    }
    return sum;
  }

  int tree_depth(const Node *tree)
  {
    int max_depth = 0;
    for (auto it = tree->children.begin(); it != tree->children.end(); ++it) {
      int d = tree_depth(*it);
      if (d > max_depth) {
	max_depth = d;
      }
    }
    return 1 + max_depth;
  }
}
