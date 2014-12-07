#pragma once

#include "animation.h"
#include "common.h"
#include "engines.h"
#include "text.h"
#include "utils.h"
#include "map_engine.h"
#include "player.h"

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

    Text status_text;

    Image treasure_status;
};
