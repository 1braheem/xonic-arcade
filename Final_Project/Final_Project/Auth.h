#ifndef AUTH_H
#define AUTH_H

#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Player.h"

using namespace std;

class Auth {
public:
    // Function to regster a new player
    bool registerPlayer(const string& username, const string& password) {
        if (isUsernameTaken(username)) {
            return false;
        }
        if (!isPasswordStrong(password)) {
            return false;
        }

        ofstream file("Data.csv", ios::app);
        file << username << "," << password << ",0,,,,,\n";
        file.close();

        ofstream fl(username + "friendlist.txt");
        ofstream rr(username + "receivedrequest.txt");

        fl.close();
        rr.close();

        return true;
    }

    // Function to login a player if he has registered in past
    string trim(const string& str) {
        size_t first = str.find_first_not_of(" \t\r\n");
        if (first == string::npos)
            return "";
        size_t last = str.find_last_not_of(" \t\r\n");
        return str.substr(first, last - first + 1);
    }

    bool loginPlayer(const string& username, const string& password) {
        ifstream file("Data.csv");
        string line, user, pass;

        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, user, ',');
            getline(ss, pass, ',');

            user = trim(user);
            pass = trim(pass);

            if (user == username && pass == password) {
                return true;
            }
        }
        return false;
    }

    // Function to search a player in Data file
    bool search_player(const string& username) {
        ifstream file("Data.csv");

        string line, user, pass;
        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, user, ',');
            getline(ss, pass, ',');

            if (user == username) {
                return true;
            }
        }
        return false;
    }

    // Function to update the score of player in file every time he plays the game
    void updateScore(const string& username, int scoreToAdd) {
        ifstream inFile("Data.csv");
        ofstream outFile("Temp.csv");
        string line;
        bool userFound = false;

        while (getline(inFile, line)) {
            stringstream ss(line);
            string uname, pass, scoreStr;
            string history[5] = { "", "", "", "", "" };

            // Read exactly what we expect
            if (!getline(ss, uname, ',')) continue;
            if (!getline(ss, pass, ',')) continue;
            if (!getline(ss, scoreStr, ',')) continue;

            // Read history if exists
            for (int i = 0; i < 5 && getline(ss, history[i], ','); i++);

            if (uname == username) {
                int prevScore = stoi(scoreStr);
                int newScore = prevScore + scoreToAdd;
                outFile << uname << "," << pass << "," << newScore;
                for (int i = 0; i < 5; i++) {
                    outFile << "," << history[i];
                }
                outFile << endl;
                userFound = true;
            }
            else {
                outFile << line << endl;
            }
        }

        inFile.close();
        outFile.close();
        remove("Data.csv");
        rename("Temp.csv", "Data.csv");
    }

    // Function to sort the Data in Data.csv in decending order on the basis of their scores
    void sortData() {
        ifstream fin("Data.csv");
        string line;

        // Skip the header 
        getline(fin, line);

        int count = 0;
        while (getline(fin, line))
            count++;

        fin.clear();
        fin.seekg(0);
        getline(fin, line);

        Player* players = new Player[count];

        for (int i = 0; i < count && getline(fin, line); ++i) {
            stringstream ss(line);
            string username, password, scoreStr;
            string history[5] = { "", "", "", "", "" };

            getline(ss, username, ',');
            getline(ss, password, ',');
            getline(ss, scoreStr, ',');
            for (int j = 0; j < 5 && getline(ss, history[j], ','); j++);

            int score = stoi(scoreStr);
            players[i].setUsername(username);
            players[i].setPassword(password);
            players[i].setScore(score);
            players[i].setHistory(history);
        }

        fin.close();

        // Sort players by score
        for (int i = 0; i < count - 1; ++i) {
            for (int j = i + 1; j < count; ++j) {
                if (players[i].getScore() < players[j].getScore()) {
                    Player temp = players[i];
                    players[i] = players[j];
                    players[j] = temp;
                }
            }
        }

        ofstream fout("Data.csv");
        if (!fout.is_open()) {
            cerr << "Error writing to file!" << endl;
            delete[] players;
            return;
        }

        fout << "Username,Password,Score,History1,History2,History3,History4,History5\n";
        for (int i = 0; i < count; ++i) {
            fout << players[i].getUsername() << ","
                << players[i].getPassword() << ","
                << players[i].getScore();
            const string* hist = players[i].getHistory();
            for (int j = 0; j < 5; j++) {
                fout << "," << hist[j];
            }
            fout << "\n";
        }

        fout.close();
        delete[] players;
    }

    // Function to store data for last five games
    void updateGameHistory(const string& username, const string result) {
        ifstream fin("Data.csv");
        ofstream fout("temp.csv");

        string line;
        while (getline(fin, line)) {
            stringstream ss(line);
            string uname, pass, score;

            getline(ss, uname, ',');
            getline(ss, pass, ',');
            getline(ss, score, ',');

            string history[5] = { "", "", "", "", "" };
            for (int i = 0; i < 5 && ss.good(); ++i)
                getline(ss, history[i], ',');

            if (uname == username) {
                for (int i = 4; i > 0; --i) {
                    history[i] = history[i - 1];
                }
                history[0] = result;
            }

            fout << uname << "," << pass << "," << score;
            for (int i = 0; i < 5; ++i) {
                fout << "," << history[i];
            }
            fout << "\n";
        }

        fin.close();
        fout.close();

        remove("Data.csv");
        rename("temp.csv", "Data.csv");
    }

private:
    // Function to check if the username is already taken by a player or not
    bool isUsernameTaken(const string& username) {
        ifstream file("Data.csv");
        string line, user, pass;

        while (getline(file, line)) {
            stringstream ss(line);
            getline(ss, user, ',');
            if (user == username) {
                return true;
            }
        }
        return false;
    }

    // Function that checks if the length of password is greater than or equal to 6 or not
    bool isPasswordStrong(const string& password) {
        return password.length() >= 6;
    }
};

#endif