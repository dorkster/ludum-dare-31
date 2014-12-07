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

    SDL_Texture* cacheLookup(const std::string& filename);

private:
    void cacheStore(const Image* image);
    void cacheRemove(const std::string& filename);

    SDL_Window *window;
    SDL_Renderer *renderer;

    std::vector<Texture> cache;

    TTF_Font* font;
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

