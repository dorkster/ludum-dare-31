#include "enemy.h"
#include "engines.h"

Enemy::Enemy(const int type) {
    if (type == ENEMY_SLIME) {
        anim_normal.load("data/slime.png", 0, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        anim_hurt.load("data/slime.png", 1, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
        anim_die.load("data/slime.png", 2, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
        hp = maxhp = (rand()%10) + 10;
        attack = (rand()%5) + 10;
        defense = (rand()%5) + 1;
    }

    init();
}

Enemy::~Enemy() {
}

void Enemy::init() {
    is_turn = false;
    current_anim.setTo(anim_normal);

    player = NULL;
    animating = false;
}

void Enemy::startTurn() {
    is_turn = true;
    action_ticks = ENEMY_COOLDOWN;
}

void Enemy::setPlayer(Player* _player) {
    player = _player;
}

void Enemy::actionAttack() {
    if (!player) return;

    int dmg = ((rand() % attack) + (attack/2)) - ((rand() % player->defense) + (player->defense/2));
    if (dmg <= 0) dmg = 1;

    player->takeDamage(dmg);
}

bool Enemy::isNearPlayer(int range) {
    if (!player) return false;

    FPoint horizontal(player->pos.x, pos.y);
    FPoint vertical(pos.x, player->pos.y);

    if (calcDistance(horizontal, pos) > range || calcDistance(vertical, pos) > range) {
        return false;
    }
    else {
        return true;
    }
}

