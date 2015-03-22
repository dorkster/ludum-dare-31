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


#pragma once

#include "common.h"
#include "utils.h"

#include <SDL_ttf.h>

class Image;

extern int VIEW_W;
extern int VIEW_H;
extern bool SOFTWARE_RENDER;

typedef struct {
    std::string filename;
    SDL_Texture* texture;
    int ref;
} Texture;

class RenderEngine {
public:
    RenderEngine();
    ~RenderEngine();
    void clear();
    void commitFrame();
    void loadImage(Image* image, const std::string& filename);
    void destroyImage(Image* image);
    void renderImage(const Image* image);
    void renderText(Image* image, const std::string& text, Color _color);
    void toggleFullscreen();

    SDL_Texture* cacheLookup(const std::string& filename);

private:
    void cacheStore(const Image* image);
    void cacheRemove(const std::string& filename);

    SDL_Window *window;
    SDL_Renderer *renderer;

    std::vector<Texture> cache;

    TTF_Font* font;

    bool fullscreen;
};

class Image {
public:
    Image();
    ~Image();
    void init(RenderEngine* _render_engine);
    void load(RenderEngine* _render_engine, const std::string _filename);
    void render();
    int getWidth();
    int getHeight();
    void setPos(int _x, int _y);
    void setClip(int _x, int _y, int _w, int _h);
    void setAngle(double _angle);
    void setAlpha(int _alpha = 255);
    void ref();

    std::string filename;
    SDL_Texture* texture;

    SDL_Rect dest;
    SDL_Rect clip;
    double angle;

private:
    RenderEngine* render_engine;
    int width;
    int height;
};

