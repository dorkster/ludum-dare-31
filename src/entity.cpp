#include "engines.h"
#include "entity.h"

Entity::Entity() {
    hp_bar.load(render_engine, "data/hp_bar.png");

    sound_engine->loadSound(&sfx_hurt, "data/hurt.wav");
    sound_engine->loadSound(&sfx_die, "data/die.wav");
    sound_engine->loadSound(&sfx_move, "data/move.wav");
}

Entity::~Entity() {
}

void Entity::init() {
}

void Entity::startTurn() {
}

void Entity::render() {
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

void Entity::setPos(int x, int y) {
    pos.x = x;
    pos.y = y;
}

void Entity::takeDamage(int dmg) {
    if (dmg <= 0) return;

    hp -= dmg;
    if (hp <= 0) {
        hp = 0;
        current_anim.setTo(anim_die);
        sfx_die.play();
        animating = true;
    }
    else {
        current_anim.setTo(anim_hurt);
        sfx_hurt.play();
        animating = true;
    }
}

bool Entity::isAnimating() {
    return animating;
}
