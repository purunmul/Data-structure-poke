#ifndef ROOM_H
#define ROOM_H

#include "Item.h"
#include "Monster.h"
#include "ds/DynamicArray.h"
#include <string>

class Room {
private:
    int id;
    std::string name;
    std::string description;
    DynamicArray<Item> items;
    DynamicArray<Monster> monsters;
    bool visited;

    // Gun Kim 추가: 이동 시스템용 데이터 -dc
    int width;
    int height;
    int encounterRate;
    int eventTriggerStep;

public:
    Room();
    Room(int id, const std::string& name, const std::string& description,
         int encounterRate = 0, int eventStep = -1);

    int getId() const;
    std::string getName() const;
    std::string getDescription() const;

    void setVisited(bool value);
    bool hasBeenVisited() const;

    void addItem(const Item& item);
    bool takeItem(const std::string& itemName, Item& output);
    int itemCount() const;
    const Item& getItem(int index) const;

    void addMonster(const Monster& monster);
    int monsterCount() const;
    const Monster& getMonster(int index) const;

    // Gun Kim 추가: Getter -dc
    int getWidth() const { return width; }
    int getHeight() const { return height; }
    int getEncounterRate() const { return encounterRate; }
    int getEventTriggerStep() const { return eventTriggerStep; }

    void printDescription() const;
};

#endif
