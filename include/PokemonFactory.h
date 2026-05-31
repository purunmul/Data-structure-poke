#ifndef POKEMON_FACTORY_H
#define POKEMON_FACTORY_H

#include "BattleSystem.h"
#include <string>

struct PokemonData {
    const char* name;
    int maxHp;
    int attack;
    int defense;
    int speed;
    ElementType element;
    const char* sprite;
};

const PokemonData* findPokemonData(const std::string& name);
const PokemonData& getDefaultPokemonData();
EnemyBattle createEnemyBattle(const PokemonData& data);

#endif
