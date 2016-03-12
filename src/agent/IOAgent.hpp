
#pragma once

#include "../common/Common.hpp"
#include "Agent.hpp"

class IOAgent : public Agent {
public:
  uptr<Action> ChooseAction(State *state) override;
};
