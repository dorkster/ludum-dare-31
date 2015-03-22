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


#include "player.h"
#include "engines.h"

Player::Player() {
    anim_normal.load("data/player.png", 0, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
    anim_hurt.load("data/player.png", 1, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
    anim_die.load("data/player.png", 2, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
}

Player::~Player() {
}

void Player::init() {
    current_anim.setTo(anim_normal);
    is_turn = false;
    hp = maxhp = 50;
    attack = 10;
    defense = 10;
    has_treasure = false;
    animating = false;
    view_distance = 6;
    ap = maxap = 2;
}

void Player::startTurn() {
    is_turn = true;
    if (ap == 0)
        ap = maxap;
}

void Player::actionMove(int x, int y) {
    ap -= calcDistance(pos, Point(x,y));
    setPos(x, y);
    is_turn = false;
}

void Player::actionAttack(Enemy* e) {
    if (!e) return;

    int dmg = ((rand() % attack) + (attack/2)) - ((rand() % e->defense) + (e->defense/2));
    if (dmg <= 0) dmg = 1;

    e->takeDamage(dmg);

    ap = 0;
}

void Player::bonusAttack(int amount) {
    attack += amount;
    if (attack < PLAYER_MIN_ATTACK)
        attack = PLAYER_MIN_ATTACK;
}

void Player::bonusDefense(int amount) {
    defense += amount;
    if (defense < PLAYER_MIN_DEFENSE)
        defense = PLAYER_MIN_DEFENSE;
}

void Player::bonusHP(int amount) {
    if (amount == 0) return;

    int real_amount = (amount*maxhp)/100;
    hp += real_amount;
    if (hp > maxhp)
        hp = maxhp;

    // poisons can't kill the player outright
    if (hp <= 0)
        hp = 1;
}

void Player::bonusTreasure() {
    has_treasure = true;
}

