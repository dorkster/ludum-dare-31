#pragma once

#include "common.h"
#include "render_engine.h"
#include "utils.h"
#include "player.h"
#include "enemy.h"
#include "powerup.h"

const int MAP_W = 14;
const int MAP_H = 8;

const int CUR_UP = 0;
const int CUR_DOWN = 1;
const int CUR_LEFT = 2;
const int CUR_RIGHT = 3;

class MapEngine {
public:
    MapEngine();
    ~MapEngine();
    void prevLevel();
    void nextLevel();
    void render();
    void moveCursor(int direction);
    void setPlayer(Player* _player);
    void playerStartTurn();
    bool playerAction();
    void enemyStartTurn();
    bool enemyAction();

private:
    void setContextTiles();
    bool isWalkable(int x, int y);
    bool isEnemy(int x, int y);
    bool isPowerup(int x, int y);
    void spawnEnemies();
    void spawnPowerups();
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
};
