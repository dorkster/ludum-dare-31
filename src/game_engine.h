#pragma once

#include "animation.h"
#include "common.h"
#include "engines.h"
#include "text.h"
#include "utils.h"

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    void logic();
    void render();

    bool done;

private:
    Animation current_animation;
    Animation animation_1;
    Animation animation_2;
    Sound test_sound;
    FPoint pos;
    double angle;
    float speed;
    float turn_speed;
    bool flashing;

    FRect screen_rect;

    Text test_text;
};
