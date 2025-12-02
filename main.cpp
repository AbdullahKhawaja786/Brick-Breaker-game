#include <SFML/Graphics.hpp>
#include <cstdlib>
#include <ctime>
#include <cstring>
#include <iostream>
#include "constants.h"
#include "game_logic.h"
#include "collision.h"
#include "rendering.h"
#include "menu_system.h"
#include "file_operations.h"

int main() {
    srand(static_cast<unsigned>(time(0)));
    // Create window
    sf::RenderWindow window(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    window.setFramerateLimit(60);
    // Load font
    sf::Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        std::cerr << "ERROR: Failed to load font. Exiting." << std::endl;
        return -1;
    }
    // Load background images
    sf::Texture menuBackground, gameBackground;
    sf::Sprite menuBgSprite, gameBgSprite;
    bool hasMenuBg = menuBackground.loadFromFile("menu_background.png");
    if (hasMenuBg) {
        menuBgSprite.setTexture(menuBackground);
        float scaleX = static_cast<float>(WINDOW_WIDTH) / menuBackground.getSize().x;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / menuBackground.getSize().y;
        menuBgSprite.setScale(scaleX, scaleY);
    }
    bool hasGameBg = gameBackground.loadFromFile("game_background.png");
    if (hasGameBg) {
        gameBgSprite.setTexture(gameBackground);
        float scaleX = static_cast<float>(WINDOW_WIDTH) / gameBackground.getSize().x;
        float scaleY = static_cast<float>(WINDOW_HEIGHT) / gameBackground.getSize().y;
        gameBgSprite.setScale(scaleX, scaleY);
    }
    // Load brick textures
    sf::Texture greenBrickIntact, greenBrickCracked;
    bool hasGreenBrick = greenBrickIntact.loadFromFile("green_brick.png");
    greenBrickCracked.loadFromFile("green_brick_cracked.png");
    sf::Texture yellowBrickIntact, yellowBrickCracked;
    bool hasYellowBrick = yellowBrickIntact.loadFromFile("yellow_brick.png");
    yellowBrickCracked.loadFromFile("yellow_brick_cracked.png");
    sf::Texture redBrickIntact, redBrickCracked;
    bool hasRedBrick = redBrickIntact.loadFromFile("red_brick.png");
    redBrickCracked.loadFromFile("red_brick_cracked.png");
    sf::Texture grayBrick;
    bool hasGrayBrick = grayBrick.loadFromFile("gray_brick.png");
    bool hasBrickTextures = hasGreenBrick;
    if (!hasBrickTextures) {
        std::cerr << "WARNING: Could not load brick textures - using fallback colors" << std::endl;
    }
    // Load other textures
    sf::Texture paddleTexture, ballTexture;
    bool hasPaddleTexture = paddleTexture.loadFromFile("paddle.png");
    bool hasBallTexture = ballTexture.loadFromFile("ball.png");
    sf::Texture heartTexture, starTexture;
    bool hasHeartTexture = heartTexture.loadFromFile("heart.png");
    bool hasStarTexture = starTexture.loadFromFile("star.png");
    // Game state
    int gameState = STATE_MAIN_MENU;
    int selectedMenuOption = 0;
    int selectedPauseOption = 0;
    // Game variables
    int level = 1;
    int score = 0;
    int lives = INITIAL_LIVES;
    float ballX = WINDOW_WIDTH / 2.0f;
    float ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    float ballVelX = 0.0f;
    float ballVelY = 0.0f;
    float paddleX = WINDOW_WIDTH / 2.0f - PADDLE_WIDTH / 2.0f;
    float currentPaddleWidth = PADDLE_WIDTH;
    float currentBallSpeed = BALL_SPEED;
    bool ballLaunched = false;
    // Bricks
    int bricks[TOTAL_BRICKS];
    int brickTypes[TOTAL_BRICKS];
    // Power-ups
    float powerUpX[MAX_POWERUPS];
    float powerUpY[MAX_POWERUPS];
    int powerUpType[MAX_POWERUPS];
    bool powerUpActive[MAX_POWERUPS];
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerUpActive[i] = false;
    }

    // Particle system
    float particleX[MAX_PARTICLES];
    float particleY[MAX_PARTICLES];
    float particleVelX[MAX_PARTICLES];
    float particleVelY[MAX_PARTICLES];
    float particleLife[MAX_PARTICLES];
    bool particleActive[MAX_PARTICLES];

    // Initialize particles
    for (int i = 0; i < MAX_PARTICLES; i++) {
        particleActive[i] = false;
    }

    // Settings
    int difficulty = 1;
    loadSettings(SETTINGS_FILE, difficulty);
    // High scores
    int highScores[MAX_HIGH_SCORES];
    char highScoreNames[MAX_HIGH_SCORES][MAX_NAME_LENGTH];
    loadHighScores(HIGHSCORE_FILE, highScores, highScoreNames);
    // Name input
    char playerName[MAX_NAME_LENGTH] = "";
    int nameLength = 0;
    bool enteringName = false;
    bool finishedNameInput = false;
    // Clock
    sf::Clock clock;
    // Game loop
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();
        // Input handling
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
            if (gameState == STATE_MAIN_MENU) {
                int choice = handleMainMenuInput(event, selectedMenuOption);
                if (choice == MENU_START_GAME) {
                    initializeGame(level, score, lives, ballX, ballY, ballVelX, ballVelY,
                        paddleX, bricks, ballLaunched, difficulty, currentPaddleWidth, currentBallSpeed);
                    for (int i = 0; i < TOTAL_BRICKS; i++) {
                        brickTypes[i] = bricks[i];
                    }
                    gameState = STATE_PLAYING;
                }
                else if (choice == MENU_LOAD_GAME) {
                    if (loadGameState(SAVE_FILE, level, score, lives, ballX, ballY,
                        ballVelX, ballVelY, paddleX, bricks, ballLaunched)) {
                        for (int i = 0; i < TOTAL_BRICKS; i++) {
                            if (bricks[i] > 0) {
                                brickTypes[i] = bricks[i];
                            }
                        }
                        currentPaddleWidth = getDifficultyPaddleWidth(difficulty);
                        currentBallSpeed = getDifficultyBallSpeed(difficulty);
                        gameState = STATE_PLAYING;
                    }
                }
                else if (choice == MENU_HIGH_SCORES) {
                    gameState = STATE_HIGH_SCORES;
                }
                else if (choice == MENU_SETTINGS) {
                    gameState = STATE_SETTINGS;
                }
                else if (choice == MENU_EXIT) {
                    window.close();
                }
            }
            else if (gameState == STATE_PLAYING) {
                if (event.type == sf::Event::KeyPressed) {
                    if (event.key.code == sf::Keyboard::Space) {
                        launchBall(ballVelX, ballVelY, ballLaunched, currentBallSpeed);
                    }
                    else if (event.key.code == sf::Keyboard::Escape) {
                        gameState = STATE_PAUSED;
                        selectedPauseOption = 0;
                    }
                }
            }
            else if (gameState == STATE_PAUSED) {
                int choice = handlePauseMenuInput(event, selectedPauseOption);
                if (choice == PAUSE_RESUME) {
                    gameState = STATE_PLAYING;
                }
                else if (choice == PAUSE_SAVE) {
                    saveGameState(SAVE_FILE, level, score, lives, ballX, ballY,
                        ballVelX, ballVelY, paddleX, bricks, ballLaunched);
                    gameState = STATE_PLAYING;
                }
                else if (choice == PAUSE_MAIN_MENU) {
                    gameState = STATE_MAIN_MENU;
                    selectedMenuOption = 0;
                }
            }
            else if (gameState == STATE_SETTINGS) {
                bool exitSettings = false;
                handleSettingsInput(event, difficulty, exitSettings);
                if (exitSettings) {
                    saveSettings(SETTINGS_FILE, difficulty);
                    gameState = STATE_MAIN_MENU;
                }
            }
            else if (gameState == STATE_HIGH_SCORES) {
                if (handleHighScoresInput(event)) {
                    gameState = STATE_MAIN_MENU;
                }
            }
            else if (gameState == STATE_GAME_OVER) {
                if (enteringName) {
                    if (getPlayerNameInput(event, playerName, nameLength, finishedNameInput)) {
                        saveHighScore(HIGHSCORE_FILE, score, playerName, highScores, highScoreNames);
                        loadHighScores(HIGHSCORE_FILE, highScores, highScoreNames);
                        enteringName = false;
                        gameState = STATE_MAIN_MENU;
                        selectedMenuOption = 0;
                    }
                }
                else if (event.type == sf::Event::KeyPressed &&
                    event.key.code == sf::Keyboard::Return) {
                    bool isHighScore = false;
                    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
                        if (score > highScores[i]) {
                            isHighScore = true;
                            break;
                        }
                    }
                    if (isHighScore) {
                        enteringName = true;
                        nameLength = 0;
                        playerName[0] = '\0';
                        finishedNameInput = false;
                    }
                    else {
                        gameState = STATE_MAIN_MENU;
                        selectedMenuOption = 0;
                    }
                }
            }
        }
        // Game logic update
        if (gameState == STATE_PLAYING) {
            bool leftPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Left) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::A);
            bool rightPressed = sf::Keyboard::isKeyPressed(sf::Keyboard::Right) ||
                sf::Keyboard::isKeyPressed(sf::Keyboard::D);
            updatePaddlePosition(paddleX, leftPressed, rightPressed, deltaTime, currentPaddleWidth);
            updateBallPosition(ballX, ballY, ballVelX, ballVelY, deltaTime, ballLaunched, paddleX, currentPaddleWidth);
            if (ballLaunched) {
                checkWallCollisions(ballX, ballY, BALL_RADIUS, ballVelX, ballVelY);
                if (checkPaddleCollision(ballX, ballY, BALL_RADIUS, paddleX, PADDLE_Y,
                    currentPaddleWidth, PADDLE_HEIGHT)) {
                    handlePaddleCollision(ballX, paddleX, currentPaddleWidth, ballVelX, ballVelY);
                }
                int hitBrick = checkBrickCollision(ballX, ballY, BALL_RADIUS, bricks);
                if (hitBrick != -1) {
                    if (bricks[hitBrick] != -1) {
                        handleBrickCollision(ballX, ballY, hitBrick, ballVelX, ballVelY);
                        int row = hitBrick / GRID_WIDTH;
                        int col = hitBrick % GRID_WIDTH;
                        float brickX = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING) + BRICK_WIDTH / 2;
                        float brickY = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING) + BRICK_HEIGHT / 2;
                        score += calculateBrickScore(bricks[hitBrick]);
                        bricks[hitBrick]--;
                        if (bricks[hitBrick] == 0) {
                            spawnPowerUp(brickX, brickY, powerUpX, powerUpY, powerUpType, powerUpActive);
                            // Create particles when brick is destroyed
                            createParticles(brickX, brickY, particleX, particleY,
                                particleVelX, particleVelY, particleLife, particleActive);
                        }
                    }
                    else {
                        handleBrickCollision(ballX, ballY, hitBrick, ballVelX, ballVelY);
                    }
                }
                if (checkBallLost(ballY, BALL_RADIUS)) {
                    lives--;
                    if (lives <= 0) {
                        gameState = STATE_GAME_OVER;
                    }
                    else {
                        resetBall(ballX, ballY, ballVelX, ballVelY, paddleX, ballLaunched, currentPaddleWidth);
                    }
                }
                if (isLevelComplete(bricks)) {
                    if (level >= MAX_LEVELS) {
                        gameState = STATE_GAME_OVER;
                    }
                    else {
                        nextLevel(level, score, ballX, ballY, ballVelX, ballVelY,
                            paddleX, bricks, ballLaunched, currentPaddleWidth);
                        for (int i = 0; i < TOTAL_BRICKS; i++) {
                            brickTypes[i] = bricks[i];
                        }
                    }
                }
            }
            updatePowerUps(powerUpX, powerUpY, powerUpActive, deltaTime);

            // Update particles
            updateParticles(particleX, particleY, particleVelX, particleVelY,
                particleLife, particleActive, deltaTime);

            int hitPowerUp = checkPowerUpCollision(paddleX, PADDLE_Y, currentPaddleWidth,
                PADDLE_HEIGHT, powerUpX, powerUpY, powerUpActive);
            if (hitPowerUp != -1) {
                applyPowerUp(powerUpType[hitPowerUp], lives, currentPaddleWidth, currentBallSpeed,
                    getDifficultyPaddleWidth(difficulty));
                score += SCORE_POWERUP;
                powerUpActive[hitPowerUp] = false;
            }
        }
        // Rendering
        if (gameState == STATE_MAIN_MENU || gameState == STATE_SETTINGS ||
            gameState == STATE_HIGH_SCORES || gameState == STATE_GAME_OVER) {
            window.clear(sf::Color(10, 10, 30));
            if (hasMenuBg) {
                window.draw(menuBgSprite);
            }
        }
        else {
            window.clear(sf::Color(10, 15, 40));
            if (hasGameBg) {
                window.draw(gameBgSprite);
            }
        }
        if (gameState == STATE_MAIN_MENU) {
            drawMainMenuSimple(window, font, selectedMenuOption);
        }
        else if (gameState == STATE_PLAYING || gameState == STATE_PAUSED) {
            drawBricksWithTypes(window, bricks, brickTypes,
                greenBrickIntact, greenBrickCracked,
                yellowBrickIntact, yellowBrickCracked,
                redBrickIntact, redBrickCracked,
                grayBrick, hasBrickTextures);
            drawPaddle(window, paddleX, currentPaddleWidth, paddleTexture, hasPaddleTexture);
            drawBall(window, ballX, ballY, ballTexture, hasBallTexture);
            drawPowerUpsSimple(window, powerUpX, powerUpY, powerUpType, powerUpActive,
                heartTexture, starTexture, hasHeartTexture || hasStarTexture);

            // Draw particles
            drawSimpleParticles(window, particleX, particleY, particleActive);

            drawHUD(window, font, score, lives, level);
            if (gameState == STATE_PAUSED) {
                drawPauseMenu(window, font, selectedPauseOption);
            }
        }
        else if (gameState == STATE_GAME_OVER) {
            if (enteringName) {
                drawGameOver(window, font, score);
                drawNameInput(window, font, playerName);
            }
            else {
                drawGameOver(window, font, score);
            }
        }
        else if (gameState == STATE_SETTINGS) {
            drawSettings(window, font, difficulty);
        }
        else if (gameState == STATE_HIGH_SCORES) {
            drawHighScores(window, font, highScores, highScoreNames);
        }
        window.display();
    }
    return 0;
}
