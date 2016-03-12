
#pragma once

#include "../common/Action.hpp"
#include "../common/Common.hpp"
#include "CellState.hpp"
#include "GameState.hpp"

class GameAction : public Action {
public:
  GameAction(unsigned x, unsigned y);
  virtual ~GameAction();

  void Apply(GameState &state) const;

  uptr<Action> Clone(void) const override;
  bool operator==(const Action &obj) const override;
  size_t HashCode(void) const override;
  void Output(std::ostream &out) const override;

private:
  class GameActionImpl;
  uptr<GameActionImpl> impl;
};
