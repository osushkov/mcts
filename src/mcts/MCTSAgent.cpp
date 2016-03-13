
#include "MCTSAgent.hpp"

struct MCTSAgent::MCTSAgentImpl {

  uptr<Action> ChooseAction(State *state) { return nullptr; }
};

MCTSAgent::MCTSAgent() : impl(new MCTSAgentImpl()) {}

MCTSAgent::~MCTSAgent() = default;

uptr<Action> MCTSAgent::ChooseAction(State *state) { return impl->ChooseAction(state); };
