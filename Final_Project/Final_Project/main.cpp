#include <iostream>
#include <fstream>
#include <sstream>
#include "Game.h"
#include "Player.h"
#include <string>
#include <vector>
#include"friendstuff.h"//for debugging
#include"Queue.h"

using namespace std;

int main() {
    //debugging
 //   Hashtable obj;
   // obj.create_keys();
    //obj.buildFriendChains();
    //obj.printFriendChains();
   // Queue o;
   // o.enqueueFromFile();
    ifstream fin("Data.csv");
    if (!fin.is_open()) {
        cerr << "Error opening file!" << endl;
        return 1;
    }

    string line;

    // Skip the header
    getline(fin, line); // discard first line (Username,Password)

    vector<Player> players;

    // Read player data
    while (getline(fin, line)) {
        if (line.empty()) {
            continue;
        }

        stringstream ss(line);
        string username, password, scoreStr;
        getline(ss, username, ',');
        getline(ss, password, ',');
        getline(ss, scoreStr);

        if (username.empty() || password.empty() || scoreStr.empty()) {
            continue;
        }

        Player player;
        player.setUsername(username);
        player.setPassword(password);
        player.setScore(stoi(scoreStr));
        players.push_back(player);
    }


    fin.close();

    runGame();

    return 0;
}
