
#include "GameState.hpp"
#include "GameAction.hpp"
#include <cassert>


struct GameState::GameStateImpl {

  unsigned boardWidth;
  unsigned boardHeight;
  vector<CellState> cells;


  GameStateImpl(unsigned width, unsigned height, const vector<CellState> &cells) :
      boardWidth(width), boardHeight(height), cells(cells) {
    assert(width > 0);
    assert(height > 0);
    assert(cells.size() == width*height);
  }

  unsigned width(void) const {
    return boardWidth;
  }

  unsigned height(void) const {
    return boardHeight;
  }

  bool placeToken(unsigned x, unsigned y) {
    assert(x < boardWidth && y < boardHeight);
    assert(cells[x + y*boardWidth] == CellState::EMPTY);

    cells[x + y*boardWidth] = CellState::MY_TOKEN;
    return true;
  }

  CellState getCell(unsigned x, unsigned y) const {
    assert(x < boardWidth && y < boardHeight);
    return cells[x + y*boardWidth];
  }

  void flipState(void) {
    for (auto& cs : cells) {
      if (cs == CellState::MY_TOKEN) {
        cs = CellState::OPPONENT_TOKEN;
      } else if (cs == CellState::OPPONENT_TOKEN) {
        cs = CellState::MY_TOKEN;
      }
    }
  }

  bool operator== (const GameStateImpl& other) const {
    if (boardWidth != other.boardWidth
        || boardHeight != other.boardHeight
        || cells.size() != other.cells.size()) {
      return false;
    }

    for (unsigned i = 0; i < cells.size(); i++) {
      if (cells[i] != other.cells[i]) {
        return false;
      }
    }

    return true;
  }

  size_t hashCode(void) const {
    size_t result = 0;
    for (auto& c : cells) {
      result *= 3;
      result += (int) c;
    }
    return result;
  }

  void output(std::ostream &out) const {
    for (unsigned y = 0; y < boardHeight; y++) {
      for (unsigned x = 0; x < boardWidth; x++) {
        out << cells[x + y*boardWidth];
      }
      out << endl;
    }
  }

  std::vector<unique_ptr<Action>> availableActions(void) const {
    vector<unique_ptr<Action>> result;

    for (unsigned y = 0; y < boardHeight; y++) {
      for (unsigned x = 0; x < boardWidth; x++) {
        CellState cs = cells[x + y*boardWidth];
        if (cs == CellState::EMPTY) {
          result.push_back(make_unique<GameAction>(x, y));
        }
      }
    }

    return result;
  }

  unique_ptr<GameState> successorState(const Action &action) const {
    const GameAction& ga = dynamic_cast<const GameAction&>(action);

    auto result = unique_ptr<GameState>(new GameState(boardWidth, boardHeight, cells));
    ga.apply(*result);

    return move(result);
  }

};


GameState* GameState::newEmptyGameState(unsigned width, unsigned height) {
  assert(width > 0 && height > 0);

  vector<CellState> cells(width*height, CellState::EMPTY);
  return new GameState(width, height, cells);
}

GameState::GameState(unsigned width, unsigned height, const vector<CellState> &cells) :
    impl(new GameStateImpl(width, height, cells)) {}

GameState::~GameState() = default;

unsigned GameState::width(void) const {
  return impl->width();
}

unsigned GameState::height(void) const {
    return impl->height();
}

bool GameState::placeToken(unsigned x, unsigned y) {
  return impl->placeToken(x, y);
}

CellState GameState::getCell(unsigned x, unsigned y) const{
  return impl->getCell(x, y);
}

void GameState::flipState(void) {
  impl->flipState();
}

unique_ptr<State> GameState::clone(void) const {
  return unique_ptr<State>(new GameState(impl->boardWidth, impl->boardHeight, impl->cells));
}

bool GameState::operator== (const State& obj) const {
  const GameState *gs = dynamic_cast<const GameState*>(&obj);
  if (gs == nullptr) {
    return false;
  }

  return *impl == *gs->impl;
}

size_t GameState::hashCode(void) const {
  return impl->hashCode();
}

void GameState::output(std::ostream &out) const {
  impl->output(out);
}

std::vector<unique_ptr<Action>> GameState::availableActions(void) const {
  return impl->availableActions();
}

unique_ptr<State> GameState::successorState(const Action &action) const {
  return impl->successorState(action);
}
