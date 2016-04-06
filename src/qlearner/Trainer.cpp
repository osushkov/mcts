
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

  const unsigned numRounds = 1000000;

  const double startLearnRate = 0.5;
  const double endLearnRate = 0.001;

  const double startRandomness = 0.3;

  const double winReward = 1.0;
  const double lossReward = -1.0;
  const double drawReward = 0.0;

  TrainerImpl() = default;

  uptr<Agent> TrainAgent(void) {
    vector<uptr<LearningAgent>> agentsPool;
    for (unsigned i = 0; i < 5; i++) {
      agentsPool.emplace_back(new LearningAgent(0.9));
    }

    for (unsigned round = 0; round < numRounds; round++) {
      unsigned a0i = rand() % agentsPool.size();
      unsigned a1i = (a0i + rand() % (agentsPool.size() - 1)) % agentsPool.size();

      LearningAgent *agent0 = agentsPool[a0i].get();
      LearningAgent *agent1 = agentsPool[a1i].get();

      double roundFrac = (double)round / (double)numRounds;
      double learnRate = startLearnRate + roundFrac * (endLearnRate - startLearnRate);

      agent0->SetLearnRate(learnRate);
      agent0->SetTemperature(startRandomness * (1.0 - roundFrac));

      agent1->SetLearnRate(learnRate);
      agent1->SetTemperature(startRandomness * (1.0 - roundFrac));

      trainingRound(agent0, agent1);
    }

    agentsPool.front()->SetTemperature(0.0);
    return move(agentsPool.front());
  }

private:
  void trainingRound(LearningAgent *learner, LearningAgent *opponent) {
    GameRules *rules = GameRules::instance();

    vector<LearningAgent *> players{learner, opponent};
    unsigned curIndex = rand() % players.size();

    uptr<State> gameState = rules->InitialState();
    pair<uptr<State>, uptr<Action>> prevPerformed(nullptr, nullptr);

    unsigned turns = 0;
    while (true) {
      LearningAgent *curPlayer = players[curIndex];
      LearningAgent *otherPlayer = players[(curIndex + 1) % players.size()];

      uptr<Action> action = curPlayer->ChooseAction(gameState.get());
      uptr<State> actionApplied = gameState->SuccessorState(*action);

      uptr<State> successor = actionApplied->Clone();
      static_cast<GameState *>(successor.get())->FlipState(); // TODO: this is a bit hacky.

      bool isWin = rules->IsWin(*actionApplied);
      bool isFinished = rules->IsTerminalState(*actionApplied);

      if (prevPerformed.first.get() != nullptr) {
        assert(prevPerformed.second != nullptr);

        double opponentReward = 0.0;
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
