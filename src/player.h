#pragma once

#include "entity.h"
#include "enemy.h"

const int PLAYER_MIN_ATTACK = 1;
const int PLAYER_MIN_DEFENSE = 1;

class Enemy;

class Player : public Entity {
public:
    Player();
    ~Player();
    void actionMove(int x, int y);
    void actionAttack(Enemy* e);
    void bonusAttack(int amount);
    void bonusDefense(int amount);
    void bonusHP(int amount);
    void bonusTreasure();

    void init();
    void startTurn();

    bool has_treasure;

    int view_distance;
};
