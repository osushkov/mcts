
#include "GameAction.hpp"
#include <cassert>
#include <iostream>

struct GameAction::GameActionImpl {
  const unsigned x;
  const unsigned y;

  GameActionImpl(unsigned x, unsigned y) : x(x), y(y) {}

  void Apply(GameState &state) const { state.PlaceToken(x, y); }

  bool operator==(const GameActionImpl &obj) const { return x == obj.x && y == obj.y; }

  size_t HashCode(void) const {
    static const size_t MUL_CONST = 378551;

    size_t result = 0;
    result = result * MUL_CONST + x;
    result = result * MUL_CONST + y;
    return result;
  }

  void Output(std::ostream &out) const { out << "GA(" << x << "," << y << ")" << endl; }
};

GameAction::GameAction(unsigned x, unsigned y) : impl(new GameActionImpl(x, y)) {}

GameAction::~GameAction() = default;

void GameAction::Apply(GameState &state) const { impl->Apply(state); }

uptr<Action> GameAction::Clone(void) const { return make_unique<GameAction>(impl->x, impl->y); }

bool GameAction::operator==(const Action &obj) const {
  const GameAction *ga = dynamic_cast<const GameAction *>(&obj);
  if (ga == nullptr) {
    return false;
  }

  return *impl == *ga->impl;
}

size_t GameAction::HashCode(void) const { return impl->HashCode(); }

void GameAction::Output(std::ostream &out) const { impl->Output(out); }
