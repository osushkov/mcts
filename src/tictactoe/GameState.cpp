
#include "GameState.hpp"
#include "GameAction.hpp"
#include <cassert>

struct GameState::GameStateImpl {

  unsigned boardWidth;
  unsigned boardHeight;
  vector<CellState> cells;

  GameStateImpl(unsigned width, unsigned height, const vector<CellState> &cells)
      : boardWidth(width), boardHeight(height), cells(cells) {
    assert(width > 0);
    assert(height > 0);
    assert(cells.size() == width * height);
  }

  unsigned Width(void) const { return boardWidth; }

  unsigned Height(void) const { return boardHeight; }

  bool PlaceToken(unsigned x, unsigned y) {
    assert(x < boardWidth && y < boardHeight);
    assert(cells[x + y * boardWidth] == CellState::EMPTY);

    cells[x + y * boardWidth] = CellState::MY_TOKEN;
    return true;
  }

  CellState GetCell(unsigned x, unsigned y) const {
    assert(x < boardWidth && y < boardHeight);
    return cells[x + y * boardWidth];
  }

  void FlipState(void) {
    for (auto &cs : cells) {
      if (cs == CellState::MY_TOKEN) {
        cs = CellState::OPPONENT_TOKEN;
      } else if (cs == CellState::OPPONENT_TOKEN) {
        cs = CellState::MY_TOKEN;
      }
    }
  }

  bool operator==(const GameStateImpl &other) const {
    if (boardWidth != other.boardWidth || boardHeight != other.boardHeight ||
        cells.size() != other.cells.size()) {
      return false;
    }

    for (unsigned i = 0; i < cells.size(); i++) {
      if (cells[i] != other.cells[i]) {
        return false;
      }
    }

    return true;
  }

  size_t HashCode(void) const {
    size_t result = 0;
    for (auto &c : cells) {
      result *= 3;
      result += (int)c;
    }
    return result;
  }

  void Output(std::ostream &out) const {
    for (unsigned y = 0; y < boardHeight; y++) {
      for (unsigned x = 0; x < boardWidth; x++) {
        out << cells[x + y * boardWidth];
      }
      out << endl;
    }
  }

  vector<uptr<Action>> AvailableActions(void) const {
    vector<uptr<Action>> result;

    for (unsigned y = 0; y < boardHeight; y++) {
      for (unsigned x = 0; x < boardWidth; x++) {
        CellState cs = cells[x + y * boardWidth];
        if (cs == CellState::EMPTY) {
          result.push_back(make_unique<GameAction>(x, y));
        }
      }
    }

    return result;
  }

  uptr<GameState> SuccessorState(const Action &action) const {
    const GameAction &ga = dynamic_cast<const GameAction &>(action);

    auto result = make_unique<GameState>(boardWidth, boardHeight, cells);
    ga.Apply(*result);

    return move(result);
  }
};

GameState *GameState::NewEmptyGameState(unsigned width, unsigned height) {
  assert(width > 0 && height > 0);

  vector<CellState> cells(width * height, CellState::EMPTY);
  return new GameState(width, height, cells);
}

GameState::GameState(unsigned width, unsigned height, const vector<CellState> &cells)
    : impl(new GameStateImpl(width, height, cells)) {}

GameState::~GameState() = default;

unsigned GameState::Width(void) const { return impl->Width(); }

unsigned GameState::Height(void) const { return impl->Height(); }

bool GameState::PlaceToken(unsigned x, unsigned y) { return impl->PlaceToken(x, y); }

CellState GameState::GetCell(unsigned x, unsigned y) const { return impl->GetCell(x, y); }

void GameState::FlipState(void) { impl->FlipState(); }

uptr<State> GameState::Clone(void) const {
  return make_unique<GameState>(impl->boardWidth, impl->boardHeight, impl->cells);
}

bool GameState::operator==(const State &obj) const {
  const GameState *gs = dynamic_cast<const GameState *>(&obj);
  if (gs == nullptr) {
    return false;
  }

  return *impl == *gs->impl;
}

size_t GameState::HashCode(void) const { return impl->HashCode(); }

void GameState::Output(std::ostream &out) const { impl->Output(out); }

std::vector<uptr<Action>> GameState::AvailableActions(void) const {
  return impl->AvailableActions();
}

uptr<State> GameState::SuccessorState(const Action &action) const {
  return impl->SuccessorState(action);
}
