#include "player.h"
#include "engines.h"

Player::Player() {
    anim_normal.load("data/player.png", 0, 1, 100, 0, 16, 16, 0, 0);
    current_anim.setTo(anim_normal);

    hp_bar.load(render_engine, "data/hp_bar.png");

    is_turn = false;
    hp = maxhp = 16;
    attack = 4;
    defense = 2;
    potions = 0;
}

Player::~Player() {
}

void Player::logic() {
    current_anim.logic();
}

void Player::render() {
    current_anim.setPos(pos.x*16, pos.y*16);
    current_anim.render();

    hp_bar.setPos(pos.x*16, pos.y*16);
    hp_bar.setClip(0, 0 , ((float)hp/maxhp)*hp_bar.getWidth(), hp_bar.getHeight());
    hp_bar.render();
}

void Player::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}

void Player::takeDamage(int dmg) {
    if (dmg < 0) return;

    hp -= dmg;
    if (hp < 0) hp = 0;
}

void Player::actionMove(int x, int y) {
    setPos(x, y);
    is_turn = false;
}

void Player::actionAttack(Enemy* e) {
    if (!e) return;

    e->takeDamage(attack);
}
