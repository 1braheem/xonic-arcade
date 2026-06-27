#ifndef FRIENDSTUFF_H
#define FRIENDSTUFF_H
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
using namespace std;
int size = 1;
struct Playerlist {
    Playerlist* next;
    string username;

};

class Hashtable {
    Playerlist* playerlist;
    int size;
    int capacity;
    string cleanName(string name) {
        while (!name.empty() && (name.back() == '\r' || name.back() == '\n' || name.back() == ' ' || name.back() == '\t')) {
            name.pop_back();
        }
        while (!name.empty() && (name.front() == '\r' || name.front() == '\n' || name.front() == ' ' || name.front() == '\t')) {
            name.erase(name.begin());
        }
        return name;
    }

    void clearChain(Playerlist* node) {
        while (node != nullptr) {
            Playerlist* next = node->next;
            delete node;
            node = next;
        }
    }

    bool fileContainsName(const string& filename, const string& name) {
        ifstream file(filename);
        string line;
        while (getline(file, line)) {
            if (cleanName(line) == name) {
                return true;
            }
        }
        return false;
    }

    bool chainContains(Playerlist* node, const string& name) {
        while (node != nullptr) {
            if (node->username == name) {
                return true;
            }
            node = node->next;
        }
        return false;
    }

    void appendUniqueName(const string& filename, const string& name) {
        if (name.empty() || fileContainsName(filename, name)) {
            return;
        }
        ofstream file(filename, ios::app);
        if (file.is_open()) {
            file << name << endl;
        }
    }

public:
    Hashtable() {
        capacity = 20;
        size = 0;
        playerlist = nullptr;
    }
    void create_keys() {
        ifstream fin("Data.csv");
        fin.is_open();



        string line;

        // Skip the header
        getline(fin, line); // discard first line (Username,Password,Score)

        // Count remaining lines (players)
        int count = 0;
        while (getline(fin, line))
            count++;

        fin.clear();        // Clear EOF flag
        fin.seekg(0);       // Go back to start
        getline(fin, line); // skip header again

        if (playerlist != nullptr) {
            for (int i = 0; i < size; ++i) {
                clearChain(playerlist[i].next);
            }
            delete[] playerlist;
            playerlist = nullptr;
        }

        playerlist = new Playerlist[count];
        size = count;


        for (int i = 0; i < count && getline(fin, line); ++i) {
            string username = "";
            for (int j = 0; j < line.length(); ++j) {
                if (line[j] == ',') break;
                username += line[j];
            }

            playerlist[i].username = cleanName(username);
            playerlist[i].next = nullptr;
          //  cout << "playerlist[" << i << "].username = " << username << endl;
        }

        fin.close();
    }
    int getIndexByUsername(string name) {
        name = cleanName(name);
        for (int i = 0; i < size; ++i) {
            if (playerlist[i].username == name)
                return i;
        }
        return -1; // Not found
    }
    void buildFriendChains() {
        for (int i = 0; i < size; ++i) {
            clearChain(playerlist[i].next);
            playerlist[i].next = nullptr;
            ifstream fin(playerlist[i].username + "friendlist.txt"); 

            fin.is_open();
            Playerlist* last = nullptr;  // To link the friends

            string friendName;
            while (getline(fin, friendName)) {
                friendName = cleanName(friendName);
                if (friendName.empty()) continue;
                int friendIndex = getIndexByUsername(friendName);
                if (friendIndex == -1) continue;
                if (chainContains(playerlist[i].next, friendName)) continue;

                Playerlist* newFriend = new Playerlist;
                newFriend->username = friendName;
                newFriend->next = nullptr;

                if (last == nullptr) {
                    playerlist[i].next = newFriend;
                }
                else {
                    last->next = newFriend;
                }

                last = newFriend;
            }

            fin.close();
        }
    }

    void printFriendChains() {
        for (int i = 0; i < size; ++i) {
            cout << playerlist[i].username << " |-> ";
            Playerlist* current = playerlist[i].next;
            while (current != nullptr) {
                cout << current->username << " -> ";
                current = current->next;
            }
            cout << "nullptr\n";
        }
    }
    Playerlist* getFriendChain(const string& username) {
        string cleanUsername = cleanName(username);
        for (int i = 0; i < size; ++i) {
            if (playerlist[i].username == cleanUsername) {
                return playerlist[i].next;  // Return head of friend chain
            }
        }
        return nullptr;  // Not found
    }




    void acceptFriendRequest(string user, string requestAccepted) {
        user = cleanName(user);
        requestAccepted = cleanName(requestAccepted);
        if (user.empty() || requestAccepted.empty()) {
            return;
        }

        appendUniqueName(user + "friendlist.txt", requestAccepted);
        appendUniqueName(requestAccepted + "friendlist.txt", user);

        // Removing the accepted request
        ifstream inFile(user + "receivedrequest.txt");

        string line;
        string updatedContent = "";

        // Read all lines and remove the accepted request
        while (getline(inFile, line)) {
            line = cleanName(line);
            if (!line.empty() && line != requestAccepted) {
                updatedContent += line + "\n";
            }
        }

        inFile.close(); 

        //Writing the updated content
        ofstream outFile(user + "receivedrequest.txt");  // Open the same received request file to overwrite it
        if (outFile.is_open()) {
            outFile << updatedContent;
            outFile.close();
        }
    }

    void rejectFriendRequest(string user, string requestRejected) {
        user = cleanName(user);
        requestRejected = cleanName(requestRejected);
        if (user.empty() || requestRejected.empty()) {
            return;
        }

        ifstream inFile(user + "receivedrequest.txt");
        string line;
        string updatedContent = "";

        while (getline(inFile, line)) {
            line = cleanName(line);
            if (!line.empty() && line != requestRejected) {
                updatedContent += line + "\n";
            }
        }

        inFile.close();

        ofstream outFile(user + "receivedrequest.txt");
        if (outFile.is_open()) {
            outFile << updatedContent;
            outFile.close();
        }
    }

    void sendFriendRequest(const string& fromUser, const string& toUser) {
        string cleanFrom = cleanName(fromUser);
        string cleanTo = cleanName(toUser);
        if (cleanFrom.empty() || cleanTo.empty()) {
            return;
        }

        ifstream friendFile(cleanTo + "friendlist.txt");
        string friendName;
        bool alreadyFriends = false;

        while (getline(friendFile, friendName)) {
            if (cleanName(friendName) == cleanFrom) {
                alreadyFriends = true;
                break;
            }
        }
        friendFile.close();

        if (alreadyFriends) {
            return; //return and it will not send request
        }

        appendUniqueName(cleanTo + "receivedrequest.txt", cleanFrom);

    }


};



#endif
