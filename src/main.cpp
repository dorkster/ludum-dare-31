/*
Copyright © 2014-2015 Justin Jacobs

This file is part of Noman's Dungeon.

Noman's Dungeon is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Noman's Dungeon is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Noman's Dungeon.  If not, see http://www.gnu.org/licenses/
*/


#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <ctime>

#include "common.h"
#include "engines.h"
#include "game_engine.h"
#include "utils.h"

#ifdef __EMSCRIPTEN__
#include "emscripten.h"
#endif

GameEngine* game_engine = NULL;

static void init() {
    // SDL Inits
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_JOYSTICK) < 0) {
        logError("main: Could not initialize SDL: %s\n", SDL_GetError());
        exit(1);
    }

    render_engine = new RenderEngine();
    input_engine = new InputEngine();
    sound_engine = new SoundEngine();
    game_engine = new GameEngine();
}

static void mainLoop () {
    bool done = false;
    int delay = int(floor((1000.f/MAX_FRAMES_PER_SEC)+0.5f));
    int logic_ticks = SDL_GetTicks();

    while (!done) {
        int loops = 0;
        int now_ticks = SDL_GetTicks();
        int prev_ticks = now_ticks;

        while (now_ticks > logic_ticks && loops < MAX_FRAMES_PER_SEC) {
            // Frames where data loading happens take a long time, so our loop here will 
            // think that the game "lagged" and try to compensate. To prevent this 
            // compensation, we mark those frames as "loading frames" and update the logic 
            // ticker without actually executing logic.
            //
            // if (game_engine->isLoading()) {
            //     logic_ticks = now_ticks;
            //     break;
            // }

            input_engine->logic();
            game_engine->logic();

            // Engine done means the user escapes the main game menu.
            // Input done means the user closes the window.
            done = game_engine->done || input_engine->done;

            logic_ticks += delay;
            loops++;
        }

        render_engine->clear();
        game_engine->render();
        render_engine->commitFrame();

        // delay quick frames
        now_ticks = SDL_GetTicks();
        if (now_ticks - prev_ticks < delay) {
            SDL_Delay(delay - (now_ticks - prev_ticks));
        }

    }
}

#ifdef __EMSCRIPTEN__
static void emscriptenMainLoop() {
    input_engine->logic();
    game_engine->logic();

    render_engine->clear();
    game_engine->render();
    render_engine->commitFrame();
}
#endif

static void cleanup() {
    delete game_engine;
    delete sound_engine;
    delete input_engine;
    delete render_engine;

    SDL_Quit();
}

int main(int argc, char* argv[]) {
    bool done = false;

    for (int i=1; i<argc; i++) {
        std::string arg(argv[i]);
        if (arg == "--version") {
            logInfo("Version 0.01");
            done = true;
        }
        else if (arg == "--software")
            SOFTWARE_RENDER = true;
        else
            logError("Unknown option '%s'\n", argv[i]);
    }

    if (!done) {
        srand((unsigned int)time(NULL));
        init();

#ifdef __EMSCRIPTEN__
        emscripten_set_main_loop(emscriptenMainLoop, 60, 1);
#endif

        mainLoop();
        cleanup();
    }

    return 0;
}
