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


#include "input_engine.h"

InputEngine::InputEngine()
    : done(false)
    , pressing_mouse(false)
    , lock_mouse(false)
{
    for (int i=0; i<KEY_COUNT; i++) {
        pressing[i] = false;
        lock[i] = false;
    }

    binding[ACTION] = SDLK_RETURN;
    binding[EXIT] = SDLK_ESCAPE;
    binding[LEFT] = SDLK_a;
    binding[RIGHT] = SDLK_d;
    binding[UP] = SDLK_w;
    binding[DOWN] = SDLK_s;
    binding[RESTART] = SDLK_r;
    binding[FULLSCREEN_TOGGLE] = SDLK_f;
}

InputEngine::~InputEngine() {
}

void InputEngine::logic() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch(event.type) {
            case SDL_KEYDOWN:
                for (int i=0; i<KEY_COUNT; i++) {
                    if (event.key.keysym.sym == binding[i]) {
                        pressing[i] = true;
                    }
                }
                break;
            case SDL_KEYUP:
                for (int i=0; i<KEY_COUNT; i++) {
                    if (event.key.keysym.sym == binding[i]) {
                        pressing[i] = false;
                        lock[i] = false;
                    }
                }
                break;
            case SDL_MOUSEMOTION:
                mouse.x = event.motion.x;
                mouse.y = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                mouse.x = event.motion.x;
                mouse.y = event.motion.y;
                if (event.button.button == SDL_BUTTON_LEFT) {
                    pressing_mouse = true;
                }
                break;
            case SDL_MOUSEBUTTONUP:
                mouse.x = event.motion.x;
                mouse.y = event.motion.y;
                if (event.button.button == SDL_BUTTON_LEFT) {
                    pressing_mouse = false;
                    lock_mouse = false;
                }
                break;
            case SDL_FINGERMOTION:
                mouse.x = (int)((event.tfinger.x + event.tfinger.dx) * SCREEN_WIDTH);
                mouse.y = (int)((event.tfinger.y + event.tfinger.dy) * SCREEN_HEIGHT);
                break;
            case SDL_QUIT:
                done = true;
                break;
        }
    }
}
