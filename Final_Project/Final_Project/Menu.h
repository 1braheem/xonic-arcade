#ifndef MENU_H
#define MENU_H

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include"Auth.h"
#include "LeaderBoard.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cctype>
#include <algorithm>
#include"friendstuff.h"
#include"multiplayer.h"
#include "ThemeTree.h"
#include "Queue.h"

string nameP1, nameP2;

using namespace sf;
using namespace std;

int width = 720;
int height = 504;

static int checkRun = 0;

class Menu {
private:
    RenderWindow* window;
    Sprite backgroundSprite, backgroundsprite2, backgroundspriteM, themedBackgroundSprite;
    Font font;
    Text startButton, exitButton, registerButton, loginButton, newGameButton, loadGameButton, leaderboardButton, friendsButton, instructionsButton, backButton, MultiplayerButton, inventoryButton, profileButton, friendListButton, searchButton, receivedRequestsButton, acceptButton[20], rejectButton[20];
    Text historyHeader, scoreTextP, passwordTextP, usernameTextP, instructionText, instructionText2, usernameText, profileHeader, usernameText_search, passwordText, passwordToggleText, profilePasswordToggleText, feedbackText, pagenameText;
    Texture background1, background2, backgroundm;
    Texture menuIceBlue, menuRedLava, menuForestGreen, menuRetro, menuSunsetGlow, menuNeonViolet, menuCyberGold, menuMidnightSteel;
    string instructionKey, currentScreen, chosenAuthOption;
    string usernameInput, usernameInput_search, passwordInput;
    bool enteringUsername;
    bool passwordVisible;
    bool profilePasswordVisible;
    Auth auth;
    string leaderboardUsernames[10];
    int leaderboardScores[10];
    int leaderboardCount;
    Text leaderboardEntries[10];
    Hashtable hashtable;
    string requestList[20];
    ThemeTree themeTree;
    Text themeDisplay[10];
    Text themeInstruction;
    int selectedThemeID;
    int selectedThemeIndex;
    int themeCount;
    int themeScrollOffset;
    int themeIDs[10];
    string themeIDInput;
    bool enteringThemeID;
    Text themeIDText;
    Text themeConfirmText;
    Queue q;
    string userName;
    string password;
    string history[5];
    int totalScore;
    int counter = 0;

    bool isPrintableText(sf::Uint32 unicode) const {
        return unicode >= 32 && unicode < 127;
    }

    bool isBackspace(sf::Uint32 unicode) const {
        return unicode == 8;
    }

    void appendInputCharacter(string& input, sf::Uint32 unicode) {
        if (isPrintableText(unicode)) {
            input += static_cast<char>(unicode);
        }
    }

    void togglePasswordVisibility() {
        passwordVisible = !passwordVisible;
    }

    void toggleProfilePasswordVisibility() {
        profilePasswordVisible = !profilePasswordVisible;
    }

    void resetAuthInput() {
        usernameInput.clear();
        passwordInput.clear();
        enteringUsername = true;
        passwordVisible = false;
        feedbackText.setString("");
    }

    void resetThemeInput() {
        themeIDInput.clear();
        enteringThemeID = false;
        selectedThemeIndex = 0;
        themeScrollOffset = 0;
        themeConfirmText.setString("");
        themeConfirmText.setFillColor(Color::Green);
    }

    void resetProfileDisplay() {
        userName.clear();
        password.clear();
        totalScore = 0;
        profilePasswordVisible = false;
        leaderboardCount = 0;
        for (int i = 0; i < 5; ++i) {
            history[i] = "-";
        }
    }

    void resetFriendSearch() {
        usernameInput_search.clear();
        feedbackText.setString("");
    }

    bool isMouseOverText(const Text& text, Vector2f mousePosition) const {
        FloatRect bounds = text.getGlobalBounds();
        bounds.left -= 18.f;
        bounds.top -= 8.f;
        bounds.width += 36.f;
        bounds.height += 16.f;
        return bounds.contains(mousePosition);
    }

    void buildProceduralTheme(Texture& texture, Color top, Color bottom, Color accent) {
        Image image;
        image.create(width, height, top);
        for (unsigned int y = 0; y < static_cast<unsigned int>(height); ++y) {
            float t = static_cast<float>(y) / static_cast<float>(height - 1);
            for (unsigned int x = 0; x < static_cast<unsigned int>(width); ++x) {
                float stripe = ((x + y / 2) % 110 < 7) ? 0.18f : 0.f;
                float glow = (x > 260 && x < 460 && y > 130 && y < 360) ? 0.10f : 0.f;
                Color c(
                    static_cast<Uint8>(top.r * (1.f - t) + bottom.r * t),
                    static_cast<Uint8>(top.g * (1.f - t) + bottom.g * t),
                    static_cast<Uint8>(top.b * (1.f - t) + bottom.b * t)
                );
                c.r = static_cast<Uint8>(min(255.f, c.r + accent.r * (stripe + glow)));
                c.g = static_cast<Uint8>(min(255.f, c.g + accent.g * (stripe + glow)));
                c.b = static_cast<Uint8>(min(255.f, c.b + accent.b * (stripe + glow)));
                image.setPixel(x, y, c);
            }
        }
        texture.loadFromImage(image);
    }

    void buildGridTheme(Texture& texture) {
        Image image;
        image.create(width, height, Color(8, 8, 10));
        for (unsigned int y = 0; y < static_cast<unsigned int>(height); ++y) {
            for (unsigned int x = 0; x < static_cast<unsigned int>(width); ++x) {
                bool gridLine = (x % 42 == 0) || (y % 42 == 0);
                bool block = ((x / 42 + y / 42) % 2 == 0);
                Color c = block ? Color(18, 18, 22) : Color(38, 38, 44);
                if (gridLine) c = Color(225, 225, 235);
                image.setPixel(x, y, c);
            }
        }
        texture.loadFromImage(image);
    }

    void buildAuroraTheme(Texture& texture) {
        Image image;
        image.create(width, height, Color(8, 18, 28));
        for (unsigned int y = 0; y < static_cast<unsigned int>(height); ++y) {
            for (unsigned int x = 0; x < static_cast<unsigned int>(width); ++x) {
                float wave = static_cast<float>((x + y * 2) % 180) / 180.f;
                float band = (wave > 0.42f && wave < 0.56f) ? 1.f : 0.f;
                Color c(
                    static_cast<Uint8>(10 + y * 30 / height + band * 60),
                    static_cast<Uint8>(26 + x * 70 / width + band * 150),
                    static_cast<Uint8>(48 + y * 70 / height + band * 110)
                );
                image.setPixel(x, y, c);
            }
        }
        texture.loadFromImage(image);
    }

    void drawThemedMenuBackground() {
        Texture* activeTexture = &background2;

        if (selectedThemeID == 101) {
            activeTexture = &menuIceBlue;
        }
        else if (selectedThemeID == 102) {
            activeTexture = &menuRedLava;
        }
        else if (selectedThemeID == 103) {
            activeTexture = &menuForestGreen;
        }
        else if (selectedThemeID == 104) {
            activeTexture = &menuRetro;
        }
        else if (selectedThemeID == 105) {
            activeTexture = &menuSunsetGlow;
        }
        else if (selectedThemeID == 106) {
            activeTexture = &menuNeonViolet;
        }
        else if (selectedThemeID == 107) {
            activeTexture = &menuCyberGold;
        }
        else if (selectedThemeID == 108) {
            activeTexture = &menuMidnightSteel;
        }

        Vector2f viewSize = window->getView().getSize();
        themedBackgroundSprite.setTexture(*activeTexture, true);
        themedBackgroundSprite.setPosition(0.f, 0.f);
        themedBackgroundSprite.setScale(viewSize.x / activeTexture->getSize().x, viewSize.y / activeTexture->getSize().y);
        window->draw(themedBackgroundSprite);

        RectangleShape readabilityOverlay(viewSize);
        readabilityOverlay.setFillColor(Color(0, 0, 0, selectedThemeID == -1 ? 0 : 95));
        window->draw(readabilityOverlay);
    }

    void drawPanel(float x, float y, float panelWidth, float panelHeight, Color outlineColor = Color(255, 215, 120)) {
        RectangleShape panel(Vector2f(panelWidth, panelHeight));
        panel.setPosition(x, y);
        panel.setFillColor(Color(15, 20, 28, 220));
        panel.setOutlineThickness(2.f);
        panel.setOutlineColor(outlineColor);
        window->draw(panel);

        RectangleShape accent(Vector2f(panelWidth, 3.f));
        accent.setPosition(x, y);
        accent.setFillColor(Color(outlineColor.r, outlineColor.g, outlineColor.b, 210));
        window->draw(accent);
    }

    void drawCenteredTitle(const string& title, float y, unsigned int size = 28, Color color = Color(255, 215, 120)) {
        Text text;
        text.setFont(font);
        text.setString(title);
        text.setCharacterSize(size);
        text.setStyle(Text::Bold);
        text.setFillColor(color);
        FloatRect bounds = text.getLocalBounds();
        text.setPosition((width - bounds.width) / 2.f, y);
        window->draw(text);
    }

    void drawMenuOption(Text& option, const string& key, float x, float y, float optionWidth, float optionHeight) {
        bool selected = instructionKey == key;
        RectangleShape row(Vector2f(optionWidth, optionHeight));
        row.setPosition(x, y);
        row.setFillColor(selected ? Color(255, 215, 120, 48) : Color(255, 255, 255, 14));
        row.setOutlineThickness(selected ? 2.f : 1.f);
        row.setOutlineColor(selected ? Color(255, 215, 120) : Color(255, 255, 255, 36));
        window->draw(row);

        option.setFillColor(selected ? Color(255, 215, 120) : Color(230, 236, 245));
        FloatRect bounds = option.getLocalBounds();
        option.setPosition(x + (optionWidth - bounds.width) / 2.f - bounds.left, y + (optionHeight - bounds.height) / 2.f - bounds.top - 2.f);
        window->draw(option);
    }

    Texture* getThemeTextureByIndex(int index) {
        if (index == 0) return &menuIceBlue;
        if (index == 1) return &menuRedLava;
        if (index == 2) return &menuForestGreen;
        if (index == 3) return &menuRetro;
        if (index == 4) return &menuSunsetGlow;
        if (index == 5) return &menuNeonViolet;
        if (index == 6) return &menuCyberGold;
        return &menuMidnightSteel;
    }



public:
    int gameShouldStart = -1;

    Menu(RenderWindow* win) : window(win) {

        gameShouldStart = -1;
        instructionKey = "Start";
        currentScreen = "main";
        chosenAuthOption = "";
        enteringUsername = true;
        passwordVisible = false;
        profilePasswordVisible = false;

        window->setFramerateLimit(60);

        background1.loadFromFile("Textures/xonix.png");
        background2.loadFromFile("Textures/background1.jpg");
        backgroundm.loadFromFile("Textures/vs.png");
        menuIceBlue.loadFromFile("Textures/iceBlue.jpeg");
        menuRedLava.loadFromFile("Textures/redLava.jpeg");
	        menuForestGreen.loadFromFile("Textures/forestGreen.jpeg");
	        menuRetro.loadFromFile("Textures/retro.jpeg");
	        menuSunsetGlow.loadFromFile("Textures/sunsetGlow.jpeg");
	        buildGridTheme(menuNeonViolet);
	        buildAuroraTheme(menuCyberGold);
	        buildProceduralTheme(menuMidnightSteel, Color(8, 14, 24), Color(22, 38, 56), Color(120, 210, 255));

        backgroundSprite.setTexture(background1);
        backgroundSprite.setScale((float)width / background1.getSize().x, (float)height / background1.getSize().y);

        backgroundsprite2.setTexture(background2);
        backgroundsprite2.setScale((float)width / background2.getSize().x, (float)height / background2.getSize().y);

        backgroundspriteM.setTexture(backgroundm);
        backgroundspriteM.setScale((float)width / backgroundm.getSize().x, (float)height / backgroundm.getSize().y);

        font.loadFromFile("Fonts/sqbold.otf");

        // Main menu
        startButton.setFont(font);
        startButton.setString("Start");
        startButton.setCharacterSize(36);
        FloatRect startBounds = startButton.getLocalBounds();
        startButton.setPosition((width - startBounds.width) / 2, height / 2 - 50);

        exitButton.setFont(font);
        exitButton.setString("Exit");
        exitButton.setCharacterSize(36);
        FloatRect exitBounds = exitButton.getLocalBounds();
        exitButton.setPosition((width - exitBounds.width) / 2, height / 2 + 20);

        // Auth menu
        registerButton.setFont(font);
        registerButton.setString("Register");
        registerButton.setCharacterSize(36);
        FloatRect registerBounds = registerButton.getLocalBounds();
        registerButton.setPosition((width - registerBounds.width) / 2, height / 2 - 50);

        loginButton.setFont(font);
        loginButton.setString("Login");
        loginButton.setCharacterSize(36);
        FloatRect loginBounds = loginButton.getLocalBounds();
        loginButton.setPosition((width - loginBounds.width) / 2, height / 2 + 20);

        auto setupPostLoginButton = [&](Text& button, const string& label, float y) {
            button.setFont(font);
            button.setString(label);
            button.setCharacterSize(26);
            FloatRect bounds = button.getLocalBounds();
            button.setPosition((width - bounds.width) / 2, y);
        };

        // Post-login menu
        setupPostLoginButton(newGameButton, "New Game", 18);
        setupPostLoginButton(loadGameButton, "Load Game", 70);
        setupPostLoginButton(MultiplayerButton, "Multiplayer", 122);
        setupPostLoginButton(inventoryButton, "Inventory", 174);
        setupPostLoginButton(leaderboardButton, "LeaderBoard", 226);
        setupPostLoginButton(profileButton, "Profile", 278);
        setupPostLoginButton(friendsButton, "Friends", 330);
        setupPostLoginButton(instructionsButton, "Instructions", 382);

        //Friendlist screen buttons
        friendListButton.setFont(font);
        friendListButton.setString("Friend List");
        friendListButton.setCharacterSize(36);
        friendListButton.setPosition(100, 150);

        searchButton.setFont(font);
        searchButton.setString("Search");
        searchButton.setCharacterSize(36);
        searchButton.setPosition(100, 200);

        receivedRequestsButton.setFont(font);
        receivedRequestsButton.setString("Received Requests");
        receivedRequestsButton.setCharacterSize(36);
        receivedRequestsButton.setPosition(100, 250); // Adjust the position as needed

        //back button
        backButton.setFont(font);
        backButton.setCharacterSize(18);
        backButton.setFillColor(sf::Color(120, 235, 255));
        backButton.setString("Press B or Esc to go back");
        backButton.setPosition((width - backButton.getLocalBounds().width) / 2, 410);

        // Text fields
        instructionText.setFont(font);
        instructionText.setCharacterSize(28);
        instructionText.setFillColor(sf::Color(200, 200, 200));
        instructionText.setPosition(100, 100);

        instructionText2.setFont(font);
        instructionText2.setCharacterSize(28);
        instructionText2.setFillColor(sf::Color(200, 200, 200));
        instructionText2.setPosition(100, 100);

        pagenameText.setFont(font);
        pagenameText.setCharacterSize(28);
        pagenameText.setFillColor(sf::Color(200, 200, 200));
        pagenameText.setPosition(100, 0);

        usernameText.setFont(font);
        usernameText.setCharacterSize(24);
        usernameText.setFillColor(sf::Color(128, 0, 32));
        usernameText.setPosition(100, 200);

        usernameText_search.setFont(font);
        usernameText_search.setCharacterSize(24);
        usernameText_search.setFillColor(sf::Color(128, 0, 32));
        usernameText_search.setPosition(100, 200);

        passwordText.setFont(font);
        passwordText.setCharacterSize(24);
        passwordText.setFillColor(sf::Color(128, 0, 32));
        passwordText.setPosition(100, 250);

        passwordToggleText.setFont(font);
        passwordToggleText.setCharacterSize(20);
        passwordToggleText.setFillColor(sf::Color(200, 200, 200));
        passwordToggleText.setPosition(100, 292);

        feedbackText.setFont(font);
        feedbackText.setCharacterSize(24);
        feedbackText.setFillColor(Color::Red);
        feedbackText.setPosition(100, 400);

        profileHeader.setFont(font);
        profileHeader.setString("PLAYER PROFILE");
        profileHeader.setCharacterSize(28);
        profileHeader.setFillColor(Color(128, 0, 32));
        profileHeader.setPosition((window->getSize().x - profileHeader.getLocalBounds().width) / 2, 40);

        usernameTextP.setFont(font);
        usernameTextP.setString("Username: " + userName);
        usernameTextP.setCharacterSize(22);
        usernameTextP.setFillColor(sf::Color::White);
        usernameTextP.setPosition(100, 100);

        passwordTextP.setFont(font);
        passwordTextP.setString("Password: " + password);
        passwordTextP.setCharacterSize(22);
        passwordTextP.setFillColor(sf::Color::White);
        passwordTextP.setPosition(100, 135);

        profilePasswordToggleText.setFont(font);
        profilePasswordToggleText.setCharacterSize(18);
        profilePasswordToggleText.setFillColor(sf::Color(200, 200, 200));
        profilePasswordToggleText.setPosition(100, 165);

        scoreTextP.setFont(font);
        scoreTextP.setString("Total Score: " + to_string(totalScore));
        scoreTextP.setCharacterSize(22);
        scoreTextP.setFillColor(sf::Color::White);
        scoreTextP.setPosition(100, 195);

        historyHeader.setFont(font);
        historyHeader.setString("Recent Match History:");
        historyHeader.setCharacterSize(22);
        historyHeader.setFillColor(Color(128, 0, 32)); // Light blue
        historyHeader.setStyle(Text::Underlined);
        historyHeader.setPosition(100, 235);

        // Themes loaded to AVL tree(TheemTree.h)
        themeTree.insert(101, "Classic Blue", "Cool blue tile theme", "Blue");
        themeTree.insert(102, "Lava Red", "Fiery red aesthetic", "Red");
        themeTree.insert(103, "Forest Green", "Nature-inspired green", "Green");
        themeTree.insert(104, "Retro", "Old-school arcade vibe", "Purple");
        themeTree.insert(105, "Sunset Glow", "Warm orange & pink tones", "Orange-Pink");
        themeTree.insert(106, "Mono Grid", "Black-white tactical grid", "Mono");
        themeTree.insert(107, "Aurora Flow", "Soft flowing neon bands", "Aurora");
        themeTree.insert(108, "Midnight Steel", "Cool steel blue", "Steel");

        selectedThemeID = -1;
        selectedThemeIndex = 0;
        themeCount = 0;
        themeIDInput = "";
        enteringThemeID = false;

        themeIDText.setFont(font);
        themeIDText.setCharacterSize(24);
        themeIDText.setFillColor(sf::Color(200, 200, 200));
        themeIDText.setPosition(100, 300);

        themeConfirmText.setFont(font);
        themeConfirmText.setCharacterSize(24);
        themeConfirmText.setFillColor(Color::Green);
        themeConfirmText.setPosition(100, 340);
    }
    bool chalgaya = true;

    string getusername() {
        return usernameInput;
    }

    int getThemeID() {
        return selectedThemeID;
    }


    int run() {
        Music musicq;

        // Play the first music file
        musicq.openFromFile("Sound Effects/menus.mp3");
        musicq.setVolume(100);
        if (chalgaya) {
            chalgaya = false;
            musicq.play();
        }

        if (checkRun > 0) {
            window->clear();
            currentScreen = "postLogin";
            instructionKey = "New Game";
        }
        if (checkRun == 0) {
            checkRun++;
        }
        Music music2;
        music2.openFromFile("Sound Effects/select.mp3");

        Music music3;
        music3.openFromFile("Sound Effects/textm.mp3");
        music3.setVolume(3000);

        auto updateHoverSelection = [&](Vector2f mousePosition) {
            if (currentScreen == "main") {
                if (isMouseOverText(startButton, mousePosition)) { instructionKey = "Start"; return true; }
                if (isMouseOverText(exitButton, mousePosition)) { instructionKey = "Exit"; return true; }
            }
            else if (currentScreen == "auth") {
                if (isMouseOverText(registerButton, mousePosition)) { instructionKey = "Register"; return true; }
                if (isMouseOverText(loginButton, mousePosition)) { instructionKey = "Login"; return true; }
            }
            else if (currentScreen == "postLogin") {
                if (isMouseOverText(newGameButton, mousePosition)) { instructionKey = "New Game"; return true; }
                if (isMouseOverText(loadGameButton, mousePosition)) { instructionKey = "Load Game"; return true; }
                if (isMouseOverText(MultiplayerButton, mousePosition)) { instructionKey = "Multiplayer"; return true; }
                if (isMouseOverText(inventoryButton, mousePosition)) { instructionKey = "Inventory"; return true; }
                if (isMouseOverText(leaderboardButton, mousePosition)) { instructionKey = "LeaderBoard"; return true; }
                if (isMouseOverText(profileButton, mousePosition)) { instructionKey = "Profile"; return true; }
                if (isMouseOverText(friendsButton, mousePosition)) { instructionKey = "Friends"; return true; }
                if (isMouseOverText(instructionsButton, mousePosition)) { instructionKey = "Instructions"; return true; }
            }
            else if (currentScreen == "dashboard") {
                if (isMouseOverText(friendListButton, mousePosition)) { instructionKey = "Friend List"; return true; }
                if (isMouseOverText(searchButton, mousePosition)) { instructionKey = "Search"; return true; }
                if (isMouseOverText(receivedRequestsButton, mousePosition)) { instructionKey = "Received Requests"; return true; }
            }
            else if (currentScreen == "Inventory" && themeCount > 0) {
                int visibleCount = 5;
                for (int i = 0; i < themeCount; ++i) {
                    int visibleRow = i - themeScrollOffset;
                    if (visibleRow < 0 || visibleRow >= visibleCount) continue;
                    FloatRect rowBounds(72.f, 96.f + visibleRow * 58.f, 575.f, 50.f);
                    if (rowBounds.contains(mousePosition)) {
                        selectedThemeIndex = i;
                        themeConfirmText.setString("");
                        return true;
                    }
                }
            }

            return false;
        };

        while (window->isOpen()) {
            Event e;
            while (window->pollEvent(e)) {
                bool activateCurrent = false;
                if (e.type == Event::Closed) {
                    window->close();
                    return -1;
                }
                if (e.type == Event::MouseMoved) {
                    updateHoverSelection(window->mapPixelToCoords(Vector2i(e.mouseMove.x, e.mouseMove.y)));
                }
                if (e.type == Event::MouseWheelScrolled && currentScreen == "Inventory" && themeCount > 0) {
                    if (e.mouseWheelScroll.delta < 0 && themeScrollOffset < max(0, themeCount - 5)) {
                        themeScrollOffset++;
                    }
                    else if (e.mouseWheelScroll.delta > 0 && themeScrollOffset > 0) {
                        themeScrollOffset--;
                    }
                }
                if (e.type == Event::MouseButtonPressed && e.mouseButton.button == Mouse::Left) {
                    music2.play();
                    // Get mouse position as integers
                    int mouseX = e.mouseButton.x;
                    int mouseY = e.mouseButton.y;
                    Vector2f mousePosition = window->mapPixelToCoords(Vector2i(mouseX, mouseY));
                    bool clickedSelectable = updateHoverSelection(mousePosition);

                    if (currentScreen == "input" && isMouseOverText(instructionText2, mousePosition)) {
                        activateCurrent = true;
                    }
                    else if (currentScreen == "input" && !enteringUsername && passwordToggleText.getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY))) {
                        togglePasswordVisibility();
                    }
                    else if (currentScreen == "input") {
                        if (FloatRect(150.f, 225.f, 420.f, 42.f).contains(mousePosition)) {
                            enteringUsername = true;
                        }
                        else if (FloatRect(150.f, 290.f, 420.f, 42.f).contains(mousePosition)) {
                            enteringUsername = false;
                        }
                    }
                    else if (currentScreen == "Profile" && profilePasswordToggleText.getGlobalBounds().contains(static_cast<float>(mouseX), static_cast<float>(mouseY))) {
                        toggleProfilePasswordVisibility();
                    }
                    else if ((currentScreen == "auth" || currentScreen == "leaderboard" || currentScreen == "Profile" || currentScreen == "friendlist" || currentScreen == "receivedRequests" || currentScreen == "instructions") && isMouseOverText(backButton, mousePosition)) {
                        if (currentScreen == "friendlist" || currentScreen == "receivedRequests") {
                            currentScreen = "dashboard";
                            instructionKey = "Friend List";
                            resetFriendSearch();
                        }
                        else if (currentScreen == "auth") {
                            currentScreen = "main";
                            instructionKey = "Start";
                        }
                        else {
                            currentScreen = "postLogin";
                            instructionKey = "New Game";
                            resetThemeInput();
                            resetProfileDisplay();
                        }
                    }
                    else if (currentScreen == "searchFriend" && isMouseOverText(backButton, mousePosition)) {
                        currentScreen = "dashboard";
                        instructionKey = "Friend List";
                        resetFriendSearch();
                    }
                    else if (currentScreen == "searchFriend" && isMouseOverText(instructionText2, mousePosition)) {
                        activateCurrent = true;
                    }
                    else if (currentScreen == "Inventory" && isMouseOverText(backButton, mousePosition)) {
                        currentScreen = "postLogin";
                        instructionKey = "New Game";
                        resetThemeInput();
                    }
                    else if (clickedSelectable && (currentScreen == "main" || currentScreen == "auth" || currentScreen == "postLogin" || currentScreen == "dashboard" || currentScreen == "Inventory")) {
                        activateCurrent = true;
                    }
                    else if (currentScreen == "receivedRequests") {
		                        // Check for click on Accept and Reject buttons for each player
		                        for (int i = 0; i < 20; ++i) {
	                            // Check Accept Button
	                            if (acceptButton[i].getGlobalBounds().left <= mouseX && mouseX <= acceptButton[i].getGlobalBounds().left + acceptButton[i].getGlobalBounds().width &&
	                                acceptButton[i].getGlobalBounds().top <= mouseY && mouseY <= acceptButton[i].getGlobalBounds().top + acceptButton[i].getGlobalBounds().height) {

	                                if (!requestList[i].empty()) {
	                                    hashtable.acceptFriendRequest(usernameInput, requestList[i]);
	                                    requestList[i].clear();
	                                }
	                                break;
	                            }

	                            // Check Reject Button
	                            if (rejectButton[i].getGlobalBounds().left <= mouseX && mouseX <= rejectButton[i].getGlobalBounds().left + rejectButton[i].getGlobalBounds().width &&
	                                rejectButton[i].getGlobalBounds().top <= mouseY && mouseY <= rejectButton[i].getGlobalBounds().top + rejectButton[i].getGlobalBounds().height) {
	                                if (!requestList[i].empty()) {
	                                    hashtable.rejectFriendRequest(usernameInput, requestList[i]);
	                                    requestList[i].clear();
	                                }
	                                break;
	                            }
                        }
                    }
                }

                if (e.type == Event::TextEntered && currentScreen == "input") {
                    if (isBackspace(e.text.unicode)) {
                        music3.play();
                        if (enteringUsername && !usernameInput.empty())
                            usernameInput.pop_back();
                        else if (!enteringUsername && !passwordInput.empty())
                            passwordInput.pop_back();
                    }
                    else if (isPrintableText(e.text.unicode)) {
                        music3.play();
                        if (enteringUsername)
                            appendInputCharacter(usernameInput, e.text.unicode);
                        else
                            appendInputCharacter(passwordInput, e.text.unicode);
                        nameP1 = usernameInput;
                    }
                }

                if (e.type == Event::TextEntered && currentScreen == "searchFriend") {
                    if (isBackspace(e.text.unicode)) {
                        music3.play();
                        if (!usernameInput_search.empty()) {
                            usernameInput_search.pop_back();
                        }
                    }
                    else if (isPrintableText(e.text.unicode)) {
                        music3.play();
                        appendInputCharacter(usernameInput_search, e.text.unicode);
                    }
                }

                if (e.type == Event::KeyPressed || activateCurrent) {
                    Keyboard::Key keyCode = activateCurrent ? Keyboard::Enter : e.key.code;
                    if (currentScreen == "main") {
                        if (keyCode == Keyboard::Up)
                            instructionKey = "Start";
                        else if (keyCode == Keyboard::Down)
                            instructionKey = "Exit";
                        else if (keyCode == Keyboard::Enter) {
                            music2.play();
                            if (instructionKey == "Start") {
                                musicq.openFromFile("Sound Effects/menu.mp3");
                                musicq.setVolume(100);
                                musicq.play();
                                currentScreen = "auth";
                                instructionKey = "Register";
                            }
                            else if (instructionKey == "Exit") {
                                window->close();
                                return false;
                            }
                        }
                    }
                    else if (currentScreen == "auth") {

                        if (keyCode == Keyboard::Escape || keyCode == Keyboard::B) {
                            music2.play();
                            currentScreen = "main";
                            instructionKey = "Start";
                        }
                        if (keyCode == Keyboard::Up)
                            instructionKey = "Register";
                        else if (keyCode == Keyboard::Down)
                            instructionKey = "Login";
                        else if (keyCode == Keyboard::Enter) {
                            music2.play();
                            chosenAuthOption = instructionKey;
                            currentScreen = "input";
                            enteringUsername = true;
                            resetAuthInput();
                        }
                    }
                    else if (currentScreen == "input") {

                        if (keyCode == Keyboard::Escape) {
                            music2.play();
                            currentScreen = "auth";
                            instructionKey = "Register";
                            resetAuthInput();
                        }

                        if (keyCode == Keyboard::Enter) {
                            music2.play();
                            if (enteringUsername) {
                                enteringUsername = false;
                                passwordVisible = false;
                            }
                            else {
                                bool valid = false;
                                if (chosenAuthOption == "Register")
                                    valid = auth.registerPlayer(usernameInput, passwordInput);
                                else if (chosenAuthOption == "Login")
                                    valid = auth.loginPlayer(usernameInput, passwordInput);

                                if (valid) {
                                    feedbackText.setString(chosenAuthOption + " successful! Starting game...");
                                    feedbackText.setFillColor(Color::Green);
                                    gameShouldStart = 0;
                                    currentScreen = "postLogin";
                                    instructionKey = "New Game";
                                }
                                else {
                                    feedbackText.setString(chosenAuthOption + " failed. Try again.");
                                    feedbackText.setFillColor(Color::Red);
                                    usernameInput.clear();
                                    passwordInput.clear();
                                    enteringUsername = true;
                                    passwordVisible = false;
                                }
                            }
                        }
                    }
                    else if (currentScreen == "searchFriend") {
                        if (keyCode == Keyboard::Enter) {
                            music2.play();
                            bool valid = auth.search_player(usernameInput_search);

                            if (valid) {
                                if (usernameInput == usernameInput_search) {
                                    feedbackText.setString("You can't send a request to yourself.");
                                    feedbackText.setFillColor(Color::Red);
                                }
                                else {

                                    bool alreadyFriend = false;
                                    ifstream friendFile(usernameInput + "friendlist.txt");
                                    string fname;
                                    while (getline(friendFile, fname)) {
                                        if (fname == usernameInput_search) {
                                            alreadyFriend = true;
                                            break;
                                        }
                                    }
                                    friendFile.close();

                                    if (alreadyFriend) {
                                        feedbackText.setString(usernameInput_search + " is already your friend.");
                                        feedbackText.setFillColor(Color::Red);
                                    }
                                    else {
                                        bool alreadyRequested = false;
                                        ifstream requestFile(usernameInput_search + "receivedrequest.txt");
                                        while (getline(requestFile, fname)) {
                                            if (fname == usernameInput) {
                                                alreadyRequested = true;
                                                break;
                                            }
                                        }
                                        requestFile.close();

                                        if (alreadyRequested) {
                                            feedbackText.setString("Friend request already sent to " + usernameInput_search);
                                            feedbackText.setFillColor(Color::Red);
                                        }
                                        else {
                                            feedbackText.setString("Friend Request sent to: " + usernameInput_search);
                                            feedbackText.setFillColor(Color::Green);
                                            hashtable.sendFriendRequest(usernameInput, usernameInput_search);
                                        }
                                    }

                                    usernameInput_search.clear();
                                }
                            }
                            else {
                                feedbackText.setString("User not found.");
                                feedbackText.setFillColor(Color::Red);
                                usernameInput_search.clear();
                            }
                        }
                    }

                    else if (currentScreen == "postLogin") {
                        if (keyCode == Keyboard::Escape || keyCode == Keyboard::B) {
                            music2.play();
                            currentScreen = "auth";
                            instructionKey = "Register";
                            resetThemeInput();
                            resetProfileDisplay();
                            resetFriendSearch();
                        }
	                        if (keyCode == Keyboard::Up) {
	                            if (instructionKey == "New Game") instructionKey = "Instructions";
	                            else if (instructionKey == "Load Game") instructionKey = "New Game";
	                            else if (instructionKey == "Multiplayer") instructionKey = "Load Game";
	                            else if (instructionKey == "Inventory") instructionKey = "Multiplayer";
	                            else if (instructionKey == "LeaderBoard") instructionKey = "Inventory";
	                            else if (instructionKey == "Profile") instructionKey = "LeaderBoard";
	                            else if (instructionKey == "Friends") instructionKey = "Profile";
	                            else if (instructionKey == "Instructions") instructionKey = "Friends";
	                        }
	                        else if (keyCode == Keyboard::Down) {
	                            if (instructionKey == "New Game") instructionKey = "Load Game";
	                            else if (instructionKey == "Load Game") instructionKey = "Multiplayer";
	                            else if (instructionKey == "Multiplayer") instructionKey = "Inventory";
	                            else if (instructionKey == "Inventory") instructionKey = "LeaderBoard";
	                            else if (instructionKey == "LeaderBoard") instructionKey = "Profile";
	                            else if (instructionKey == "Profile") instructionKey = "Friends";
	                            else if (instructionKey == "Friends") instructionKey = "Instructions";
	                            else if (instructionKey == "Instructions") instructionKey = "New Game";
                        }
                        else if (keyCode == Keyboard::Enter) {

                            if (instructionKey == "New Game") {
                                musicq.stop();
                                gameShouldStart = 1;
                                return gameShouldStart;
                            }
                            else if (instructionKey == "Load Game") {

                                gameShouldStart = 2;
                                return gameShouldStart;
                            }
                            else if (instructionKey == "Multiplayer") {

                                sf::Clock clock;
                                sf::Music music;
                                music.openFromFile("Sound Effects/vs.mp3");

                                music.play();  // Start the music before the loop
                                bool checkBand = false;
                                if (counter == 0) {
                                    q.enqueueFromFile();
                                    counter++;
                                }
                                string t = q.peek().getUsername();
                                int opponentAttempts = 0;
                                while (!t.empty() && t == usernameInput && opponentAttempts < 20) {
                                    q.dequeue();
                                    t = q.peek().getUsername();
                                    opponentAttempts++;
                                }
                                nameP2 = t.empty() ? "Player 2" : t;
                                q.dequeue();

                                while (clock.getElapsedTime().asSeconds() < 6.0f) {
                                    sf::Event event;
                                    while (window->pollEvent(event)) {
                                        if (event.type == sf::Event::Closed) {
                                            window->close();
                                            return -1;
                                            checkBand = true;
                                        }
                                    }

                                    window->clear();  // Only one clear!

                                    if (checkBand) {
                                        break;
                                    }

                                    drawThemedMenuBackground();
                                    RectangleShape p1Wash(Vector2f(width / 2.f, (float)height));
                                    p1Wash.setPosition(0.f, 0.f);
                                    p1Wash.setFillColor(Color(0, 210, 255, 45));
                                    window->draw(p1Wash);

                                    RectangleShape p2Wash(Vector2f(width / 2.f, (float)height));
                                    p2Wash.setPosition(width / 2.f, 0.f);
                                    p2Wash.setFillColor(Color(255, 85, 190, 42));
                                    window->draw(p2Wash);

                                    RectangleShape vsOverlay(Vector2f((float)width, (float)height));
                                    vsOverlay.setFillColor(Color(0, 0, 0, 75));
                                    window->draw(vsOverlay);

                                    RectangleShape centerLine(Vector2f(2.f, 270.f));
                                    centerLine.setPosition(width / 2.f - 1.f, 115.f);
                                    centerLine.setFillColor(Color(255, 215, 120, 105));
                                    window->draw(centerLine);

                                    drawCenteredTitle("MULTIPLAYER MATCH", 70, 28, Color(255, 215, 120));

                                    RectangleShape p1Card(Vector2f(245.f, 130.f));
                                    p1Card.setPosition(65.f, 150.f);
                                    p1Card.setFillColor(Color(8, 18, 32, 225));
                                    p1Card.setOutlineThickness(2.f);
                                    p1Card.setOutlineColor(Color(100, 225, 255));
                                    window->draw(p1Card);

                                    RectangleShape p2Card(Vector2f(245.f, 130.f));
                                    p2Card.setPosition(410.f, 150.f);
                                    p2Card.setFillColor(Color(32, 10, 26, 225));
                                    p2Card.setOutlineThickness(2.f);
                                    p2Card.setOutlineColor(Color(255, 120, 220));
                                    window->draw(p2Card);

                                    Text p1Label, p2Label, vsText, statusText;
                                    p1Label.setFont(font);
                                    p2Label.setFont(font);
                                    vsText.setFont(font);
                                    statusText.setFont(font);

                                    p1Label.setString("PLAYER 1\n" + usernameInput);
                                    p1Label.setCharacterSize(22);
                                    p1Label.setFillColor(Color(100, 225, 255));
                                    p1Label.setPosition(92.f, 185.f);

                                    p2Label.setString("PLAYER 2\n" + nameP2);
                                    p2Label.setCharacterSize(22);
                                    p2Label.setFillColor(Color(255, 140, 225));
                                    p2Label.setPosition(438.f, 185.f);

                                    vsText.setString("VS");
                                    vsText.setCharacterSize(42);
                                    vsText.setStyle(Text::Bold);
                                    vsText.setFillColor(Color::White);
                                    FloatRect vsBounds = vsText.getLocalBounds();
                                    vsText.setPosition((width - vsBounds.width) / 2.f, 190.f);

                                    statusText.setString("Preparing arena...");
                                    statusText.setCharacterSize(18);
                                    statusText.setFillColor(Color(220, 230, 245));
                                    FloatRect statusBounds = statusText.getLocalBounds();
                                    statusText.setPosition((width - statusBounds.width) / 2.f, 315.f);

                                    RectangleShape progressBack(Vector2f(420.f, 10.f));
                                    progressBack.setPosition(150.f, 350.f);
                                    progressBack.setFillColor(Color(255, 255, 255, 45));
                                    window->draw(progressBack);

                                    float progress = clock.getElapsedTime().asSeconds() / 6.0f;
                                    if (progress > 1.0f) progress = 1.0f;
                                    RectangleShape progressFill(Vector2f(420.f * progress, 10.f));
                                    progressFill.setPosition(150.f, 350.f);
                                    progressFill.setFillColor(Color(255, 215, 120));
                                    window->draw(progressFill);

                                    window->draw(p1Label);
                                    window->draw(p2Label);
                                    window->draw(vsText);
                                    window->draw(statusText);
                                    window->display();
                                }

                                if (checkBand) {
                                    return -1;
                                }
                                gameShouldStart = 3;
                                return gameShouldStart;  // If that's your desired behavior
                            }
                            else if (instructionKey == "Inventory") {
                                string names[10];
                                string desc[10];
                                int ids[10];
                                int index = 0;
                                themeTree.getThemesInOrder(ids, names, desc, index);  // Write this function in your ThemeTree

                                for (int i = 0; i < index; ++i) {
                                    themeIDs[i] = ids[i];
                                    themeDisplay[i].setFont(font);
                                    themeDisplay[i].setCharacterSize(14);
                                    themeDisplay[i].setFillColor(sf::Color(200, 200, 200));
                                    themeDisplay[i].setString(to_string(ids[i]) + "  " + names[i] + "\n" + desc[i]);
                                }
                                themeCount = index;
                                resetThemeInput();
                                for (int i = 0; i < themeCount; ++i) {
                                    if (ids[i] == selectedThemeID) {
                                        selectedThemeIndex = i;
                                        themeScrollOffset = max(0, min(selectedThemeIndex - 2, max(0, themeCount - 5)));
                                        break;
                                    }
                                }
                                enteringThemeID = true;
                                currentScreen = "Inventory";

                            }
                            else if (instructionKey == "Profile") {
                                resetProfileDisplay();
                                userName = getusername();
                                ifstream file("Data.csv");
                                string line;

                                // Skip header
                                getline(file, line);

                                string foundUser, foundPass;


                                while (getline(file, line)) {
                                    stringstream ss(line);
                                    string token;
                                    getline(ss, token, ','); // username

                                    if (token == userName) {
                                        foundUser = token;
                                        userName = foundUser;
                                        getline(ss, foundPass, ',');         // password
                                        password = foundPass;
                                        string scoreStr;
                                        getline(ss, scoreStr, ',');          // score
                                        totalScore = stoi(scoreStr);

                                        for (int i = 0; i < 5; ++i) {
                                            if (!getline(ss, history[i], ',')) history[i] = "-";
                                        }
                                        break;
                                    }
                                }
                                file.close();
                                currentScreen = "Profile";
                            }
                            else if (instructionKey == "LeaderBoard") {
                                ifstream file("LeaderBoard.csv");
                                string line;
                                getline(file, line); // Skip header
                                leaderboardCount = 0;

                                for (int i = 0; i < 10 && getline(file, line); ++i) {
                                    size_t commaPos = line.find(',');
                                    if (commaPos != string::npos) {
                                        leaderboardUsernames[i] = line.substr(0, commaPos);
                                        leaderboardScores[i] = stoi(line.substr(commaPos + 1));

                                        leaderboardEntries[i].setFont(font);
                                        leaderboardEntries[i].setCharacterSize(18);
                                        leaderboardEntries[i].setFillColor(sf::Color(200, 200, 200));
                                        leaderboardEntries[i].setString(
                                            leaderboardUsernames[i]
                                        );
                                        leaderboardEntries[i].setPosition(210, 112 + i * 28);
                                        leaderboardCount++;
                                    }
                                }
                                currentScreen = "leaderboard";
                            }
	                            else if (instructionKey == "Friends") {
	                                resetFriendSearch();
	                                currentScreen = "dashboard";
	                                instructionKey = "Friend List";
	                            }
	                            else if (instructionKey == "Instructions") {
	                                currentScreen = "instructions";
	                            }
	                            else {
                                window->clear();
                                window->draw(backgroundSprite);
                                window->display();
                            }
                        }
                    }
                    else if (currentScreen == "dashboard") {
                        if (keyCode == Keyboard::Up) {
                            if (instructionKey == "Friend List") instructionKey = "Received Requests";
                            else if (instructionKey == "Search") instructionKey = "Friend List";
                            else if (instructionKey == "Received Requests") instructionKey = "Search";
                        }
                        else if (keyCode == Keyboard::Down) {
                            if (instructionKey == "Friend List") instructionKey = "Search";
                            else if (instructionKey == "Search") instructionKey = "Received Requests";
                            else if (instructionKey == "Received Requests") instructionKey = "Friend List";
                        }
                        else if (keyCode == Keyboard::Enter) {
                            music2.play();
                            if (instructionKey == "Friend List") {
                                resetFriendSearch();
                                // Transition to the next page for Friend List with the same background
                                currentScreen = "friendlist";
                            }
                            else if (instructionKey == "Search") {
                                resetFriendSearch();
                                currentScreen = "searchFriend";
                            }
                            else if (instructionKey == "Received Requests") {
                                resetFriendSearch();
                                // Transition to the next page for Received Requests with the same background
                                currentScreen = "receivedRequests";
                                ifstream file(usernameInput + "receivedrequest.txt");

                                if (file.is_open()) {
                                    string requestUser;
                                    int lineCount = 0;

                                    while (getline(file, requestUser)) {

                                        instructionText.setString(to_string(lineCount + 1) + ". " + requestUser);

                                        // Position each line appropriately on the screen (with vertical spacing)
                                        instructionText.setPosition(100, 100 + (lineCount * 30));

                                        // Draw the updated text on the screen
                                        window->draw(instructionText);

                                        lineCount++;
                                    }

                                    file.close();
                                }
                                else {
                                    // If file couldn't be opened, display an error on the screen
                                    instructionText.setString("Error: Could not open file.");
                                    instructionText.setPosition(100, 100);
                                    window->draw(instructionText);
                                }

                            }
                        }
                        else if (keyCode == Keyboard::B || keyCode == Keyboard::Escape) {
                            // Back to the post-login screen if Escape is pressed
                            music2.play();
                            currentScreen = "postLogin";
                            instructionKey = "New Game";
                            resetFriendSearch();
                        }
                    }
	                    else if (currentScreen == "leaderboard") {
	                        if (keyCode == Keyboard::B || keyCode == Keyboard::Escape) {
	                            music2.play();
	                            currentScreen = "postLogin";
	                            instructionKey = "New Game";
	                        }
	                    }
	                    else if (currentScreen == "instructions") {
	                        if (keyCode == Keyboard::B || keyCode == Keyboard::Escape) {
	                            music2.play();
	                            currentScreen = "postLogin";
	                            instructionKey = "New Game";
	                        }
	                    }
                    else if (currentScreen == "Inventory") {
                        music3.play();
                        if (keyCode == Keyboard::B || keyCode == Keyboard::Escape) {
                            music2.play();
                            currentScreen = "postLogin";
                            instructionKey = "New Game";
                            resetThemeInput();
                        }
                        else if (keyCode == Keyboard::Up && themeCount > 0) {
                            music2.play();
                            selectedThemeIndex = (selectedThemeIndex + themeCount - 1) % themeCount;
                            if (selectedThemeIndex < themeScrollOffset) themeScrollOffset = selectedThemeIndex;
                            if (selectedThemeIndex == themeCount - 1) themeScrollOffset = max(0, themeCount - 5);
                            themeConfirmText.setString("");
                        }
                        else if (keyCode == Keyboard::Down && themeCount > 0) {
                            music2.play();
                            selectedThemeIndex = (selectedThemeIndex + 1) % themeCount;
                            if (selectedThemeIndex >= themeScrollOffset + 5) themeScrollOffset = selectedThemeIndex - 4;
                            if (selectedThemeIndex == 0) themeScrollOffset = 0;
                            themeConfirmText.setString("");
                        }
                        else if (keyCode == Keyboard::Enter && themeCount > 0) {
                            music2.play();
                            selectedThemeID = themeIDs[selectedThemeIndex];
                            themeConfirmText.setFillColor(Color::Green);
                            themeConfirmText.setString("Theme " + to_string(selectedThemeID) + " selected!");
                        }
                    }
                    else if (currentScreen == "Profile") {
                        music3.play();
                        if (keyCode == Keyboard::B || keyCode == Keyboard::Escape) {
                            music2.play();
                            currentScreen = "postLogin";
                            instructionKey = "New Game";
                            resetProfileDisplay();
                        }
                        else if (keyCode == Keyboard::H) {
                            music2.play();
                            toggleProfilePasswordVisibility();
                        }
                    }
                    if (e.type == Event::KeyPressed) {
                        if (currentScreen == "searchFriend") {
                            if (keyCode == Keyboard::Escape) {
                                music2.play();
                                currentScreen = "dashboard";
                                instructionKey = "Friend List";
                                resetFriendSearch();
                            }
                        }
                        else if (currentScreen == "friendlist" || currentScreen == "receivedRequests") {
                            if (keyCode == Keyboard::Escape || keyCode == Keyboard::B) {
                                music2.play();
                                currentScreen = "dashboard";
                                instructionKey = "Friend List";
                                resetFriendSearch();
                            }
                        }
                    }
                }
            }

            window->clear();
            window->draw(backgroundSprite);

	            if (currentScreen == "main") {
	                RectangleShape mainOverlay(Vector2f(static_cast<float>(width), static_cast<float>(height)));
	                mainOverlay.setFillColor(Color(0, 0, 0, 80));
	                window->draw(mainOverlay);
	                drawPanel(205.f, 118.f, 310.f, 260.f, Color(255, 215, 120));
	                drawCenteredTitle("MAIN MENU", 145.f, 30, Color(255, 215, 120));
	                Text subtitle;
	                subtitle.setFont(font);
	                subtitle.setString("Arcade Territory Battle");
	                subtitle.setCharacterSize(15);
	                subtitle.setFillColor(Color(190, 205, 220));
	                FloatRect subtitleBounds = subtitle.getLocalBounds();
	                subtitle.setPosition((width - subtitleBounds.width) / 2.f, 198.f);
	                window->draw(subtitle);
	                drawMenuOption(startButton, "Start", 250.f, 245.f, 220.f, 46.f);
	                drawMenuOption(exitButton, "Exit", 250.f, 305.f, 220.f, 46.f);
	            }
	            else if (currentScreen == "auth") {
	                drawThemedMenuBackground();
	                drawPanel(190.f, 130.f, 340.f, 245.f, Color(120, 235, 255));
	                drawCenteredTitle("PLAYER ACCESS", 158.f, 26, Color(120, 235, 255));
	                drawMenuOption(registerButton, "Register", 245.f, 230.f, 230.f, 46.f);
	                drawMenuOption(loginButton, "Login", 245.f, 290.f, 230.f, 46.f);
	                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 430.f);
	                window->draw(backButton);
	            }
            else if (currentScreen == "input") {
                drawThemedMenuBackground();
                drawPanel(105.f, 125.f, 510.f, 245.f, Color(120, 235, 255));
                drawCenteredTitle(enteringUsername ? "ENTER USERNAME" : "ENTER PASSWORD", 155.f, 24, Color(120, 235, 255));

                usernameText.setString("Username");
                passwordText.setString("Password");
                passwordToggleText.setString(passwordVisible ? "Hide Password" : "Show Password");
                usernameText.setCharacterSize(18);
                passwordText.setCharacterSize(18);
                usernameText.setFillColor(Color(255, 215, 120));
                passwordText.setFillColor(Color(255, 215, 120));
                usernameText.setPosition(160.f, 210.f);
                passwordText.setPosition(160.f, 275.f);

                Text usernameValue, passwordValue;
                usernameValue.setFont(font);
                passwordValue.setFont(font);
                usernameValue.setCharacterSize(22);
                passwordValue.setCharacterSize(22);
                usernameValue.setFillColor(enteringUsername ? Color::White : Color(185, 195, 210));
                passwordValue.setFillColor(!enteringUsername ? Color::White : Color(185, 195, 210));
                usernameValue.setString(usernameInput.empty() ? "Type username..." : usernameInput);
                passwordValue.setString(passwordInput.empty() ? "Type password..." : (passwordVisible ? passwordInput : string(passwordInput.length(), '*')));
                usernameValue.setPosition(160.f, 235.f);
                passwordValue.setPosition(160.f, 300.f);

                RectangleShape usernameLine(Vector2f(400.f, 2.f));
                RectangleShape passwordLine(Vector2f(400.f, 2.f));
                usernameLine.setPosition(160.f, 265.f);
                passwordLine.setPosition(160.f, 330.f);
                usernameLine.setFillColor(enteringUsername ? Color(120, 235, 255) : Color(255, 255, 255, 70));
	                passwordLine.setFillColor(!enteringUsername ? Color(120, 235, 255) : Color(255, 255, 255, 70));
	                instructionText2.setString(enteringUsername ? "Continue" : chosenAuthOption);
	                instructionText2.setCharacterSize(17);
	                instructionText2.setPosition(455.f, 338.f);
	                instructionText2.setFillColor(isMouseOverText(instructionText2, window->mapPixelToCoords(Mouse::getPosition(*window))) ? Color(255, 215, 120) : Color(120, 235, 255));

	                window->draw(usernameText);
	                window->draw(usernameValue);
                window->draw(usernameLine);
                window->draw(passwordText);
                window->draw(passwordValue);
                window->draw(passwordLine);
	                if (!enteringUsername) {
	                    passwordToggleText.setCharacterSize(16);
	                    passwordToggleText.setFillColor(Color(120, 235, 255));
	                    passwordToggleText.setPosition(160.f, 338.f);
	                    window->draw(passwordToggleText);
	                }
	                window->draw(instructionText2);
	                feedbackText.setPosition(145.f, 385.f);
	                window->draw(feedbackText);
            }
	            else if (currentScreen == "postLogin") {
	                drawThemedMenuBackground();
	                drawPanel(165.f, 30.f, 390.f, 420.f, Color(255, 215, 120));
	                drawCenteredTitle("GAME HUB", 52.f, 28, Color(255, 215, 120));
	                drawMenuOption(newGameButton, "New Game", 205.f, 100.f, 310.f, 35.f);
	                drawMenuOption(loadGameButton, "Load Game", 205.f, 145.f, 310.f, 35.f);
	                drawMenuOption(MultiplayerButton, "Multiplayer", 205.f, 190.f, 310.f, 35.f);
	                drawMenuOption(inventoryButton, "Inventory", 205.f, 235.f, 310.f, 35.f);
	                drawMenuOption(leaderboardButton, "LeaderBoard", 205.f, 280.f, 310.f, 35.f);
	                drawMenuOption(profileButton, "Profile", 205.f, 325.f, 310.f, 35.f);
	                drawMenuOption(friendsButton, "Friends", 205.f, 370.f, 310.f, 35.f);
	                drawMenuOption(instructionsButton, "Instructions", 205.f, 415.f, 310.f, 35.f);
	            }
            else if (currentScreen == "leaderboard") {
                drawThemedMenuBackground();
                RectangleShape leaderboardPanel(Vector2f(500.f, 340.f));
                leaderboardPanel.setFillColor(Color(15, 20, 28, 220));
                leaderboardPanel.setOutlineThickness(2.f);
                leaderboardPanel.setOutlineColor(Color(255, 215, 120));
                leaderboardPanel.setPosition(110.f, 55.f);
                window->draw(leaderboardPanel);

                Text leaderboardTitle;
                leaderboardTitle.setFont(font);
                leaderboardTitle.setString("LEADERBOARD");
                leaderboardTitle.setCharacterSize(28);
                leaderboardTitle.setStyle(Text::Bold);
                leaderboardTitle.setFillColor(Color(255, 215, 120));
                FloatRect titleBounds = leaderboardTitle.getLocalBounds();
                leaderboardTitle.setPosition((width - titleBounds.width) / 2.f, 70.f);
                window->draw(leaderboardTitle);

                Text rankText, scoreText;
                rankText.setFont(font);
                rankText.setCharacterSize(18);
                rankText.setStyle(Text::Bold);
                scoreText.setFont(font);
                scoreText.setCharacterSize(18);
                scoreText.setStyle(Text::Bold);

                for (int i = 0; i < leaderboardCount; ++i) {
                    RectangleShape row(Vector2f(420.f, 24.f));
                    row.setPosition(150.f, 108.f + i * 28.f);
                    row.setFillColor(i % 2 == 0 ? Color(255, 255, 255, 28) : Color(255, 255, 255, 12));
                    window->draw(row);

                    Color rowColor = Color(230, 236, 245);
                    if (i == 0) rowColor = Color(255, 215, 120);
                    else if (i == 1) rowColor = Color(210, 220, 235);
                    else if (i == 2) rowColor = Color(220, 155, 95);

                    rankText.setFillColor(rowColor);
                    rankText.setString(to_string(i + 1));
                    rankText.setPosition(165.f, 110.f + i * 28.f);
                    window->draw(rankText);

                    leaderboardEntries[i].setFillColor(rowColor);
                    window->draw(leaderboardEntries[i]);

                    scoreText.setFillColor(rowColor);
                    scoreText.setString(to_string(leaderboardScores[i]));
                    FloatRect scoreBounds = scoreText.getLocalBounds();
                    scoreText.setPosition(545.f - scoreBounds.width, 110.f + i * 28.f);
                    window->draw(scoreText);
                }
                backButton.setFillColor(Color(120, 235, 255));
                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 410.f);
	                window->draw(backButton);
	            }
	            else if (currentScreen == "instructions") {
	                drawThemedMenuBackground();
	                drawPanel(80.f, 48.f, 560.f, 360.f, Color(255, 215, 120));
	                drawCenteredTitle("CONTROLS", 68.f, 28, Color(255, 215, 120));

		                const string labels[] = {
		                    "Menus", "Select", "Back",
		                    "Solo", "P1 Move", "P1 Power",
		                    "P2 Move", "P2 Power",
		                    "Login", "Profile"
		                };
		                const string actions[] = {
		                    "Arrows or mouse hover", "Enter or mouse click", "B, Esc, or back hint",
		                    "Arrow keys", "Arrow keys", "P",
		                    "W A S D", "Space",
		                    "Click field to edit", "H or Show/Hide"
		                };

		                Text labelText, actionText;
		                labelText.setFont(font);
		                actionText.setFont(font);
		                labelText.setCharacterSize(15);
		                actionText.setCharacterSize(15);
		                labelText.setStyle(Text::Bold);

		                for (int i = 0; i < 10; ++i) {
		                    float y = 122.f + i * 26.f + (i >= 3 ? 16.f : 0.f) + (i >= 8 ? 16.f : 0.f);
		                    labelText.setString(labels[i]);
		                    labelText.setFillColor(Color(255, 215, 120));
		                    labelText.setPosition(145.f, y);
		                    actionText.setString(actions[i]);
		                    actionText.setFillColor(Color(230, 236, 245));
		                    actionText.setPosition(310.f, y);
		                    window->draw(labelText);
		                    window->draw(actionText);
		                }

	                backButton.setFillColor(Color(120, 235, 255));
	                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 430.f);
	                window->draw(backButton);
	            }
	            else if (currentScreen == "Inventory") {
                drawThemedMenuBackground();
                drawPanel(50.f, 35.f, 620.f, 365.f);
                drawCenteredTitle("SELECT THEME", 52.f);

                int visibleCount = 5;
                int lastVisible = min(themeCount, themeScrollOffset + visibleCount);
                for (int i = themeScrollOffset; i < lastVisible; ++i) {
                    int visibleRow = i - themeScrollOffset;
                    float rowX = 72.f;
                    float rowY = 96.f + visibleRow * 58.f;

                    RectangleShape row(Vector2f(575.f, 50.f));
                    row.setPosition(rowX, rowY);
                    row.setFillColor(i == selectedThemeIndex ? Color(255, 215, 120, 72) : Color(255, 255, 255, 16));
                    row.setOutlineThickness(i == selectedThemeIndex ? 2.f : 1.f);
                    row.setOutlineColor(i == selectedThemeIndex ? Color(255, 215, 120) : Color(255, 255, 255, 45));
                    window->draw(row);

	                    Texture* previewTexture = getThemeTextureByIndex(i);
	                    Sprite preview(*previewTexture);
                    preview.setTextureRect(IntRect(0, 0, previewTexture->getSize().x, previewTexture->getSize().y));
                    preview.setPosition(rowX + 12.f, rowY + 8.f);
                    preview.setScale(92.f / previewTexture->getSize().x, 34.f / previewTexture->getSize().y);
                    window->draw(preview);

                    themeDisplay[i].setFillColor(i == selectedThemeIndex ? Color(255, 245, 210) : Color(225, 232, 242));
                    themeDisplay[i].setPosition(rowX + 125.f, rowY + 9.f);
                    window->draw(themeDisplay[i]);
                }

                if (themeCount > visibleCount) {
                    RectangleShape scrollTrack(Vector2f(4.f, 285.f));
                    scrollTrack.setPosition(655.f, 96.f);
                    scrollTrack.setFillColor(Color(255, 255, 255, 45));
                    window->draw(scrollTrack);
                    float thumbHeight = 285.f * static_cast<float>(visibleCount) / static_cast<float>(themeCount);
                    float thumbY = 96.f + (285.f - thumbHeight) * static_cast<float>(themeScrollOffset) / static_cast<float>(max(1, themeCount - visibleCount));
                    RectangleShape scrollThumb(Vector2f(4.f, thumbHeight));
                    scrollThumb.setPosition(655.f, thumbY);
                    scrollThumb.setFillColor(Color(255, 215, 120));
                    window->draw(scrollThumb);
                }

                if (!themeConfirmText.getString().isEmpty()) {
                    themeConfirmText.setCharacterSize(18);
                    themeConfirmText.setFillColor(Color(255, 215, 120));
                    FloatRect confirmBounds = themeConfirmText.getLocalBounds();
                    themeConfirmText.setPosition((width - confirmBounds.width) / 2.f, 358.f);
                    window->draw(themeConfirmText);
                }
                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 410.f);
                window->draw(backButton);
            }
            else if (currentScreen == "Profile") {
                drawThemedMenuBackground();
                drawPanel(100.f, 48.f, 520.f, 360.f, Color(120, 235, 255));
                drawCenteredTitle("PLAYER PROFILE", 68.f, 27, Color(120, 235, 255));

                usernameTextP.setString("Username: " + userName);
                passwordTextP.setString("Password: " + (profilePasswordVisible ? password : string(password.length(), '*')));
                profilePasswordToggleText.setString(profilePasswordVisible ? "Hide Password" : "Show Password");
                scoreTextP.setString("Total Score: " + to_string(totalScore));
                usernameTextP.setCharacterSize(22);
                passwordTextP.setCharacterSize(22);
                scoreTextP.setCharacterSize(22);
                historyHeader.setCharacterSize(20);
                usernameTextP.setPosition(145.f, 116.f);
                passwordTextP.setPosition(145.f, 154.f);
                profilePasswordToggleText.setPosition(145.f, 188.f);
                profilePasswordToggleText.setFillColor(Color(120, 235, 255));
                scoreTextP.setPosition(145.f, 232.f);
                historyHeader.setPosition(145.f, 278.f);
                historyHeader.setFillColor(Color(255, 215, 120));

                window->draw(usernameTextP);
                window->draw(passwordTextP);
                window->draw(profilePasswordToggleText);
                window->draw(scoreTextP);
                window->draw(historyHeader);

                for (int i = 0; i < 5; i++) {
                    Text historyEntry;
                    historyEntry.setFont(font);

                    if (history[i] == 'W' || history[i] == 'w') {
                        historyEntry.setString("Match " + to_string(i + 1) + ": Win");
                        historyEntry.setFillColor(Color(0, 255, 0)); // Green for wins
                    }
                    else if (history[i] == 'L' || history[i] == 'l') {
                        historyEntry.setString("Match " + to_string(i + 1) + ": Loss");
                        historyEntry.setFillColor(Color(220, 20, 60)); // Crimson for losses
                    }
                    else {
                        // Show placeholder for empty history slots
                        historyEntry.setString("Match " + to_string(i + 1) + ": --");
                        historyEntry.setFillColor(sf::Color::White); // Grey for empty slots
                    }

                    historyEntry.setCharacterSize(17);
                    historyEntry.setPosition(175.f, 314.f + (i * 18.f));
                    window->draw(historyEntry);
                }

                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 430.f);
                window->draw(backButton);
            }
	            else if (currentScreen == "dashboard") {
	                drawThemedMenuBackground();
	                drawPanel(125.f, 65.f, 470.f, 315.f, Color(120, 235, 255));
	                drawCenteredTitle("FRIENDS", 92.f, 28, Color(120, 235, 255));
	                friendListButton.setCharacterSize(25);
	                searchButton.setCharacterSize(25);
	                receivedRequestsButton.setCharacterSize(23);
	                drawMenuOption(friendListButton, "Friend List", 180.f, 145.f, 360.f, 45.f);
	                drawMenuOption(searchButton, "Search", 180.f, 205.f, 360.f, 45.f);
	                drawMenuOption(receivedRequestsButton, "Received Requests", 180.f, 265.f, 360.f, 45.f);
	                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 430.f);
	                window->draw(backButton);

            }
            else if (currentScreen == "searchFriend") {
                drawThemedMenuBackground();
                drawPanel(110.f, 105.f, 500.f, 230.f, Color(120, 235, 255));
                drawCenteredTitle("SEARCH PLAYERS", 130.f, 24, Color(120, 235, 255));
                instructionText.setString("Username");
                instructionText.setCharacterSize(18);
                instructionText.setFillColor(Color(190, 205, 220));
                instructionText.setPosition(165.f, 185.f);
                usernameText_search.setString(usernameInput_search.empty() ? "Type a username..." : usernameInput_search);
                usernameText_search.setCharacterSize(22);
                usernameText_search.setFillColor(usernameInput_search.empty() ? Color(145, 155, 170) : Color::White);
                usernameText_search.setPosition(165.f, 220.f);
                RectangleShape inputLine(Vector2f(390.f, 2.f));
	                inputLine.setPosition(165.f, 255.f);
	                inputLine.setFillColor(Color(120, 235, 255));
	                instructionText2.setString("Send Request");
	                instructionText2.setCharacterSize(18);
	                instructionText2.setPosition(165.f, 280.f);
	                instructionText2.setFillColor(isMouseOverText(instructionText2, window->mapPixelToCoords(Mouse::getPosition(*window))) ? Color(255, 215, 120) : Color(120, 235, 255));
	                window->draw(instructionText);
	                window->draw(usernameText_search);
	                window->draw(inputLine);
	                window->draw(instructionText2);
	                feedbackText.setPosition(150.f, 320.f);
	                window->draw(feedbackText);
	                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 430.f);
	                window->draw(backButton);
	            }
            else if (currentScreen == "friendlist") {
                drawThemedMenuBackground();
                drawPanel(115.f, 55.f, 490.f, 335.f, Color(120, 235, 255));
                drawCenteredTitle("FRIEND LIST", 78.f, 26, Color(120, 235, 255));

                int lineCount = 0;
                hashtable.create_keys();
                hashtable.buildFriendChains();
                //                hashtable.printFriendChains();
                Playerlist* friendChain = hashtable.getFriendChain(usernameInput);
                if (friendChain == nullptr) {
                    instructionText.setString("You have no friends yet.");
                    instructionText.setCharacterSize(22);
                    instructionText.setFillColor(Color(220, 230, 245));
                    instructionText.setPosition(220, 205);
                    window->draw(instructionText);
                }
                else {
                    while (friendChain != nullptr) {
                        RectangleShape row(Vector2f(400.f, 32.f));
                        row.setPosition(160.f, 125.f + lineCount * 42.f);
                        row.setFillColor(lineCount % 2 == 0 ? Color(255, 255, 255, 24) : Color(255, 255, 255, 12));
                        window->draw(row);
                        instructionText.setString(to_string(lineCount + 1) + "    " + friendChain->username);
                        instructionText.setCharacterSize(20);
                        instructionText.setFillColor(Color(230, 236, 245));
                        instructionText.setPosition(180, 129 + (lineCount * 42));
                        window->draw(instructionText);

                        friendChain = friendChain->next;
                        lineCount++;
                    }
                }
                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 410.f);
                window->draw(backButton);
            }
            else if (currentScreen == "receivedRequests") {
                drawThemedMenuBackground();
                drawPanel(95.f, 55.f, 530.f, 335.f, Color(120, 235, 255));
                drawCenteredTitle("RECEIVED REQUESTS", 78.f, 24, Color(120, 235, 255));
	                for (int i = 0; i < 20; ++i) {
	                    requestList[i].clear();
	                }
	                ifstream file(usernameInput + "receivedrequest.txt");

                if (file.is_open()) {
                    string requestUser;
                    int lineCount = 0;

	                    while (getline(file, requestUser) && lineCount < 20) {
	                        while (!requestUser.empty() && (requestUser.back() == '\r' || requestUser.back() == '\n')) {
	                            requestUser.pop_back();
	                        }
	                        if (requestUser.empty()) {
	                            continue;
	                        }
	                        requestList[lineCount] = requestUser;
                        RectangleShape row(Vector2f(455.f, 34.f));
                        row.setPosition(130.f, 122.f + lineCount * 46.f);
                        row.setFillColor(lineCount % 2 == 0 ? Color(255, 255, 255, 24) : Color(255, 255, 255, 12));
                        window->draw(row);

                        instructionText.setString(requestUser);
                        instructionText.setCharacterSize(18);
                        instructionText.setFillColor(Color(230, 236, 245));
                        instructionText.setPosition(150, 128 + (lineCount * 46));
                        window->draw(instructionText);


                        acceptButton[lineCount].setFont(font);
                        acceptButton[lineCount].setString("Accept");
                        acceptButton[lineCount].setCharacterSize(16);
                        acceptButton[lineCount].setFillColor(Color(90, 255, 155));
                        acceptButton[lineCount].setPosition(365, 128 + lineCount * 46);

                        window->draw(acceptButton[lineCount]);
                        // Reject Button
                        rejectButton[lineCount].setFont(font);
                        rejectButton[lineCount].setString("Reject");
                        rejectButton[lineCount].setCharacterSize(16);
                        rejectButton[lineCount].setFillColor(Color(255, 120, 120));
                        rejectButton[lineCount].setPosition(470, 128 + lineCount * 46);
                        window->draw(rejectButton[lineCount]);

                        lineCount++;
                    }
                    if (lineCount == 0) {
                        instructionText.setString("No pending requests.");
                        instructionText.setCharacterSize(22);
                        instructionText.setFillColor(Color(220, 230, 245));
                        instructionText.setPosition(225, 205);
                        window->draw(instructionText);
                    }

                    file.close();
                }
                backButton.setPosition((width - backButton.getLocalBounds().width) / 2.f, 410.f);
                window->draw(backButton);
            }

            window->display();
        }
        return gameShouldStart;
    }
};

#endif
