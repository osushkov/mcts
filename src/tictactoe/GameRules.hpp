
#pragma once

#include "../common/Common.hpp"
#include "../common/Rules.hpp"
#include "CellState.hpp"
#include "GameState.hpp"
#include <memory>

using namespace std;


class GameRules : public Rules {
public:

  GameRules(unsigned completionRun);
  virtual ~GameRules();

  bool isTerminalState(const State &state) const override;
  bool isWin(const State &state) const override;

private:
  class GameRulesImpl;
  uptr<GameRulesImpl> impl;
};
