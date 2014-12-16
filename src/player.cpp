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
    view_distance = 3;
}

void Player::startTurn() {
    is_turn = true;
}

void Player::actionMove(int x, int y) {
    setPos(x, y);
    is_turn = false;
}

void Player::actionAttack(Enemy* e) {
    if (!e) return;

    int dmg = ((rand() % attack) + (attack/2)) - ((rand() % e->defense) + (e->defense/2));
    if (dmg <= 0) dmg = 1;

    e->takeDamage(dmg);
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

