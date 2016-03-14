
#include "Node.hpp"
#include "../tictactoe/GameState.hpp"
#include "../util/Util.hpp"
#include <cassert>

Node::Node(uptr<State> state, unsigned playerIndex)
    : state(move(state)), playerIndex(playerIndex), isLeaf(true), totalTrials(0), sumUtility(0.0) {}

bool Node::IsLeaf(void) const { return isLeaf; }

unsigned Node::PlayerIndex(void) const { return playerIndex; }

State *Node::GetState(void) const { return state.get(); }

vector<pair<Action *, double>> Node::GetActionUtilities(void) const {
  vector<pair<Action *, double>> result;
  for (const auto &edge : children) {
    result.push_back(make_pair(edge.first.get(), edge.second->ExpectedUtility(playerIndex)));
  }
  return result;
}

Node *Node::Expand(void) {
  assert(isLeaf);

  vector<uptr<Action>> available = nonExpandedActions();
  if (available.empty()) { // This can happen at a terminal game state.
    return nullptr;
  }

  if (available.size() == 1) {
    isLeaf = false;
  }

  auto &chosenAction = available[rand() % available.size()];
  uptr<State> childState = state->SuccessorState(*chosenAction);

  // TODO: this is a bit hacky. I should maybe move this into State
  static_cast<GameState *>(childState.get())->FlipState();

  children.emplace_back(move(chosenAction), make_unique<Node>(move(childState), 1 - playerIndex));
  return children.back().second.get();
}

Node *Node::Select(double pRandom) {
  assert(!isLeaf);
  assert(!children.empty());

  bool chooseRandom = Util::RandInterval(0.0, 1.0) < pRandom;
  if (chooseRandom) {
    return children[rand() % children.size()].second.get();
  } else {
    Node *result = nullptr;
    double bestUtility = 0.0;

    for (auto &edge : children) {
      double utility = edge.second->ExpectedUtility(this->playerIndex);
      if (result == nullptr || utility > bestUtility) {
        bestUtility = utility;
        result = edge.second.get();
      }
    }

    assert(result != nullptr);
    return result;
  }
}

void Node::AddUtility(double utility) {
  totalTrials++;
  sumUtility += utility;
}

// This is just a quick and dirty hack, can be more efficient but on small branch factor
// problems doesnt really make a difference.
vector<uptr<Action>> Node::nonExpandedActions(void) {
  vector<uptr<Action>> stateActions = state->AvailableActions();

  vector<uptr<Action>> result;
  for (auto &sa : stateActions) {
    bool shouldAdd = true;
    for (auto &c : children) {
      if (*sa.get() == *c.first.get()) {
        shouldAdd = false;
        break;
      }
    }
    if (shouldAdd) {
      result.push_back(move(sa));
    }
  }
  return result;
}

double Node::ExpectedUtility(unsigned playerIndex) const {
  assert(totalTrials > 0);

  double p = sumUtility / totalTrials;
  if (this->playerIndex == playerIndex) {
    return p;
  } else {
    return -p;
  }
}
