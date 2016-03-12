
#pragma once

#include "Action.hpp"
#include <ostream>
#include <vector>
#include <memory>
#include <iostream>

using namespace std;


class State {
public:
  virtual ~State() {}

  virtual unique_ptr<State> clone(void) const = 0;

  virtual bool operator== (const State& obj) const = 0;

  // Returns a hash of this state.
  virtual size_t hashCode() const = 0;

  // Print this state to the output stream.
  virtual void output(std::ostream &out) const = 0;

  // Returns a list of all possible actions in this state.
  virtual vector<unique_ptr<Action>> availableActions(void) const = 0;

  // Returns a successor state when the given action is applied in this state.
  // This function can be stochastic and return a different State every time it is called.
  virtual unique_ptr<State> successorState(const Action &action) const = 0;


  struct StatePEquals {
    bool operator()(State* s1, State* s2) const {
      return *s1 == *s2;
    }
  };
};

namespace std {
  template<>
  struct hash<State *> {
    inline size_t operator()(State const *state) const {
      return state->hashCode();
    }
  };
}
