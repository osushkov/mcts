
#pragma once

#include "Agent.hpp"
#include <memory>

using namespace std;


class IOAgent : public Agent {
public:

  uptr<Action> chooseAction(State* state) override;
  void actionOutcome(pair<State*, Action*> performed, pair<State*, double> outcome) override;
  void setLearnRate(double learnRate) override;
  void setPRandom(double pRandom) override;

};
