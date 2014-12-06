#pragma once

#include "animation.h"
#include "utils.h"

const int POWERUP_ATTACK = 0;
const int POWERUP_DEFENSE = 1;
const int POWERUP_POTION = 2;

class Powerup {
public:
    Powerup(const int _type);
    ~Powerup();
    void logic();
    void render();
    void setPos(int x, int y);

    Point pos;
    int type;
    int amount;

private:
    Animation anim_normal;
    Animation current_anim;
};
