#ifndef SAVELOAD_H
#define SAVELOAD_H

#include <SFML/Graphics.hpp>
#include "Enemy.h"
#include "ScoreManager.h"
#include "GridManager.h"

using namespace std;

void saveGame(const string& username, int grid[M][N], int playerX, int playerY, Enemy enemy[], int enemyCount, ScoreManager score) {
    ofstream outFile(username + ".txt");
    cout << "Saving game for user: " << username << endl;
    cout << "Saving grid:" << endl;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            outFile << grid[i][j] << ' ';
            cout << grid[i][j] << ' ';
        }
        outFile << '\n';
        cout << '\n';
    }
    outFile << playerX << ' ' << playerY << '\n';
    cout << "Player position saved: X=" << playerX << ", Y=" << playerY << endl;

    outFile << enemyCount << '\n';
    cout << "Number of enemies: " << enemyCount << endl;

    for (int i = 0; i < enemyCount; ++i) {
        outFile << enemy[i].x << ' ' << enemy[i].y << '\n';
        cout << "Enemy " << i + 1 << " position: X=" << enemy[i].x << ", Y=" << enemy[i].y << endl;
    }

    int finalScore = score.getScore();
    outFile << finalScore << '\n';
    cout << "Score saved: " << finalScore << endl;

    outFile.close();
    cout << "Game saved successfully to file: " << username << ".txt" << endl;
}

void loadGame(string username, int grid[M][N], int playerx, int playery, Enemy enemy[], int enemyCount, ScoreManager& score, Sprite sTile, Sprite sEnemy) {
    ifstream inFile(username + ".txt");
    if (!inFile) {
        cerr << "No saved game found for user: " << username << endl;
        enemyCount = 0;
        return;
    }

    cout << "Loading game for user: " << username << endl;
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            inFile >> grid[i][j];
        }
    }

    inFile >> playerx >> playery;
    cout << "Player position loaded: (" << playerx << ", " << playery << ")" << endl;

    inFile >> enemyCount;
    cout << "Enemy count loaded: " << enemyCount << endl;

    for (int i = 0; i < enemyCount; ++i) {
        inFile >> enemy[i].x >> enemy[i].y;
        cout << "Enemy " << i + 1 << " position loaded: (" << enemy[i].x << ", " << enemy[i].y << ")" << endl;
    }

    int loadedScore;
    inFile >> loadedScore;
    score.setScore(loadedScore);
    cout << "Score loaded: " << loadedScore << endl;

    inFile.close();

    for (int i = 0; i < M; i++) {
        for (int j = 0; j < N; j++) {
            if (grid[i][j] == 1) {
                sTile.setTextureRect(IntRect(0, 0, ts, ts));
            }
            if (grid[i][j] == 2) {
                sTile.setTextureRect(IntRect(54, 0, ts, ts));
            }
            sTile.setPosition(j * ts, i * ts);
        }
    }

    sTile.setTextureRect(IntRect(36, 0, ts, ts));
    sTile.setPosition(playerx * ts, playery * ts);
    sEnemy.setOrigin(20, 20);
    for (int i = 0; i < enemyCount; ++i) {
        sEnemy.setPosition(enemy[i].x, enemy[i].y);
    }
}

#endif