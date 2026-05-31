#ifndef GAME_H
#define GAME_H

#include "GameEvent.h"
#include "Player.h"
#include "PokemonParty.h"
#include "ds/DungeonGraph.h"
#include "ds/Queue.h"
#include "ds/ScoreTree.h"
#include "ds/Stack.h"
#include <string>

class Game {
private:
    DungeonGraph dungeon;
    Player player;
    PokemonParty pokemonParty; // 포획한 포켓몬 파티 -dc
    Stack<int> moveHistory;
    Queue<GameEvent> eventQueue;
    ScoreTree scoreTree;
    bool running;

    void buildSampleWorld();
    void seedScores();
    void printHelp() const;
    void look() const;
    void move(Direction direction);
    void undoMove();
    void displayMap() const; // Gun Kim 추가: 40x40 맵 출력 -dc
    void takeItem(const std::string& itemName);
    void processOneEvent();
    void showScores() const;
    void showSortedRoomItems() const;
    void processCommand(const std::string& line);

public:
    Game();
    void run();
};

#endif
