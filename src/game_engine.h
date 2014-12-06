#pragma once

#include "animation.h"
#include "common.h"
#include "engines.h"
#include "text.h"
#include "utils.h"
#include "map_engine.h"
#include "player.h"

const int MSG_COOLDOWN = 300;

class GameEngine {
public:
    GameEngine();
    ~GameEngine();
    void logic();
    void render();

    bool done;

private:
    MapEngine* map_engine;
    Player player;
    Text msg;
    int msg_ticks;

    Text status_text;
};
