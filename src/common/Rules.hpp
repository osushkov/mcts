
#pragma once

#include "Common.hpp"
#include "State.hpp"

class Rules {
public:
  virtual ~Rules() = default;

  virtual uptr<State> InitialState(void) const = 0;

  // Returns whether it is possible to continue playing.
  virtual bool IsTerminalState(const State &state) const = 0;

  // Returns whether the current actor has won the game.
  virtual bool IsWin(const State &state) const = 0;

  // Returns whether the current actor has lost the game.
  virtual bool IsLoss(const State &state) const = 0;
};
