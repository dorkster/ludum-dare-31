#include "enemy.h"
#include "engines.h"

Enemy::Enemy(const int type) {
    if (type == ENEMY_SLIME) {
        anim_normal.load("data/slime.png", 0, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        anim_hurt.load("data/slime.png", 1, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
        anim_die.load("data/slime.png", 2, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
        hp = maxhp = 16;
        attack = 6;
        defense = 2;
    }

    is_turn = false;
    current_anim.setTo(anim_normal);

    hp_bar.load(render_engine, "data/hp_bar.png");

    player = NULL;
    animating = false;
}

Enemy::~Enemy() {
}

void Enemy::render() {
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

        hp_bar.setClip(0, 0 , ((float)hp/maxhp)*hp_bar.getWidth(), hp_bar.getHeight());
        hp_bar.render();
    }
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

bool Enemy::isAnimating() {
    return animating;
}
