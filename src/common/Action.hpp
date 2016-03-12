
#pragma once

#include "Common.hpp"
#include <ostream>


class Action {
public:
  virtual ~Action() {}

  virtual uptr<Action> clone(void) const = 0;

  virtual bool operator== (const Action& obj) const = 0;

  // Returns a hash of this action.
  virtual size_t hashCode(void) const = 0;
  virtual void output(std::ostream &out) const = 0;
};

namespace std {
  template<>
  struct hash<Action *> {
    inline size_t operator()(Action const *action) const {
      return action->hashCode();
    }
  };
}
