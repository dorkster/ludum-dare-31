#pragma once

#include "animation.h"
#include "utils.h"
#include "render_engine.h"
#include "player.h"

const int ENEMY_COOLDOWN = 30;

const int ENEMY_SLIME = 0;

class Player;

class Enemy {
public:
    Enemy(const int type);
    ~Enemy();
    void logic();
    void render();
    void setPos(int x, int y);
    void startTurn();
    void takeDamage(int dmg);
    void setPlayer(Player* _player);
    void actionAttack();

    Point pos;
    bool is_turn;
    int action_ticks;
    bool move_to_player;

    int hp;
    int maxhp;
    int attack;
    int defense;

private:
    Animation anim_normal;
    Animation current_anim;
    Image hp_bar;

    Player* player;
};
