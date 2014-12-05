#include "text.h"
#include "engines.h"

Text::Text()
    : text("")
{
}

Text::~Text() {
}

void Text::setText(const std::string& _text) {
    text = _text;
    render_engine->renderText(&image, text);
}

void Text::setPos(int x, int y) {
    image.setPos(x, y);
}

void Text::render() {
    image.render();
}
