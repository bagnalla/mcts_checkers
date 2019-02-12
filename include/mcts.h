#ifndef MCTS_H
#define MCTS_H

#include "tree.h"

namespace checkers
{
  namespace MCTS
  {
    Action UCTSearch(const State &state, int time_limit_ms);
    /* std::pair<Action,  UCTSearch(const State &state, int time_limit_ms); */
    Node* TreePolicy(Node *root);
    Node* Expand(Node *root);
    Node* BestChild(const Node *node);
    double DefaultPolicy(const State &state);
    void Backup(Node *node, double reward);
  }
}

#endif
