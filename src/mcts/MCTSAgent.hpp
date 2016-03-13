
#pragma once

#include "../agent/Agent.hpp"
#include "../common/Common.hpp"

class MCTSAgent : public Agent {
public:
  MCTSAgent();
  virtual ~MCTSAgent();

  uptr<Action> ChooseAction(State *state) override;

private:
  struct MCTSAgentImpl;
  uptr<MCTSAgentImpl> impl;
};
