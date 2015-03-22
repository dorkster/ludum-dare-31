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


#include "powerup.h"

Powerup::Powerup(const int _type) {
    type = _type;

    if (type == POWERUP_ATTACK) {
        anim_normal.load("data/powerups.png", 0, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = (rand() % 5) + 1;
    }
    else if (type == POWERUP_DEFENSE) {
        anim_normal.load("data/powerups.png", 1, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = (rand() % 5) + 1;
    }
    else if (type == POWERUP_POTION) {
        anim_normal.load("data/powerups.png", 2, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = (rand()%40) + 10;
    }
    else if (type == POWERUP_TREASURE) {
        anim_normal.load("data/powerups.png", 3, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = 1;
    }

    current_anim.setTo(anim_normal);
}

Powerup::~Powerup() {
}

void Powerup::logic() {
    current_anim.logic();
}

void Powerup::render() {
    current_anim.setPos(pos.x*TILE_SIZE, pos.y*TILE_SIZE);
    current_anim.render();
}

void Powerup::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}
