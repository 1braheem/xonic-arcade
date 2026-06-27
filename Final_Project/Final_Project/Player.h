#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>

using namespace std;

class Player {
private:
    string username;
    string password;
    long long int score;
    string history[5];

public:
    // Default constructor to initilize everything with zero
    Player() {
        username = "";
        password = "";
        score = 0;
        for (int i = 0; i < 5; i++) {
            history[i] = "";
        }
    }

    // Paramiterized constructor to initiallize everything with deired values
    Player(string name, string code, long long int sc = 0, string arr[] = nullptr) {
        username = name;
        password = code;
        score = sc;
        for (int i = 0; i < 5; i++) {
            history[i] = (arr != nullptr) ? arr[i] : "";
        }
    }

    // Setter functions
    void setUsername(string name) {
        username = name;
    }

    void setPassword(string code) {
        password = code;
    }

    void setScore(long long int sc) {
        score = sc;
    }

    void setHistory(string arr[]) {
        for (int i = 0; i < 5; i++) {
            history[i] = arr[i];
        }
    }

    // getter functions
    long long int getScore() const {
        return score;
    }

    string getUsername() const {
        return username;
    }

    string getPassword() const {
        return password;
    }

    const string* getHistory() const {
        return history;
    }
};

#endif