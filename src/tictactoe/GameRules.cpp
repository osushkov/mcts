
#include "GameRules.hpp"
#include <cassert>

struct GameRules::GameRulesImpl {
  const unsigned completionRun;

  GameRulesImpl(unsigned completionRun) : completionRun(completionRun) {
    assert(completionRun > 0);
  }

  bool IsTerminalState(const GameState &state) const {
    if (IsWin(state) || IsLoss(state)) {
      return true;
    }

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
    return haveVerticalRun(state, completionRun, CellState::MY_TOKEN) ||
           haveHorizontalRun(state, completionRun, CellState::MY_TOKEN) ||
           haveDiagonalRun(state, completionRun, CellState::MY_TOKEN);
  }

  bool IsLoss(const GameState &state) const {
    return haveVerticalRun(state, completionRun, CellState::OPPONENT_TOKEN) ||
           haveHorizontalRun(state, completionRun, CellState::OPPONENT_TOKEN) ||
           haveDiagonalRun(state, completionRun, CellState::OPPONENT_TOKEN);
  }

private:
  bool haveVerticalRun(const GameState &state, unsigned length, CellState targetToken) const {
    for (unsigned y = 0; y < state.Height() - length + 1; y++) {
      for (unsigned x = 0; x < state.Width(); x++) {

        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.GetCell(x, y + i) != targetToken) {
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

  bool haveHorizontalRun(const GameState &state, unsigned length, CellState targetToken) const {
    for (unsigned y = 0; y < state.Height(); y++) {
      for (unsigned x = 0; x < state.Width() - length + 1; x++) {

        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.GetCell(x + i, y) != targetToken) {
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

  bool haveDiagonalRun(const GameState &state, unsigned length, CellState targetToken) const {
    for (unsigned y = 0; y < state.Height() - length + 1; y++) {
      for (unsigned x = 0; x < state.Width() - length + 1; x++) {
        bool found = true;
        for (unsigned i = 0; i < length; i++) {
          if (state.GetCell(x + i, y + i) != targetToken) {
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
          if (state.GetCell(x - i, y + i) != targetToken) {
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

static uptr<GameRules> gameRulesInstance;

GameRules* GameRules::instance(void) {
  if (gameRulesInstance == nullptr) {
    gameRulesInstance = uptr<GameRules>(new GameRules(4));
  }

  return gameRulesInstance.get();
}

GameRules::GameRules(unsigned completionRun) : impl(new GameRulesImpl(completionRun)) {}

GameRules::~GameRules() = default;

uptr<State> GameRules::InitialState(void) const {
  return uptr<State>(GameState::NewEmptyGameState(4, 4));
}

bool GameRules::IsTerminalState(const State &state) const {
  auto gs = static_cast<const GameState *>(&state);
  return impl->IsTerminalState(*gs);
}

bool GameRules::IsWin(const State &state) const {
  auto gs = static_cast<const GameState *>(&state);
  return impl->IsWin(*gs);
}

bool GameRules::IsLoss(const State &state) const {
  auto gs = static_cast<const GameState *>(&state);
  return impl->IsLoss(*gs);
}
