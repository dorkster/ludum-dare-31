#include "game_engine.h"
#include "engines.h"
#include "utils.h"

#include <sstream>

GameEngine::GameEngine()
    : done(false)
{
    map_engine = new MapEngine();
    map_engine->setPlayer(&player);

    map_engine->nextLevel();
    map_engine->playerStartTurn();

    msg.setText("WASD = cursor. Enter = action.");
    msg_ticks = MSG_COOLDOWN;

    status_text.setPos(0, 112);
}

GameEngine::~GameEngine() {
    delete map_engine;
}

void GameEngine::logic() {
    // exit game
    if (input_engine->pressing[CANCEL])
        done = true;

    std::stringstream ss;
    ss << "Att:" << player.attack << " | Def:" << player.defense << " | Potion:" << player.potions;
    status_text.setText(ss.str());

    if (player.is_turn) {
        if (input_engine->pressing[UP] && !input_engine->lock[UP]) {
            input_engine->lock[UP] = true;
            map_engine->moveCursor(CUR_UP);
        }
        if (input_engine->pressing[DOWN] && !input_engine->lock[DOWN]) {
            input_engine->lock[DOWN] = true;
            map_engine->moveCursor(CUR_DOWN);
        }
        if (input_engine->pressing[LEFT] && !input_engine->lock[LEFT]) {
            input_engine->lock[LEFT] = true;
            map_engine->moveCursor(CUR_LEFT);
        }
        if (input_engine->pressing[RIGHT] && !input_engine->lock[RIGHT]) {
            input_engine->lock[RIGHT] = true;
            map_engine->moveCursor(CUR_RIGHT);
        }

        if (input_engine->pressing[ACCEPT] && !input_engine->lock[ACCEPT]) {
            input_engine->lock[ACCEPT] = true;
            if (map_engine->playerAction()) {
                player.is_turn = false;
                map_engine->enemyStartTurn();
                msg.setText("Enemy turn");
                msg_ticks = MSG_COOLDOWN;
            }
        }
    }
    else {
        if (map_engine->enemyAction()) {
            map_engine->playerStartTurn();
            msg.setText("Player turn");
            msg_ticks = MSG_COOLDOWN;
        }
    }

    if (msg_ticks > 0)
        msg_ticks--;
}

void GameEngine::render() {
    map_engine->render();

    // render help text
    if (msg_ticks > 0)
        msg.render();

    status_text.render();
}
