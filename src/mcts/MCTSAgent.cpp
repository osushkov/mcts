
#include "MCTSAgent.hpp"
#include "MCTS.hpp"

struct MCTSAgent::MCTSAgentImpl {

  uptr<Action> ChooseAction(State *state) {
    MCTS mcts(GameRules(4)); // TODO: game rules should really be injected or a singleton.

    vector<ActionUtility> actions = mcts.ComputeUtilities(state);
    return actions.front().first->Clone();
  }
};

MCTSAgent::MCTSAgent() : impl(new MCTSAgentImpl()) {}

MCTSAgent::~MCTSAgent() = default;

uptr<Action> MCTSAgent::ChooseAction(State *state) { return impl->ChooseAction(state); };
