
#pragma once

#include "State.hpp"

class Rules {
public:
  virtual ~Rules() = default;

  // Returns whether it is possible to continue playing.
  virtual bool IsTerminalState(const State &state) const = 0;

  // Returns whether the current actor has won the game.
  virtual bool IsWin(const State &state) const = 0;
};
