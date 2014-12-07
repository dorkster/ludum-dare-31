#pragma once

#include "entity.h"
#include "player.h"

const int ENEMY_COOLDOWN = 15;

const int ENEMY_SLIME = 0;

class Player;

class Enemy : public Entity {
public:
    Enemy(const int type);
    ~Enemy();
    void setPlayer(Player* _player);
    void actionAttack();
    bool isNearPlayer(int range);

    void init();
    void startTurn();

    int action_ticks;

private:
    Player* player;
};
