
#pragma once

#include "../common/State.hpp"
#include "CellState.hpp"
#include <memory>
#include <vector>

using namespace std;


class GameState : public State {
public:

  // Create a game state with an empty board.
  static GameState* newEmptyGameState(unsigned width, unsigned height);

  GameState(unsigned width, unsigned height, const vector<CellState> &cells);
  virtual ~GameState();

  unsigned width(void) const;
  unsigned height(void) const;

  bool placeToken(unsigned x, unsigned y);
  CellState getCell(unsigned x, unsigned y) const;
  void flipState(void);

  unique_ptr<State> clone(void) const override;
  bool operator== (const State& obj) const override;
  size_t hashCode() const override;
  void output(std::ostream &out) const override;

  vector<unique_ptr<Action>> availableActions(void) const override;
  unique_ptr<State> successorState(const Action &action) const override;

private:
  class GameStateImpl;
  unique_ptr<GameStateImpl> impl;
};

// namespace std {
// template<>
// struct hash<GameState*> {
//   size_t operator()(GameState const *gs) const {
//     return gs->hashCode();
//   }
// };
// }
