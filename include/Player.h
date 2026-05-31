#ifndef PLAYER_H
#define PLAYER_H

#include "Item.h"
#include "ds/Inventory.h"
#include <string>

class Player {
private:
    std::string name;
    int health;
    int score;
    int currentRoomId;
    Inventory inventory;

    // Gun Kim 추가: 40x40 좌표 추적 -dc
    int x, y;

public:
    Player(const std::string& name = "Explorer");

    std::string getName() const;
    int getHealth() const;
    int getScore() const;
    int getCurrentRoomId() const;

    void setCurrentRoomId(int roomId);
    void addScore(int amount);
    void changeHealth(int delta);
    bool isAlive() const;

    Inventory& getInventory();
    const Inventory& getInventory() const;

    // Gun Kim 추가: 위치 관련 -dc
    int getX() const { return x; }
    int getY() const { return y; }
    void setPosition(int newX, int newY);

    void printStatus() const;
};

#endif
