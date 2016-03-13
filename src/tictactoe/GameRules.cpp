
#include "GameRules.hpp"
#include <cassert>

struct GameRules::GameRulesImpl {
  const unsigned completionRun;

  GameRulesImpl(unsigned completionRun) : completionRun(completionRun) {
    assert(completionRun > 0);
  }

  bool IsTerminalState(const GameState &state) const {
    for (unsigned y = 0; y < state.Height(); y++) {
      for (unsigned x = 0; x < state.Width(); x++) {
        if (state.GetCell(x, y) == CellState::EMPTY) {
          return false;
        }
      }
    }

    return true;
  }

  bool IsWin(const GameState &state) const {
    return haveVerticalRun(state, completionRun) || haveHorizontalRun(state, completionRun) ||
           haveDiagonalRun(state, completionRun);
  }

private:
  bool haveVerticalRun(const GameState &state, unsigned length) const {
    for (unsigned y = 0; y < state.Height() - length + 1; y++) {
      for (unsigned x = 0; x < state.Width(); x++) {

        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.GetCell(x, y + i) != CellState::MY_TOKEN) {
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
    for (unsigned y = 0; y < state.Height(); y++) {
      for (unsigned x = 0; x < state.Width() - length + 1; x++) {

        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.GetCell(x + i, y) != CellState::MY_TOKEN) {
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
    for (unsigned y = 0; y < state.Height() - length + 1; y++) {
      for (unsigned x = 0; x < state.Width() - length + 1; x++) {
        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.GetCell(x + i, y + i) != CellState::MY_TOKEN) {
            found = false;
            break;
          }
        }

        if (found) {
          return true;
        }
      }
    }

    for (unsigned y = 0; y < state.Height() - length + 1; y++) {
      for (unsigned x = length - 1; x < state.Width(); x++) {
        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.GetCell(x - i, y + i) != CellState::MY_TOKEN) {
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

GameRules::GameRules(unsigned completionRun) : impl(new GameRulesImpl(completionRun)) {}

GameRules::GameRules(const GameRules &other) : impl(new GameRulesImpl(other.impl->completionRun)) {}

GameRules::~GameRules() = default;

bool GameRules::IsTerminalState(const State &state) const {
  auto gs = static_cast<const GameState *>(&state);
  return impl->IsTerminalState(*gs);
}

bool GameRules::IsWin(const State &state) const {
  auto gs = static_cast<const GameState *>(&state);
  return impl->IsWin(*gs);
}
