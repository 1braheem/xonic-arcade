#ifndef GAME_H
#define GAME_H

#include <SFML/Graphics.hpp>
#include <ctime>
#include "Menu.h"
#include "Enemy.h"
#include "ScoreManager.h"
#include "GridManager.h"
#include "Auth.h"
#include "Player.h"
#include "SaveLoad.h"
#include "LeaderBoard.h"
#include "multiplayer.h"

#define ResolutionX 720
#define ResolutionY 450

using namespace sf;
using namespace std;

const int ts = 18;
int freezeTimer = 0;
bool enemiesFrozen = false;

void runGame() {
    
    srand(static_cast<unsigned int>(time(0)));

    const int hudHeight = 54;
    const int boardHeight = M * ts;
    RenderWindow window(VideoMode(N * ts, boardHeight + hudHeight), "Xonix Game!");
    window.setFramerateLimit(60);

    Sprite backgroundSprite;
    Texture iceBlue, redLava, forestGreen, retro, sunsetGlow, monoGrid, auroraFlow, midnightSteel;

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    sf::Music gameOverMusic, music2;
    music2.openFromFile("Sound Effects/select.mp3");

    Font font;
    font.loadFromFile("Fonts/sqbold.otf");

    const int width = 720;
    const int height = 450;

    auto buildMonoGridGameplayTexture = [&]() {
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
        monoGrid.loadFromImage(image);
    };

    auto buildAuroraGameplayTexture = [&]() {
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
        auroraFlow.loadFromImage(image);
    };

    auto buildMidnightSteelGameplayTexture = [&]() {
        Image image;
        image.create(width, height, Color(12, 18, 28));
        for (unsigned int y = 0; y < static_cast<unsigned int>(height); ++y) {
            float t = static_cast<float>(y) / static_cast<float>(height - 1);
            for (unsigned int x = 0; x < static_cast<unsigned int>(width); ++x) {
                float stripe = ((x + y / 2) % 110 < 7) ? 0.18f : 0.f;
                float glow = (x > 260 && x < 460 && y > 130 && y < 360) ? 0.10f : 0.f;
                Color c(
                    static_cast<Uint8>(12 * (1.f - t) + 5 * t),
                    static_cast<Uint8>(18 * (1.f - t) + 38 * t),
                    static_cast<Uint8>(28 * (1.f - t) + 54 * t)
                );
                c.r = static_cast<Uint8>(min(255.f, c.r + 120.f * (stripe + glow)));
                c.g = static_cast<Uint8>(min(255.f, c.g + 170.f * (stripe + glow)));
                c.b = static_cast<Uint8>(min(255.f, c.b + 220.f * (stripe + glow)));
                image.setPixel(x, y, c);
            }
        }
        midnightSteel.loadFromImage(image);
    };

    auto applyBackgroundTheme = [&](int themeId) {
        Texture* activeTexture = &iceBlue;
        Color backgroundTint = Color::White;
        iceBlue.loadFromFile("Textures/iceBlue.jpeg");

        if (themeId == 102) {
            redLava.loadFromFile("Textures/redLava.jpeg");
            activeTexture = &redLava;
        }
        else if (themeId == 103) {
            forestGreen.loadFromFile("Textures/forestGreen.jpeg");
            activeTexture = &forestGreen;
        }
        else if (themeId == 104) {
            retro.loadFromFile("Textures/retro.jpeg");
            activeTexture = &retro;
        }
        else if (themeId == 106) {
            buildMonoGridGameplayTexture();
            activeTexture = &monoGrid;
        }
        else if (themeId == 105) {
            sunsetGlow.loadFromFile("Textures/sunsetGlow.jpeg");
            activeTexture = &sunsetGlow;
        }
        else if (themeId == 107) {
            buildAuroraGameplayTexture();
            activeTexture = &auroraFlow;
        }
        else if (themeId == 108) {
            buildMidnightSteelGameplayTexture();
            activeTexture = &midnightSteel;
        }

        backgroundSprite.setTexture(*activeTexture, true);
        backgroundSprite.setColor(backgroundTint);
        backgroundSprite.setScale((float)ResolutionX / activeTexture->getSize().x, (float)ResolutionY / activeTexture->getSize().y);
    };

    ScoreManager scoreSystem;

    RectangleShape hudBar(Vector2f(static_cast<float>(ResolutionX), static_cast<float>(hudHeight)));
    hudBar.setFillColor(Color(12, 16, 22, 190));
    hudBar.setPosition(0.f, static_cast<float>(boardHeight));

    Text scoreText, powerUpText;
    scoreText.setFont(font);
    scoreText.setCharacterSize(18);
    scoreText.setStyle(Text::Bold);
    scoreText.setFillColor(Color(245, 248, 255));

    powerUpText.setFont(font);
    powerUpText.setCharacterSize(18);
    powerUpText.setStyle(Text::Bold);
    powerUpText.setFillColor(Color(255, 220, 120));

    scoreText.setPosition(18.f, static_cast<float>(boardHeight + 15));
    powerUpText.setPosition(205.f, static_cast<float>(boardHeight + 15));
    sf::Music music4;
    Clock clock;

    bool isPowerUpActive = false;
    float powerUpTimer = 0.0f;
    const float powerUpDuration = 3.0f; // 3 second time for a powerup

    Menu menu(&window);


    // Loop if player die once it takes player back to menu
    bool checkClose = false;
    while (true) {
        if (checkClose == true) {
            break;
        }

        int check = menu.run();
        // If garbage value is returned from menu it will end the game
        if (check <= 0 || check >= 4) {
            return;
        }

        int gameDifficulty = 1;

        // Allows user to choose difficulty level of his choice.
        if (check == 1) {
            Text easyButton, mediumButton, hardButton;

            string instructionKey = "Easy";

            easyButton.setFont(font);
            easyButton.setString("Easy");
            easyButton.setCharacterSize(36);
            FloatRect easyBounds = easyButton.getLocalBounds();
            easyButton.setPosition((width - easyBounds.width) / 2, 140);

            mediumButton.setFont(font);
            mediumButton.setString("Medium");
            mediumButton.setCharacterSize(36);
            FloatRect mediumBounds = mediumButton.getLocalBounds();
            mediumButton.setPosition((width - mediumBounds.width) / 2, 220);

            hardButton.setFont(font);
            hardButton.setString("Hard");
            hardButton.setCharacterSize(36);
            FloatRect hardBounds = hardButton.getLocalBounds();
            hardButton.setPosition((width - hardBounds.width) / 2, 300);

            Text prompt;
            prompt.setFont(font);
            prompt.setCharacterSize(30);
            prompt.setFillColor(sf::Color(128, 0, 32));
            prompt.setString("Choose Difficulty Level:\n");
            prompt.setPosition((width - prompt.getLocalBounds().width) / 2, 50);

            RectangleShape darkOverlay;
            darkOverlay.setSize(Vector2f(N * ts, M * ts));
            darkOverlay.setFillColor(Color(0, 0, 0, 200));

            bool difficultyChosen = false;

            while (!difficultyChosen) {
                Event e;
                while (window.pollEvent(e)) {
                    if (e.type == Event::Closed) {
                        window.close();
                        return;
                    }

                    if (e.type == Event::KeyPressed) {

                        if (e.key.code == Keyboard::Up) {
                            if (instructionKey == "Easy") instructionKey = "Hard";
                            else if (instructionKey == "Medium") instructionKey = "Easy";
                            else if (instructionKey == "Hard") instructionKey = "Medium";
                        }
                        else if (e.key.code == Keyboard::Down) {
                            if (instructionKey == "Easy") instructionKey = "Medium";
                            else if (instructionKey == "Medium") instructionKey = "Hard";
                            else if (instructionKey == "Hard") instructionKey = "Easy";
                        }
                        else if (e.key.code == Keyboard::Enter) {
                            music2.play();
                            if (instructionKey == "Easy") {
                               
                                gameDifficulty = 1;
                                difficultyChosen = true;
                            }
                            else if (instructionKey == "Medium") {      
                                gameDifficulty = 2;
                                difficultyChosen = true;
                            }
                            else if (instructionKey == "Hard") {
                                gameDifficulty = 3;
                                difficultyChosen = true;
                            }
                        }
                    }
                }

                window.clear();

                int flag = menu.getThemeID();
                applyBackgroundTheme(flag);

                easyButton.setFillColor(sf::Color(200, 200, 200));
                mediumButton.setFillColor(sf::Color(200, 200, 200));
                hardButton.setFillColor(sf::Color(200, 200, 200));

                if (instructionKey == "Easy") {
                    easyButton.setFillColor(sf::Color(128, 0, 32));
                }
                else if (instructionKey == "Medium") {
                    mediumButton.setFillColor(sf::Color(128, 0, 32));
                }
                else if (instructionKey == "Hard") {
                    hardButton.setFillColor(sf::Color(128, 0, 32));
                }

                window.clear();
                window.draw(backgroundSprite);
                window.draw(darkOverlay);
                window.draw(prompt);
                window.draw(easyButton);
                window.draw(mediumButton);
                window.draw(hardButton);
                window.display();
            }
        }

        // If user chooces to play a multiplayer game it will call multiplayer function
        int flag = menu.getThemeID();
        if (check == 3) {
            int check2 = multiplayer(&window, flag);
            if (check2 == 1) {
                continue;


            }
            else {
                return;
            }
        }

        // Applying a background theme of users choics
        applyBackgroundTheme(flag);

        const int enemyCount = 3;
        Enemy enemies[enemyCount] = { Enemy(gameDifficulty), Enemy(gameDifficulty), Enemy(gameDifficulty) };

        bool Game = true;
        int x = 0, y = 0, dx = 0, dy = 0;
        float timer = 0, delay = 0.07f;

        // setting boundry wall (1 means wall, 0 means empty space)
        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                if (i == 0 || j == 0 || i == M - 1 || j == N - 1) {
                    grid[i][j] = 1;
                }
            }
        }

        // Reset grid to initial state (walls on borders, rest empty)
        if (check == 1) {
            music4.openFromFile("Sound Effects/sp.mp3");
            music4.setLoop(true);  // this makes it loop forever
            music4.play();
            for (int i = 0; i < M; ++i) {
                for (int j = 0; j < N; ++j) {
                    if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                        grid[i][j] = 1;
                    else
                        grid[i][j] = 0;
                }
            }
            scoreSystem.setScore(0);
            scoreSystem.setPowerUps(0);

        }

        // If user wants to load a previously saved game.
        if (check == 2) {
            string username = menu.getusername();
            loadGame(username, grid, x, y, enemies, enemyCount, scoreSystem, sTile, sEnemy);
            scoreText.setString("Score: " + to_string(scoreSystem.getScore()));
       
            music4.openFromFile("Sound Effects/sp.mp3");
                music4.setLoop(true);  // this makes it loop forever
                music4.play();
            
        }

        while (window.isOpen()) {

            float time = clock.restart().asSeconds();
            timer += time;

            Event e;
            while (window.pollEvent(e)) {
                if (e.type == Event::Closed) {
                    checkClose = true;
                    window.close();
                }
                // When u press esc this function will reomve all the area captured by player.
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
                    for (int i = 1; i < M - 1; i++) {
                        for (int j = 1; j < N - 1; j++) {
                            grid[i][j] = 0;
                        }
                    }

                    x = 0;
                    y = 0;
                    Game = true;
                }

                // Pressing space user will use one powerup
                if (e.type == Event::KeyPressed && e.key.code == Keyboard::Space) {
                    if (scoreSystem.usePowerUp()) {
                        isPowerUpActive = true;
                        powerUpTimer = 0.0f; // Reset the power-up timer
                        for (int i = 0; i < enemyCount; ++i) {
                            enemies[i].savedDx = enemies[i].dx;  //saving current speed level ke hisaab se.
                            enemies[i].savedDy = enemies[i].dy;
                            enemies[i].dx = 0;
                            enemies[i].dy = 0;  // Freeze karne ke liye 0 kar diye.
                        }
                    }
                }
            }

            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                dx = -1;
                dy = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                dx = 1;
                dy = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                dx = 0;
                dy = -1;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                dx = 0;
                dy = 1;
            }

            // Press S if u want to save your game
            if (Keyboard::isKeyPressed(Keyboard::S)) {
                string pass = menu.getusername();
                saveGame(pass, grid, x, y, enemies, enemyCount, scoreSystem);
            }

            // Loop that draw tiles on screen
            for (int i = 0; i < M; i++)
                for (int j = 0; j < N; j++) {
                    if (grid[i][j] == 1 || grid[i][j] == 2) {
                        sTile.setPosition(j * ts, i * ts);
                        window.draw(sTile);
                    }
                }

            sTile.setPosition(x * ts, y * ts);
            window.draw(sTile);

            for (int i = 0; i < enemyCount; ++i) {
                sEnemy.setPosition(enemies[i].x, enemies[i].y);
                window.draw(sEnemy);
            }

            if (!Game)
                continue;

            if (timer > delay) {
                x += dx;
                y += dy;
                x = clamp(x, 0, N - 1);
                y = clamp(y, 0, M - 1);

                if (grid[y][x] == 2)
                {music4.stop();

                    gameOverMusic.openFromFile("Sound Effects/gameover.mp3");
                    // Set playback to start at 27 seconds
                    gameOverMusic.setPlayingOffset(sf::seconds(27));
                    gameOverMusic.play();
                    Game = false;
                }
                if (grid[y][x] == 0) {
                    grid[y][x] = 2;
                }
                timer = 0;
            }

            // Resume enemy movement after the power-up duration ends
            if (isPowerUpActive) {
                powerUpTimer += time;
                if (powerUpTimer >= powerUpDuration) {
                    isPowerUpActive = false;
                    for (int i = 0; i < enemyCount; ++i) {
                        enemies[i].dx = enemies[i].savedDx;
                        enemies[i].dy = enemies[i].savedDy;
                    }
                }
            }

            // Move enemies if the power-up is not active
            if (!isPowerUpActive) {
                for (int i = 0; i < enemyCount; ++i) {
                    enemies[i].move();
                }
            }

            if (grid[y][x] == 1) {
                dx = dy = 0;

                for (int i = 0; i < enemyCount; ++i)
                    drop(enemies[i].y / ts, enemies[i].x / ts);

                // Fills the area captured by player
                int captured = 0;
                for (int i = 0; i < M; i++) {
                    for (int j = 0; j < N; j++) {
                        if (grid[i][j] == 2) {
                            captured++;
                            grid[i][j] = 1;
                        }
                        else if (grid[i][j] == 0) {
                            captured++;
                            grid[i][j] = 1;
                        }
                        else if (grid[i][j] == -1) {
                            grid[i][j] = 0; // reset flooded area
                        }
                    }
                }
                scoreSystem.updateScore(captured); // update score according to the area captured
            }

            for (int i = 0; i < enemyCount; ++i) {
                if (grid[enemies[i].y / ts][enemies[i].x / ts] == 2) {
                    music4.stop();
                    gameOverMusic.openFromFile("Sound Effects/gameover.mp3");
                    // Set playback to start at 27 seconds
                    gameOverMusic.setPlayingOffset(sf::seconds(27));
                    gameOverMusic.play();

                    Game = false;
                }
            }

            // Update the score and power-up texts
            scoreText.setString("SCORE  " + to_string(scoreSystem.getScore()));
            powerUpText.setString("POWER-UPS  " + to_string(scoreSystem.getPowerUps()));

            // Draw everything
            window.clear();
            window.draw(backgroundSprite);

	            int borderTile = 0;
	            int trailTile = 18;
	            int playerTile = 36;
	            if (flag == 102) {
	                borderTile = 36; trailTile = 72; playerTile = 54;
	            }
	            else if (flag == 103) {
	                borderTile = 54; trailTile = 72; playerTile = 108;
	            }
	            else if (flag == 104) {
	                borderTile = 18; trailTile = 90; playerTile = 36;
	            }
	            else if (flag == 105) {
	                borderTile = 108; trailTile = 0; playerTile = 36;
	            }
	            else if (flag == 106) {
	                borderTile = 90; trailTile = 0; playerTile = 18;
	            }
	            else if (flag == 107) {
	                borderTile = 54; trailTile = 108; playerTile = 72;
	            }
	            else if (flag == 108) {
	                borderTile = 0; trailTile = 90; playerTile = 18;
	            }

	            bool useGeneratedTileTheme = (flag == 106 || flag == 107 || flag == 108);
	            RectangleShape themedTile(Vector2f(static_cast<float>(ts), static_cast<float>(ts)));
	            themedTile.setOutlineThickness(1.5f);
	            RectangleShape innerTile(Vector2f(static_cast<float>(ts - 5), static_cast<float>(ts - 5)));
	            RectangleShape tileLine;
	            auto liftColor = [](Color c, int amount) {
	                return Color(
	                    static_cast<Uint8>(min(255, c.r + amount)),
	                    static_cast<Uint8>(min(255, c.g + amount)),
	                    static_cast<Uint8>(min(255, c.b + amount))
	                );
	            };
	            auto shadeColor = [](Color c, int amount) {
	                return Color(
	                    static_cast<Uint8>(max(0, c.r - amount)),
	                    static_cast<Uint8>(max(0, c.g - amount)),
	                    static_cast<Uint8>(max(0, c.b - amount))
	                );
	            };
	            auto singleTileColor = [&](int value, bool isPlayer) {
	                if (flag == 106) {
	                    if (isPlayer) return Color(245, 245, 250);
	                    return value == 1 ? Color(72, 74, 84) : Color(232, 234, 242);
	                }
	                if (flag == 107) {
	                    if (isPlayer) return Color(255, 220, 110);
	                    return value == 1 ? Color(0, 220, 205) : Color(215, 80, 255);
	                }
	                if (isPlayer) return Color(232, 242, 255);
	                return value == 1 ? Color(96, 122, 148) : Color(178, 212, 242);
	            };
	            auto singleOutlineColor = [&]() {
	                if (flag == 106) return Color(18, 20, 24);
	                if (flag == 107) return Color(8, 35, 48);
	                return Color(20, 30, 42);
	            };
	            auto drawGeneratedSingleTile = [&](RenderWindow& target, int gridValue, bool isPlayer, float px, float py) {
	                Color base = singleTileColor(gridValue, isPlayer);
	                themedTile.setFillColor(shadeColor(base, 24));
	                themedTile.setOutlineColor(singleOutlineColor());
	                themedTile.setPosition(px, py);
	                target.draw(themedTile);

	                innerTile.setFillColor(base);
	                innerTile.setOutlineThickness(0.f);
	                innerTile.setPosition(px + 2.f, py + 2.f);
	                target.draw(innerTile);

	                tileLine.setFillColor(liftColor(base, 35));
	                tileLine.setSize(Vector2f(static_cast<float>(ts - 4), 2.f));
	                tileLine.setPosition(px + 2.f, py + 2.f);
	                target.draw(tileLine);
	                tileLine.setSize(Vector2f(2.f, static_cast<float>(ts - 4)));
	                target.draw(tileLine);

	                tileLine.setFillColor(shadeColor(base, 45));
	                tileLine.setSize(Vector2f(static_cast<float>(ts - 4), 2.f));
	                tileLine.setPosition(px + 2.f, py + ts - 4.f);
	                target.draw(tileLine);
	                tileLine.setSize(Vector2f(2.f, static_cast<float>(ts - 4)));
	                tileLine.setPosition(px + ts - 4.f, py + 2.f);
	                target.draw(tileLine);
	            };

	            for (int i = 0; i < M; i++) {
	                for (int j = 0; j < N; j++) {
	                    if (grid[i][j] == 0) {
	                        continue;
	                    }
	                    if (useGeneratedTileTheme) {
	                        drawGeneratedSingleTile(window, grid[i][j], false, static_cast<float>(j * ts), static_cast<float>(i * ts));
	                    }
	                    else {
	                        sTile.setTextureRect(IntRect(grid[i][j] == 1 ? borderTile : trailTile, 0, ts, ts));
	                        sTile.setPosition(j * ts, i * ts);
	                        window.draw(sTile);
	                    }
	                }
	            }

	            if (useGeneratedTileTheme) {
	                drawGeneratedSingleTile(window, 2, true, static_cast<float>(x * ts), static_cast<float>(y * ts));
	            }
	            else {
	                sTile.setTextureRect(IntRect(playerTile, 0, ts, ts));
	                sTile.setPosition(x * ts, y * ts);
	                window.draw(sTile);
	            }

            sEnemy.rotate(10);
            for (int i = 0; i < enemyCount; ++i) {

                sEnemy.setPosition(enemies[i].x, enemies[i].y);
                window.draw(sEnemy);
            }

            if (!Game) {
                string name = menu.getusername();
                Auth auth;
                auth.updateScore(name, scoreSystem.getScore());
                auth.sortData();
                leaderBoard();
                window.draw(sGameover);
                window.display();
                sf::sleep(sf::seconds(3));
                break;
            }

            window.draw(hudBar);
            window.draw(scoreText);
            window.draw(powerUpText);
            window.display();
        }
    }
}

#endif
