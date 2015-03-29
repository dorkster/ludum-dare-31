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


#include "common.h"
#include "utils.h"
#include "render_engine.h"

int VIEW_W = 0;
int VIEW_H = 0;
bool SOFTWARE_RENDER = false;

RenderEngine::RenderEngine()
    : font(NULL)
{
    SDL_DisplayMode desktop;
    SDL_GetDesktopDisplayMode(0, &desktop);

#ifdef __ANDROID__
    fullscreen = true;
#else
    fullscreen = false;
#endif

    VIEW_W = 224;
    VIEW_H = 128;

    int window_w = (int)(desktop.w / 1.5f /VIEW_W) * VIEW_W;
    int window_h = (int)(desktop.h / 1.5f /VIEW_H) * VIEW_H;

    if (fullscreen) {
        window = SDL_CreateWindow("Noman's Dungeon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP);
    }
    else {
        window = SDL_CreateWindow("Noman's Dungeon", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, window_w, window_h, 0);
    }

    // touch input needs to know window size
    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;

    if (SOFTWARE_RENDER)
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_SOFTWARE);
    else
        renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_TARGETTEXTURE | SDL_RENDERER_PRESENTVSYNC);

    // SDL_SetHint(SDL_HINT_RENDER_SCALE_QUALITY, "linear");
    SDL_RenderSetLogicalSize(renderer, VIEW_W, VIEW_H);

    // load default text font
    if (TTF_Init() != -1)
        font = TTF_OpenFont("data/dejavu_sans_mono.ttf", 10);
}

RenderEngine::~RenderEngine() {
    for (unsigned i=0; i<cache.size(); ++i) {
        if (cache[i].ref > 0)
            logInfo("RenderEngine: Image file '%s' still has %d references\n", cache[i].filename.c_str(), cache[i].ref);
    }

    if (font)
        TTF_CloseFont(font);

    TTF_Quit();

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
}

void RenderEngine::clear() {
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_RenderClear(renderer);
}

void RenderEngine::commitFrame() {
    SDL_RenderPresent(renderer);
}

void RenderEngine::loadImage(Image* image, const std::string& filename) {
    if (!image) return;

    SDL_Texture* texture = NULL;
    
    // check texture cache first
    texture = cacheLookup(filename);
    if (texture) {
        image->filename = filename;
        image->texture = texture;
        image->init(this);
        return;
    }

    // load the texture from disk and cache it
    texture = IMG_LoadTexture(renderer, filename.c_str());
    if (texture) {
        image->filename = filename;
        image->texture = texture;
        cacheStore(image);
        // logInfo("Loaded image: %s\n", filename.c_str());
    }
    image->init(this);

    if (image->filename == "")
        logError("RenderEngine: Could not load image: '%s'\n", filename.c_str());
}

void RenderEngine::destroyImage(Image* image) {
    if (image) {
        if (image->filename != "")
            cacheRemove(image->filename);
        else if (image->texture != NULL) {
            SDL_DestroyTexture(image->texture);
            image->texture = NULL;
        }
    }
}

void RenderEngine::renderImage(const Image* image) {
    SDL_RenderCopyEx(renderer, image->texture, &(image->clip), &(image->dest), image->angle, NULL, SDL_FLIP_NONE);
}

void RenderEngine::renderText(Image* image, const std::string& text, Color _color) {
    if (!font || !image) return;

    image->filename = "";

    if (image->texture) {
        SDL_DestroyTexture(image->texture);
        image->texture = NULL;
    }

    SDL_Color color = _color;
    SDL_Surface* surface = TTF_RenderUTF8_Solid(font, text.c_str(), color);
    if (surface) {
        image->texture = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    image->init(this);
}

void RenderEngine::toggleFullscreen() {
#ifdef __ANDROID__
    return;
#endif

    if (fullscreen)
        SDL_SetWindowFullscreen(window, 0);
    else
        SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    fullscreen = !fullscreen;

    // touch input needs to know window size
    int w,h;
    SDL_GetWindowSize(window, &w, &h);
    SCREEN_WIDTH = w;
    SCREEN_HEIGHT = h;
}

void RenderEngine::cacheStore(const Image* image) {
    if (image->filename != "" && image->texture != NULL) {
        Texture tex;
        tex.filename = image->filename;
        tex.texture = image->texture;
        tex.ref = 1;
        cache.push_back(tex);
    }
}

void RenderEngine::cacheRemove(const std::string& filename) {
    for (unsigned i=0; i<cache.size(); ++i) {
        if (filename == cache[i].filename) {
            cache[i].ref--;
            if (cache[i].ref <= 0) {
                // logInfo("Deleted image: %s\n", filename.c_str());
                SDL_DestroyTexture(cache[i].texture);
                cache.erase(cache.begin()+i);
            }
            return;
        }
    }
}

SDL_Texture* RenderEngine::cacheLookup(const std::string& filename) {
    for (unsigned i=0; i<cache.size(); ++i) {
        if (filename == cache[i].filename) {
            cache[i].ref++;
            return cache[i].texture;
        }
    }

    return NULL;
}



// Image class

Image::Image()
    : filename("")
    , texture(NULL)
    , angle(0)
    , render_engine(NULL)
    , width(0)
    , height(0)
{
    dest.x = dest.y = dest.w = dest.h = 0;
    clip.x = clip.y = clip.w = clip.h = 0;
}

Image::~Image() {
    if (render_engine)
        render_engine->destroyImage(this);
}

void Image::init(RenderEngine* _render_engine) {
    render_engine = _render_engine;
    SDL_QueryTexture(texture, NULL, NULL, &width, &height);
    dest.w = clip.w = width;
    dest.h = clip.h = height;
}

void Image::load(RenderEngine* _render_engine, const std::string _filename) {
    if (_render_engine)
        _render_engine->loadImage(this, _filename);
}

void Image::render() {
    if (render_engine)
        render_engine->renderImage(this);
}

int Image::getWidth() {
    return width;
}

int Image::getHeight() {
    return height;
}

void Image::setPos(int _x, int _y) {
    dest.x = _x;
    dest.y = _y;
}

void Image::setClip(int _x, int _y, int _w, int _h) {
    clip.x = _x;
    clip.y = _y;
    clip.w = dest.w = _w;
    clip.h = dest.h = _h;
}

void Image::setAngle(double _angle) {
    angle = _angle;
}

void Image::setAlpha(int _alpha) {
    SDL_SetTextureAlphaMod(texture, _alpha);
}

void Image::ref() {
    if (render_engine)
        render_engine->cacheLookup(filename);
}
