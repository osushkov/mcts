
#include "IOAgent.hpp"
#include "../tictactoe/GameAction.hpp"
#include <iostream>
#include <vector>

static bool isActionInSet(Action *action, const vector<uptr<Action>> &available) {
  for (auto &av : available) {
    if (*action == *av.get()) {
      return true;
    }
  }
  return false;
}

uptr<Action> IOAgent::ChooseAction(State *state) {
  state->Output(cout);
  cout << "your move:" << endl;
  vector<uptr<Action>> available = state->AvailableActions();

  while (true) {
    unsigned x, y;
    cin >> x;
    cin >> y;
    auto pa = make_unique<GameAction>(x, y);

    if (isActionInSet(pa.get(), available)) {
      pa->Output(cout);
      return move(pa);
    } else {
      cout << "invalid move" << endl;
    }
  }

  return nullptr;
}
