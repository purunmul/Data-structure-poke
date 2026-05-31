#ifndef DUNGEON_GRAPH_H
#define DUNGEON_GRAPH_H

#include "Direction.h"
#include "Room.h"
#include <string>

// 특정 좌표(Gate)를 통해 다른 방으로 연결되는 정보 -dc (Gun Kim 구현)
struct Coordinate {
    int x, y;
};

struct Gate {
    Coordinate entryPos; // 이 방에서 나가는 좌표
    int targetRoomId;    // 이동할 방 ID
    Coordinate exitPos;  // 이동한 방에서 새로 생길 좌표
};

class DungeonGraph {
private:
    static const int MAX_ROOMS = 10;
    static const int MAX_GATES_PER_ROOM = 5;

    Room* rooms[MAX_ROOMS];
    Gate gates[MAX_ROOMS][MAX_GATES_PER_ROOM];
    int gateCounts[MAX_ROOMS];
    int roomCount;

    bool isValidRoomId(int roomId) const;

    DungeonGraph(const DungeonGraph& other) = delete;
    DungeonGraph& operator=(const DungeonGraph& other) = delete;

public:
    DungeonGraph();
    ~DungeonGraph();

    int addRoom(const std::string& name, const std::string& description,
                int encounterRate = 0, int eventStep = -1);

    // 좌표 기반 게이트 연결 -dc (Gun Kim 구현)
    void connectRoomsByGate(int fromId, int fromX, int fromY,
                            int toId, int toX, int toY);

    // 플레이어가 현재 좌표에서 이동 가능한 게이트가 있는지 확인 -dc (Gun Kim 구현)
    bool checkGate(int currentRoomId, int x, int y,
                   int& outTargetRoomId, int& outTargetX, int& outTargetY) const;

    // 텍스트 커맨드 이동용: 방향으로 게이트 탐색 -dc
    bool findGateByDirection(int roomId, Direction dir,
                             int& outTargetRoomId, int& outTargetX, int& outTargetY) const;

    Room* getRoom(int roomId);
    const Room* getRoom(int roomId) const;
    int size() const;
};

#endif
