#include <chrono>
#include <cmath>
#include <map>
#include <random>
#include "mcts.h"

using namespace std;

#define C_p (1.0 / sqrt(2))

#define NONTERMINAL(node) \
  (!node->unvisited_actions.empty() || !node->children.empty())

namespace checkers
{
  namespace MCTS
  {
    namespace
    {
      static auto gen = ranlux48_base(random_device()());
      static auto dist = uniform_real_distribution<>(0.0, 1.0);

      static map<State, pair<double, int>> store;

      void update_store(const Node *node)
      {
	if (store.count(node->state)) {
	  store.at(node->state) =
	    make_pair(node->total_reward, node->visit_count);
	}
	else {
	  store.emplace(node->state,
			make_pair(node->total_reward,
				  node->visit_count));
	}
	for (auto it = node->children.begin();
	     it != node->children.end(); ++it) {
	  update_store(*it);
	}
      }

      // Load a node from the store
      Node* load_node(Node *parent, const State &s, const Action &a)
      {
      	Node *node = new Node(parent, s, a);
      	if (store.count(s)) {
      	  auto p = store[s];
      	  node->total_reward = p.first;
      	  node->visit_count = p.second;
      	  node->avg_reward = p.first / p.second;
      	}
      	return node;
      }
    }

    // Forward declare everything used by UCTSearch.
    Node* TreePolicy(Node *root);
    Node* Expand(Node *root);
    Node* BestChild(const Node *node);
    double DefaultPolicy(const State &state);
    void Backup(Node *node, double reward);

    // The primary search function to be used from outside.
    Action UCTSearch(const State &state, int time_limit_ms)
    {
      // Load the root node from the store if possible.
      Node *root = load_node(nullptr, state, Action::nil());
      int count = 0;

      auto start_time = chrono::steady_clock::now();
      while (chrono::duration_cast<chrono::milliseconds>
	     (chrono::steady_clock::now() - start_time).count() <
	     time_limit_ms) {
	Node *v = TreePolicy(root);
	double reward = DefaultPolicy(v->state);
	Backup(v, reward);
	++count;
      }

      cout << "mcts ran for " << count << " iterations" << endl;
      cout << "tree depth: " << tree_depth(root) << endl;
      cout << "updating store..." << endl;
      update_store(root);
      cout << "store size: " << store.size() << endl;

      Action best = BestChild(root)->action;
      delete root;
      return best;
    }

    Node* TreePolicy(Node *root)
    {
      while (NONTERMINAL(root)) {
	if (!root->unvisited_actions.empty()) {
	  return Expand(root);
	}
	else {
	  root = BestChild(root);
	}
      }
      return root;
    }

    Node* Expand(Node *root)
    {
      Action a = root->unvisited_actions.back();
      root->unvisited_actions.pop_back();
      State s(root->state);
      s.apply_action(a);
      Node *child = load_node(root, s, a);
      root->children.push_back(child);
      return child;
    }

    Node* BestChild(const Node *node)
    {
      double best_value = numeric_limits<double>::lowest();
      Node *best_child = nullptr;
      for (auto it = node->children.begin();
	   it != node->children.end(); ++it) {
	auto child = *it;
	// Can the child's visit count ever be 0? Seems that a child
	// is always visited when it is first created, so visit_count
	// should always be at least 1.
	double value = child->avg_reward +
	  C_p * sqrt((2 * log(node->visit_count)) / child->visit_count);
	if (value > best_value) {
	  best_value = value;
	  best_child = child;
	}
      }
      
      if (!best_child) {
	cout << "WARNING: BestChild: returning null pointer" << endl;
      }
      return best_child;
    }

    // Uniform random playout.
    double DefaultPolicy(const State &state)
    {
      State s(state);
      auto actions = s.board.legal_actions(s.get_cur_player());
      while (!actions.empty()) {
    	double x = dist(gen);
    	int i = static_cast<int>(x * actions.size());
    	Action a = actions[i];
        s.apply_action(a);
    	actions = s.board.legal_actions(s.get_cur_player());
      }
      // return s.get_cur_player() == state.get_cur_player() ? -1.0 : 1.0;
      return s.get_cur_player() == state.get_cur_player() ? 1.0 : 0.0;
    }

    // Piece differential board evaluation function.
    // double DefaultPolicy(const State &state)
    // {
    //   // return state.evaluate(state.get_cur_player()) / -15.0;
    //   return (-state.evaluate(state.get_cur_player()) + 15.0) / 30.0;
    // }

    void Backup(Node *node, double reward)
    {
      while (node != nullptr) {
	++node->visit_count;
	node->total_reward += reward;
	node->avg_reward = node->total_reward / node->visit_count;
	reward = -reward;
	node = node->parent;
      }
    }
  }
}
