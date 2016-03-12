
#pragma once

#include "Agent.hpp"
#include "common/State.hpp"
#include "common/Action.hpp"
#include <memory>
#include <utility>

using namespace std;


class LearningAgent : public Agent {
public:

  LearningAgent(double futureDiscount);
  virtual ~LearningAgent();

  uptr<Action> chooseAction(State* state) override;
  void actionOutcome(pair<State*, Action*> performed, pair<State*, double> outcome) override;
  void setLearnRate(double learnRate) override;
  void setPRandom(double pRandom) override;

private:
  class LearningAgentImpl;
  unique_ptr<LearningAgentImpl> impl;
};
