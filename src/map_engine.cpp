/*
Copyright © 2014-2015 Justin Jacobs

This file is part of Noman's Dungeon.

Noman's Dungeon is free software: you can redistribute it and/or modify it under the terms
of the GNU General Public License as published by the Free Software Foundation,
either version 3 of the License, or (at your option) any later version.

Noman's Dungeon is distributed in the hope that it will be useful, but WITHOUT ANY
WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR A
PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with
Noman's Dungeon.  If not, see http://www.gnu.org/licenses/
*/


#include "map_engine.h"
#include "engines.h"

MapEngine::MapEngine() {
    tileset.load(render_engine, "data/tileset.png");
    cursor.load(render_engine, "data/cursor.png");
    fog.load(render_engine, "data/fog.png");

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
                    setFogOfWar();
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
        enemies.resize(levels.size());
        powerups.resize(levels.size());
        spawnTreasure(stairs_pos);
        spawnEnemies();
        spawnPowerups();
    }

    cursor_pos = player->pos;
    setContextTiles();
    setFogOfWar();
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

    // render fog of war
    for (int i=0; i<MAP_H; i++) {
        for (int j=0; j<MAP_W; j++) {
            fog.setAlpha(fog_tiles[i][j]);
            fog.setPos(j*TILE_SIZE, i*TILE_SIZE);
            fog.render();
        }
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
            if (dist <= 1 && isEnemy(j, i)) {
                context_tiles[i][j] = CONTEXT_ENEMY;
            }
            else if (dist <= player->ap && isWalkable(j, i)) {
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
    Point old_pos = cursor_pos;

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

    // can't select fully fogged tiles
    if (fog_tiles[cursor_pos.y][cursor_pos.x] == 255)
        cursor_pos = old_pos;

    // no movement
    if (cursor_pos.x == old_pos.x && cursor_pos.y == old_pos.y)
        return;

    sfx_cursor.play();
}

void MapEngine::playerStartTurn() {
    if (player->isAnimating())
        return;

    player->startTurn();
    setContextTiles();
    setFogOfWar();
    cursor_pos = player->pos;
    if (!first_turn)
        msg.setText("Player turn");
}

bool MapEngine::playerAction() {
    if (first_turn) {
        first_turn = false;
    }

    // TODO guard when targeting self?
    // for now, just end the turn
    if (player->pos.x == cursor_pos.x && player->pos.y == cursor_pos.y) {
        player->ap = 0;
        return true;
    }

    if (context_tiles[cursor_pos.y][cursor_pos.x] == CONTEXT_WALKABLE) {
        player->actionMove(cursor_pos.x, cursor_pos.y);
        player->sfx_move.play();
        setContextTiles();
        setFogOfWar();
        checkPowerup();
        if (levels[current_level][cursor_pos.y][cursor_pos.x] == TILE_STAIRS_UP) {
            prevLevel();
            player->ap = player->maxap;
        }
        else if (levels[current_level][cursor_pos.y][cursor_pos.x] == TILE_STAIRS_DOWN) {
            nextLevel();
            player->ap = player->maxap;
        }
    }
    else if (context_tiles[cursor_pos.y][cursor_pos.x] == CONTEXT_ENEMY) {
        player->actionAttack(getEnemy(cursor_pos.x, cursor_pos.y));
    }

    if (enemies[current_level].empty())
        player->ap = player->maxap;

    return player->ap == 0;
}

bool MapEngine::playerActionClick() {
    Point click_pos;
    click_pos.x = input_engine->mouse.x / TILE_SIZE;
    click_pos.y = input_engine->mouse.y / TILE_SIZE;

    if (click_pos.x >= 0 && click_pos.x < MAP_W && click_pos.y >= 0 && click_pos.y < MAP_H && fog_tiles[click_pos.y][click_pos.x] != 255) {
        cursor_pos = click_pos;
        if (playerAction())
            return true;
        else {
            sfx_cursor.play();
            return false;
        }
    }

    return false;
}

void MapEngine::spawnEnemies() {
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
    int type = POWERUP_TREASURE;

    int fail_count = 10;
    while (fail_count > 0) {
        if (isWalkable(spawn_pos.x, spawn_pos.y) && !isPowerup(spawn_pos.x, spawn_pos.y)) {
            Powerup* p = new Powerup(type);
            p->setPos(spawn_pos.x, spawn_pos.y);
            powerups[current_level].push_back(p);
            break; // success
        }
        else {
            logError("Could not place the treasure on the designated tile. Trying random tile...\n");
            spawn_pos.x = (rand() % (MAP_W-2)) + 1;
            spawn_pos.y = (rand() % (MAP_H-2)) + 1;
            fail_count--;
        }
    }
}

Enemy* MapEngine::getEnemy(int x, int y) {
    for (unsigned i=0; i<enemies[current_level].size(); i++) {
        const Point& enemy_pos = enemies[current_level][i]->pos;
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

        if (e->isAnimating())
            return false;

        if (e->hp == 0) {
            if (!e->isAnimating()) {
                removeEnemy(e);
                if (enemies[current_level].empty())
                    active_enemy = -1;
                else {
                    active_enemy = 0;
                    enemies[current_level][active_enemy]->startTurn();
                }
            }
            return false;
        }
    }

    Enemy* e = enemies[current_level][active_enemy];
    if (!e) {
        // enemies aren't supposed to be NULL here, bailing out
        active_enemy = -1;
        return true;
    }

    if (e->is_turn)
        cursor_pos = e->pos;

    e->logic(this);

    // turn can end during logic()
    if (!e->is_turn) {
        if ((unsigned)(active_enemy+1) < enemies[current_level].size()) {
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

void MapEngine::setFogOfWar() {
    for (int i=0; i<MAP_H; i++) {
        for (int j=0; j<MAP_W; j++) {
            fog_tiles[i][j] = 255;
            float dist = calcDistance(player->pos, FPoint(j, i));

            if (dist < player->view_distance) {
                fog_tiles[i][j] = dist * (256/player->view_distance);
            }
        }
    }
}

void MapEngine::checkWinLoss() {
    // win
    if (player->has_treasure && current_level == 0 && levels[current_level][player->pos.y][player->pos.x] == TILE_STAIRS_UP) {
        game_state = GAME_WIN;
        msg.setText("You win! R to play again.");
        sfx_win.play();
    }
    // loss
    else if (player->hp == 0) {
        game_state = GAME_LOSE;
        msg.setText("You lose. R to retry.");
    }
}

Point MapEngine::findPath(const Point& start, const Point& end) {
    int nodes[MAP_H][MAP_W];
    std::vector<Point> adj;

    for (int i=0; i<MAP_H; i++) {
        for (int j=0; j<MAP_W; j++) {
            nodes[i][j] = -1;
        }
    }

    int count = 0;
    int adj_found = 0;
    nodes[start.y][start.x] = count;
    bool finished = false;

    while (!finished) {
        adj_found = 0;
        for (int i=0; i<MAP_H; i++) {
            for (int j=0; j<MAP_W; j++) {
                if (nodes[i][j] == count) {
                    adj.clear();
                    adj.push_back(Point(j-1, i));
                    adj.push_back(Point(j, i-1));
                    adj.push_back(Point(j+1, i));
                    adj.push_back(Point(j, i+1));

                    for (unsigned k=0; k<adj.size(); k++) {
                        Point& p = adj[k];

                        if (p.x == end.x && p.y == end.y) {
                            finished = true;
                            break;
                        }

                        if (!isWalkable(p.x, p.y))
                            continue;

                        if (nodes[p.y][p.x] == -1 || nodes[p.y][p.x] > count+1) {
                            nodes[p.y][p.x] = count+1;
                            adj_found++;
                        }
                    }
                }

                // MAP_W
                if (finished)
                    break;
            }

            // MAP_H
            if (finished)
                break;
        }

        if (adj_found == 0) {
            // could not find full path
            finished = true;
        }

        if (!finished)
            count++;
    }

    Point current = end;
    Point next = current;
    while (count > 0) {
        int lowest = count+1;
        int node = -1;

        for (int i=-1; i<=1; i++) {
            if (i == 0) continue;

            node = nodes[current.y+i][current.x];
            if (node != -1 && node < lowest) {
                next.y = current.y+i;
                lowest = node;
            }
            node = nodes[current.y][current.x+i];
            if (node != -1 && node < lowest) {
                next.x = current.x+i;
                lowest = node;
            }
        }

        // broken path
        if (current.x == next.x && current.y == next.y)
            return start;

        current = next;
        count--;
    }

    return current;
}
