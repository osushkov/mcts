
#include "agent/Agent.hpp"
#include "agent/IOAgent.hpp"
#include "common/Common.hpp"
#include "mcts/MCTSAgent.hpp"
#include "qlearner/Trainer.hpp"
#include "tictactoe/GameAction.hpp"
#include "tictactoe/GameRules.hpp"
#include "tictactoe/GameState.hpp"

#include <cstdlib>
#include <ctime>
#include <iostream>
#include <random>
#include <unordered_map>
#include <vector>
#include <vector>

void playGameAgainst(Agent *agent0, Agent *agent1) {
  GameRules rules(4);

  vector<Agent *> players{agent1, agent0};
  unsigned curIndex = 0; // rand()%players.size();

  uptr<State> gameState(GameState::NewEmptyGameState(4, 4));

  while (true) {
    Agent *curPlayer = players[curIndex];

    uptr<Action> action = curPlayer->ChooseAction(gameState.get());
    uptr<State> successor = gameState->SuccessorState(*action);

    bool isWin = rules.IsWin(*successor);
    bool isFinished = rules.IsTerminalState(*successor);

    if (isWin || isFinished) {
      cout << "end of game!" << endl;
      if (isWin) {
        cout << "player" << curIndex << " is the winner" << endl;
      } else {
        cout << "its a draw" << endl;
      }
      successor->Output(cout);

      break;
    }

    gameState = successor->Clone();
    static_cast<GameState *>(gameState.get())->FlipState(); // TODO: this is a bit hacky.
    curIndex = (curIndex + 1) % players.size();
  }
}

int main() {
  srand(time(NULL));

  cout << "starting" << endl;

  // Play againt an MCTSAgent from the command line.
  // MCTSAgent mctsAgent;
  // IOAgent ioAgent;
  // playGameAgainst(&mctsAgent, &ioAgent);

  // MCTS Agent vs Q-learning agent.
  Trainer trainer;
  uptr<Agent> trainedAgent(trainer.TrainAgent());
  MCTSAgent mctsAgent;
  playGameAgainst(&mctsAgent, trainedAgent.get());

  cout << "finished" << endl;

  return 0;
}
