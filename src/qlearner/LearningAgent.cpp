
#include "LearningAgent.hpp"
#include "../util/Util.hpp"
#include <cassert>
#include <cstdlib>
#include <iostream>
#include <random>
#include <unordered_map>

static double randomInitialValue(void) {
  static mt19937 gen;
  static normal_distribution<> distribution(0.0, 0.1);
  return distribution(gen);
}

struct ActionValue {
  uptr<Action> action;
  double value;

  ActionValue(uptr<Action> action, double value) : action(move(action)), value(value) {}

  ActionValue(uptr<Action> action) : ActionValue(move(action), randomInitialValue()) {}
};

struct LearningAgent::LearningAgentImpl {

  double futureDiscount;
  double learnRate;

  double temperature; // Botzmann temperature

  unordered_map<State *, vector<uptr<ActionValue>>, hash<State *>, State::StatePEquals>
      stateActions;

  // This is here mainly because I dont think you can use unique_ptr as a key in the map easily.
  vector<uptr<State>> ownedStates;

  LearningAgentImpl(double futureDiscount)
      : futureDiscount(futureDiscount), learnRate(0.5), temperature(0.0) {
    assert(futureDiscount >= 0.0 && futureDiscount <= 1.0);
    assert(learnRate >= 0.0 && learnRate <= 1.0);
  }

  uptr<Action> ChooseAction(State *state) {
    if (!haveSeenState(state)) {
      handleNewState(state);
    }

    return chooseActionForState(state);
  }

  void ActionOutcome(pair<State *, Action *> performed, pair<State *, double> outcome) {
    State *startState = performed.first;
    Action *actionPerformed = performed.second;

    State *resultState = outcome.first;
    double reward = outcome.second;

    if (!haveSeenState(startState)) {
      handleNewState(startState);
    }

    if (!haveSeenState(resultState)) {
      handleNewState(resultState);
    }

    ActionValue *curValue = findActionValue(startState, actionPerformed);
    assert(curValue != nullptr);

    double newQ = reward + futureDiscount * maxQ(resultState);
    // cout << "r: " << reward << " fd: " << futureDiscount << " newQ: " << newQ << endl;
    curValue->value += learnRate * (newQ - curValue->value);
    // cout << "v : " << curValue->value << endl;
    // getchar();
  }

private:
  ActionValue *findActionValue(State *state, Action *action) {
    for (auto &av : stateActions[state]) {
      if (*av->action == *action) {
        return av.get();
      }
    }

    return nullptr;
  }

  double maxQ(State *state) {
    double result = 0.0;
    bool haveResult = false;

    for (auto &av : stateActions[state]) {
      if (!haveResult || av->value > result) {
        result = av->value;
        haveResult = true;
      }
    }

    return result;
  }

  uptr<Action> chooseActionForState(State *state) {
    if (Util::RandInterval(0.0, 1.0) >= temperature) {
      return chooseGreedy(state);
    } else {
      auto &sas = stateActions[state];
      return sas[rand() % sas.size()]->action->Clone();
    }
  }

  uptr<Action> chooseGreedy(State *state) {
    double bestValue;
    Action *bestAction = nullptr;

    for (auto &av : stateActions[state]) {
      // if (temperature < 0.001) {
      //   av->action->Output(cout);
      //   cout << "av: " << av->value << endl;
      // }

      if (bestAction == nullptr || av->value > bestValue) {
        bestValue = av->value;
        bestAction = av->action.get();
      }
    }

    assert(bestAction != nullptr);
    return bestAction->Clone();
  }

  void handleNewState(State *state) {
    uptr<State> newState(state->Clone());
    vector<uptr<Action>> actions(newState->AvailableActions());

    stateActions[newState.get()] = vector<uptr<ActionValue>>();
    auto &actionValues(stateActions[newState.get()]);
    for (auto &action : actions) {
      actionValues.push_back(make_unique<ActionValue>(move(action)));
    }

    ownedStates.push_back(move(newState));
  }

  bool haveSeenState(State *state) {
    assert(state != nullptr);
    return stateActions.count(state) > 0;
  }
};

LearningAgent::LearningAgent(double futureDiscount) : impl(new LearningAgentImpl(futureDiscount)) {}

LearningAgent::~LearningAgent() = default;

uptr<Action> LearningAgent::ChooseAction(State *state) { return impl->ChooseAction(state); }

void LearningAgent::ActionOutcome(pair<State *, Action *> performed,
                                  pair<State *, double> outcome) {
  impl->ActionOutcome(performed, outcome);
}

void LearningAgent::SetLearnRate(double learnRate) {
  assert(learnRate >= 0.0 && learnRate <= 1.0);
  impl->learnRate = learnRate;
}

void LearningAgent::SetTemperature(double t) { impl->temperature = t; }
