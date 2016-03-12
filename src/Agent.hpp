
#pragma once

#include "common/State.hpp"
#include "common/Action.hpp"
#include <utility>

using namespace std;


class Agent {
public:
  virtual ~Agent() = default;

  virtual uptr<Action> chooseAction(State* state) = 0;
  virtual void actionOutcome(pair<State*, Action*> performed, pair<State*, double> outcome) = 0;
  virtual void setLearnRate(double learnRate) = 0;
  virtual void setPRandom(double pRandom) = 0;
};
