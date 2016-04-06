
#include "GameAction.hpp"
#include <cassert>
#include <iostream>

struct GameAction::GameActionImpl {
  const unsigned x;

  GameActionImpl(unsigned x) : x(x) {}

  void Apply(GameState &state) const { state.PlaceToken(x); }

  bool operator==(const GameActionImpl &obj) const { return x == obj.x; }

  size_t HashCode(void) const {
    static const size_t MUL_CONST = 378551;

    size_t result = 0;
    result = result * MUL_CONST + x;
    return result;
  }

  void Output(std::ostream &out) const { out << "GA(" << x << ")" << endl; }
};

GameAction::GameAction(unsigned x) : impl(new GameActionImpl(x)) {}

GameAction::~GameAction() = default;

void GameAction::Apply(GameState &state) const { impl->Apply(state); }

uptr<Action> GameAction::Clone(void) const { return make_unique<GameAction>(impl->x); }

bool GameAction::operator==(const Action &obj) const {
  const GameAction *ga = dynamic_cast<const GameAction *>(&obj);
  if (ga == nullptr) {
    return false;
  }

  return *impl == *ga->impl;
}

size_t GameAction::HashCode(void) const { return impl->HashCode(); }

void GameAction::Output(std::ostream &out) const { impl->Output(out); }
