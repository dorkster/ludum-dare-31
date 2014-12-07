#pragma once

#include "common.h"
#include "enemy.h"
#include "player.h"
#include "powerup.h"
#include "render_engine.h"
#include "sound_engine.h"
#include "text.h"
#include "utils.h"

const int MAP_W = 14;
const int MAP_H = 8;

const int CUR_UP = 0;
const int CUR_DOWN = 1;
const int CUR_LEFT = 2;
const int CUR_RIGHT = 3;

const int GAME_PLAY = 0;
const int GAME_WIN = 1;
const int GAME_LOSE = 2;

const int TILE_FLOOR = 0;
const int TILE_ROCK = 1;
const int TILE_STAIRS_UP = 2;
const int TILE_STAIRS_DOWN = 3;
const int TILE_STAIRS_BLOCKED = 4;
const int TILE_WALL_T = 5;
const int TILE_WALL_B = 6;
const int TILE_WALL_TL = 7;
const int TILE_WALL_TR = 8;
const int TILE_WALL_L = 9;
const int TILE_WALL_R = 10;
const int TILE_WALL_BL = 11;
const int TILE_WALL_BR = 12;

const int CONTEXT_WALKABLE = 0;
const int CONTEXT_ENEMY = 1;

class MapEngine {
public:
    MapEngine();
    ~MapEngine();
    void init(Player* _player);
    void logic();
    void clear();
    void prevLevel();
    void nextLevel();
    void render();
    void moveCursor(int direction);
    void playerStartTurn();
    bool playerAction();
    void enemyStartTurn();
    bool enemyAction();

    int game_state;
private:
    void setContextTiles();
    bool isWalkable(int x, int y);
    bool isEnemy(int x, int y);
    bool isPowerup(int x, int y);
    void spawnEnemies();
    void spawnPowerups();
    void spawnTreasure();
    Enemy* getEnemy(int x, int y);
    void removeEnemy(Enemy* e);
    void checkPowerup();

    unsigned current_level;
    Image tileset;
    std::vector<int**> levels;
    int context_tiles[MAP_H][MAP_W];

    Player* player;

    std::vector< std::vector<Enemy*> > enemies;
    int active_enemy;

    std::vector< std::vector<Powerup*> > powerups;

    Image cursor;
    Point cursor_pos;

    unsigned dungeon_depth;

    Text msg;
    bool first_turn; // displays help text when the player starts for the first time

    Image hud;

    Sound sfx_cursor;
    Sound sfx_powerup;
    Sound sfx_treasure;
    Sound sfx_win;
};
