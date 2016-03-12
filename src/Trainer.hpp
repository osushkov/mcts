
#pragma once

#include "Agent.hpp"
#include <memory>

using namespace std;


class Trainer {
public:

  Trainer();
  virtual ~Trainer();

  unique_ptr<Agent> trainAgent(void);

private:
  struct TrainerImpl;
  unique_ptr<TrainerImpl> impl;

};
