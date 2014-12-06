#pragma once

#include "animation.h"
#include "render_engine.h"
#include "utils.h"
#include "enemy.h"

const int PLAYER_MIN_ATTACK = 1;
const int PLAYER_MIN_DEFENSE = 1;

class Enemy;

class Player {
public:
    Player();
    ~Player();
    void logic();
    void render();
    void setPos(int x, int y);
    void takeDamage(int dmg);
    void actionMove(int x, int y);
    void actionAttack(Enemy* e);

    Point pos;
    bool is_turn;

    int hp;
    int maxhp;
    int attack;
    int defense;
    int potions;

private:
    Animation anim_normal;
    Animation current_anim;
    Image hp_bar;
};
