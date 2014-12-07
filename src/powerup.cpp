#include "powerup.h"

Powerup::Powerup(const int _type) {
    type = _type;

    if (type == POWERUP_ATTACK) {
        anim_normal.load("data/powerups.png", 0, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = (rand() % 5) + 1;
    }
    else if (type == POWERUP_DEFENSE) {
        anim_normal.load("data/powerups.png", 1, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = (rand() % 5) + 1;
    }
    else if (type == POWERUP_POTION) {
        anim_normal.load("data/powerups.png", 2, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = (rand()%40) + 10;
    }
    else if (type == POWERUP_TREASURE) {
        anim_normal.load("data/powerups.png", 3, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        amount = 1;
    }

    current_anim.setTo(anim_normal);
}

Powerup::~Powerup() {
}

void Powerup::logic() {
    current_anim.logic();
}

void Powerup::render() {
    current_anim.setPos(pos.x*TILE_SIZE, pos.y*TILE_SIZE);
    current_anim.render();
}

void Powerup::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}
