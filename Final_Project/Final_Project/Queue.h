#ifndef QUEUE_H
#define QUEUE_H
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Player.h"

using namespace std;

struct Node {
    Player obj;  
    Node* next;
};

class Queue {
private:
    Node* front;
    Node* rear;

public:
    Queue() {
        front = rear = nullptr;
    }


    void enqueueFromFile() {
        ifstream leaderboardFile("LeaderBoard.csv");
        ofstream matchmakingFile("matchmaking.txt", ios::trunc);

        string line, username, scoreStr;
        getline(leaderboardFile, line);

        while (getline(leaderboardFile, line)) {
            stringstream ss(line);
            getline(ss, username, ',');
            getline(ss, scoreStr);

            if (username.empty() || scoreStr.empty()) {
                continue;
            }

            int score = stoi(scoreStr);
            Player p(username, "", score);
            enqueue(p);
            matchmakingFile << username << "," << score << endl;
        }

        leaderboardFile.close();
        matchmakingFile.close();
    }



    void enqueue( Player& ob) {
        Node* newNode = new Node;
        newNode->obj = ob;
        newNode->next = nullptr;

        if (rear == nullptr) {
            front = rear = newNode;
        }
        else {
            rear->next = newNode;
            rear = newNode;
        }
    }

    void dequeue() {
        if (front == nullptr) {
            return;
        }

        Node* temp = front;
        Player dequeuedPlayer = front->obj;

        cout << "Dequeued: " << dequeuedPlayer.getUsername() << endl;

        if (front == rear) {
            front = rear = nullptr;
        }
        else {
            front = front->next;
        }

        delete temp;

        enqueue(dequeuedPlayer);
        cout << "Re-enqueued at rear: " << dequeuedPlayer.getUsername() << endl;
        ofstream matchmakingFile("matchmaking.txt", ios::trunc);
        if (!matchmakingFile) {
            return;
        }

        Node* current = front;
        while (current != nullptr) {
            matchmakingFile << current->obj.getUsername() << "," << current->obj.getScore() << endl;
            current = current->next;
        }

        matchmakingFile.close();
    }




    Player peek() {
        if (front == nullptr) {
            return Player();
        }
        return front->obj;
    }
    ~Queue() {
        while (front != nullptr) {
            Node* temp = front;
            front = front->next;
            delete temp;
        }
        rear = nullptr;
    }
};

#endif
