
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

  const unsigned numEpochs = 5;
  const unsigned numRounds = 100000;

  const double startLearnRate = 0.1;
  const double endLearnRate = 0.01;

  const double startRandomness = 0.2;

  const double winReward = 10.0;
  const double lossReward = -10.0;
  const double drawReward = 0.0;

  TrainerImpl() = default;

  uptr<Agent> TrainAgent(void) {
    auto trainedAgent = make_unique<LearningAgent>(0.9);
    uptr<LearningAgent> opponent = nullptr;

    for (unsigned epoch = 0; epoch < numEpochs; epoch++) {
      cout << "epoch: " << epoch << endl;

      opponent = move(trainedAgent);
      opponent->SetTemperature(0.0);

      trainedAgent = make_unique<LearningAgent>(0.9);

      for (unsigned round = 0; round < numRounds; round++) {
        double roundFrac = (double)round / (double)numRounds;
        double learnRate = startLearnRate + roundFrac * (endLearnRate - startLearnRate);

        trainedAgent->SetLearnRate(learnRate);
        trainedAgent->SetTemperature(startRandomness);// * (1.0 - roundFrac));

        trainingRound(trainedAgent.get(), opponent.get());
      }
    }

    trainedAgent->SetTemperature(0.0);
    return move(trainedAgent);
  }

private:
  void trainingRound(LearningAgent *learner, Agent *opponent) {
    GameRules *rules = GameRules::instance();

    vector<Agent *> players{learner, opponent};
    unsigned curIndex = rand() % players.size();

    uptr<State> gameState = rules->InitialState();
    pair<uptr<State>, uptr<Action>> prevPerformed(nullptr, nullptr);

    unsigned turns = 0;
    while (true) {
      Agent *curPlayer = players[curIndex];
      Agent *otherPlayer = players[(curIndex + 1) % players.size()];

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

        if (otherPlayer == learner) {
          rewardAgent(learner, prevPerformed.first.get(), prevPerformed.second.get(),
                      successor.get(), opponentReward);
        }
      }

      if (isWin || isFinished) {
        double myReward = isWin ? winReward : drawReward;

        if (curPlayer == learner) {
          rewardAgent(learner, gameState.get(), action.get(), actionApplied.get(), myReward);
        }

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
