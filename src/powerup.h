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

#include "animation.h"
#include "utils.h"

const int POWERUP_ATTACK = 0;
const int POWERUP_DEFENSE = 1;
const int POWERUP_POTION = 2;
const int POWERUP_TREASURE = 3;

class Powerup {
public:
    Powerup(const int _type);
    ~Powerup();
    void logic();
    void render();
    void setPos(int x, int y);

    Point pos;
    int type;
    int amount;

private:
    Animation anim_normal;
    Animation current_anim;
};
