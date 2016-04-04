
#pragma once

#include "../agent/Agent.hpp"
#include "../common/Common.hpp"

// Trains an agent against an MCTS player.
class TrainerMcts {
public:
  TrainerMcts();
  virtual ~TrainerMcts();

  uptr<Agent> TrainAgent(void);

private:
  struct TrainerMctsImpl;
  uptr<TrainerMctsImpl> impl;
};
