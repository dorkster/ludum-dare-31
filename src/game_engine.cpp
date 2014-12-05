#include "game_engine.h"
#include "engines.h"
#include "utils.h"

GameEngine::GameEngine()
    : done(false)
{
    pos.x = VIEW_W/2;
    pos.y = VIEW_H/2;
    angle = 0;
    speed = 1.5;
    turn_speed = 5;
    flashing = false;

    animation_1.load("data/tank.png", 0, 1, 100, 0, 32, 32, 16, 16);
    animation_2.load("data/tank.png", 1, 2, 100, 4, 32, 32, 16, 16);

    current_animation = animation_1;
    current_animation.setPos(pos.x, pos.y);

    test_sound.load(sound_engine, "data/bleep.wav");

    screen_rect.x = screen_rect.y = 0;
    screen_rect.w = VIEW_W;
    screen_rect.h = VIEW_H;

    test_text.setText("Use the arrow keys to move around");
    test_text.setPos(0,0);
}

GameEngine::~GameEngine() {
}

void GameEngine::logic() {
    // exit game
    if (input_engine->pressing[CANCEL])
        done = true;

    if (input_engine->pressing[ACCEPT] && !input_engine->lock[ACCEPT] && !flashing) {
        input_engine->lock[ACCEPT] = true;
        current_animation = animation_2;
        flashing = true;
        test_sound.play();
    }
    else if (flashing && !input_engine->lock[ACCEPT]) {
        current_animation = animation_1;
        flashing = false;
    }
    else if (flashing && current_animation.isFinished()) {
        current_animation=animation_1;
        flashing = false;
    }

    // turn tank
    if (input_engine->pressing[LEFT])
        angle = rotate(angle, -turn_speed);
    if (input_engine->pressing[RIGHT])
        angle = rotate(angle, turn_speed);

    // move forwards/backwards
    FPoint new_pos = pos;
    if (input_engine->pressing[UP]) {
        new_pos = calcVector(pos, angle, speed);
    }
    if (input_engine->pressing[DOWN]) {
        new_pos = calcVector(pos, angle+180, speed);
    }

    if (isWithin(screen_rect, new_pos))
        pos = new_pos;

    // animations
    current_animation.setPos(pos.x, pos.y);
    current_animation.setAngle(angle);
    current_animation.logic();
}

void GameEngine::render() {
    current_animation.render();
    test_text.render();
}
