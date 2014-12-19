#include "game_engine.h"
#include "engines.h"
#include "utils.h"

#include <sstream>

GameEngine::GameEngine()
    : done(false)
{
    map_engine = new MapEngine();
    map_engine->init(&player);

    status_text.setPos(2, VIEW_H-14);

    treasure_status.load(render_engine, "data/treasure_status.png");
    treasure_status.setPos(VIEW_W-TILE_SIZE, VIEW_H-TILE_SIZE);
}

GameEngine::~GameEngine() {
    delete map_engine;
}

void GameEngine::logic() {
    // exit game
    if (input_engine->pressing[EXIT]) {
        done = true;
        return;
    }

    if (input_engine->pressing[FULLSCREEN_TOGGLE] && !input_engine->lock[FULLSCREEN_TOGGLE]) {
        input_engine->lock[FULLSCREEN_TOGGLE] = true;
        render_engine->toggleFullscreen();
    }

    std::stringstream ss;
    ss << "AP:" << player.ap << " | Att:" << player.attack << " | Def:" << player.defense << " | Lvl:" << map_engine->getCurrentLevel();
    status_text.setText(ss.str());

    if (map_engine->game_state == GAME_PLAY) {
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

            if (input_engine->pressing[ACTION] && !input_engine->lock[ACTION]) {
                input_engine->lock[ACTION] = true;
                if (map_engine->playerAction()) {
                    map_engine->enemyStartTurn();
                }
            }
        }
        else {
            if (map_engine->enemyAction()) {
                map_engine->playerStartTurn();
            }
        }

        // check to see if we won the game or died
        map_engine->checkWinLoss();
    }
    else {
        // win and lose both have the same post-game logic
        if (input_engine->pressing[RESTART] && !input_engine->lock[RESTART]) {
            input_engine->lock[RESTART] = true;
            // restart the game
            map_engine->init(&player);
        }
    }
}

void GameEngine::render() {
    map_engine->render();

    // player stats
    status_text.render();

    // treasure status
    if (player.has_treasure)
        treasure_status.setClip(0,TILE_SIZE,TILE_SIZE,TILE_SIZE);
    else
        treasure_status.setClip(0,0,TILE_SIZE,TILE_SIZE);

    treasure_status.render();
}
