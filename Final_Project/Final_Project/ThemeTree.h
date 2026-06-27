#ifndef THEME_TREE_H
#define THEME_TREE_H

#include <iostream>
#include <string>

using namespace std;

class ThemeNode {
public:
    int themeID;
    string themeName;
    string description;
    string colorPreview;
    ThemeNode* left;
    ThemeNode* right;

    // Paramiterized constructo to initialiize every node of AVL with desired values
    ThemeNode(int id, string name, string desc, string color, ThemeNode* l = nullptr, ThemeNode* r = nullptr) {
        themeID = id;
        themeName = name;
        description = desc;
        colorPreview = color;
        left = l;
        right = r;
    }
};

class ThemeTree {
private:
    ThemeNode* root;

    // To calculate height of tree
    int height(ThemeNode* node) {
        if (!node) {
            return -1;
        }
        return max(height(node->left), height(node->right)) + 1;
    }

    // To calculate balance factor of tree
    int balanceFactor(ThemeNode* node) {
        if (node != nullptr) {
            return height(node->left) - height(node->right);
        }
        else {
            return 0;
        }
    }

    // To rotate right if it is right imbalanced
    ThemeNode* rightRotate(ThemeNode* y) {
        ThemeNode* x = y->left;
        ThemeNode* T2 = x->right;
        x->right = y;
        y->left = T2;
        return x;
    }

    // To rotate left if it is left imbalanced
    ThemeNode* leftRotate(ThemeNode* x) {
        ThemeNode* y = x->right;
        ThemeNode* T2 = y->left;
        y->left = x;
        x->right = T2;
        return y;
    }

    // To insert a new node
    ThemeNode* insert(ThemeNode* node, int id, string name, string desc, string color) {
        if (!node) {
            return new ThemeNode(id, name, desc, color);
        }
        if (id < node->themeID) {
            node->left = insert(node->left, id, name, desc, color);
        }
        else if (id > node->themeID) {
            node->right = insert(node->right, id, name, desc, color);
        }
        else {
            return node; // If the id is matched with exisiting id so dorect return no insertion
        }
        int bf = balanceFactor(node);

        // To balance the tree
        if (bf > 1 && id < node->left->themeID) {
            return rightRotate(node);
        }
        if (bf < -1 && id > node->right->themeID) {
            return leftRotate(node);
        }
        if (bf > 1 && id > node->left->themeID) {
            node->left = leftRotate(node->left);
            return rightRotate(node);
        }
        if (bf < -1 && id < node->right->themeID) {
            node->right = rightRotate(node->right);
            return leftRotate(node);
        }

        return node;
    }

    // Inorder traversal of tree
    void inorder(ThemeNode* node) {
        if (!node) return;
        inorder(node->left);
        cout << "Theme ID: " << node->themeID << endl;
        cout << "Name: " << node->themeName << endl;
        cout << "Description: " << node->description << endl;
        cout << "Color: " << node->colorPreview << endl;
        cout << "--------------------------" << endl;
        inorder(node->right);
    }

    // To search a theme by id
    ThemeNode* searchByID(ThemeNode* node, int id) {
        if (!node || node->themeID == id) {
            return node;
        }
        if (id < node->themeID) {
            return searchByID(node->left, id);
        }
        return searchByID(node->right, id);
    }

    // To fill the arrays i am passing from menu.h
    void inorderFill(ThemeNode* node, int* ids, string* names, string* desc, int& index) {
        if (!node) return;
        inorderFill(node->left, ids, names, desc, index);
        ids[index] = node->themeID;
        names[index] = node->themeName;
        desc[index] = node->description;
        index++;
        inorderFill(node->right, ids, names, desc, index);
    }


public:
    ThemeTree() { root = nullptr; }

    // These are all helper functions to call private functions for functionalities
    void insert(int id, string name, string desc, string color) {
        root = insert(root, id, name, desc, color);
    }

    void displayThemes() {
        inorder(root);
    }

    ThemeNode* findThemeByID(int id) {
        return searchByID(root, id);
    }

    void getThemesInOrder(int* ids, string* names, string* desc, int& index) {
        index = 0;
        inorderFill(root, ids, names, desc, index);
    }
};

#endif