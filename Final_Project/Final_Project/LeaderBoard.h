#ifndef LEADERBOARD_H
#define LEADERBOARD_H

#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include "Player.h"
#include "MinHeap.h"

using namespace std;

void leaderBoard() {
    ifstream fin("Data.csv");

    string line;
    getline(fin, line);

    // Reading 10 players from Data.csv
    const int maxPlayers = 10;
    Player players[maxPlayers];
    int count = 0;

    while (count < maxPlayers && getline(fin, line)) {
        stringstream ss(line);
        string username, password, scoreStr;

        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, scoreStr);

        int score = stoi(scoreStr);

        players[count].setUsername(username);
        players[count].setPassword(password);
        players[count].setScore(score);

        count++;
    }

    fin.close();

    // Creating a Min Heap and inserting the data of 10 players in it.
    MinHeap heap;
    for (int i = 0; i < 10; i++) {
        heap.insertIntoMinHeap(players[i]);
    }

    // Deleteing every element from min heap in order to get data in ascending order.
    for (int i = 0; i < 10; i++) {
        heap.deleteFromMinHeap();
    }

    // Writing Data in LeaderBoard.csv
    ofstream fout("LeaderBoard.csv");
    if (!fout.is_open()) {
        cerr << "Error opening LeaderBoard.csv!" << endl;
        return;
    }

    fout << "Username,Score\n";

    Player* heapArr = heap.getHeapArray();

    for (int i = 1; i <= 10; i++) {
        fout << heapArr[i].getUsername() << ","
            << heapArr[i].getScore() << "\n";
    }
    fout.close();
}

#endif