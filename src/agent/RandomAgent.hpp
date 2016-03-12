
#pragma once

#include "../common/Common.hpp"
#include "Agent.hpp"
#include <vector>

class RandomAgent : public Agent {
public:
  uptr<Action> ChooseAction(State *state) override {
    vector<uptr<Action>> actions(state->AvailableActions());
    unsigned index = rand() % actions.size();
    return move(actions[index]);
  }
};
