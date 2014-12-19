#pragma once

#include "animation.h"
#include "render_engine.h"
#include "sound_engine.h"
#include "utils.h"

class Entity {
public:
    Entity();
    virtual ~Entity();
    void render();
    void setPos(int x, int y);
    void takeDamage(int dmg);
    bool isAnimating();

    virtual void init();
    virtual void startTurn();

    Point pos;
    bool is_turn;
    int view_distance;

    int hp;
    int maxhp;
    int attack;
    int defense;
    int ap;
    int maxap;

    Animation anim_normal;
    Animation anim_hurt;
    Animation anim_die;
    Animation current_anim;

    Image hp_bar;
    bool animating;

    Sound sfx_hurt;
    Sound sfx_die;
    Sound sfx_move;
};
