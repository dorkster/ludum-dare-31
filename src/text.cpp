#include "text.h"
#include "engines.h"

Text::Text()
    : text("")
{
}

Text::~Text() {
}

void Text::setText(const std::string& _text) {
    if (text == _text) return;

    text = _text;
    render_engine->renderText(&image, text, Color(255,255,255));
    render_engine->renderText(&image_shadow, text, Color(0,0,0));
    setPos(pos.x,pos.y);
}

void Text::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;

    image.setPos(x, y);
    image_shadow.setPos(x+1, y+1);
}

void Text::render() {
    image_shadow.render();
    image.render();
}
