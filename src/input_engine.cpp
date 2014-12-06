#include "input_engine.h"

InputEngine::InputEngine()
    : done(false)
{
    for (int i=0; i<KEY_COUNT; i++) {
        pressing[i] = false;
        lock[i] = false;
    }

    binding[ACCEPT] = SDLK_RETURN;
    binding[CANCEL] = SDLK_ESCAPE;
    binding[LEFT] = SDLK_a;
    binding[RIGHT] = SDLK_d;
    binding[UP] = SDLK_w;
    binding[DOWN] = SDLK_s;
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
            case SDL_QUIT:
                done = true;
                break;
        }
    }
}
