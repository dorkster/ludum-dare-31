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
    void init();
    void render();
    void setPos(int x, int y);
    void takeDamage(int dmg);
    void actionMove(int x, int y);
    void actionAttack(Enemy* e);
    void bonusAttack(int amount);
    void bonusDefense(int amount);
    void bonusHP(int amount);
    void bonusTreasure();
    bool isAnimating();

    Point pos;
    bool is_turn;

    int hp;
    int maxhp;
    int attack;
    int defense;
    bool has_treasure;

private:
    Animation anim_normal;
    Animation anim_hurt;
    Animation anim_die;
    Animation current_anim;
    Image hp_bar;
    bool animating;
};
