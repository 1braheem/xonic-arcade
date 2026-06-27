#ifndef ENEMY_H
#define ENEMY_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <cstdlib>

using namespace std;
using namespace sf;

extern const int ts;
extern int grid[25][40];

class Enemy {
public:
    int x, y, dx, dy;
    int savedDx, savedDy;

    Enemy(){}
    // Constructor to initialize enemy position and movement
    Enemy(int difficulty) {
        x = y = 300;
        int baseSpeed = 2;
        if (difficulty == 2) baseSpeed = 6;
        else if (difficulty == 3) baseSpeed = 12;

        dx = baseSpeed - rand() % (2 * baseSpeed + 1);
        dy = baseSpeed - rand() % (2 * baseSpeed + 1);

        if (dx == 0) dx = 1;
        if (dy == 0) dy = 1;

        savedDx = dx;
        savedDy = dy;
    }

    // Move and handle wall collisions
    void move() {
        x += dx;
        int tileX = x / ts;
        int tileY = y / ts;

        if (grid[tileY][tileX] == 1 || grid[tileY][tileX] == 3 || grid[tileY][tileX] == 4 || grid[tileY][tileX] == 5) {
            dx = -dx;
            x += dx;
        }

        y += dy;

        tileX = x / ts;
        tileY = y / ts;

        if (grid[tileY][tileX] == 1 || grid[tileY][tileX] == 3 || grid[tileY][tileX] == 4 || grid[tileY][tileX] == 5) {
            dy = -dy;
            y += dy;
        }
    }
};

#endif