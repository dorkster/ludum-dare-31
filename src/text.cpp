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
