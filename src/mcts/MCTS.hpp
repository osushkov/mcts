
#pragma once

#include "../common/Common.hpp"
#include "../common/State.hpp"
#include "../tictactoe/GameRules.hpp"

// An action and the probability of winning by taking that action.
using ActionUtility = pair<Action *, double>;

// Monte-Carlo Tree Search.
class MCTS {
public:
  MCTS();
  virtual ~MCTS();

  // Sorted list of action utilities.
  vector<ActionUtility> ComputeUtilities(State *curState);

private:
  struct MCTSImpl;
  uptr<MCTSImpl> impl;
};
