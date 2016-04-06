
#pragma once

#include "../common/Common.hpp"
#include "../common/State.hpp"
#include "CellState.hpp"
#include <vector>

class GameState : public State {
public:
  // Create a game state with an empty board.
  // TODO: this should return a uptr!
  static GameState *NewEmptyGameState(unsigned width, unsigned height);

  GameState(unsigned width, unsigned height, const vector<CellState> &cells);
  virtual ~GameState();

  unsigned Width(void) const;
  unsigned Height(void) const;

  bool PlaceToken(unsigned x);
  CellState GetCell(unsigned x, unsigned y) const;

  // Whenever we make a move and want another agent to make a move, then we should "flip" the
  // board such that what are currently "our" tokens become "oppponent" tokens, and vice versa.
  void FlipState(void);

  uptr<State> Clone(void) const override;
  bool operator==(const State &obj) const override;
  size_t HashCode() const override;
  void Output(std::ostream &out) const override;

  vector<uptr<Action>> AvailableActions(void) const override;
  uptr<State> SuccessorState(const Action &action) const override;

private:
  class GameStateImpl;
  uptr<GameStateImpl> impl;
};
