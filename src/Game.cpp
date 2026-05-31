#include "Game.h"
// #include "ds/Sorting.h"
#include "BattleSystem.h"
#include <iostream>
#include <limits>
#include <sstream>

Game::Game() : dungeon(), player("탐험가"), moveHistory(), eventQueue(), scoreTree(), running(true)
{
    // 전투 테스트 모드: 던전 구성과 점수 초기화는 잠시 꺼둔다.
    // buildSampleWorld();
    // seedScores();
}

#if 0
void Game::buildSampleWorld()
{
    int entrance = dungeon.addRoom("입구 홀", "푸른 횃불이 차갑게 빛나는 석조 홀이다.");
    int library = dungeon.addRoom("도서관", "먼지 쌓인 책들이 벽을 뒤덮고 있다. 사다리는 어두운 발코니로 이어진다.");
    int armory = dungeon.addRoom("무기고", "부서진 방패와 오래된 창들이 바닥에 흩어져 있다.");
    int vault = dungeon.addRoom("수정 금고", "공중에 떠 있는 수정에서 빛이 흘러나오는 조용한 방이다.");

    Room *entranceRoom = dungeon.getRoom(entrance);
    Room *libraryRoom = dungeon.getRoom(library);
    Room *armoryRoom = dungeon.getRoom(armory);
    Room *vaultRoom = dungeon.getRoom(vault);

    if (entranceRoom != nullptr)
    {
        entranceRoom->addItem(Item("열쇠", "작은 쇠 열쇠.", 10));
    }
    if (libraryRoom != nullptr)
    {
        libraryRoom->addItem(Item("두루마리", "잊힌 길에 대한 단서가 적힌 두루마리.", 25));
    }
    if (armoryRoom != nullptr)
    {
        armoryRoom->addItem(Item("창", "녹슨 창. 아직은 충분히 날카롭다.", 20));
        armoryRoom->addMonster(Monster("훈련용 더미", 10, 0, 5));
    }
    if (vaultRoom != nullptr)
    {
        vaultRoom->addItem(Item("수정", "높은 점수를 받을 수 있는 빛나는 수정.", 50));
    }

    dungeon.connectRooms(entrance, Direction::East, library, true);
    dungeon.connectRooms(entrance, Direction::South, armory, true);
    dungeon.connectRooms(library, Direction::South, vault, true);
    dungeon.connectRooms(armory, Direction::East, vault, true);

    eventQueue.enqueue(GameEvent("차가운 바람이 던전을 스쳐 지나간다.", 0, 0));
    eventQueue.enqueue(GameEvent("오래된 표식을 발견해 통찰을 얻었다.", 5, 0));
    eventQueue.enqueue(GameEvent("헐거운 돌이 근처에 떨어졌다.", 0, -5));
}

void Game::seedScores()
{
    scoreTree.insert(ScoreRecord("Ada", 80));
    scoreTree.insert(ScoreRecord("Grace", 95));
    scoreTree.insert(ScoreRecord("Linus", 60));
}
#endif

void Game::printHelp() const
{
    std::cout << "\n명령어:\n";
    std::cout << "  도움말(help)      명령어 목록 보기\n";
    // std::cout << "  보기(look)        현재 방 살펴보기\n";
    // std::cout << "  이동(move) <방향> 북/남/동/서로 이동\n";
    // std::cout << "  줍기(take) <아이템> 현재 방의 아이템 줍기\n";
    // std::cout << "  가방(inventory)   인벤토리 확인\n";
    // std::cout << "  되돌리기(undo)    이전 이동 취소\n";
    // std::cout << "  이벤트(event)     대기 중인 이벤트 하나 처리\n";
    // std::cout << "  점수(scores)      점수 트리 보기\n";
    // std::cout << "  정렬(sortitems)   현재 방 아이템을 가치순으로 보기\n";
    // std::cout << "  지도(map)         던전 지도 보기\n";
    std::cout << "  상태(status)      플레이어 상태 보기\n";
    std::cout << "  종료(quit)        게임 종료\n";
    std::cout << "  전투(battle) [이름] 포켓몬 전투 시작\n";
}

#if 0
void Game::look() const
{
    const Room *room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr)
    {
        std::cout << "현재 방 정보가 올바르지 않습니다.\n";
        return;
    }

    room->printDescription();
    std::cout << "출구:";
    bool hasExit = false;
    for (int i = 0; i < 4; ++i)
    {
        Direction direction = static_cast<Direction>(i);
        int neighbor = dungeon.getNeighbor(room->getId(), direction);
        if (neighbor != -1)
        {
            std::cout << " " << directionToString(direction);
            hasExit = true;
        }
    }
    if (!hasExit)
    {
        std::cout << " 없음 또는 그래프 미구현";
    }
    std::cout << "\n";
}

void Game::move(Direction direction)
{
    if (direction == Direction::Invalid)
    {
        std::cout << "올바르지 않은 방향입니다. 북, 남, 동, 서 중 하나를 입력하세요.\n";
        return;
    }

    int current = player.getCurrentRoomId();
    int next = dungeon.getNeighbor(current, direction);
    if (next == -1)
    {
        std::cout << "여기서는 " << directionToString(direction) << " 방향으로 이동할 수 없습니다.\n";
        return;
    }

    moveHistory.push(current);
    player.setCurrentRoomId(next);
    player.addScore(1);
    look();
}

void Game::undoMove()
{
    int previousRoom = -1;
    if (!moveHistory.pop(previousRoom))
    {
        std::cout << "되돌릴 이전 이동이 없습니다.\n";
        return;
    }

    player.setCurrentRoomId(previousRoom);
    std::cout << "이전 방으로 되돌아갑니다.\n";
    look();
}

void Game::takeItem(const std::string &itemName)
{
    if (itemName.empty())
    {
        std::cout << "무엇을 주울까요? 예: 줍기 열쇠\n";
        return;
    }

    Room *room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr)
    {
        std::cout << "현재 방 정보가 올바르지 않습니다.\n";
        return;
    }

    Item item;
    if (!room->takeItem(itemName, item))
    {
        std::cout << "여기에는 '" << itemName << "' 아이템이 없습니다.\n";
        return;
    }

    player.getInventory().addItem(item);
    player.addScore(item.getValue());
    std::cout << "획득: " << item.getName() << "\n";
}

void Game::processOneEvent()
{
    GameEvent event;
    if (!eventQueue.dequeue(event))
    {
        std::cout << "대기 중인 이벤트가 없습니다.\n";
        return;
    }

    std::cout << "이벤트: " << event.description << "\n";
    player.addScore(event.scoreDelta);
    player.changeHealth(event.healthDelta);
    player.printStatus();
}

void Game::showScores() const
{
    std::cout << "점수 기록:\n";
    scoreTree.printDescending();
}

void Game::showSortedRoomItems() const
{
    const Room *room = dungeon.getRoom(player.getCurrentRoomId());
    if (room == nullptr)
    {
        std::cout << "현재 방 정보가 올바르지 않습니다.\n";
        return;
    }

    int count = room->itemCount();
    if (count == 0)
    {
        std::cout << "이 방에는 정렬할 아이템이 없습니다.\n";
        return;
    }

    Item *copy = new Item[count];
    for (int i = 0; i < count; ++i)
    {
        copy[i] = room->getItem(i);
    }

    sortItemsByValueDescending(copy, count);

    std::cout << "가치가 높은 순서의 아이템:\n";
    for (int i = 0; i < count; ++i)
    {
        std::cout << "  - ";
        copy[i].print();
    }

    delete[] copy;
}
#endif

void Game::processCommand(const std::string &line)
{
    std::istringstream input(line);
    std::string command;
    input >> command;

    if (command == "help" || command == "도움말")
    {
        printHelp();
    }
#if 0
    else if (command == "look" || command == "보기")
    {
        look();
    }
    else if (command == "move" || command == "이동")
    {
        std::string directionText;
        input >> directionText;
        move(parseDirection(directionText));
    }
    else if (command == "take" || command == "줍기")
    {
        std::string itemName;
        input >> itemName;
        takeItem(itemName);
    }
    else if (command == "inventory" || command == "가방")
    {
        player.getInventory().print();
    }
    else if (command == "undo" || command == "되돌리기")
    {
        undoMove();
    }
    else if (command == "event" || command == "이벤트")
    {
        processOneEvent();
    }
    else if (command == "scores" || command == "점수")
    {
        showScores();
    }
    else if (command == "sortitems" || command == "정렬")
    {
        showSortedRoomItems();
    }
    else if (command == "map" || command == "지도")
    {
        dungeon.printMap();
    }
#endif
    else if (command == "status" || command == "상태")
    {
        player.printStatus();
    }
    else if (command == "quit" || command == "종료")
    {
        running = false;
    }
    else if (command.empty())
    {
        // Ignore blank lines.
    }
    else if (command == "battle" || command == "전투")
    {
        std::string pokemonName;
        input >> pokemonName;

        BattleSystem battleSystem;
        if (pokemonName.empty())
        {
            battleSystem.startTestBattle();
        }
        else
        {
            battleSystem.startBattleByPokemonName(pokemonName);
        }
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else
    {
        std::cout << "알 수 없는 명령어입니다. '도움말'을 입력해 명령어를 확인하세요.\n";
    }
}

void Game::run()
{
    std::cout << "전투 테스트 모드\n";
    std::cout << "'도움말'을 입력하면 명령어를 볼 수 있습니다.\n";
    // look();

    while (running && player.isAlive())
    {
        std::cout << "\n> ";
        std::string line;
        if (!std::getline(std::cin, line))
        {
            break;
        }
        processCommand(line);
    }

    // scoreTree.insert(ScoreRecord(player.getName(), player.getScore()));
    std::cout << "\n최종 상태:\n";
    player.printStatus();
    std::cout << "게임을 종료합니다.\n";
}
