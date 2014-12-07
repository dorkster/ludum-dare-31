#include "player.h"
#include "engines.h"

Player::Player() {
    anim_normal.load("data/player.png", 0, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
    anim_hurt.load("data/player.png", 1, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
    anim_die.load("data/player.png", 2, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);

    hp_bar.load(render_engine, "data/hp_bar.png");
}

Player::~Player() {
}

void Player::init() {
    current_anim.setTo(anim_normal);
    is_turn = false;
    hp = maxhp = 32;
    attack = 15;
    defense = 3;
    has_treasure = false;
    animating = false;
}

void Player::render() {
    current_anim.logic();

    if (animating && current_anim.isFinished()) {
        current_anim.setTo(anim_normal);
        animating = false;
    }

    if (hp > 0 || animating) {
        current_anim.setPos(pos.x*TILE_SIZE, pos.y*TILE_SIZE);
        current_anim.render();

        hp_bar.setPos(pos.x*TILE_SIZE, pos.y*TILE_SIZE);

        int bar_length = ((float)hp/maxhp)*hp_bar.getWidth();
        if (hp > 0 && bar_length < 1)
            bar_length = 1;

        hp_bar.setClip(0, 0 , bar_length, hp_bar.getHeight());
        hp_bar.render();
    }
}

void Player::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}

void Player::takeDamage(int dmg) {
    if (dmg <= 0) return;

    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        current_anim.setTo(anim_die);
        animating = true;
    }
    else {
        current_anim.setTo(anim_hurt);
        animating = true;
    }
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
    hp += amount;
    if (hp > maxhp)
        hp = maxhp;

    // poisons can't kill the player outright
    if (hp <= 0)
        hp = 1;
}

void Player::bonusTreasure() {
    has_treasure = true;
}

bool Player::isAnimating() {
    return animating;
}
