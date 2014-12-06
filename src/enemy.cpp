#include "enemy.h"
#include "engines.h"

Enemy::Enemy(const int type) {
    if (type == ENEMY_SLIME) {
        anim_normal.load("data/slime.png", 0, 1, 100, 0, 16, 16, 0, 0);
        hp = maxhp = 16;
        attack = 4;
        defense = 2;
    }

    is_turn = false;
    current_anim.setTo(anim_normal);

    hp_bar.load(render_engine, "data/hp_bar.png");

    player = NULL;
    move_to_player = false;
}

Enemy::~Enemy() {
}

void Enemy::logic() {
    if (is_turn) {
        if (action_ticks > 0)
            action_ticks--;
        else {
            // move towards player
            if (player && calcDistance(player->pos, pos) > 1) {
                move_to_player = true;
            }
            // attack
            else {
                move_to_player = false;
                actionAttack();
            }

            is_turn = false;
        }
    }
    current_anim.logic();
}

void Enemy::render() {
    current_anim.setPos(pos.x*16, pos.y*16);
    current_anim.render();

    hp_bar.setPos(pos.x*16, pos.y*16);
    hp_bar.setClip(0, 0 , ((float)hp/maxhp)*hp_bar.getWidth(), hp_bar.getHeight());
    hp_bar.render();
}

void Enemy::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}

void Enemy::startTurn() {
    is_turn = true;
    action_ticks = ENEMY_COOLDOWN;
}

void Enemy::takeDamage(int dmg) {
    if (dmg < 0) return;

    hp -= dmg;
    if (hp < 0) hp = 0;
}

void Enemy::setPlayer(Player* _player) {
    player = _player;
}

void Enemy::actionAttack() {
    if (!player) return;

    player->takeDamage(attack - player->defense);
}
