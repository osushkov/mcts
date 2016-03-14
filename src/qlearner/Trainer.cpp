
#include "Trainer.hpp"
#include "../agent/RandomAgent.hpp"
#include "../common/Common.hpp"
#include "../tictactoe/GameRules.hpp"
#include "../tictactoe/GameState.hpp"
#include "../util/Util.hpp"
#include "LearningAgent.hpp"
#include <cassert>
#include <cmath>

struct Trainer::TrainerImpl {

  const unsigned numRounds = 100000;

  const double startLearnRate = 0.5;
  const double endLearnRate = 0.001;

  const double startRandomness = 0.3;

  const double winReward = 100.0;
  const double lossReward = -100.0;
  const double drawReward = 0.0;

  const GameRules rules;

  TrainerImpl() : rules(4) {}

  uptr<Agent> TrainAgent(void) {
    auto trainedAgent = make_unique<LearningAgent>(0.9);
    auto opponent = make_unique<LearningAgent>(0.9);

    for (unsigned round = 0; round < numRounds; round++) {
      double roundFrac = (double)round / (double)numRounds;
      double learnRate = startLearnRate + roundFrac * (endLearnRate - startLearnRate);

      trainedAgent->SetLearnRate(learnRate);
      trainedAgent->SetPRandom(startRandomness * (1.0 - roundFrac));

      opponent->SetLearnRate(learnRate);
      opponent->SetPRandom(startRandomness * (1.0 - roundFrac));

      trainingRound(trainedAgent.get(), opponent.get());
    }

    trainedAgent->SetPRandom(0.0);
    return move(trainedAgent);
  }

private:
  void trainingRound(LearningAgent *agent0, LearningAgent *agent1) {
    vector<LearningAgent *> players{agent0, agent1};
    unsigned curIndex = rand() % players.size();

    // TODO: take in a factory that creates a new empty state.
    uptr<State> gameState(GameState::NewEmptyGameState(4, 4));
    pair<uptr<State>, uptr<Action>> prevPerformed(nullptr, nullptr);

    unsigned turns = 0;
    while (true) {
      LearningAgent *curPlayer = players[curIndex];
      LearningAgent *otherPlayer = players[(curIndex + 1) % players.size()];

      uptr<Action> action = curPlayer->ChooseAction(gameState.get());
      uptr<State> actionApplied = gameState->SuccessorState(*action);

      uptr<State> successor = actionApplied->Clone();
      static_cast<GameState *>(successor.get())->FlipState(); // TODO: this is a bit hacky.

      bool isWin = rules.IsWin(*actionApplied);
      bool isFinished = rules.IsTerminalState(*actionApplied);

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
        rewardAgent(curPlayer, gameState.get(), action.get(), actionApplied.get(), myReward);

        break;
      }

      prevPerformed.first = move(gameState);
      prevPerformed.second = move(action);

      gameState = move(successor);

      curIndex = (curIndex + 1) % players.size();
      turns++;
    }
  }

  void rewardAgent(LearningAgent *agent, State *state, Action *action, State *outcome,
                   double reward) {
    agent->ActionOutcome(pair<State *, Action *>(state, action),
                         pair<State *, double>(outcome, reward));
  }
};

Trainer::Trainer() : impl(new TrainerImpl()) {}

Trainer::~Trainer() = default;

uptr<Agent> Trainer::TrainAgent(void) { return impl->TrainAgent(); }
