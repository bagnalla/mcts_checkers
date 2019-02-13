#ifndef TREE_H
#define TREE_H

#include <map>
#include <vector>
#include "state.h"

namespace checkers
{
  // Game search tree nodes.
  struct Node
  {
    Node(Node *parent, const State &state, const Action &action);
    ~Node();
    int id;
    Node *parent;
    State state;
    Action action;
    double total_reward;
    double avg_reward;
    unsigned int visit_count;
    std::vector<Action> unvisited_actions;
    std::vector<Node*> children;
  private:
    void init(Node *parent, const State &state, const Action &action);
  };

  int tree_size(const Node *tree);
  int tree_depth(const Node *tree);
}

#endif
