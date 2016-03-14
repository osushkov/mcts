
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

void playGameAgainst(Agent *agent) {
  GameRules rules(3);
  auto myPlayer = make_unique<IOAgent>();

  vector<Agent *> players{agent, myPlayer.get()};
  unsigned curIndex = 0; // rand()%players.size();

  uptr<State> gameState(GameState::NewEmptyGameState(3, 3));

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
  // srand(time(NULL));

  cout << "starting" << endl;

  MCTSAgent mctsAgent;
  playGameAgainst(&mctsAgent);

  // Trainer trainer;
  // uptr<Agent> trainedAgent(trainer.TrainAgent());
  //
  // playGameAgainst(trainedAgent.get());

  cout << "finished" << endl;

  return 0;
}
