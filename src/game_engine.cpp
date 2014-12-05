#include "game_engine.h"
#include "engines.h"
#include "utils.h"

GameEngine::GameEngine()
    : done(false)
{
}

GameEngine::~GameEngine() {
}

void GameEngine::logic() {
    // exit game
    if (input_engine->pressing[CANCEL])
        done = true;

}

void GameEngine::render() {
}
