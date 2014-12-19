#include "enemy.h"
#include "engines.h"
#include "map_engine.h"

Enemy::Enemy(const int type) {
    if (type == ENEMY_SLIME) {
        anim_normal.load("data/slime.png", 0, 1, 100, 0, TILE_SIZE, TILE_SIZE, 0, 0);
        anim_hurt.load("data/slime.png", 1, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
        anim_die.load("data/slime.png", 2, 4, 266, 1, TILE_SIZE, TILE_SIZE, 0, 0);
        hp = maxhp = (rand()%10) + 10;
        attack = (rand()%5) + 10;
        defense = (rand()%5) + 1;
        ap = maxap = 2;
        view_distance = 6;
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

    if (ap == 0)
        ap = maxap;
}

void Enemy::setPlayer(Player* _player) {
    player = _player;
}

void Enemy::logic(MapEngine* map_engine) {
    if (is_turn && !isAnimating()) {
        if (action_ticks > 0)
            action_ticks--;
        else if (ap > 0) {
            // attack
            if (isNearPlayer(1)) {
                actionAttack();
            }
            // move towards player
            else {
                actionMove(map_engine);
            }

            if (ap > 0)
                action_ticks = ENEMY_COOLDOWN;

        }

        is_turn = (ap > 0 || player->isAnimating());
    }
}

void Enemy::actionAttack() {
    if (!player) return;

    int dmg = ((rand() % attack) + (attack/2)) - ((rand() % player->defense) + (player->defense/2));
    if (dmg <= 0) dmg = 1;

    player->takeDamage(dmg);

    ap = 0;
}

void Enemy::actionMove(MapEngine* map_engine) {
    Point dest = pos;
    bool move_randomly = true;
    if (isNearPlayer(view_distance)) {
        dest = map_engine->findPath(pos, player->pos);
        if (!(dest.x == pos.x && dest.y == pos.y)) {
            move_randomly = false;
        }
    }
    if (move_randomly) {
        std::vector<Point> open;
        for (int i=0; i<MAP_H; i++) {
            for (int j=0; j<MAP_W; j++) {
                float dist = calcDistance(pos, FPoint(j, i));
                if (dist <= 1 && map_engine->isWalkable(j, i)) {
                    open.push_back(Point(j,i));
                }
            }
        }
        if (!open.empty()) {
            dest = open[rand() % open.size()];
        }
    }
    if (map_engine->isWalkable(dest.x, dest.y)) {
        // logInfo("Start: (%d, %d), End: (%d, %d)\n", pos.x, pos.y, dest.x, dest.y);
        ap -= calcDistance(pos, dest);
        setPos(dest.x, dest.y);
        sfx_move.play();
    }
    else {
        // can't move, end turn
        ap = 0;
    }
}

bool Enemy::isNearPlayer(int range) {
    if (!player) return false;

    return calcDistance(pos, player->pos) <= range;
}

