#pragma once

#include "entity.h"
#include "player.h"
#include "map_engine.h"

const int ENEMY_COOLDOWN = 10;

const int ENEMY_SLIME = 0;

class Player;
class MapEngine;

class Enemy : public Entity {
public:
    Enemy(const int type);
    ~Enemy();
    void setPlayer(Player* _player);
    void logic(MapEngine* map_engine);
    void actionAttack();
    void actionMove(MapEngine* map_engine);
    bool isNearPlayer(int range);

    void init();
    void startTurn();

    int action_ticks;

private:
    Player* player;
};
