
#pragma once

#include "../agent/Agent.hpp"
#include "../common/Common.hpp"

class Trainer {
public:
  Trainer();
  virtual ~Trainer();

  uptr<Agent> TrainAgent(void);

private:
  struct TrainerImpl;
  uptr<TrainerImpl> impl;
};
