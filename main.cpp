#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
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

using namespace std;
using namespace sf;

int main() {
    srand(static_cast<unsigned>(time(0)));
    RenderWindow window(VideoMode(window_width, window_height), WINDOW_TITLE);
    window.setFramerateLimit(60);

    Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "ERROR: Failed to load font. Exiting." << endl;
        return -1;
    }

    Font titleFont;
    bool hasTitleFont = titleFont.loadFromFile("GameFont.ttf");
    if (!hasTitleFont) {
        cerr << "WARNING: Could not load custom title font, using default" << endl;
    }

    Font menuFont;
    bool hasMenuFont = menuFont.loadFromFile("MenuFont.ttf");
    if (!hasMenuFont) {
        cerr << "WARNING: Could not load custom menu font, using default" << endl;
    }

    Music menuMusic;
    bool hasMenuMusic = menuMusic.openFromFile(MENU_MUSIC_FILE);
    if (!hasMenuMusic) {
        cerr << "WARNING: Could not load menu_music.ogg" << endl;
    }
    else {
        menuMusic.setLoop(true);
    }

    SoundBuffer brickBreakBuffer;
    bool hasBrickSound = brickBreakBuffer.loadFromFile(BRICK_BREAK_SOUND_FILE);
    if (!hasBrickSound) {
        cerr << "WARNING: Could not load brick_break.ogg" << endl;
    }

    SoundBuffer paddleHitBuffer;
    bool hasPaddleSound = paddleHitBuffer.loadFromFile("paddle_hit.ogg");
    if (!hasPaddleSound) {
        cerr << "WARNING: Could not load paddle_hit.ogg" << endl;
    }

    Sound brickBreakSounds[MAX_SOUND_CHANNELS];
    Sound paddleHitSounds[MAX_SOUND_CHANNELS];
    int currentBrickSoundChannel = 0;
    int currentPaddleSoundChannel = 0;

    if (hasBrickSound) {
        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
            brickBreakSounds[i].setBuffer(brickBreakBuffer);
        }
    }

    if (hasPaddleSound) {
        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
            paddleHitSounds[i].setBuffer(paddleHitBuffer);
        }
    }

    Texture menuBackground, gameBackground;
    Sprite menuBgSprite, gameBgSprite;
    bool hasMenuBg = menuBackground.loadFromFile("menu_background.png");
    if (hasMenuBg) {
        menuBgSprite.setTexture(menuBackground);
        float scaleX = static_cast<float>(window_width) / menuBackground.getSize().x;
        float scaleY = static_cast<float>(window_height) / menuBackground.getSize().y;
        menuBgSprite.setScale(scaleX, scaleY);
    }

    bool hasGameBg = gameBackground.loadFromFile("game_background.png");
    if (hasGameBg) {
        gameBgSprite.setTexture(gameBackground);
        float scaleX = static_cast<float>(window_width) / gameBackground.getSize().x;
        float scaleY = static_cast<float>(window_height) / gameBackground.getSize().y;
        gameBgSprite.setScale(scaleX, scaleY);
    }

    Texture logoTexture;
    Sprite logoSprite;
    bool hasLogo = logoTexture.loadFromFile("logo.png");
    if (hasLogo) {
        logoSprite.setTexture(logoTexture);
        float scaleX = static_cast<float>(window_width) / logoTexture.getSize().x;
        float scaleY = static_cast<float>(window_height) / logoTexture.getSize().y;
        logoSprite.setScale(scaleX, scaleY);
        logoSprite.setPosition(0, 0);
    }
    else {
        cerr << "WARNING: Could not load logo.png" << endl;
    }

    Texture greenBrickIntact;
    bool hasGreenBrick = greenBrickIntact.loadFromFile("green_brick.png");

    Texture yellowBrickIntact, yellowBrickCracked;
    bool hasYellowBrick = yellowBrickIntact.loadFromFile("yellow_brick.png");
    yellowBrickCracked.loadFromFile("yellow_brick_cracked.png");

    Texture redBrickIntact, redBrickCracked;
    bool hasRedBrick = redBrickIntact.loadFromFile("red_brick.png");
    redBrickCracked.loadFromFile("red_brick_cracked.png");

    Texture grayBrick;
    bool hasGrayBrick = grayBrick.loadFromFile("gray_brick.png");

    bool hasBrickTextures = hasGreenBrick;
    if (!hasBrickTextures) {
        cerr << "WARNING: Could not load brick textures" << endl;
    }

    Texture paddleTexture, ballTexture;
    bool hasPaddleTexture = paddleTexture.loadFromFile("paddle.png");
    bool hasBallTexture = ballTexture.loadFromFile("ball.png");

    Texture heartTexture, starTexture, skullTexture, coinTexture;
    bool hasHeartTexture = heartTexture.loadFromFile("heart.png");
    bool hasStarTexture = starTexture.loadFromFile("star.png");
    bool hasSkullTexture = skullTexture.loadFromFile("skull.png");
    bool hasCoinTexture = coinTexture.loadFromFile("coin.png");

    int gameState = STATE_INTRO;
    int selectedMenuOption = 0;
    int selectedPauseOption = 0;
    int selectedSetting = 0;
    int selectedPauseSetting = 0;

    int level = 1;
    int score = 0;
    int lives = INITIAL_LIVES;
    float ballX = window_width / 2.0f;
    float ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    float ballVelX = 0.0f;
    float ballVelY = 0.0f;
    float paddleX = window_width / 2.0f - PADDLE_WIDTH / 2.0f;
    float currentPaddleWidth = PADDLE_WIDTH;
    float currentBallSpeed = BALL_SPEED;
    float ballSpeedMultiplier = 1.0f;
    bool ballLaunched = false;

    float powerUpTimer = 0.0f;
    int activePowerUpType = POWERUP_NONE;

    int bricks[TOTAL_BRICKS];
    int brickTypes[TOTAL_BRICKS];

    float powerUpX[MAX_POWERUPS];
    float powerUpY[MAX_POWERUPS];
    int powerUpType[MAX_POWERUPS];
    bool powerUpActive[MAX_POWERUPS];
    for (int i = 0; i < MAX_POWERUPS; i++) {
        powerUpActive[i] = false;
    }

    float particleX[MAX_PARTICLES];
    float particleY[MAX_PARTICLES];
    float particleVelX[MAX_PARTICLES];
    float particleVelY[MAX_PARTICLES];
    float particleLife[MAX_PARTICLES];
    bool particleActive[MAX_PARTICLES];

    for (int i = 0; i < MAX_PARTICLES; i++) {
        particleActive[i] = false;
    }

    int difficulty = 1;
    int musicVolume = default_music;
    int gameVolume = default_gamevolume;
    loadSettings(SETTINGS_FILE, difficulty, musicVolume, gameVolume);

    if (hasMenuMusic) {
        menuMusic.setVolume(static_cast<float>(musicVolume));
    }
    if (hasBrickSound) {
        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
            brickBreakSounds[i].setVolume(static_cast<float>(gameVolume));
        }
    }
    if (hasPaddleSound) {
        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
            paddleHitSounds[i].setVolume(static_cast<float>(gameVolume));
        }
    }

    int highScores[maximum_scores];
    char highScoreNames[maximum_scores][maximun_name_length];
    loadHighScores(HIGHSCORE_FILE, highScores, highScoreNames);

    char playerName[maximun_name_length] = "";
    int nameLength = 0;
    bool enteringName = false;
    bool finishedNameInput = false;

    char messageText[100] = "";
    float messageTimer = 0.0f;

    Clock clock;

    int previousGameState = STATE_INTRO;

    if (hasMenuMusic && gameState == STATE_INTRO) {
        menuMusic.play();
    }

    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        if (deltaTime > 0.05f) {
            deltaTime = 0.05f;
        }

        // Update message timer
        if (messageTimer > 0) {
            messageTimer -= deltaTime;
            if (messageTimer <= 0) {
                messageText[0] = '\0';
            }
        }

        // Handle music state transitions
        if (gameState != previousGameState) {
            if (hasMenuMusic) {
                if (gameState == STATE_INTRO ||
                    gameState == STATE_MAIN_MENU ||
                    gameState == STATE_SETTINGS ||
                    gameState == STATE_HIGH_SCORES ||
                    gameState == STATE_GAME_OVER ||
                    gameState == STATE_GAME_COMPLETE) {
                    if (menuMusic.getStatus() != Music::Playing) {
                        menuMusic.play();
                    }
                }
                else {
                    menuMusic.stop();
                }
            }
            previousGameState = gameState;
        }

        // Ensure music keeps playing in menu states
        if (hasMenuMusic) {
            if (gameState == STATE_INTRO ||
                gameState == STATE_MAIN_MENU ||
                gameState == STATE_SETTINGS ||
                gameState == STATE_HIGH_SCORES ||
                gameState == STATE_GAME_OVER ||
                gameState == STATE_GAME_COMPLETE) {
                if (menuMusic.getStatus() == Music::Stopped) {
                    menuMusic.play();
                }
            }
        }

        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                saveSettings(SETTINGS_FILE, difficulty, musicVolume, gameVolume);
                window.close();
            }

            if (gameState == STATE_INTRO) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Space) {
                        gameState = STATE_MAIN_MENU;
                    }
                }
            }
            else if (gameState == STATE_MAIN_MENU) {
                int choice = handleMainMenuInput(event, selectedMenuOption);
                if (choice == MENU_START_GAME) {
                    initializeGame(level, score, lives, ballX, ballY, ballVelX, ballVelY,
                        paddleX, bricks, ballLaunched, difficulty, currentPaddleWidth, currentBallSpeed);
                    for (int i = 0; i < TOTAL_BRICKS; i++) {
                        brickTypes[i] = bricks[i];
                    }
                    powerUpTimer = 0;
                    activePowerUpType = POWERUP_NONE;
                    ballSpeedMultiplier = 1.0f;
                    messageText[0] = '\0';
                    messageTimer = 0;
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
                        currentBallSpeed = getLevelBallSpeed(level, difficulty);
                        powerUpTimer = 0;
                        activePowerUpType = POWERUP_NONE;
                        ballSpeedMultiplier = 1.0f;
                        for (int i = 0; i < MAX_POWERUPS; i++) {
                            powerUpActive[i] = false;
                        }
                        messageText[0] = '\0';
                        messageTimer = 0;
                        gameState = STATE_PLAYING;
                    }
                    else {
                        safeStringCopy(messageText, "No saved game found!");
                        messageTimer = MESSAGE_DISPLAY_TIME;
                    }
                }
                else if (choice == MENU_HIGH_SCORES) {
                    gameState = STATE_HIGH_SCORES;
                }
                else if (choice == MENU_SETTINGS) {
                    selectedSetting = 0;
                    gameState = STATE_SETTINGS;
                }
                else if (choice == MENU_EXIT) {
                    window.close();
                }
            }
            else if (gameState == STATE_PLAYING) {
                if (event.type == Event::KeyPressed) {
                    if (event.key.code == Keyboard::Space) {
                        launchBall(ballVelX, ballVelY, ballLaunched, currentBallSpeed, ballSpeedMultiplier);
                    }
                    else if (event.key.code == Keyboard::Escape) {
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
                    if (saveGameState(SAVE_FILE, level, score, lives, ballX, ballY,
                        ballVelX, ballVelY, paddleX, bricks, ballLaunched)) {
                        safeStringCopy(messageText, "Game saved successfully!");
                    }
                    else {
                        safeStringCopy(messageText, "Failed to save game!");
                    }
                    messageTimer = MESSAGE_DISPLAY_TIME;
                    gameState = STATE_PLAYING;
                }
                else if (choice == PAUSE_SETTINGS) {
                    selectedPauseSetting = 0;
                    gameState = STATE_PAUSE_SETTINGS;
                }
                else if (choice == PAUSE_MAIN_MENU) {
                    gameState = STATE_MAIN_MENU;
                    selectedMenuOption = 0;
                    messageText[0] = '\0';
                    messageTimer = 0;
                }
            }
            else if (gameState == STATE_PAUSE_SETTINGS) {
                bool exitSettings = false;
                bool volumeChanged = false;
                handlePauseSettingsInput(event, musicVolume, gameVolume,
                    selectedPauseSetting, exitSettings, volumeChanged);

                if (volumeChanged) {
                    if (hasMenuMusic) {
                        menuMusic.setVolume(static_cast<float>(musicVolume));
                    }
                    if (hasBrickSound) {
                        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                            brickBreakSounds[i].setVolume(static_cast<float>(gameVolume));
                        }
                    }
                    if (hasPaddleSound) {
                        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                            paddleHitSounds[i].setVolume(static_cast<float>(gameVolume));
                        }
                    }
                    saveSettings(SETTINGS_FILE, difficulty, musicVolume, gameVolume);
                }

                if (exitSettings) {
                    saveSettings(SETTINGS_FILE, difficulty, musicVolume, gameVolume);
                    gameState = STATE_PAUSED;
                }
            }
            else if (gameState == STATE_SETTINGS) {
                bool exitSettings = false;
                bool volumeChanged = false;
                handleSettingsInput(event, difficulty, musicVolume, gameVolume,
                    selectedSetting, exitSettings, volumeChanged);

                if (volumeChanged) {
                    if (hasMenuMusic) {
                        menuMusic.setVolume(static_cast<float>(musicVolume));
                    }
                    if (hasBrickSound) {
                        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                            brickBreakSounds[i].setVolume(static_cast<float>(gameVolume));
                        }
                    }
                    if (hasPaddleSound) {
                        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                            paddleHitSounds[i].setVolume(static_cast<float>(gameVolume));
                        }
                    }
                    saveSettings(SETTINGS_FILE, difficulty, musicVolume, gameVolume);
                }

                if (exitSettings) {
                    saveSettings(SETTINGS_FILE, difficulty, musicVolume, gameVolume);
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
                else if (event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Return) {
                    bool isHighScore = false;
                    for (int i = 0; i < maximum_scores; i++) {
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
            else if (gameState == STATE_GAME_COMPLETE) {
                if (enteringName) {
                    if (getPlayerNameInput(event, playerName, nameLength, finishedNameInput)) {
                        saveHighScore(HIGHSCORE_FILE, score, playerName, highScores, highScoreNames);
                        loadHighScores(HIGHSCORE_FILE, highScores, highScoreNames);
                        enteringName = false;
                        gameState = STATE_MAIN_MENU;
                        selectedMenuOption = 0;
                    }
                }
                else if (event.type == Event::KeyPressed &&
                    event.key.code == Keyboard::Return) {
                    bool isHighScore = false;
                    for (int i = 0; i < maximum_scores; i++) {
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

        if (gameState == STATE_PLAYING) {
            bool leftPressed = Keyboard::isKeyPressed(Keyboard::Left) ||
                Keyboard::isKeyPressed(Keyboard::A);
            bool rightPressed = Keyboard::isKeyPressed(Keyboard::Right) ||
                Keyboard::isKeyPressed(Keyboard::D);

            updatePaddlePosition(paddleX, leftPressed, rightPressed, deltaTime, currentPaddleWidth);
            updateBallPosition(ballX, ballY, ballVelX, ballVelY, deltaTime, ballLaunched, paddleX, currentPaddleWidth);

            updatePowerUpTimers(powerUpTimer, activePowerUpType, currentPaddleWidth,
                ballSpeedMultiplier, difficulty, deltaTime);

            if (ballLaunched) {
                checkWallCollisions(ballX, ballY, BALL_RADIUS, ballVelX, ballVelY);

                if (checkPaddleCollision(ballX, ballY, BALL_RADIUS, paddleX, PADDLE_Y,
                    currentPaddleWidth, PADDLE_HEIGHT)) {
                    handlePaddleCollision(ballX, paddleX, currentPaddleWidth, ballVelX, ballVelY);

                    if (hasPaddleSound) {
                        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                            if (paddleHitSounds[currentPaddleSoundChannel].getStatus() != Sound::Playing) {
                                break;
                            }
                            currentPaddleSoundChannel = (currentPaddleSoundChannel + 1) % MAX_SOUND_CHANNELS;
                        }
                        paddleHitSounds[currentPaddleSoundChannel].play();
                        currentPaddleSoundChannel = (currentPaddleSoundChannel + 1) % MAX_SOUND_CHANNELS;
                    }
                }

                int hitBrick = checkBrickCollision(ballX, ballY, BALL_RADIUS, bricks);
                if (hitBrick != -1) {
                    handleBrickCollision(ballX, ballY, hitBrick, ballVelX, ballVelY);

                    if (bricks[hitBrick] > 0) {
                        int row = hitBrick / GRID_WIDTH;
                        int col = hitBrick % GRID_WIDTH;
                        float brickX = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING) + BRICK_WIDTH / 2;
                        float brickY = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING) + BRICK_HEIGHT / 2;

                        score += calculateBrickScore(bricks[hitBrick]);
                        bricks[hitBrick]--;

                        if (hasBrickSound) {
                            for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                                if (brickBreakSounds[currentBrickSoundChannel].getStatus() != Sound::Playing) {
                                    break;
                                }
                                currentBrickSoundChannel = (currentBrickSoundChannel + 1) % MAX_SOUND_CHANNELS;
                            }
                            brickBreakSounds[currentBrickSoundChannel].play();
                            currentBrickSoundChannel = (currentBrickSoundChannel + 1) % MAX_SOUND_CHANNELS;
                        }

                        if (bricks[hitBrick] == 0) {
                            spawnPowerUp(brickX, brickY, powerUpX, powerUpY, powerUpType, powerUpActive);
                            createParticles(brickX, brickY, particleX, particleY,
                                particleVelX, particleVelY, particleLife, particleActive);
                        }
                    }
                }

                if (checkBallLost(ballY, BALL_RADIUS)) {
                    lives--;
                    if (lives <= 0) {
                        gameState = STATE_GAME_OVER;
                    }
                    else {
                        float basePaddleWidth = getDifficultyPaddleWidth(difficulty);
                        resetBall(ballX, ballY, ballVelX, ballVelY, paddleX, ballLaunched,
                            basePaddleWidth, currentBallSpeed, currentPaddleWidth,
                            difficulty, level, true);
                        powerUpTimer = 0;
                        activePowerUpType = POWERUP_NONE;
                        ballSpeedMultiplier = 1.0f;
                    }
                }

                if (isLevelComplete(bricks)) {
                    if (level >= MAX_LEVELS) {
                        gameState = STATE_GAME_COMPLETE;
                    }
                    else {
                        nextLevel(level, score, ballX, ballY, ballVelX, ballVelY,
                            paddleX, bricks, ballLaunched, currentPaddleWidth,
                            currentBallSpeed, difficulty, powerUpTimer, activePowerUpType, ballSpeedMultiplier);
                        for (int i = 0; i < TOTAL_BRICKS; i++) {
                            brickTypes[i] = bricks[i];
                        }
                    }
                }
            }

            updatePowerUps(powerUpX, powerUpY, powerUpActive, deltaTime);
            updateParticles(particleX, particleY, particleVelX, particleVelY,
                particleLife, particleActive, deltaTime);

            int hitPowerUp = checkPowerUpCollision(paddleX, PADDLE_Y, currentPaddleWidth,
                PADDLE_HEIGHT, powerUpX, powerUpY, powerUpActive, powerUpType);
            if (hitPowerUp != -1) {
                int powerUpTypeCollected = powerUpType[hitPowerUp];
                applyPowerUp(powerUpTypeCollected, lives, currentPaddleWidth, ballSpeedMultiplier,
                    getDifficultyPaddleWidth(difficulty), powerUpTimer, activePowerUpType, score);
                powerUpActive[hitPowerUp] = false;
            }
        }

        // Clear and draw background
        if (gameState == STATE_INTRO) {
            window.clear(Color(10, 10, 30));
            if (hasLogo) {
                window.draw(logoSprite);
            }
        }
        else if (gameState == STATE_MAIN_MENU ||
            gameState == STATE_SETTINGS ||
            gameState == STATE_HIGH_SCORES ||
            gameState == STATE_GAME_OVER ||
            gameState == STATE_GAME_COMPLETE) {
            window.clear(Color(10, 10, 30));
            if (hasMenuBg) {
                window.draw(menuBgSprite);
            }
        }
        else {
            window.clear(Color(10, 15, 40));
            if (hasGameBg) {
                window.draw(gameBgSprite);
            }
        }

        // Render based on game state
        if (gameState == STATE_INTRO) {
            drawIntroscreen(window, logoSprite, hasLogo, font, titleFont, hasTitleFont);
        }
        else if (gameState == STATE_MAIN_MENU) {
            drawMainMenuSimple(window, font, selectedMenuOption, titleFont, hasTitleFont, menuFont, hasMenuFont);

            if (messageTimer > 0) {
                Text message(messageText, font, 24);
                message.setFillColor(Color::Red);
                message.setOutlineColor(Color::Black);
                message.setOutlineThickness(2);
                FloatRect msgBounds = message.getGlobalBounds();
                message.setPosition(window_width / 2.0f - msgBounds.width / 2.0f, 500);
                window.draw(message);
            }
        }
        else if (gameState == STATE_PLAYING || gameState == STATE_PAUSED) {
            drawBricksWithTypes(window, bricks, brickTypes,
                greenBrickIntact,
                yellowBrickIntact, yellowBrickCracked,
                redBrickIntact, redBrickCracked,
                grayBrick, hasBrickTextures);
            drawPaddle(window, paddleX, currentPaddleWidth, paddleTexture, hasPaddleTexture);
            drawBall(window, ballX, ballY, ballTexture, hasBallTexture);
            drawPowerUpsSimple(window, powerUpX, powerUpY, powerUpType, powerUpActive,
                heartTexture, starTexture, skullTexture, coinTexture, hasHeartTexture || hasStarTexture || hasSkullTexture || hasCoinTexture);
            drawSimpleParticles(window, particleX, particleY, particleActive);
            drawHUD(window, font, score, lives, level);

            if (messageTimer > 0) {
                Text message(messageText, font, 20);
                message.setFillColor(Color::Green);
                message.setOutlineColor(Color::Black);
                message.setOutlineThickness(2);
                FloatRect msgBounds = message.getGlobalBounds();
                message.setPosition(window_width / 2.0f - msgBounds.width / 2.0f, 40);
                window.draw(message);
            }

            if (gameState == STATE_PAUSED) {
                drawPauseMenu(window, font, selectedPauseOption, menuFont, hasMenuFont);
            }
        }
        else if (gameState == STATE_PAUSE_SETTINGS) {
            window.clear(Color(10, 15, 40));
            if (hasGameBg) {
                window.draw(gameBgSprite);
            }
            drawBricksWithTypes(window, bricks, brickTypes,
                greenBrickIntact,
                yellowBrickIntact, yellowBrickCracked,
                redBrickIntact, redBrickCracked,
                grayBrick, hasBrickTextures);
            drawPaddle(window, paddleX, currentPaddleWidth, paddleTexture, hasPaddleTexture);
            drawBall(window, ballX, ballY, ballTexture, hasBallTexture);
            drawPowerUpsSimple(window, powerUpX, powerUpY, powerUpType, powerUpActive,
                heartTexture, starTexture, skullTexture, coinTexture, hasHeartTexture || hasStarTexture || hasSkullTexture || hasCoinTexture);
            drawSimpleParticles(window, particleX, particleY, particleActive);
            drawHUD(window, font, score, lives, level);

            drawPauseSettings(window, font, musicVolume, gameVolume, selectedPauseSetting);
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
        else if (gameState == STATE_GAME_COMPLETE) {
            if (enteringName) {
                // Draw game complete screen
                window.clear(Color(10, 20, 50));
                Text title("CONGRATULATIONS!", font, 60);
                title.setFillColor(Color(255, 215, 0));
                title.setOutlineColor(Color(200, 150, 0));
                title.setOutlineThickness(3);
                FloatRect titleBounds = title.getGlobalBounds();
                title.setPosition(window_width / 2.0f - titleBounds.width / 2.0f, 120);
                window.draw(title);

                Text subtitle("You've Completed All Levels!", font, 32);
                subtitle.setFillColor(Color::White);
                FloatRect subtitleBounds = subtitle.getGlobalBounds();
                subtitle.setPosition(window_width / 2.0f - subtitleBounds.width / 2.0f, 200);
                window.draw(subtitle);

                char buffer[100];
                safeStringCopy(buffer, "Final Score: ");
                char scoreStr[20];
                intToString(score, scoreStr);
                concatStrings(buffer, scoreStr);
                Text scoreText(buffer, font, 32);
                scoreText.setFillColor(Color(64, 224, 208));
                FloatRect scoreBounds = scoreText.getGlobalBounds();
                scoreText.setPosition(window_width / 2.0f - scoreBounds.width / 2.0f, 260);
                window.draw(scoreText);

                drawNameInput(window, font, playerName);
            }
            else {
                // Draw game complete screen
                window.clear(Color(10, 20, 50));
                Text title("CONGRATULATIONS!", font, 60);
                title.setFillColor(Color(255, 215, 0));
                title.setOutlineColor(Color(200, 150, 0));
                title.setOutlineThickness(3);
                FloatRect titleBounds = title.getGlobalBounds();
                title.setPosition(window_width / 2.0f - titleBounds.width / 2.0f, 120);
                window.draw(title);

                Text subtitle("You've Completed All Levels!", font, 32);
                subtitle.setFillColor(Color::White);
                FloatRect subtitleBounds = subtitle.getGlobalBounds();
                subtitle.setPosition(window_width / 2.0f - subtitleBounds.width / 2.0f, 200);
                window.draw(subtitle);

                char buffer[100];
                safeStringCopy(buffer, "Final Score: ");
                char scoreStr[20];
                intToString(score, scoreStr);
                concatStrings(buffer, scoreStr);
                Text scoreText(buffer, font, 32);
                scoreText.setFillColor(Color(64, 224, 208));
                FloatRect scoreBounds = scoreText.getGlobalBounds();
                scoreText.setPosition(window_width / 2.0f - scoreBounds.width / 2.0f, 280);
                window.draw(scoreText);

                Text instruction("Press Enter to Continue", font, 20);
                instruction.setFillColor(Color(150, 150, 150));
                FloatRect instrBounds = instruction.getGlobalBounds();
                instruction.setPosition(window_width / 2.0f - instrBounds.width / 2.0f, 370);
                window.draw(instruction);
            }
        }
        else if (gameState == STATE_SETTINGS) {
            drawSettings(window, font, difficulty, musicVolume, gameVolume, selectedSetting);
        }
        else if (gameState == STATE_HIGH_SCORES) {
            drawHighScores(window, font, highScores, highScoreNames);
        }

        window.display();
    }

    return 0;
}
