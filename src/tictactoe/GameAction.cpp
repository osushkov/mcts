
#include "GameAction.hpp"
#include <cassert>
#include <iostream>

using namespace std;


struct GameAction::GameActionImpl {
  const unsigned x;
  const unsigned y;


  GameActionImpl(unsigned x, unsigned y) : x(x), y(y) {}

  void apply(GameState &state) const {
    state.placeToken(x, y);
  }

  bool operator== (const GameActionImpl& obj) const {
    return x == obj.x && y == obj.y;
  }

  size_t hashCode(void) const {
    static const size_t MUL_CONST = 378551;

    size_t result = 0;
    result = result * MUL_CONST + x;
    result = result * MUL_CONST + y;
    return result;
  }

  void output(std::ostream &out) const {
    out << "GA(" << x << "," << y << ")" << endl;
  }

};

GameAction::GameAction(unsigned x, unsigned y) :
    impl(new GameActionImpl(x, y)) {}

GameAction::~GameAction() = default;

void GameAction::apply(GameState &state) const {
  impl->apply(state);
}

uptr<Action> GameAction::clone(void) const {
  return make_unique<GameAction>(impl->x, impl->y);
}

bool GameAction::operator== (const Action& obj) const {
  const GameAction *ga = dynamic_cast<const GameAction*>(&obj);
  if (ga == nullptr) {
    return false;
  }

  return *impl == *ga->impl;
}

size_t GameAction::hashCode(void) const {
  return impl->hashCode();
}

void GameAction::output(std::ostream &out) const {
  impl->output(out);
}
