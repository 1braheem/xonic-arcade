#ifndef GRIDMANAGER_H
#define GRIDMANAGER_H

const int M = 25;
const int N = 40;
int grid[M][N] = { 0 };

// Function that fills inside the loop when player closes a loop
void drop(int y, int x) {
    if (grid[y][x] == 0) grid[y][x] = -1;
    if (grid[y - 1][x] == 0) drop(y - 1, x);
    if (grid[y + 1][x] == 0) drop(y + 1, x);
    if (grid[y][x - 1] == 0) drop(y, x - 1);
    if (grid[y][x + 1] == 0) drop(y, x + 1);
}

// Function that keeps everything in bound of grid
int clamp(int value, int min, int max) {
    if (value < min) return min;
    if (value > max) return max;
    return value;
}

#endif