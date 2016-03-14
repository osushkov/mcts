
#pragma once

#include "../common/Common.hpp"
#include "../common/Rules.hpp"
#include "CellState.hpp"
#include "GameState.hpp"

class GameRules : public Rules {
public:
  GameRules(unsigned completionRun);
  GameRules(const GameRules &other);

  virtual ~GameRules();

  bool IsTerminalState(const State &state) const override;
  bool IsWin(const State &state) const override;
  bool IsLoss(const State &state) const override;

private:
  struct GameRulesImpl;
  uptr<GameRulesImpl> impl;
};
