
#include "MCTS.hpp"
#include "Node.hpp"

static const double P_RANDOM = 0.2;
static const unsigned MC_ITER = 1000;

struct MCTS::MCTSImpl {
  GameRules rules;

  MCTSImpl(const GameRules &rules) : rules(rules) {}

  vector<ActionUtility> ComputeUtilities(State *startState) {
    auto root = make_unique<Node>(startState->Clone(), 0);

    for (unsigned i = 0; i < MC_ITER; i++) {
      mcIteration(root.get());
    }

    vector<ActionUtility> result = root->GetActionUtilities();

    return result;
  }

  // single iteration of monte-carlo tree search.
  void mcIteration(Node *root) {
    vector<Node *> pathFromRoot;

    Node *cur = root;
    while (!cur->IsLeaf()) {
      pathFromRoot.push_back(cur);
      cur = cur->Select(P_RANDOM);
    }
    pathFromRoot.push_back(cur);

    Node *playoutNode = cur->Expand();
    if (playoutNode == nullptr) {
      playoutNode = cur;
    } else {
      pathFromRoot.push_back(playoutNode);
    }

    double utility = playout(playoutNode);
    for (int i = pathFromRoot.size() - 1; i >= 0; i--) {
      pathFromRoot[i]->AddUtility(utility);
      utility = -utility;
    }
  }

  double playout(Node *startNode) { return 0.0; }
};

MCTS::MCTS(const GameRules &rules) : impl(new MCTSImpl(rules)) {}

MCTS::~MCTS() = default;

vector<ActionUtility> MCTS::ComputeUtilities(State *curState) {
  return impl->ComputeUtilities(curState);
}
