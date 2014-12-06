#include "map_engine.h"
#include "engines.h"

MapEngine::MapEngine() {
    tileset.load(render_engine, "data/tileset.png");
    cursor.load(render_engine, "data/cursor.png");
    player = NULL;
    active_enemy = -1;
}

MapEngine::~MapEngine() {
    for (unsigned i=0; i<levels.size(); i++) {
        for (unsigned k=0; k<enemies[i].size(); k++) {
            delete enemies[i][k];
        }
        for (unsigned k=0; k<powerups[i].size(); k++) {
            delete powerups[i][k];
        }
        for (int j=0; j<MAP_H; j++) {
            delete levels[i][j];
        }
        delete levels[i];
    }
}

void MapEngine::prevLevel() {
    if ((int)current_level-1 >= 0) {
        current_level--;
        for (int i=0; i<MAP_H; i++) {
            for (int j=0; j<MAP_W; j++) {
                if (levels[current_level][i][j] == 3) {
                    player->setPos(j, i);
                    cursor_pos = player->pos;
                    setContextTiles();
                    return;
                }
            }
        }
    }
}

void MapEngine::nextLevel() {
    if (current_level+1 < levels.size()) {
        // load existing level
        current_level++;
        bool found_stairs = false;
        for (int i=0; i<MAP_H; i++) {
            for (int j=0; j<MAP_W; j++) {
                if (levels[current_level][i][j] == 2) {
                    player->setPos(j, i);
                    found_stairs = true;
                    break;
                }
            }
            if (found_stairs)
                break;
        }
    }
    else {
        // generate new level
        Point player_pos(1,1);
        Point stairs_pos(MAP_W-2, (rand() % (MAP_H-2)) + 1);

        if (player) {
            player_pos.x = 1;
            player_pos.y = (rand() % (MAP_H-2)) + 1;
            player->setPos(player_pos.x, player_pos.y);
        }

        int **room = new int*[MAP_H];
        for (int k=0; k<MAP_H; k++)
            room[k] = new int[MAP_W];

        for (int i=0; i<MAP_H; i++) {
            for (int j=0; j<MAP_W; j++) {
                room[i][j] = 0;

                // borders
                if (i == 0 || i == MAP_H-1)
                    room[i][j] = 1;
                if (j == 0 || j == MAP_W-1)
                    room[i][j] = 1;

                // up stairs
                if (i == player_pos.y && j == player_pos.x)
                    room[i][j] = 2;

                // down stairs
                if (i == stairs_pos.y && j == stairs_pos.x)
                    room[i][j] = 3;
            }
        }

        levels.push_back(room);
        current_level = levels.size()-1;
        spawnEnemies();
        spawnPowerups();
    }

    cursor_pos = player->pos;
    setContextTiles();
}

void MapEngine::render() {
    for (int i=0; i<MAP_H; i++) {
        for (int j=0; j<MAP_W; j++) {
            int tile = levels[current_level][i][j];
            tileset.setClip(tile*16, 0, 16, 16);
            tileset.setPos(j*16, i*16);
            tileset.render();
        }
    }

    if (player) {
        // render context tiles
        if (player->is_turn) {
            for (int i=0; i<MAP_H; i++) {
                for (int j=0; j<MAP_W; j++) {
                    int tile = context_tiles[i][j];
                    tileset.setClip(tile*16, 16, 16, 16);
                    tileset.setPos(j*16, i*16);
                    tileset.render();
                }
            }

            cursor.setPos(cursor_pos.x*16, cursor_pos.y*16);
            cursor.render();
        }

        // render player
        player->render();
    }

    // render powerups
    for (unsigned i=0; i<powerups[current_level].size(); i++) {
        powerups[current_level][i]->logic(); // powerup logic is only animation
        powerups[current_level][i]->render();
    }

    // render enemies
    for (unsigned i=0; i<enemies[current_level].size(); i++) {
        enemies[current_level][i]->render();
    }
}

void MapEngine::setContextTiles() {
    for (int i=0; i<MAP_H; i++) {
        for (int j=0; j<MAP_W; j++) {
            context_tiles[i][j] = -1;
            int dist = calcDistance(player->pos, FPoint(j, i));
            if (dist <= 1 && isEnemy(j, i)) {
                context_tiles[i][j] = 1;
            }
            else if (dist <= 1 && isWalkable(j, i)) {
                context_tiles[i][j] = 0;
            }
        }
    }
}

bool MapEngine::isWalkable(int x, int y) {
    if (x < 1 || x > MAP_W-1) return false;
    if (y < 1 || y > MAP_H-1) return false;

    int tile = levels[current_level][y][x];

    if (tile == 1)
        return false;

    if (isEnemy(x,y))
        return false;

    if (player && x == player->pos.x && y == player->pos.y)
        return false;

    return true;
}

bool MapEngine::isEnemy(int x, int y) {
    if (x < 1 || x > MAP_W-1) return false;
    if (y < 1 || y > MAP_H-1) return false;

    // search for enemies that are on this tile
    Enemy* e = getEnemy(x,y);
    if (e != NULL)
        return true;


    return false;
}

bool MapEngine::isPowerup(int x, int y) {
    if (x < 1 || x > MAP_W-1) return false;
    if (y < 1 || y > MAP_H-1) return false;

    for (unsigned i=0; i<powerups[current_level].size(); i++) {
        Powerup* p = powerups[current_level][i];
        if (x == p->pos.x && y == p->pos.y)
            return true;
    }

    return false;
}

void MapEngine::moveCursor(int direction) {
    if (direction == CUR_UP)
        cursor_pos.y--;
    else if (direction == CUR_DOWN)
        cursor_pos.y++;
    else if (direction == CUR_LEFT)
        cursor_pos.x--;
    else if (direction == CUR_RIGHT)
        cursor_pos.x++;

    if (cursor_pos.x < 0)
        cursor_pos.x = 0;
    if (cursor_pos.x >= MAP_W)
        cursor_pos.x = MAP_W-1;
    if (cursor_pos.y < 0)
        cursor_pos.y = 0;
    if (cursor_pos.y >= MAP_H)
        cursor_pos.y = MAP_H-1;
}

void MapEngine::setPlayer(Player *_player) {
    player = _player;
}

void MapEngine::playerStartTurn() {
    player->is_turn = true;
    setContextTiles();
}

bool MapEngine::playerAction() {
    if (player->pos.x == cursor_pos.x && player->pos.y == cursor_pos.y)
        return false;

    int dist = calcDistance(player->pos, cursor_pos);

    if (dist <= 1 && context_tiles[cursor_pos.y][cursor_pos.x] == 0) {
        player->actionMove(cursor_pos.x, cursor_pos.y);
        checkPowerup();
        if (levels[current_level][cursor_pos.y][cursor_pos.x] == 2) {
            prevLevel();
            return false;
        }
        else if (levels[current_level][cursor_pos.y][cursor_pos.x] == 3) {
            nextLevel();
            return false;
        }
        return true;
    }
    else if (dist <= 1 && context_tiles[cursor_pos.y][cursor_pos.x] == 1) {
        Enemy* e = getEnemy(cursor_pos.x, cursor_pos.y);
        if (e) {
            player->actionAttack(e);
            if (e->hp == 0)
                removeEnemy(e);
        }
        return true;
    }

    return false;
}

void MapEngine::spawnEnemies() {
    enemies.resize(levels.size());

    // int spawn_count = 4;
    int spawn_count = (rand() % (current_level+1)) + 1;

    int fail_count = 3; // prevent infinite loop
    while (spawn_count > 0) {
        Point spawn_pos;
        spawn_pos.x = (rand() % (MAP_W/2)) + MAP_W/2 - 1;
        spawn_pos.y = (rand() % (MAP_H-2)) + 1;

        if (isWalkable(spawn_pos.x, spawn_pos.y)) {
            Enemy* e = new Enemy(ENEMY_SLIME);
            e->setPos(spawn_pos.x, spawn_pos.y);
            e->setPlayer(player);
            enemies[current_level].push_back(e);
        }
        else
            fail_count--;

        if (fail_count == 0) {
            break;
        }

        spawn_count--;
    }
}

void MapEngine::spawnPowerups() {
    powerups.resize(levels.size());

    int spawn_count = 4;
    // int spawn_count = (rand() % (current_level+1));

    int fail_count = 3; // prevent infinite loop
    while (spawn_count > 0) {
        Point spawn_pos;
        spawn_pos.x = (rand() % (MAP_W-2)) + 1;
        spawn_pos.y = (rand() % (MAP_H-2)) + 1;

        int type = rand() % 3;

        if (isWalkable(spawn_pos.x, spawn_pos.y) && !isPowerup(spawn_pos.x, spawn_pos.y)) {
            Powerup* p = new Powerup(type);
            p->setPos(spawn_pos.x, spawn_pos.y);
            powerups[current_level].push_back(p);
        }
        else
            fail_count--;

        if (fail_count == 0) {
            break;
        }

        spawn_count--;
    }
}

Enemy* MapEngine::getEnemy(int x, int y) {
    for (unsigned i=0; i<enemies[current_level].size(); i++) {
        Point enemy_pos = enemies[current_level][i]->pos;
        if (x == enemy_pos.x && y == enemy_pos.y)
            return enemies[current_level][i];
    }
    return NULL;
}

void MapEngine::removeEnemy(Enemy* e) {
    if (!e) return;

    for (unsigned i=0; i<enemies[current_level].size(); i++) {
        if (e == enemies[current_level][i]) {
            delete enemies[current_level][i];
            enemies[current_level].erase(enemies[current_level].begin()+i);
            break;
        }
    }

    setContextTiles();
}

void MapEngine::enemyStartTurn() {
    if (!enemies[current_level].empty()) {
        active_enemy = 0;
        enemies[current_level][active_enemy]->startTurn();
    }
}

bool MapEngine::enemyAction() {
    if (active_enemy == -1)
        return true;

    Enemy* e = enemies[current_level][active_enemy];
    if (e->is_turn) {
        e->logic();

        if (e->move_to_player) {
            Point dest = e->pos;
            int chase_roll = rand() % 3;
            if (chase_roll == 0) {
                if (e->pos.x > player->pos.x)
                    dest.x--;
                else if (e->pos.x < player->pos.x)
                    dest.x++;
                if (e->pos.y > player->pos.y)
                    dest.y--;
                else if (e->pos.y < player->pos.y)
                    dest.y++;
            }
            else {
                dest.x += ((rand() % 3) - 1);
                dest.y += ((rand() % 3) - 1);
            }
            if (isWalkable(dest.x, dest.y))
                e->setPos(dest.x, dest.y);
            e->move_to_player = false;
        }
    }

    // turn can end during logic()
    if (!e->is_turn) {
        if ((unsigned)active_enemy+1 < enemies[current_level].size()) {
            active_enemy++;
            enemies[current_level][active_enemy]->startTurn();
            return false;
        }
        else {
            active_enemy = -1;
            return true;
        }
    }

    return false;
}

void MapEngine::checkPowerup() {
    for (unsigned i=0; i<powerups[current_level].size(); i++) {
        Powerup* p = powerups[current_level][i];

        if (p->pos.x == player->pos.x && p->pos.y == player->pos.y) {
            if (p->type == POWERUP_ATTACK)
                player->attack += p->amount;
            if (p->type == POWERUP_DEFENSE)
                player->defense += p->amount;
            if (p->type == POWERUP_POTION)
                player->potions += p->amount;

            // make sure we don't dip below the minimum values when pickup up bad powerups
            if (player->attack < PLAYER_MIN_ATTACK)
                player->attack = PLAYER_MIN_ATTACK;
            if (player->defense < PLAYER_MIN_DEFENSE)
                player->defense = PLAYER_MIN_DEFENSE;

            delete powerups[current_level][i];
            powerups[current_level].erase(powerups[current_level].begin()+i);
            return;
        }
    }
}
