#include "Room.h"
#include <iostream>

Room::Room()
    : id(-1), name(""), description(""), visited(false),
      width(40), height(40), encounterRate(0), eventTriggerStep(-1) {}

// Gun Kim 설계 반영: encounterRate, eventStep 파라미터 추가 -dc
Room::Room(int id, const std::string& name, const std::string& description,
           int encounterRate, int eventStep)
    : id(id), name(name), description(description), visited(false),
      width(40), height(40), encounterRate(encounterRate), eventTriggerStep(eventStep) {}

int Room::getId() const {
    return id;
}

std::string Room::getName() const {
    return name;
}

std::string Room::getDescription() const {
    return description;
}

void Room::setVisited(bool value) {
    visited = value;
}

bool Room::hasBeenVisited() const {
    return visited;
}

void Room::addItem(const Item& item) {
    items.pushBack(item);
}

bool Room::takeItem(const std::string& itemName, Item& output) {
    for (int i = 0; i < items.size(); ++i) {
        if (items[i].getName() == itemName) {
            output = items[i];
            items.removeAt(i);
            return true;
        }
    }
    return false;
}

int Room::itemCount() const {
    return items.size();
}

const Item& Room::getItem(int index) const {
    return items[index];
}

void Room::addMonster(const Monster& monster) {
    monsters.pushBack(monster);
}

int Room::monsterCount() const {
    return monsters.size();
}

const Monster& Room::getMonster(int index) const {
    return monsters[index];
}

void Room::printDescription() const {
    std::cout << "\n== " << name << " ==\n";
    std::cout << description << "\n";

    if (items.isEmpty()) {
        std::cout << "아이템: 없음\n";
    } else {
        std::cout << "아이템:\n";
        for (int i = 0; i < items.size(); ++i) {
            std::cout << "  - ";
            items[i].print();
        }
    }

    if (!monsters.isEmpty()) {
        std::cout << "몬스터:\n";
        for (int i = 0; i < monsters.size(); ++i) {
            std::cout << "  - ";
            monsters[i].print();
        }
    }
}
