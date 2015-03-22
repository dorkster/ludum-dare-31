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

const int KEY_COUNT = 8;

const int ACTION = 0;
const int EXIT = 1;
const int LEFT = 2;
const int RIGHT = 3;
const int UP = 4;
const int DOWN = 5;
const int RESTART = 6;
const int FULLSCREEN_TOGGLE = 7;

class InputEngine {
public:
    InputEngine();
    ~InputEngine();
    void logic();

    bool done;

    int binding[KEY_COUNT];
    int pressing[KEY_COUNT];
    int lock[KEY_COUNT];
};
