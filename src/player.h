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

#include "entity.h"
#include "enemy.h"

const int PLAYER_MIN_ATTACK = 1;
const int PLAYER_MIN_DEFENSE = 1;

class Enemy;

class Player : public Entity {
public:
    Player();
    ~Player();
    void actionMove(int x, int y);
    void actionAttack(Enemy* e);
    void bonusAttack(int amount);
    void bonusDefense(int amount);
    void bonusHP(int amount);
    void bonusTreasure();

    void init();
    void startTurn();

    bool has_treasure;
};
