#!/bin/bash

rm -f game_html.zip
emcc src/*.cpp -O2 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 --preload-file data -o game.html
zip -r game_html.zip game.html game.js game.data game.html.mem data/*
