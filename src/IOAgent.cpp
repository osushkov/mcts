
#include "IOAgent.hpp"
#include "tictactoe/GameAction.hpp"
#include <vector>
#include <iostream>


static bool isActionInSet(Action *action, const vector<uptr<Action>> &available) {
  for (auto& av : available) {
    if (*action == *av.get()) {
      return true;
    }
  }
  return false;
}

uptr<Action> IOAgent::chooseAction(State* state) {
  state->output(cout);
  cout << "your move:" << endl;
  vector<uptr<Action>> available = state->availableActions();

  while (true) {
    unsigned x, y;
    cin >> x;
    cin >> y;
    auto pa = make_unique<GameAction>(x, y);

    if (isActionInSet(pa.get(), available)) {
      pa->output(cout);
      return move(pa);
    } else {
      cout << "invalid move" << endl;
    }
  }

  return nullptr;
}

void IOAgent::actionOutcome(pair<State*, Action*> performed, pair<State*, double> outcome) {}
void IOAgent::setLearnRate(double learnRate) {}
void IOAgent::setPRandom(double pRandom) {}
