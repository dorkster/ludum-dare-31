#pragma once

#include "animation.h"
#include "render_engine.h"
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

    int hp;
    int maxhp;
    int attack;
    int defense;

    Animation anim_normal;
    Animation anim_hurt;
    Animation anim_die;
    Animation current_anim;

    Image hp_bar;
    bool animating;
};
