
#include "Trainer.hpp"
#include "common/Common.hpp"
#include "RandomAgent.hpp"
#include "LearningAgent.hpp"
#include "util/Util.hpp"
#include "tictactoe/GameState.hpp"
#include "tictactoe/GameRules.hpp"
#include <cassert>
#include <cmath>


struct Trainer::TrainerImpl {

  const unsigned numRounds = 10000;

  const double startLearnRate = 0.5;
  const double endLearnRate = 0.001;

  const double startRandomness = 0.3;

  const double winReward = 100.0;
  const double lossReward = -100.0;
  const double drawReward = 0.0;

  GameRules rules;


  TrainerImpl() : rules(3) {}

  unique_ptr<Agent> trainAgent(void) {
    auto trainedAgent = make_unique<LearningAgent>(0.9);
    auto opponent = make_unique<LearningAgent>(0.9);

    for (unsigned round = 0; round < numRounds; round++) {
      double roundFrac = (double) round / (double) numRounds;
      double learnRate = startLearnRate + roundFrac * (endLearnRate - startLearnRate);

      trainedAgent->setLearnRate(learnRate);
      trainedAgent->setPRandom(startRandomness * (1.0 - roundFrac));

      opponent->setLearnRate(learnRate);
      opponent->setPRandom(startRandomness * (1.0 - roundFrac));

      trainingRound(trainedAgent.get(), opponent.get());
    }

    return move(trainedAgent);
  }

private:

  void trainingRound(Agent* agent0, Agent* agent1) {
    vector<Agent*> players{agent0, agent1};
    unsigned curIndex = rand()%players.size();

    // TODO: take in a factory that creates a new empty state.
    uptr<State> gameState(GameState::newEmptyGameState(3, 3));
    pair<uptr<State>, uptr<Action>> prevPerformed(nullptr, nullptr);

    unsigned turns = 0;
    while (true) {
      Agent *curPlayer = players[curIndex];
      Agent *otherPlayer = players[(curIndex + 1) % players.size()];

      uptr<Action> action = curPlayer->chooseAction(gameState.get());
      uptr<State> actionApplied = gameState->successorState(*action);

      uptr<State> successor = actionApplied->clone();
      static_cast<GameState*>(successor.get())->flipState(); // TODO: this is a bit hacky.

      bool isWin = rules.isWin(*actionApplied);
      bool isFinished = rules.isTerminalState(*actionApplied);

      if (prevPerformed.first.get() != nullptr) {
        assert(prevPerformed.second != nullptr);

        double opponentReward = 1.0;
        if (isWin) {
          opponentReward = lossReward;
        } else if (isFinished) {
          opponentReward = drawReward;
        }

        rewardAgent(otherPlayer, prevPerformed.first.get(), prevPerformed.second.get(),
            successor.get(), opponentReward);
      }

      if (isWin || isFinished) {
        double myReward = isWin ? winReward : drawReward;
        rewardAgent(curPlayer, gameState.get(), action.get(),
            actionApplied.get(), myReward);

        break;
      }

      prevPerformed.first = move(gameState);
      prevPerformed.second = move(action);

      gameState = move(successor);

      curIndex = (curIndex + 1) % players.size();
      turns++;
    }
  }

  void rewardAgent(Agent *agent, State *state, Action *action, State *outcome, double reward) {
    agent->actionOutcome(pair<State*, Action*>(state, action) ,
                         pair<State*, double>(outcome, reward));
  }



};


Trainer::Trainer() : impl(new TrainerImpl()) {}

Trainer::~Trainer() = default;

unique_ptr<Agent> Trainer::trainAgent(void) {
  return impl->trainAgent();
}
