#!/bin/bash

rm -f game_html.zip
# -O2 is recommended here, but game fails to run
# oddly, no optimization, -O1, and -O3 work
emcc src/*.cpp -O3 -s USE_SDL=2 -s USE_SDL_IMAGE=2 -s SDL2_IMAGE_FORMATS='["png"]' -s USE_SDL_TTF=2 --preload-file data -o index.html
cp index_template.html index.html
zip -r game_html.zip index.html index.js index.data index.html.mem data/*
