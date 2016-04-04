
#pragma once

#include "../agent/Agent.hpp"
#include "../common/Action.hpp"
#include "../common/Common.hpp"
#include "../common/State.hpp"
#include <utility>

using namespace std;

class LearningAgent : public Agent {
public:
  LearningAgent(double futureDiscount);
  virtual ~LearningAgent();

  uptr<Action> ChooseAction(State *state) override;

  void ActionOutcome(pair<State *, Action *> performed, pair<State *, double> outcome);
  void SetLearnRate(double learnRate);
  void SetTemperature(double t);

private:
  struct LearningAgentImpl;
  uptr<LearningAgentImpl> impl;
};
