
#include "GameRules.hpp"
#include <cassert>


class GameRules::GameRulesImpl {
  const unsigned completionRun;

public:
  GameRulesImpl(unsigned completionRun) :
      completionRun(completionRun) {
    assert(completionRun > 0);
  }

  bool isTerminalState(const GameState &state) const {
    for (unsigned y = 0; y < state.height(); y++) {
      for (unsigned x = 0; x < state.width(); x++) {
        if (state.getCell(x, y) == CellState::EMPTY) {
          return false;
        }
      }
    }

    return true;
  }

  bool isWin(const GameState &state) const {
    return haveVerticalRun(state, completionRun) ||
        haveHorizontalRun(state, completionRun) ||
        haveDiagonalRun(state, completionRun);
  }

private:

  bool haveVerticalRun(const GameState &state, unsigned length) const {
    for (unsigned y = 0; y < state.height() - length + 1; y++) {
      for (unsigned x = 0; x < state.width(); x++) {

        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.getCell(x, y+i) != CellState::MY_TOKEN) {
            found = false;
            break;
          }
        }

        if (found) {
          return true;
        }
      }
    }

    return false;
  }

  bool haveHorizontalRun(const GameState &state, unsigned length) const {
    for (unsigned y = 0; y < state.height(); y++) {
      for (unsigned x = 0; x < state.width() - length + 1; x++) {

        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.getCell(x+i, y) != CellState::MY_TOKEN) {
            found = false;
            break;
          }
        }

        if (found) {
          return true;
        }
      }
    }

    return false;
  }

  bool haveDiagonalRun(const GameState &state, unsigned length) const {
    for (unsigned y = 0; y < state.height() - length + 1; y++) {
      for (unsigned x = 0; x < state.width() - length + 1; x++) {
        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.getCell(x+i, y+i) != CellState::MY_TOKEN) {
            found = false;
            break;
          }
        }

        if (found) {
          return true;
        }
      }
    }

    for (unsigned y = 0; y < state.height() - length + 1; y++) {
      for (unsigned x = length - 1; x < state.width(); x++) {
        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.getCell(x-i, y+i) != CellState::MY_TOKEN) {
            found = false;
            break;
          }
        }

        if (found) {
          return true;
        }
      }
    }

    return false;
  }

};


GameRules::GameRules(unsigned completionRun) :
    impl(new GameRulesImpl(completionRun)) {}

GameRules::~GameRules() = default;

bool GameRules::isTerminalState(const State &state) const {
  auto gs = static_cast<const GameState*>(&state);
  return impl->isTerminalState(*gs);
}

bool GameRules::isWin(const State &state) const {
  auto gs = static_cast<const GameState*>(&state);
  return impl->isWin(*gs);
}
