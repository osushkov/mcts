
#pragma once

#include "Agent.hpp"
#include <vector>

using namespace std;


class RandomAgent : public Agent {
public:

  uptr<Action> chooseAction(State* state) override {
    vector<uptr<Action>> actions(state->availableActions());
    unsigned index = rand() % actions.size();
    return move(actions[index]);
  }

  void actionOutcome(pair<State*, Action*> performed, pair<State*, double> outcome) override {}
  void setLearnRate(double learnRate) override {}
  void setPRandom(double pRandom) override {}

};
