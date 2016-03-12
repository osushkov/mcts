
#pragma once
#include <ostream>

enum class CellState {
  MY_TOKEN = 0,
  OPPONENT_TOKEN = 1,
  EMPTY = 2
};

inline std::ostream& operator<<(std::ostream& stream, const CellState& cs) {
  switch (cs) {
  case CellState::MY_TOKEN:
    stream << "#";
    break;
  case CellState::OPPONENT_TOKEN:
    stream << "0";
    break;
  case CellState::EMPTY:
    stream << ".";
    break;
  }
  return stream;
}
