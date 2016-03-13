
#pragma once

#include "../common/Action.hpp"
#include "../common/Common.hpp"
#include "../common/State.hpp"

// A node of a search tree.
class Node {
  using Edge = pair<uptr<Action>, uptr<Node>>;

public:
  Node(Node *parent, uptr<State> state, unsigned playerIndex);
  virtual ~Node() = default;

  bool IsLeaf(void) const;
  unsigned PlayerIndex(void) const;

  // TODO: should add a policy object as input that will choose which action to expand.
  // Returns a new node that is generated by an action from this node.
  Node *Expand(void); // Can only be done on a 'leaf'

  // TODO: should add a policy object as input that will choose which edge to select.
  // For now it's simply e-greedy.
  Node *Select(double pRandom); // Should only be done on non-leaves.

  void AddUtility(double utility);

  // From this node, what is the probability the given player will win.
  double PWin(unsigned playerIndex) const;

private:
  Node *parent;
  uptr<State> state;
  vector<Edge> children;

  // This should be an enum maybe. This signifies which players turn it is for this node,
  // since this is an adversarial game, it is not always "my" turn, where "me" is defined
  // as the player at the root of the tree.
  unsigned playerIndex; // index 0 is "me", index 1 is opponent.
  bool isLeaf;

  unsigned totalTrials;
  double sumUtility;

  vector<uptr<Action>> nonExpandedActions(void);
};