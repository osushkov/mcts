
#pragma once

#include "../common/Common.hpp"
#include "../common/Rules.hpp"
#include "CellState.hpp"
#include "GameState.hpp"

class GameRules : public Rules {
public:
  static GameRules* instance(void); // singleton rules.

  virtual ~GameRules();

  uptr<State> InitialState(void) const override;
  bool IsTerminalState(const State &state) const override;
  bool IsWin(const State &state) const override;
  bool IsLoss(const State &state) const override;

private:
  GameRules(unsigned completionRun);

  struct GameRulesImpl;
  uptr<GameRulesImpl> impl;
};
