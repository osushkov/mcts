
#pragma once

#include "../common/Common.hpp"
#include "../common/Rules.hpp"
#include "CellState.hpp"
#include "GameState.hpp"

class GameRules : public Rules {
public:
  GameRules(unsigned completionRun);
  virtual ~GameRules();

  bool IsTerminalState(const State &state) const override;
  bool IsWin(const State &state) const override;

private:
  class GameRulesImpl;
  uptr<GameRulesImpl> impl;
};
