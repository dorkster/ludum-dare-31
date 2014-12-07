#include "map_engine.h"
#include "engines.h"

MapEngine::MapEngine() {
    tileset.load(render_engine, "data/tileset.png");
    cursor.load(render_engine, "data/cursor.png");

    hud.load(render_engine, "data/hud.png");

    sound_engine->loadSound(&sfx_cursor, "data/cursor.wav");
    sound_engine->loadSound(&sfx_powerup, "data/powerup.wav");
    sound_engine->loadSound(&sfx_treasure, "data/treasure.wav");
    sound_engine->loadSound(&sfx_win, "data/victory.wav");
}

MapEngine::~MapEngine() {
    clear();
}

void MapEngine::init(Player* _player) {
    clear();

    player = _player;
    player->init();

    active_enemy = -1;
    dungeon_depth = 5;
    current_level = 0;

    msg.setText("WASD = cursor. Enter = action.");
    msg.setPos(2,0);
    first_turn = true;

    game_state = GAME_PLAY;

    nextLevel();
    playerStartTurn();
}

void MapEngine::logic() {
}

void MapEngine::clear() {
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

    enemies.clear();
    powerups.clear();
    levels.clear();
}

void MapEngine::prevLevel() {
    if ((int)current_level-1 >= 0) {
        current_level--;
        for (int i=0; i<MAP_H; i++) {
            for (int j=0; j<MAP_W; j++) {
                if (levels[current_level][i][j] == TILE_STAIRS_DOWN) {
                    player->setPos(j, i);
                    cursor_pos = player->pos;
                    if (player->has_treasure) {
                        spawnEnemies();
                        spawnPowerups();
                        levels[current_level][i][j] = TILE_STAIRS_BLOCKED;
                    }
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
                if (levels[current_level][i][j] == TILE_STAIRS_UP) {
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
                room[i][j] = TILE_FLOOR;

                // borders
                if (i == 0 && j == 0)
                    room[i][j] = TILE_WALL_TL;
                else if (i == MAP_H-1 && j == 0)
                    room[i][j] = TILE_WALL_BL;
                else if (i == 0 && j == MAP_W-1)
                    room[i][j] = TILE_WALL_TR;
                else if (i == MAP_H-1 && j == MAP_W-1)
                    room[i][j] = TILE_WALL_BR;
                else if (i == 0)
                    room[i][j] = TILE_WALL_T;
                else if (i == MAP_H-1)
                    room[i][j] = TILE_WALL_B;
                else if (j == 0)
                    room[i][j] = TILE_WALL_L;
                else if (j == MAP_W-1)
                    room[i][j] = TILE_WALL_R;
                else {
                    if (rand() % 4 == 0)
                        room[i][j] = (rand()%2) + TILE_ROCK;
                }

                // up stairs
                if (i == player_pos.y && j == player_pos.x)
                    room[i][j] = TILE_STAIRS_UP;

                if (dungeon_depth > levels.size()+1) {
                    // down stairs
                    if (i == stairs_pos.y && j == stairs_pos.x)
                        room[i][j] = TILE_STAIRS_DOWN;
                }
            }
        }

        // clear a path through rocks
        Point p1 = player_pos;
        Point p2 = stairs_pos;
        while (!(p1.x == p2.x && p1.y == p2.y)) {
            if (p1.x < MAP_W-2) {
                if (rand()%2 == 0)
                    p1.x++;
                else {
                    if (p1.y > 1 && rand()%2 == 0)
                        p1.y--;
                    else if (p1.y < MAP_H-2 && rand()%2 == 0)
                        p1.y++;
                }
            }
            else {
                if (p1.y > 1 && rand()%2 == 0)
                    p1.y--;
                else if (p1.y < MAP_H-2 && rand()%2 == 0)
                    p1.y++;
            }
            if (room[p1.y][p1.x] != TILE_STAIRS_UP && room[p1.y][p1.x] != TILE_STAIRS_DOWN)
                room[p1.y][p1.x] = TILE_FLOOR;
        }

        levels.push_back(room);
        current_level = levels.size()-1;
        spawnEnemies();
        spawnPowerups();
        spawnTreasure(stairs_pos);
    }

    cursor_pos = player->pos;
    setContextTiles();
}

void MapEngine::render() {
    for (int i=0; i<MAP_H; i++) {
        for (int j=0; j<MAP_W; j++) {
            int tile = levels[current_level][i][j];
            tileset.setClip(tile*TILE_SIZE, 0, TILE_SIZE, TILE_SIZE);
            tileset.setPos(j*TILE_SIZE, i*TILE_SIZE);
            tileset.render();
        }
    }

    // render context tiles
    if (player && player->is_turn) {
        for (int i=0; i<MAP_H; i++) {
            for (int j=0; j<MAP_W; j++) {
                int tile = context_tiles[i][j];
                tileset.setClip(tile*TILE_SIZE, TILE_SIZE, TILE_SIZE, TILE_SIZE);
                tileset.setPos(j*TILE_SIZE, i*TILE_SIZE);
                tileset.render();
            }
        }
    }

    // render cursor
    cursor.setPos(cursor_pos.x*TILE_SIZE, cursor_pos.y*TILE_SIZE);
    cursor.render();

    if (player) {
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

    // render hud overlay
    hud.render();

    // render status text
    msg.render();
}

void MapEngine::setContextTiles() {
    for (int i=0; i<MAP_H; i++) {
        for (int j=0; j<MAP_W; j++) {
            context_tiles[i][j] = -1;
            float dist = calcDistance(player->pos, FPoint(j, i));

            // casting dist to int here rounds it so that we can attack our corners
            if ((int)dist <= 1 && isEnemy(j, i)) {
                context_tiles[i][j] = CONTEXT_ENEMY;
            }
            else if (dist <= 1 && isWalkable(j, i)) {
                context_tiles[i][j] = CONTEXT_WALKABLE;
            }
        }
    }
}

bool MapEngine::isWalkable(int x, int y) {
    if (x < 1 || x > MAP_W-1) return false;
    if (y < 1 || y > MAP_H-1) return false;

    int tile = levels[current_level][y][x];

    if (tile != TILE_FLOOR && tile != TILE_STAIRS_UP && tile != TILE_STAIRS_DOWN && tile != TILE_STAIRS_BLOCKED)
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

    sfx_cursor.play();
}

void MapEngine::playerStartTurn() {
    if (player->isAnimating())
        return;

    player->startTurn();
    setContextTiles();
    cursor_pos = player->pos;
    if (!first_turn)
        msg.setText("Player turn");
}

bool MapEngine::playerAction() {
    if (first_turn) {
        first_turn = false;
    }

    if (player->pos.x == cursor_pos.x && player->pos.y == cursor_pos.y)
        return false;

    int dist = calcDistance(player->pos, cursor_pos);

    if (dist <= 1 && context_tiles[cursor_pos.y][cursor_pos.x] == CONTEXT_WALKABLE) {
        player->actionMove(cursor_pos.x, cursor_pos.y);
        player->sfx_move.play();
        checkPowerup();
        if (levels[current_level][cursor_pos.y][cursor_pos.x] == TILE_STAIRS_UP) {
            if (player->has_treasure && current_level == 0) {
                game_state = GAME_WIN;
                msg.setText("You win! R to play again.");
                sfx_win.play();
            }
            else {
                prevLevel();
            }
            return false;
        }
        else if (levels[current_level][cursor_pos.y][cursor_pos.x] == TILE_STAIRS_DOWN) {
            nextLevel();
            return false;
        }
        return true;
    }
    else if (dist <= 1 && context_tiles[cursor_pos.y][cursor_pos.x] == CONTEXT_ENEMY) {
        Enemy* e = getEnemy(cursor_pos.x, cursor_pos.y);
        if (e) {
            player->actionAttack(e);
        }
        return true;
    }

    return false;
}

void MapEngine::spawnEnemies() {
    enemies.resize(levels.size());

    int spawn_count = current_level+1;
    if (player->has_treasure)
        spawn_count = dungeon_depth + (dungeon_depth - spawn_count);

    spawn_count = (rand() % spawn_count) + 3;

    int fail_count = 10; // prevent infinite loop
    while (spawn_count > 0) {
        Point spawn_pos;
        spawn_pos.y = (rand() % (MAP_H-2)) + 1;

        if (player->has_treasure)
            spawn_pos.x = (rand() % (MAP_W/2)) + 1;
        else
            spawn_pos.x = (rand() % (MAP_W/2)) + MAP_W/2 - 1;

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
    if (powerups.size() < levels.size())
        powerups.resize(levels.size());

    int spawn_count = current_level+5;
    if (player->has_treasure)
        spawn_count = dungeon_depth + (dungeon_depth - spawn_count);
    spawn_count = (rand() % spawn_count) + 1;
    if (spawn_count > 3)
        spawn_count = 3;

    int fail_count = 10; // prevent infinite loop
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

void MapEngine::spawnTreasure(const Point& pos) {
    if (dungeon_depth > levels.size())
        return;

    if (powerups.size() < levels.size())
        powerups.resize(levels.size());

    Point spawn_pos = pos;
    int spawn_count = 1;
    int type = POWERUP_TREASURE;

    while (spawn_count > 0) {
        if (isWalkable(spawn_pos.x, spawn_pos.y) && !isPowerup(spawn_pos.x, spawn_pos.y)) {
            Powerup* p = new Powerup(type);
            p->setPos(spawn_pos.x, spawn_pos.y);
            powerups[current_level].push_back(p);
            spawn_count--;
        }
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
        player->is_turn = false;
        msg.setText("Enemy turn");
        active_enemy = 0;
        enemies[current_level][active_enemy]->startTurn();
    }
}

bool MapEngine::enemyAction() {
    if (active_enemy == -1)
        return true;

    // wait for any dying enemies to finish animating
    for (unsigned i=0; i<enemies[current_level].size(); i++) {
        Enemy* e = enemies[current_level][i];

        if (e->hp == 0) {
            if (!e->isAnimating()) {
                removeEnemy(e);
                if (enemies[current_level].empty())
                    active_enemy = -1;
            }
            return false;
        }
    }

    Enemy* e = enemies[current_level][active_enemy];
    if (e && e->is_turn)
        cursor_pos = e->pos;
    if (e && e->is_turn && !e->isAnimating()) {
        if (e->action_ticks > 0)
            e->action_ticks--;
        else {
            // move towards player
            if (!e->isNearPlayer(1)) {
                Point dest = e->pos;
                if (e->isNearPlayer(4)) {
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
                    int dx = 0;
                    int dy = 0;
                    int fail_count = 3;
                    while (dx == 0 && dy == 0 && fail_count > 0) {
                        dx = (rand() % 3) - 1;
                        dy = (rand() % 3) - 1;
                        fail_count--;
                    }
                    dest.x += dx;
                    dest.y += dy;
                }
                if (isWalkable(dest.x, dest.y)) {
                    e->setPos(dest.x, dest.y);
                    e->sfx_move.play();
                }
            }
            // attack
            else {
                e->actionAttack();
            }

            e->is_turn = false;
        }

        // check if the player is dead
        if (player->hp == 0) {
            game_state = GAME_LOSE;
            msg.setText("You lose. R to retry.");
            return false;
        }
    }

    // turn can end during logic()
    if (e && !e->is_turn) {
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
                player->bonusAttack(p->amount);
            if (p->type == POWERUP_DEFENSE)
                player->bonusDefense(p->amount);
            if (p->type == POWERUP_POTION)
                player->bonusHP(p->amount);
            if (p->type == POWERUP_TREASURE)
                player->bonusTreasure();

            // play sound
            if (p->type == POWERUP_TREASURE)
                sfx_treasure.play();
            else
                sfx_powerup.play();

            delete powerups[current_level][i];
            powerups[current_level].erase(powerups[current_level].begin()+i);
            return;
        }
    }
}

unsigned MapEngine::getCurrentLevel() {
    return current_level+1;
}
