
#pragma once

#include "../common/Common.hpp"
#include "../common/Action.hpp"
#include "CellState.hpp"
#include "GameState.hpp"
#include <memory>

using namespace std;


class GameAction : public Action {
public:

  GameAction(unsigned x, unsigned y);
  virtual ~GameAction();

  void apply(GameState &state) const;

  uptr<Action> clone(void) const override;
  bool operator== (const Action& obj) const override;
  size_t hashCode(void) const override;
  void output(std::ostream &out) const override;

private:
  class GameActionImpl;
  unique_ptr<GameActionImpl> impl;
};
