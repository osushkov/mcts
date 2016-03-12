
#pragma once

#include "Common.hpp"
#include <ostream>

class Action {
public:
  virtual ~Action() {}

  virtual uptr<Action> Clone(void) const = 0;

  virtual bool operator==(const Action &obj) const = 0;

  // Returns a hash of this action.
  virtual size_t HashCode(void) const = 0;

  virtual void Output(std::ostream &out) const = 0; /// TODO: just overload the << operator
};

namespace std {
template <> struct hash<Action *> {
  inline size_t operator()(Action const *action) const { return action->HashCode(); }
};
}
