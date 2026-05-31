#include "ds/DungeonGraph.h"
#include <iostream>

DungeonGraph::DungeonGraph() : roomCount(0) {
    for (int i = 0; i < MAX_ROOMS; ++i) {
        rooms[i] = nullptr;
        gateCounts[i] = 0;
    }
}

DungeonGraph::~DungeonGraph() {
    for (int i = 0; i < roomCount; ++i) {
        delete rooms[i];
    }
}

bool DungeonGraph::isValidRoomId(int roomId) const {
    return roomId >= 0 && roomId < roomCount && rooms[roomId] != nullptr;
}

int DungeonGraph::addRoom(const std::string& name, const std::string& description,
                          int encounterRate, int eventStep) {
    if (roomCount >= MAX_ROOMS) return -1;
    rooms[roomCount] = new Room(roomCount, name, description, encounterRate, eventStep);
    return roomCount++;
}

// 좌표 기반 게이트 등록 -dc (Gun Kim 구현)
void DungeonGraph::connectRoomsByGate(int fromId, int fromX, int fromY,
                                      int toId, int toX, int toY) {
    if (!isValidRoomId(fromId) || !isValidRoomId(toId)) return;
    if (gateCounts[fromId] >= MAX_GATES_PER_ROOM) return;

    int idx = gateCounts[fromId];
    gates[fromId][idx].entryPos    = {fromX, fromY};
    gates[fromId][idx].targetRoomId = toId;
    gates[fromId][idx].exitPos      = {toX, toY};
    gateCounts[fromId]++;
}

// 플레이어가 매 걸음마다 호출하여 "여기가 출구인가?"를 확인하는 함수 -dc (Gun Kim 구현)
bool DungeonGraph::checkGate(int currentRoomId, int x, int y,
                              int& outTargetRoomId, int& outTargetX, int& outTargetY) const {
    if (!isValidRoomId(currentRoomId)) return false;

    for (int i = 0; i < gateCounts[currentRoomId]; ++i) {
        if (gates[currentRoomId][i].entryPos.x == x &&
            gates[currentRoomId][i].entryPos.y == y) {
            outTargetRoomId = gates[currentRoomId][i].targetRoomId;
            outTargetX      = gates[currentRoomId][i].exitPos.x;
            outTargetY      = gates[currentRoomId][i].exitPos.y;
            return true;
        }
    }
    return false;
}

// 텍스트 커맨드 "move 북/남/동/서" 전용: 경계 좌표로 게이트 탐색 -dc
// North: y=40, South: y=0, East: x=40, West: x=0
bool DungeonGraph::findGateByDirection(int roomId, Direction dir,
                                       int& outTargetRoomId, int& outTargetX, int& outTargetY) const {
    if (!isValidRoomId(roomId)) return false;

    for (int i = 0; i < gateCounts[roomId]; ++i) {
        const Gate& g = gates[roomId][i];
        bool match = false;
        switch (dir) {
            case Direction::North: match = (g.entryPos.y == 40); break;
            case Direction::South: match = (g.entryPos.y == 0);  break;
            case Direction::East:  match = (g.entryPos.x == 40); break;
            case Direction::West:  match = (g.entryPos.x == 0);  break;
            default: break;
        }
        if (match) {
            outTargetRoomId = g.targetRoomId;
            outTargetX      = g.exitPos.x;
            outTargetY      = g.exitPos.y;
            return true;
        }
    }
    return false;
}

Room* DungeonGraph::getRoom(int roomId) {
    return isValidRoomId(roomId) ? rooms[roomId] : nullptr;
}

const Room* DungeonGraph::getRoom(int roomId) const {
    return isValidRoomId(roomId) ? rooms[roomId] : nullptr;
}

int DungeonGraph::size() const {
    return roomCount;
}
