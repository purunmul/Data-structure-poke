#include "Game.h"
#include "ItemFactory.h"
#include "PokemonFactory.h"
#include "ds/Sorting.h"
#include <iostream>
#include <limits>
#include <sstream>

Game::Game() : dungeon(), player("탐험가"), moveHistory(), eventQueue(), scoreTree(), running(true)
{
    buildSampleWorld(); // -dc
    seedScores();       // -dc
}

// 포켓몬 테마 맵 구성 -dc
void Game::buildSampleWorld()
{
    // 방 생성 -dc
    int palletTown   = dungeon.addRoom("팔레트 타운",   "오박사의 연구소가 있는 조용한 마을이다. 모험의 시작점.");
    int route1       = dungeon.addRoom("1번 도로",      "팔레트 타운과 비리디안 시티를 잇는 풀밭 길이다.");
    int viridianCity = dungeon.addRoom("비리디안 시티", "큰 체육관이 버티고 있는 도시다. 트레이너의 기운이 느껴진다.");
    int safariZone   = dungeon.addRoom("사파리 존",     "희귀한 포켓몬이 사는 특별 보호구역이다.");

    // 방 연결: 팔레트 타운 -[north]-> 1번 도로 -[north]-> 비리디안 시티
    //                                              -[east]-> 사파리 존 -dc
    dungeon.connectRooms(palletTown,   Direction::North, route1,       true);
    dungeon.connectRooms(route1,       Direction::North, viridianCity, true);
    dungeon.connectRooms(route1,       Direction::East,  safariZone,   true);

    // 아이템 배치 -dc
    Room* pallet = dungeon.getRoom(palletTown);
    if (pallet != nullptr)
    {
        pallet->addItem(createItem("몬스터볼"));
        pallet->addItem(createItem("풀회복약"));
    }

    Room* route = dungeon.getRoom(route1);
    if (route != nullptr)
    {
        route->addItem(createItem("갑옷"));
        route->addItem(createItem("풀회복약"));
    }

    Room* safari = dungeon.getRoom(safariZone);
    if (safari != nullptr)
    {
        safari->addItem(createItem("라이플"));
    }

    // 몬스터 배치 -dc
    Room* viridian = dungeon.getRoom(viridianCity);
    if (viridian != nullptr)
    {
        viridian->addMonster(Monster("체육관 트레이너", 30, 8, 20));
    }

    // 시작 이벤트 -dc
    eventQueue.enqueue(GameEvent("오박사: '세상은 포켓몬으로 가득하다, 얘야!'", 0, 0));
    eventQueue.enqueue(GameEvent("라이벌이 나타났다! 긴장감이 흐른다.", 0, -5));
    eventQueue.enqueue(GameEvent("포켓몬의 울음소리가 들렸다. 사기가 올랐다!", 10, 0));

    // 플레이어 시작 위치 -dc
    player.setCurrentRoomId(palletTown);
}

void Game::seedScores()
{
    scoreTree.insert(ScoreRecord("오박사", 95));
    scoreTree.insert(ScoreRecord("지우",   80));
    scoreTree.insert(ScoreRecord("로켓단", 60));
}

void Game::printHelp() const
{
    std::cout << "\n명령어:\n";
    std::cout << "  도움말(help)          명령어 목록 보기\n";
    std::cout << "  보기(look)            현재 방 살펴보기\n";
    std::cout << "  이동(move) <방향>     북/남/동/서로 이동\n";
    std::cout << "  줍기(take) <아이템>   현재 방의 아이템 줍기\n";
    std::cout << "  가방(inventory)       인벤토리 확인\n";
    std::cout << "  되돌리기(undo)        이전 이동 취소\n";
    std::cout << "  이벤트(event)         대기 중인 이벤트 하나 처리\n";
    std::cout << "  점수(scores)          점수 트리 보기\n";
    std::cout << "  정렬(sortitems)       현재 방 아이템을 가치순으로 보기\n";
    std::cout << "  지도(map)             던전 지도 보기\n";
    std::cout << "  상태(status)          플레이어 상태 보기\n";
    std::cout << "  전투(battle) [이름]   포켓몬 전투 시작\n";
    std::cout << "  파티(party)           보유 포켓몬 목록 보기\n";
    std::cout << "  도감(pokedex)         개체값 순 도감 & 동행 포켓몬 선택\n";
    std::cout << "  종료(quit)            게임 종료\n";
}

void Game::look() const
{
    const Room* room = dungeon.getRoom(player.getCurrentRoomId());
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

void Game::takeItem(const std::string& itemName)
{
    if (itemName.empty())
    {
        std::cout << "무엇을 주울까요? 예: 줍기 몬스터볼\n";
        return;
    }

    Room* room = dungeon.getRoom(player.getCurrentRoomId());
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

    printItemSprite(item.getName()); // 아이템 획득 시 스프라이트 출력 -dc
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
    const Room* room = dungeon.getRoom(player.getCurrentRoomId());
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

    Item* copy = new Item[count];
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

void Game::processCommand(const std::string& line)
{
    std::istringstream input(line);
    std::string command;
    input >> command;

    if (command == "help" || command == "도움말")
    {
        printHelp();
    }
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
        std::getline(input >> std::ws, itemName);
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
        // 빈 줄 무시
    }
    else if (command == "battle" || command == "전투")
    {
        std::string pokemonName;
        input >> pokemonName;

        // 보유 포켓몬 버프 전투 전 텍스트 표시 -dc
        pokemonParty.printBuffs();

        // PokemonData 조회 -dc
        const PokemonData* data = pokemonName.empty()
            ? &getDefaultPokemonData()
            : findPokemonData(pokemonName);

        if (data == nullptr)
        {
            std::cout << "'" << pokemonName << "' 포켓몬 데이터를 찾을 수 없습니다.\n";
        }
        else
        {
            PlayerBattle playerBattle;
            EnemyBattle enemy = createEnemyBattle(*data);

            BattleSystem battleSystem;
            battleSystem.startBattle(playerBattle, enemy); // -dc

            // 전투 후 포획 판정: enemy HP > 0 && player 생존 → 포획 성공 -dc
            if (playerBattle.isAlive() && enemy.isAlive())
            {
                if (!pokemonParty.has(enemy.getName()))
                {
                    CaughtPokemon caught;
                    caught.name          = enemy.getName();
                    caught.element       = enemy.getElement();
                    caught.attack        = enemy.getAttack();
                    caught.defense       = enemy.getDefense();
                    caught.speed         = enemy.getSpeed();                  // -dc
                    caught.pokedexNumber = getPokemonNumber(enemy.getName()); // -dc
                    pokemonParty.add(caught);
                    std::cout << enemy.getName() << "이(가) 파티에 추가되었습니다!\n";
                }
                else
                {
                    std::cout << enemy.getName() << "은(는) 이미 파티에 있습니다.\n";
                }
            }
        }

        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    }
    else if (command == "party" || command == "파티") // -dc
    {
        pokemonParty.print();
    }
    else if (command == "pokedex" || command == "도감") // -dc
    {
        if (pokemonParty.isEmpty())
        {
            std::cout << "보유한 포켓몬이 없습니다. 전투에서 포획하세요.\n";
        }
        else
        {
            // 정렬 기준 선택 -dc
            std::cout << "\n정렬 기준:\n";
            std::cout << "  1. 개체값 (공격+방어)\n";
            std::cout << "  2. 공격력\n";
            std::cout << "  3. 방어력\n";
            std::cout << "  4. 포켓몬 번호\n";
            std::cout << "선택 (기본 1): ";

            std::string sortStr;
            std::getline(std::cin, sortStr);
            std::istringstream sortSS(sortStr);
            int sortChoice = 1;
            sortSS >> sortChoice;

            PokedexSortBy sortBy;
            const char* sortLabel;
            switch (sortChoice)
            {
                case 2:  sortBy = PokedexSortBy::Attack;   sortLabel = "공격력 순";      break;
                case 3:  sortBy = PokedexSortBy::Defense;  sortLabel = "방어력 순";      break;
                case 4:  sortBy = PokedexSortBy::Number;   sortLabel = "포켓몬 번호 순"; break;
                default: sortBy = PokedexSortBy::StatTotal; sortLabel = "개체값 순";     break;
            }

            int n = pokemonParty.size();
            CaughtPokemon* arr = new CaughtPokemon[n];
            pokemonParty.getSorted(sortBy, arr); // -dc

            std::cout << "\n[포켓몬 도감] " << sortLabel << "\n";
            std::cout << "----------------------------------------------\n";
            for (int i = 0; i < n; ++i)
            {
                const CaughtPokemon& p = arr[i];
                std::cout << "  " << (i + 1) << ". "
                          << "No." << p.pokedexNumber << " "
                          << p.name
                          << "  개체값:" << (p.attack + p.defense)
                          << "  공격:" << p.attack
                          << "  방어:" << p.defense;
                if (p.name == pokemonParty.getSelected())
                {
                    std::cout << "  ★ 동행 중";
                }
                std::cout << "\n";
            }
            std::cout << "----------------------------------------------\n";
            std::cout << "동행할 포켓몬 번호 입력 (0: 취소): ";

            std::string choiceStr;
            std::getline(std::cin, choiceStr);
            std::istringstream ss(choiceStr);
            int choice = 0;
            ss >> choice;

            if (choice >= 1 && choice <= n)
            {
                pokemonParty.setSelected(arr[choice - 1].name);
                std::cout << arr[choice - 1].name
                          << "을(를) 동행 포켓몬으로 선택했습니다!\n";
            }
            else if (choice != 0)
            {
                std::cout << "올바르지 않은 번호입니다.\n";
            }

            delete[] arr;
        }
    }
    else
    {
        std::cout << "알 수 없는 명령어입니다. '도움말'을 입력해 명령어를 확인하세요.\n";
    }
}

void Game::run()
{
    std::cout << "=== Poké: 던전 탐험 ===\n";
    std::cout << "'도움말'을 입력하면 명령어를 볼 수 있습니다.\n\n";
    look(); // -dc

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

    scoreTree.insert(ScoreRecord(player.getName(), player.getScore())); // -dc
    std::cout << "\n최종 상태:\n";
    player.printStatus();
    std::cout << "게임을 종료합니다.\n";
}
