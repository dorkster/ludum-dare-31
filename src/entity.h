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
#include "render_engine.h"
#include "sound_engine.h"
#include "utils.h"

class Entity {
public:
    Entity();
    virtual ~Entity();
    void render();
    void setPos(int x, int y);
    void takeDamage(int dmg);
    bool isAnimating();

    virtual void init();
    virtual void startTurn();

    Point pos;
    bool is_turn;
    int view_distance;

    int hp;
    int maxhp;
    int attack;
    int defense;
    int ap;
    int maxap;

    Animation anim_normal;
    Animation anim_hurt;
    Animation anim_die;
    Animation current_anim;

    Image hp_bar;
    bool animating;

    Sound sfx_hurt;
    Sound sfx_die;
    Sound sfx_move;
};
