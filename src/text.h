#pragma once

#include "common.h"
#include "utils.h"
#include "render_engine.h"

class Text {
public:
    Text();
    ~Text();
    void setPos(int x, int y);
    void setText(const std::string& _text);
    void render();

private:
    Image image;
    Image image_shadow;
    std::string text;
    Point pos;
};
