
#include <iostream>
#include <vector>
#include <unordered_map>
#include <random>
#include <cstdlib>
#include <ctime>
#include <vector>
#include "common/Common.hpp"
#include "tictactoe/GameState.hpp"
#include "tictactoe/GameAction.hpp"
#include "tictactoe/GameRules.hpp"
#include "Agent.hpp"
#include "IOAgent.hpp"
#include "Trainer.hpp"

using namespace std;

void playGameAgainst(Agent *agent) {
  GameRules rules(3);
  auto myPlayer = make_unique<IOAgent>();

  vector<Agent*> players{agent, myPlayer.get()};
  unsigned curIndex = 0; //rand()%players.size();

  uptr<State> gameState(GameState::newEmptyGameState(3, 3));

  while (true) {
    Agent *curPlayer = players[curIndex];

    uptr<Action> action = curPlayer->chooseAction(gameState.get());
    uptr<State> successor = gameState->successorState(*action);

    bool isWin = rules.isWin(*successor);
    bool isFinished = rules.isTerminalState(*successor);

    if (isWin || isFinished) {
      cout << "end of game!" << endl;
      if (isWin) {
        cout << "player" << curIndex << " is the winner" << endl;
      } else {
        cout << "its a draw" << endl;
      }
      successor->output(cout);

      break;
    }

    gameState = successor->clone();
    static_cast<GameState*>(gameState.get())->flipState(); // TODO: this is a bit hacky.
    curIndex = (curIndex + 1) % players.size();
  }
}

int main() {
  // srand(time(NULL));

  cout << "starting" << endl;

  Trainer trainer;
  unique_ptr<Agent> trainedAgent(trainer.trainAgent());

  trainedAgent->setPRandom(0.0);
  playGameAgainst(trainedAgent.get());

  cout << "finished" << endl;

  return 0;
}
