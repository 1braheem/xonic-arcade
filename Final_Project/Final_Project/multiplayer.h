#ifndef MULTIPLAYER_H
#define MULTIPLAYER_H

#include <SFML/Graphics.hpp>
#include <ctime>
#include <vector>
#include "Menu.h"
#include "Enemy.h"
#include "ScoreManager.h"
#include "GridManager.h"
#include "Auth.h"
#include "Player.h"
#include "LeaderBoard.h"
#include "Game.h"

#define ResolutionX 720
#define ResolutionY 450

using namespace sf;
using namespace std;

int currentPlayer = 1;

extern string nameP1;
extern string nameP2;

int multiplayer(RenderWindow* win, int theme) {
    sf::Music music4;
    if (music4.openFromFile("Sound Effects/Hype.mp3")) {
        music4.setLoop(true);  // this makes it loop forever
        music4.play();
    }
    srand(static_cast<unsigned int>(time(0)));
    RenderWindow* window = win;
    window->setFramerateLimit(60);

    Sprite backgroundSprite;
    Texture iceBlue, redLava, forestGreen, retro, sunsetGlow, monoGrid, auroraFlow, midnightSteel;
    auto buildMonoGridGameplayTexture = [&]() {
        Image image;
        image.create(ResolutionX, ResolutionY, Color(8, 8, 10));
        for (unsigned int y = 0; y < static_cast<unsigned int>(ResolutionY); ++y) {
            for (unsigned int x = 0; x < static_cast<unsigned int>(ResolutionX); ++x) {
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
        image.create(ResolutionX, ResolutionY, Color(8, 18, 28));
        for (unsigned int y = 0; y < static_cast<unsigned int>(ResolutionY); ++y) {
            for (unsigned int x = 0; x < static_cast<unsigned int>(ResolutionX); ++x) {
                float wave = static_cast<float>((x + y * 2) % 180) / 180.f;
                float band = (wave > 0.42f && wave < 0.56f) ? 1.f : 0.f;
                Color c(
                    static_cast<Uint8>(10 + y * 30 / ResolutionY + band * 60),
                    static_cast<Uint8>(26 + x * 70 / ResolutionX + band * 150),
                    static_cast<Uint8>(48 + y * 70 / ResolutionY + band * 110)
                );
                image.setPixel(x, y, c);
            }
        }
        auroraFlow.loadFromImage(image);
    };
    auto buildMidnightSteelGameplayTexture = [&]() {
        Image image;
        image.create(ResolutionX, ResolutionY, Color(12, 18, 28));
        for (unsigned int y = 0; y < static_cast<unsigned int>(ResolutionY); ++y) {
            float t = static_cast<float>(y) / static_cast<float>(ResolutionY - 1);
            for (unsigned int x = 0; x < static_cast<unsigned int>(ResolutionX); ++x) {
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

    Music gameOverMusic, music2, playerdeath, gameOverMusic2;
    music2.openFromFile("Sound Effects/select.mp3");

    Font font;
    font.loadFromFile("Fonts/sqbold.otf");

    Text deathnote, deathnote2;
    deathnote.setFont(font);
    deathnote2.setFont(font);

    bool showDeathNote = false;
    bool showDeathNote2 = false;

    Clock clock, deathClock, deathClock2;

    Texture t1, t2, t3;
    t1.loadFromFile("images/tiles.png");
    t2.loadFromFile("images/gameover.png");
    t3.loadFromFile("images/enemy.png");

    Sprite sTile(t1), sGameover(t2), sEnemy(t3);
    sGameover.setPosition(100, 100);
    sEnemy.setOrigin(20, 20);

    ScoreManager scoreP1, scoreP2;
    const float hudTop = static_cast<float>(M * ts);
    RectangleShape hudBar(Vector2f(static_cast<float>(ResolutionX), 54.f));
    hudBar.setFillColor(Color(12, 16, 22, 205));
    hudBar.setPosition(0.f, hudTop);

    RectangleShape centerDivider(Vector2f(2.f, 34.f));
    centerDivider.setFillColor(Color(255, 255, 255, 80));
    centerDivider.setPosition(static_cast<float>(ResolutionX / 2), hudTop + 10.f);

    Text scoreTextP1, powerUpTextP1, scoreTextP2, powerUpTextP2;

    scoreTextP1.setFont(font);
    scoreTextP2.setFont(font);
    scoreTextP1.setCharacterSize(17);
    scoreTextP2.setCharacterSize(17);
    scoreTextP1.setStyle(Text::Bold);
    scoreTextP2.setStyle(Text::Bold);
    scoreTextP1.setPosition(18.f, hudTop + 8.f);
    scoreTextP2.setPosition(ResolutionX - 250.f, hudTop + 8.f);
    scoreTextP1.setFillColor(Color(100, 225, 255));
    scoreTextP2.setFillColor(Color(255, 120, 220));

    powerUpTextP1.setFont(font);
    powerUpTextP2.setFont(font);
    powerUpTextP1.setCharacterSize(14);
    powerUpTextP2.setCharacterSize(14);
    powerUpTextP1.setPosition(18.f, hudTop + 31.f);
    powerUpTextP2.setPosition(ResolutionX - 250.f, hudTop + 31.f);
    powerUpTextP1.setFillColor(Color(220, 245, 255));
    powerUpTextP2.setFillColor(Color(255, 225, 248));

    Vector2f windowSize = window->getView().getSize();
    RectangleShape resultOverlay(Vector2f(windowSize.x, windowSize.y));
    resultOverlay.setFillColor(Color(0, 0, 0, 145));

    RectangleShape resultPanel(Vector2f(430.f, 210.f));
    resultPanel.setFillColor(Color(18, 23, 31, 235));
    resultPanel.setOutlineThickness(2.f);
    resultPanel.setOutlineColor(Color(255, 215, 120));
    resultPanel.setPosition((ResolutionX - resultPanel.getSize().x) / 2.f, 108.f);

    Text winnerTitle, winnerNameText, finalScoreText, resultHintText;
    winnerTitle.setFont(font);
    winnerTitle.setCharacterSize(22);
    winnerTitle.setStyle(Text::Bold);
    winnerTitle.setFillColor(Color(255, 215, 120));

    winnerNameText.setFont(font);
    winnerNameText.setCharacterSize(34);
    winnerNameText.setStyle(Text::Bold);
    winnerNameText.setFillColor(Color::White);

    finalScoreText.setFont(font);
    finalScoreText.setCharacterSize(18);
    finalScoreText.setFillColor(Color(220, 230, 245));

    resultHintText.setFont(font);
    resultHintText.setCharacterSize(14);
    resultHintText.setFillColor(Color(165, 178, 195));

    bool isPowerUpActiveP1 = false;
    bool isPowerUpActiveP2 = false;
    float powerUpTimerP1 = 0.0f;
    float powerUpTimerP2 = 0.0f;
    const float powerUpDuration = 3.0f;

    const int width = 720;
    const int heght = 450;

    int gameDifficulty = 1;
    bool resultSaved = false;

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
        while (window->pollEvent(e)) {
            if (e.type == Event::Closed) {
                window->close();
                return -1;
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

        window->clear();

        backgroundSprite.setColor(Color::White);
        if (theme == 102) {
            redLava.loadFromFile("Textures/redLava.jpeg");
            backgroundSprite.setTexture(redLava);
            backgroundSprite.setScale((float)ResolutionX / redLava.getSize().x, (float)ResolutionY / redLava.getSize().y);
        }
        else if (theme == 103) {
            forestGreen.loadFromFile("Textures/forestGreen.jpeg");
            backgroundSprite.setTexture(forestGreen);
            backgroundSprite.setScale((float)ResolutionX / forestGreen.getSize().x, (float)ResolutionY / forestGreen.getSize().y);
        }
        else if (theme == 104) {
            retro.loadFromFile("Textures/retro.jpeg");
            backgroundSprite.setTexture(retro);
            backgroundSprite.setScale((float)ResolutionX / retro.getSize().x, (float)ResolutionY / retro.getSize().y);
        }
	        else if (theme == 105) {
	            sunsetGlow.loadFromFile("Textures/sunsetGlow.jpeg");
	            backgroundSprite.setTexture(sunsetGlow);
	            backgroundSprite.setScale((float)ResolutionX / sunsetGlow.getSize().x, (float)ResolutionY / sunsetGlow.getSize().y);
	        }
	        else if (theme == 106) {
	            buildMonoGridGameplayTexture();
	            backgroundSprite.setTexture(monoGrid);
	            backgroundSprite.setScale((float)ResolutionX / monoGrid.getSize().x, (float)ResolutionY / monoGrid.getSize().y);
	        }
	        else if (theme == 107) {
	            buildAuroraGameplayTexture();
	            backgroundSprite.setTexture(auroraFlow);
	            backgroundSprite.setScale((float)ResolutionX / auroraFlow.getSize().x, (float)ResolutionY / auroraFlow.getSize().y);
	        }
	        else if (theme == 108) {
	            buildMidnightSteelGameplayTexture();
	            backgroundSprite.setTexture(midnightSteel);
	            backgroundSprite.setScale((float)ResolutionX / midnightSteel.getSize().x, (float)ResolutionY / midnightSteel.getSize().y);
	        }
	        else {
	            iceBlue.loadFromFile("Textures/iceBlue.jpeg");
	            backgroundSprite.setTexture(iceBlue);
            backgroundSprite.setScale((float)ResolutionX / iceBlue.getSize().x, (float)ResolutionY / iceBlue.getSize().y);
        }

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

        window->clear();
        window->draw(backgroundSprite);
        window->draw(darkOverlay);
        window->draw(prompt);
        window->draw(easyButton);
        window->draw(mediumButton);
        window->draw(hardButton);
        window->display();
    }

    backgroundSprite.setColor(Color::White);
    if (theme == 102) {
        redLava.loadFromFile("Textures/redLava.jpeg");
        backgroundSprite.setTexture(redLava);
        backgroundSprite.setScale((float)ResolutionX / redLava.getSize().x, (float)ResolutionY / redLava.getSize().y);
    }
    else if (theme == 103) {
        forestGreen.loadFromFile("Textures/forestGreen.jpeg");
        backgroundSprite.setTexture(forestGreen);
        backgroundSprite.setScale((float)ResolutionX / forestGreen.getSize().x, (float)ResolutionY / forestGreen.getSize().y);
    }
    else if (theme == 104) {
        retro.loadFromFile("Textures/retro.jpeg");
        backgroundSprite.setTexture(retro);
        backgroundSprite.setScale((float)ResolutionX / retro.getSize().x, (float)ResolutionY / retro.getSize().y);
    }
	    else if (theme == 105) {
	        sunsetGlow.loadFromFile("Textures/sunsetGlow.jpeg");
	        backgroundSprite.setTexture(sunsetGlow);
	        backgroundSprite.setScale((float)ResolutionX / sunsetGlow.getSize().x, (float)ResolutionY / sunsetGlow.getSize().y);
	    }
	    else if (theme == 106) {
	        buildMonoGridGameplayTexture();
	        backgroundSprite.setTexture(monoGrid);
	        backgroundSprite.setScale((float)ResolutionX / monoGrid.getSize().x, (float)ResolutionY / monoGrid.getSize().y);
	    }
	    else if (theme == 107) {
	        buildAuroraGameplayTexture();
	        backgroundSprite.setTexture(auroraFlow);
	        backgroundSprite.setScale((float)ResolutionX / auroraFlow.getSize().x, (float)ResolutionY / auroraFlow.getSize().y);
	    }
	    else if (theme == 108) {
	        buildMidnightSteelGameplayTexture();
	        backgroundSprite.setTexture(midnightSteel);
	        backgroundSprite.setScale((float)ResolutionX / midnightSteel.getSize().x, (float)ResolutionY / midnightSteel.getSize().y);
	    }
	    else {
	        iceBlue.loadFromFile("Textures/iceBlue.jpeg");
	        backgroundSprite.setTexture(iceBlue);
        backgroundSprite.setScale((float)ResolutionX / iceBlue.getSize().x, (float)ResolutionY / iceBlue.getSize().y);
    }

    const int enemyCount = 3;
    Enemy enemies[enemyCount] = { Enemy(gameDifficulty), Enemy(gameDifficulty), Enemy(gameDifficulty) };

	    bool Game = true;
	    bool p1 = true, p2 = true;
	    bool p1Drawing = false, p2Drawing = false;
	    int x = 0, y = 0, dx = 0, dy = 0;
	    int x2 = 0, y2 = 30, dx2 = 0, dy2 = 0;
	    float timer = 0, delay = 0.07f;
	
	    auto clearTrail = [](int trailValue) {
	        for (int i = 0; i < M; ++i) {
	            for (int j = 0; j < N; ++j) {
	                if (grid[i][j] == trailValue) {
	                    grid[i][j] = 0;
	                }
	            }
	        }
	    };
	
	    auto captureForPlayer = [&](int trailValue, int territoryValue, int opponentTrailValue) {
	        vector<pair<int, int>> opponentTrailCells;
	        for (int i = 0; i < M; ++i) {
	            for (int j = 0; j < N; ++j) {
	                if (grid[i][j] == opponentTrailValue) {
	                    opponentTrailCells.push_back({ i, j });
	                    grid[i][j] = 0;
	                }
	            }
	        }
	
	        for (int i = 0; i < enemyCount; ++i) {
	            drop(enemies[i].y / ts, enemies[i].x / ts);
	        }
	
	        int captured = 0;
	        for (int i = 0; i < M; ++i) {
	            for (int j = 0; j < N; ++j) {
	                if (grid[i][j] == 0 || grid[i][j] == trailValue) {
	                    grid[i][j] = territoryValue;
	                    captured++;
	                }
	                else if (grid[i][j] == -1) {
	                    grid[i][j] = 0;
	                }
	            }
	        }
	
	        for (const auto& cell : opponentTrailCells) {
	            grid[cell.first][cell.second] = opponentTrailValue;
	        }
	
	        return captured;
	    };

	    auto enemyTouchesGridValue = [&](const Enemy& enemy, int targetValue) {
	        int radius = ts - 3;
	        int left = max(0, (enemy.x - radius) / ts);
	        int right = min(N - 1, (enemy.x + radius) / ts);
	        int top = max(0, (enemy.y - radius) / ts);
	        int bottom = min(M - 1, (enemy.y + radius) / ts);
	        for (int row = top; row <= bottom; ++row) {
	            for (int col = left; col <= right; ++col) {
	                if (grid[row][col] == targetValue) {
	                    return true;
	                }
	            }
	        }
	        return false;
	    };

	    auto killPlayerOneFromEnemy = [&]() {
	        if (!p1) return;
	        deathnote.setString("P1 died!");
	        deathnote.setCharacterSize(40);
	        deathnote.setFillColor(Color::Red);
	        deathnote.setPosition(200, 150);
	        showDeathNote = true;
	        deathClock.restart();
	        p1 = false;
	        p1Drawing = false;
	        clearTrail(2);
	        x = 0, y = 0;
	        playerdeath.openFromFile("Sound Effects/pdeath.mp3");
	        playerdeath.setPlayingOffset(sf::seconds(27));
	        playerdeath.play();
	    };

	    auto killPlayerTwoFromEnemy = [&]() {
	        if (!p2) return;
	        deathnote2.setString("P2 died!");
	        deathnote2.setCharacterSize(40);
	        deathnote2.setFillColor(Color::Red);
	        deathnote2.setPosition(200, 150);
	        showDeathNote2 = true;
	        deathClock2.restart();
	        p2 = false;
	        p2Drawing = false;
	        clearTrail(4);
	        x2 = 0, y2 = 30;
	        playerdeath.openFromFile("Sound Effects/pdeath.mp3");
	        playerdeath.setPlayingOffset(sf::seconds(27));
	        playerdeath.play();
	    };

	    auto checkEnemyTrailDeaths = [&]() {
	        for (int i = 0; i < enemyCount; ++i) {
	            if (p1 && enemyTouchesGridValue(enemies[i], 2)) {
	                killPlayerOneFromEnemy();
	            }
	            if (p2 && enemyTouchesGridValue(enemies[i], 4)) {
	                killPlayerTwoFromEnemy();
	            }
	        }
	    };

    for (int i = 0; i < M; i++)
        for (int j = 0; j < N; j++)
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1) grid[i][j] = 1;

    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < N; ++j) {
            if (i == 0 || j == 0 || i == M - 1 || j == N - 1)
                grid[i][j] = 1;
            else
                grid[i][j] = 0;
        }
    }
    scoreP1.setScore(0);
    scoreP1.setPowerUps(0);
    scoreP2.setScore(0);
    scoreP2.setPowerUps(0);


    while (window->isOpen()) {
        float time = clock.restart().asSeconds();
        timer += time;

        Event e;
        while (window->pollEvent(e)) {
            if (e.type == Event::Closed) {
                window->close();
            }

            // When u press esc this function will reomve all the area captured by players
            if (e.type == Event::KeyPressed && e.key.code == Keyboard::Escape) {
                for (int i = 1; i < M - 1; i++)
                    for (int j = 1; j < N - 1; j++)
                        grid[i][j] = 0;
	                x = 10; y = 0;
	                x2 = 0, y2 = 30;
	                p1Drawing = false;
	                p2Drawing = false;
	                Game = true;
	                p1 = true, p2 = true;
	            }

            // Freeze enemies and players if they use powerups accordingly
            if (e.type == Event::KeyPressed) {
                if (e.key.code == Keyboard::P && scoreP1.usePowerUp()) {
                    isPowerUpActiveP1 = true;
                    powerUpTimerP1 = 0.0f;
                    for (int i = 0; i < enemyCount; ++i) {
                        enemies[i].savedDx = enemies[i].dx;
                        enemies[i].savedDy = enemies[i].dy;
                        enemies[i].dx = 0;
                        enemies[i].dy = 0;
                    }
                    dx2 = dy2 = 0; // freeze player 2
                }

                if (e.key.code == Keyboard::Space && scoreP2.usePowerUp()) {
                    isPowerUpActiveP2 = true;
                    powerUpTimerP2 = 0.0f;
                    for (int i = 0; i < enemyCount; ++i) {
                        enemies[i].savedDx = enemies[i].dx;
                        enemies[i].savedDy = enemies[i].dy;
                        enemies[i].dx = 0;
                        enemies[i].dy = 0;
                    }
                    dx = dy = 0; // freeze player 1
                }

            }
        }

        // Handle key press to start movement in a direction for player 1 and player 2
        if (p1) {
            if (Keyboard::isKeyPressed(Keyboard::Left)) {
                dx = -1; dy = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Right)) {
                dx = 1; dy = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::Up)) {
                dx = 0; dy = -1;
            }
            if (Keyboard::isKeyPressed(Keyboard::Down)) {
                dx = 0; dy = 1;
            }
        }

        if (p2) {
            if (Keyboard::isKeyPressed(Keyboard::A)) {
                dx2 = -1; dy2 = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::D)) {
                dx2 = 1; dy2 = 0;
            }
            if (Keyboard::isKeyPressed(Keyboard::W)) {
                dx2 = 0; dy2 = -1;
            }
            if (Keyboard::isKeyPressed(Keyboard::S)) {
                dx2 = 0; dy2 = 1;
            }
        }

        if (!Game) {
            continue;
        }

        // Resume the enemey moments and player 2 moments if powerup is not active
        if (isPowerUpActiveP1) {
            powerUpTimerP1 += time;
            dx2 = dy2 = 0;
            if (powerUpTimerP1 >= powerUpDuration) {
                isPowerUpActiveP1 = false;
                for (int i = 0; i < enemyCount; ++i) {
                    enemies[i].dx = enemies[i].savedDx;
                    enemies[i].dy = enemies[i].savedDy;
                }
            }
        }

        // Resume the enemey moments and player 1 moments if powerup is not active
        if (isPowerUpActiveP2) {
            powerUpTimerP2 += time;
            dx = dy = 0;
            if (powerUpTimerP2 >= powerUpDuration) {
                isPowerUpActiveP2 = false;
                for (int i = 0; i < enemyCount; ++i) {
                    enemies[i].dx = enemies[i].savedDx;
                    enemies[i].dy = enemies[i].savedDy;
                }
            }
        }

        checkEnemyTrailDeaths();

        // To move enemies if powerups are not active
        if (!isPowerUpActiveP1 && !isPowerUpActiveP2) {
            for (int i = 0; i < enemyCount; ++i) {
                enemies[i].move();
            }
        }
        checkEnemyTrailDeaths();

	        bool p1ClosedThisTick = false;
	        bool p2ClosedThisTick = false;
	
	        if (timer > delay) {
	            int nextX = clamp(x + dx, 0, N - 1);
	            int nextY = clamp(y + dy, 0, M - 1);

            x = nextX;
            y = nextY;

            // Player 1 collision with itself
            if (grid[y][x] == 2 && p1) {
                deathnote.setString("P1 died!");
                deathnote.setCharacterSize(40);
                deathnote.setFillColor(Color::Red);
                deathnote.setPosition(200, 150);
                showDeathNote = true;
                deathClock.restart();
                playerdeath.openFromFile("Sound Effects/pdeath.mp3");
                playerdeath.setPlayingOffset(sf::seconds(27));
                playerdeath.play();

	                // death
	                p1 = false;
	                p1Drawing = false;
	                clearTrail(2);
	                y = 0; x = 0;

                cout << "Player1 died\n";
            }
	            if (grid[y][x] == 0) {//if empty make it trail
	                grid[y][x] = 2;
	                p1Drawing = true;
	            }
	            else if (p1Drawing && (grid[y][x] == 1 || grid[y][x] == 3)) {
	                p1ClosedThisTick = true;
	                p1Drawing = false;
	            }

            int nextX2 = clamp(x2 + dx2, 0, N - 1);
            int nextY2 = clamp(y2 + dy2, 0, M - 1);
            x2 = nextX2;
            y2 = nextY2;

            if (grid[y2][x2] == 4) {//idher change kiya
                deathnote2.setString("P2 died!");
                deathnote2.setCharacterSize(40);
                deathnote2.setFillColor(Color::Red);
                deathnote2.setPosition(200, 150);
	                showDeathNote2 = true;
	                deathClock2.restart();
	                p2 = false;
	                p2Drawing = false;
	                clearTrail(4);
	                y2 = 30, x2 = 0;
                playerdeath.openFromFile("Sound Effects/pdeath.mp3");
                playerdeath.setPlayingOffset(sf::seconds(27));
                playerdeath.play();
                cout << "Player2 died\n";
            }

	            if (grid[y2][x2] == 0) {
	                grid[y2][x2] = 4;
	                p2Drawing = true;
	            }
	            else if (p2Drawing && (grid[y2][x2] == 1 || grid[y2][x2] == 5)) {
	                p2ClosedThisTick = true;
	                p2Drawing = false;
	            }

            timer = 0;
        }


        for (int i = 0; i < enemyCount; ++i) {
            enemies[i].move();
        }

        if (grid[y][x] == 1 || grid[y][x] == 3) {
            dx = dy = 0;
        }

        if (grid[y2][x2] == 1 || grid[y2][x2] == 5) {
            dx2 = dy2 = 0;
        }

	        // Update each player's score independently so simultaneous closes do not overwrite each other.
	        if (p1ClosedThisTick) {
	            dx = dy = 0;
	            scoreP1.updateScore(captureForPlayer(2, 3, 4));
	        }
	        if (p2ClosedThisTick) {
	            dx2 = dy2 = 0;
	            scoreP2.updateScore(captureForPlayer(4, 5, 2));
	        }

        if (grid[y2][x2] == 2) {
            deathnote2.setString("P2 died!");
            deathnote2.setCharacterSize(40);
            deathnote2.setFillColor(Color::Red);
            deathnote2.setPosition(200, 150);
            showDeathNote2 = true;
            deathClock2.restart();

	            p2 = false;
	            p2Drawing = false;
	            clearTrail(4);
	            x2 = 0, y2 = 30;
            playerdeath.openFromFile("Sound Effects/pdeath.mp3");
            // Set playback to start at 27 seconds
            playerdeath.setPlayingOffset(sf::seconds(27));
            playerdeath.play();
            cout << "Player2 died\n";
        }
        else if (grid[y2][x2] == 3) {
            deathnote2.setString("P2 died!");
            deathnote2.setCharacterSize(40);
            deathnote2.setFillColor(Color::Red);
            deathnote2.setPosition(200, 150);
            showDeathNote2 = true;
	            deathClock2.restart();
	            p2 = false;
	            p2Drawing = false;
	            clearTrail(4);
	            x2 = 0, y2 = 30;
            playerdeath.openFromFile("Sound Effects/pdeath.mp3");
            // Set playback to start at 27 seconds
            playerdeath.setPlayingOffset(sf::seconds(27));
            playerdeath.play();
            cout << "Player2 died\n";
        }
        else if (grid[y][x] == 4) {
            deathnote.setString("P1 died!");
            deathnote.setCharacterSize(40);
            deathnote.setFillColor(Color::Red);
            deathnote.setPosition(200, 150);
            showDeathNote = true;
            deathClock.restart();
            playerdeath.openFromFile("Sound Effects/pdeath.mp3");
            playerdeath.setPlayingOffset(sf::seconds(27));
	            playerdeath.play();
	            p1 = false;
	            p1Drawing = false;
	            clearTrail(2);
	            x = 0, y = 0;
            playerdeath.openFromFile("Sound Effects/pdeath.mp3");
            // Set playback to start at 27 seconds
            playerdeath.setPlayingOffset(sf::seconds(27));
            playerdeath.play();
            cout << "Player1 died\n";
        }
        else if (grid[y][x] == 5) {
            deathnote.setString("P1 died!");
            deathnote.setCharacterSize(40);
            deathnote.setFillColor(Color::Red);
            deathnote.setPosition(200, 150);
            // Show death note and start timer
            showDeathNote = true;
            deathClock.restart();
            // Play death sound
            playerdeath.openFromFile("Sound Effects/pdeath.mp3");
            playerdeath.setPlayingOffset(sf::seconds(27));
	            playerdeath.play();
	            p1 = false;
	            p1Drawing = false;
	            clearTrail(2);
	            x = 0, y = 0;
            playerdeath.openFromFile("Sound Effects/pdeath.mp3");
            // Set playback to start at 27 seconds
            playerdeath.setPlayingOffset(sf::seconds(27));
            playerdeath.play();
            cout << "Player1 died\n";
        }

        if (!p1 && !p2) {
            gameOverMusic2.openFromFile("Sound Effects/gameover2.mp3");
            // Set playback to start at 27 seconds
            gameOverMusic2.setPlayingOffset(sf::seconds(27));
            gameOverMusic2.play();
            Game = false;

        }

        checkEnemyTrailDeaths();

        // Draw everything
        window->clear();
        window->draw(backgroundSprite);

        int tileForValue[6] = {0, 0, 18, 36, 54, 72};
        int playerOneTile = 90;
        int playerTwoTile = 108;
        if (theme == 102) {
            tileForValue[1] = 36; tileForValue[2] = 72; tileForValue[3] = 54; tileForValue[4] = 90; tileForValue[5] = 108;
            playerOneTile = 54; playerTwoTile = 108;
        }
        else if (theme == 103) {
            tileForValue[1] = 54; tileForValue[2] = 72; tileForValue[3] = 108; tileForValue[4] = 18; tileForValue[5] = 90;
            playerOneTile = 108; playerTwoTile = 36;
        }
        else if (theme == 104) {
            tileForValue[1] = 18; tileForValue[2] = 90; tileForValue[3] = 36; tileForValue[4] = 108; tileForValue[5] = 54;
            playerOneTile = 36; playerTwoTile = 90;
        }
        else if (theme == 105) {
            tileForValue[1] = 108; tileForValue[2] = 0; tileForValue[3] = 36; tileForValue[4] = 54; tileForValue[5] = 72;
            playerOneTile = 36; playerTwoTile = 72;
        }
        else if (theme == 106) {
            tileForValue[1] = 90; tileForValue[2] = 0; tileForValue[3] = 18; tileForValue[4] = 36; tileForValue[5] = 54;
            playerOneTile = 90; playerTwoTile = 108;
        }
        else if (theme == 107) {
            tileForValue[1] = 54; tileForValue[2] = 108; tileForValue[3] = 72; tileForValue[4] = 18; tileForValue[5] = 90;
            playerOneTile = 108; playerTwoTile = 36;
        }
        else if (theme == 108) {
            tileForValue[1] = 0; tileForValue[2] = 90; tileForValue[3] = 18; tileForValue[4] = 54; tileForValue[5] = 72;
            playerOneTile = 18; playerTwoTile = 90;
        }

        bool useGeneratedTileTheme = (theme == 106 || theme == 107 || theme == 108);
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
        auto multiplayerTileColor = [&](int value) {
            if (theme == 106) {
                if (value == 1) return Color(72, 74, 84);
                if (value == 2 || value == 3) return Color(232, 234, 242);
                return Color(155, 158, 172);
            }
            if (theme == 107) {
                if (value == 1) return Color(0, 220, 205);
                if (value == 2 || value == 3) return Color(85, 245, 235);
                return Color(225, 85, 255);
            }
            if (value == 1) return Color(96, 122, 148);
            if (value == 2 || value == 3) return Color(178, 212, 242);
            return Color(120, 154, 190);
        };
        auto multiplayerPlayerColor = [&](int player) {
            if (theme == 106) return player == 1 ? Color(245, 245, 250) : Color(155, 158, 170);
            if (theme == 107) return player == 1 ? Color(105, 240, 225) : Color(255, 135, 225);
            return player == 1 ? Color(232, 242, 255) : Color(128, 170, 212);
        };
        auto multiplayerOutlineColor = [&]() {
            if (theme == 106) return Color(18, 20, 24);
            if (theme == 107) return Color(8, 35, 48);
            return Color(20, 30, 42);
        };
        auto drawGeneratedMultiplayerTile = [&](int value, Color base, float px, float py) {
            themedTile.setFillColor(shadeColor(base, 24));
            themedTile.setOutlineColor(multiplayerOutlineColor());
            themedTile.setPosition(px, py);
            window->draw(themedTile);

            innerTile.setFillColor(base);
            innerTile.setOutlineThickness(0.f);
            innerTile.setPosition(px + 2.f, py + 2.f);
            window->draw(innerTile);

            tileLine.setFillColor(liftColor(base, 35));
            tileLine.setSize(Vector2f(static_cast<float>(ts - 4), 2.f));
            tileLine.setPosition(px + 2.f, py + 2.f);
            window->draw(tileLine);
            tileLine.setSize(Vector2f(2.f, static_cast<float>(ts - 4)));
            window->draw(tileLine);

            tileLine.setFillColor(shadeColor(base, 45));
            tileLine.setSize(Vector2f(static_cast<float>(ts - 4), 2.f));
            tileLine.setPosition(px + 2.f, py + ts - 4.f);
            window->draw(tileLine);
            tileLine.setSize(Vector2f(2.f, static_cast<float>(ts - 4)));
            tileLine.setPosition(px + ts - 4.f, py + 2.f);
            window->draw(tileLine);
        };

        for (int i = 0; i < M; i++) {
            for (int j = 0; j < N; j++) {
                int val = grid[i][j];
                if (val == 0) continue;

                if (useGeneratedTileTheme) {
                    drawGeneratedMultiplayerTile(val, multiplayerTileColor(val), static_cast<float>(j * ts), static_cast<float>(i * ts));
                }
                else {
                    sTile.setTextureRect(IntRect(tileForValue[val], 0, ts, ts));
                    sTile.setPosition(j * ts, i * ts);
                    window->draw(sTile);
                }
            }
        }
        if (useGeneratedTileTheme) {
            drawGeneratedMultiplayerTile(2, multiplayerPlayerColor(1), static_cast<float>(x * ts), static_cast<float>(y * ts));
            drawGeneratedMultiplayerTile(4, multiplayerPlayerColor(2), static_cast<float>(x2 * ts), static_cast<float>(y2 * ts));
        }
        else {
            sTile.setTextureRect(IntRect(playerOneTile, 0, ts, ts));
            sTile.setPosition(x * ts, y * ts);
            window->draw(sTile);
            sTile.setTextureRect(IntRect(playerTwoTile, 0, ts, ts));
            sTile.setPosition(x2 * ts, y2 * ts);
            window->draw(sTile);
        }

        for (int i = 0; i < enemyCount; ++i) {
            sEnemy.setPosition(enemies[i].x, enemies[i].y);
            window->draw(sEnemy);
        }
        sEnemy.rotate(10);
        for (int i = 0; i < enemyCount; ++i) {
            sEnemy.setPosition(enemies[i].x, enemies[i].y);
            window->draw(sEnemy);
        }

        scoreTextP1.setString("P1 SCORE  " + to_string(scoreP1.getScore()));
        powerUpTextP1.setString("POWER-UPS  " + to_string(scoreP1.getPowerUps()));
        scoreTextP2.setString("P2 SCORE  " + to_string(scoreP2.getScore()));
        powerUpTextP2.setString("POWER-UPS  " + to_string(scoreP2.getPowerUps()));


        if (showDeathNote && deathClock.getElapsedTime().asSeconds() < 2) {
            window->draw(deathnote);
        }
        else {
            showDeathNote = false;
        }
        if (showDeathNote2 && deathClock2.getElapsedTime().asSeconds() < 2) {
            window->draw(deathnote2);
        }
        else {
            showDeathNote2 = false;
        }

        window->draw(hudBar);
        window->draw(centerDivider);
        window->draw(scoreTextP1);
        window->draw(powerUpTextP1);
        window->draw(scoreTextP2);
        window->draw(powerUpTextP2);

        // If game overs to update win loss in file
        if (!Game) {
            Auth auth;
            int finalScoreP1 = scoreP1.getScore();
            int finalScoreP2 = scoreP2.getScore();
            string playerOneName = nameP1.empty() ? "Player 1" : nameP1;
            string playerTwoName = nameP2.empty() ? "Player 2" : nameP2;
            string winnerName;
            string resultLine;

            if (finalScoreP1 > finalScoreP2) {
                winnerName = playerOneName;
                resultLine = "P1 " + to_string(finalScoreP1) + "  -  P2 " + to_string(finalScoreP2);
                if (!resultSaved) {
                    auth.updateGameHistory(nameP1, "W");
                    auth.updateGameHistory(nameP2, "L");
                    resultSaved = true;
                }
            }
            else if (finalScoreP2 > finalScoreP1) {
                winnerName = playerTwoName;
                resultLine = "P2 " + to_string(finalScoreP2) + "  -  P1 " + to_string(finalScoreP1);
                if (!resultSaved) {
                    auth.updateGameHistory(nameP2, "W");
                    auth.updateGameHistory(nameP1, "L");
                    resultSaved = true;
                }
            }
            else {
                winnerName = "Draw";
                resultLine = "P1 " + to_string(finalScoreP1) + "  -  P2 " + to_string(finalScoreP2);
                if (!resultSaved) {
                    auth.updateGameHistory(nameP1, "L");
                    auth.updateGameHistory(nameP2, "L");
                    resultSaved = true;
                }
            }

            winnerTitle.setString(winnerName == "Draw" ? "MATCH DRAW" : "WINNER");
            winnerNameText.setString(winnerName);
            finalScoreText.setString("Final Score   " + resultLine);
            resultHintText.setString("Returning to menu...");

            FloatRect titleBounds = winnerTitle.getLocalBounds();
            FloatRect nameBounds = winnerNameText.getLocalBounds();
            FloatRect scoreBounds = finalScoreText.getLocalBounds();
            FloatRect hintBounds = resultHintText.getLocalBounds();

            winnerTitle.setPosition((ResolutionX - titleBounds.width) / 2.f, 135.f);
            winnerNameText.setPosition((ResolutionX - nameBounds.width) / 2.f, 172.f);
            finalScoreText.setPosition((ResolutionX - scoreBounds.width) / 2.f, 232.f);
            resultHintText.setPosition((ResolutionX - hintBounds.width) / 2.f, 278.f);

            window->draw(resultOverlay);
            window->draw(resultPanel);
            window->draw(winnerTitle);
            window->draw(winnerNameText);
            window->draw(finalScoreText);
            window->draw(resultHintText);
            window->display();
            sf::sleep(sf::seconds(3));
            return 1;
        }
        window->display();
    }
    return 1;
}
#endif
