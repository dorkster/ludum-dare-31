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
    void render();
    void setPos(int x, int y);
    void startTurn();
    void takeDamage(int dmg);
    void setPlayer(Player* _player);
    void actionAttack();
    bool isNearPlayer(int range);
    bool isAnimating();

    Point pos;
    bool is_turn;
    int action_ticks;

    int hp;
    int maxhp;
    int attack;
    int defense;

private:
    Animation anim_normal;
    Animation anim_hurt;
    Animation anim_die;
    Animation current_anim;
    Image hp_bar;

    Player* player;
    bool animating;
};
