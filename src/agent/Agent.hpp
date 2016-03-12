
#pragma once

#include "../common/Action.hpp"
#include "../common/Common.hpp"
#include "../common/State.hpp"

class Agent {
public:
  virtual ~Agent() = default;
  virtual uptr<Action> ChooseAction(State *state) = 0;
};
