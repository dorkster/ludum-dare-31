#pragma once

#include "common.h"

const int KEY_COUNT = 8;

const int ACTION = 0;
const int EXIT = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int UP = 4;
const int DOWN = 5;
const int RESTART = 6;
const int FULLSCREEN_TOGGLE = 7;

class InputEngine {
public:
    InputEngine();
    ~InputEngine();
    void logic();

    bool done;

    int binding[KEY_COUNT];
    int pressing[KEY_COUNT];
    int lock[KEY_COUNT];
};
