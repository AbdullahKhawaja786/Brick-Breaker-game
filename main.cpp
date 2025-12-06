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

    // Create window
    RenderWindow window(VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), WINDOW_TITLE);
    window.setFramerateLimit(60);

    // Load font
    Font font;
    if (!font.loadFromFile("C:/Windows/Fonts/arial.ttf")) {
        cerr << "ERROR: Failed to load font. Exiting." << endl;
        return -1;
    }

    // Load custom title font
    Font titleFont;
    bool hasTitleFont = titleFont.loadFromFile("GameFont.ttf");
    if (!hasTitleFont) {
        cerr << "WARNING: Could not load custom title font, using default" << endl;
    }

    // Load custom menu font
    Font menuFont;
    bool hasMenuFont = menuFont.loadFromFile("MenuFont.ttf");
    if (!hasMenuFont) {
        cerr << "WARNING: Could not load custom menu font, using default" << endl;
    }

    // Load and setup menu music
    Music menuMusic;
    bool hasMenuMusic = menuMusic.openFromFile(MENU_MUSIC_FILE);
    if (!hasMenuMusic) {
        cerr << "WARNING: Could not load menu_music.ogg" << endl;
    }

    // Load brick break sound effect
    SoundBuffer brickBreakBuffer;
    bool hasBrickSound = brickBreakBuffer.loadFromFile(BRICK_BREAK_SOUND_FILE);
    if (!hasBrickSound) {
        cerr << "WARNING: Could not load brick_break.ogg" << endl;
    }

    // Load paddle hit sound effect
    SoundBuffer paddleHitBuffer;
    bool hasPaddleSound = paddleHitBuffer.loadFromFile("paddle_hit.ogg");
    if (!hasPaddleSound) {
        cerr << "WARNING: Could not load paddle_hit.ogg" << endl;
    }

    // Create multiple sound channels for simultaneous sound effects
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

    // Load background images
    Texture menuBackground, gameBackground;
    Sprite menuBgSprite, gameBgSprite;
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
    Texture greenBrickIntact, greenBrickCracked;
    bool hasGreenBrick = greenBrickIntact.loadFromFile("green_brick.png");
    greenBrickCracked.loadFromFile("green_brick_cracked.png");

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
        cerr << "WARNING: Could not load brick textures - using fallback colors" << endl;
    }

    // Load other textures
    Texture paddleTexture, ballTexture;
    bool hasPaddleTexture = paddleTexture.loadFromFile("paddle.png");
    bool hasBallTexture = ballTexture.loadFromFile("ball.png");

    Texture heartTexture, starTexture, skullTexture, coinTexture;
    bool hasHeartTexture = heartTexture.loadFromFile("heart.png");
    bool hasStarTexture = starTexture.loadFromFile("star.png");
    bool hasSkullTexture = skullTexture.loadFromFile("skull.png");
    bool hasCoinTexture = coinTexture.loadFromFile("coin.png");

    // Game state
    int gameState = STATE_MAIN_MENU;
    int selectedMenuOption = 0;
    int selectedPauseOption = 0;
    int selectedSetting = 0;  // For settings menu: 0=difficulty, 1=music, 2=game volume

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
    float ballSpeedMultiplier = 1.0f;
    bool ballLaunched = false;

    // Power-up timer tracking
    float powerUpTimer = 0.0f;
    int activePowerUpType = POWERUP_NONE;

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

    // Settings - now includes music volume and game volume
    int difficulty = 1;
    int musicVolume = DEFAULT_MUSIC_VOLUME;
    int gameVolume = DEFAULT_GAME_VOLUME;
    loadSettings(SETTINGS_FILE, difficulty, musicVolume, gameVolume);

    // Apply loaded volumes
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
    Clock clock;

    // Track previous game state for music control
    int previousGameState = STATE_MAIN_MENU;

    // Start menu music if available
    if (hasMenuMusic && gameState == STATE_MAIN_MENU) {
        menuMusic.play();
    }

    // Game loop
    while (window.isOpen()) {
        float deltaTime = clock.restart().asSeconds();

        // Cap deltaTime to prevent physics issues
        if (deltaTime > 0.05f) {
            deltaTime = 0.05f;
        }

        // Handle music based on game state changes
        if (gameState != previousGameState) {
            if (hasMenuMusic) {
                // Play music when in main menu, settings, high scores, or game over
                if (gameState == STATE_MAIN_MENU ||
                    gameState == STATE_SETTINGS ||
                    gameState == STATE_HIGH_SCORES ||
                    gameState == STATE_GAME_OVER) {
                    if (menuMusic.getStatus() != Music::Playing) {
                        menuMusic.play();
                    }
                }
                else {
                    // Stop music when playing or paused
                    menuMusic.stop();
                }
            }
            previousGameState = gameState;
        }

        // Input handling
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
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
                    powerUpTimer = 0;
                    activePowerUpType = POWERUP_NONE;
                    ballSpeedMultiplier = 1.0f;
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
                        // Clear all power-ups when loading
                        for (int i = 0; i < MAX_POWERUPS; i++) {
                            powerUpActive[i] = false;
                        }
                        gameState = STATE_PLAYING;
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
                bool volumeChanged = false;
                handleSettingsInput(event, difficulty, musicVolume, gameVolume,
                    selectedSetting, exitSettings, volumeChanged);

                // Update volumes in real-time
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
            bool leftPressed = Keyboard::isKeyPressed(Keyboard::Left) ||
                Keyboard::isKeyPressed(Keyboard::A);
            bool rightPressed = Keyboard::isKeyPressed(Keyboard::Right) ||
                Keyboard::isKeyPressed(Keyboard::D);

            updatePaddlePosition(paddleX, leftPressed, rightPressed, deltaTime, currentPaddleWidth);
            updateBallPosition(ballX, ballY, ballVelX, ballVelY, deltaTime, ballLaunched, paddleX, currentPaddleWidth);

            // Update power-up timers
            updatePowerUpTimers(powerUpTimer, activePowerUpType, currentPaddleWidth,
                ballSpeedMultiplier, difficulty, deltaTime);

            if (ballLaunched) {
                checkWallCollisions(ballX, ballY, BALL_RADIUS, ballVelX, ballVelY);

                if (checkPaddleCollision(ballX, ballY, BALL_RADIUS, paddleX, PADDLE_Y,
                    currentPaddleWidth, PADDLE_HEIGHT)) {
                    handlePaddleCollision(ballX, paddleX, currentPaddleWidth, ballVelX, ballVelY);

                    // Play paddle hit sound using round-robin channel selection
                    if (hasPaddleSound) {
                        // Find the next available or oldest sound channel
                        bool foundChannel = false;
                        for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                            if (paddleHitSounds[currentPaddleSoundChannel].getStatus() != Sound::Playing) {
                                foundChannel = true;
                                break;
                            }
                            currentPaddleSoundChannel = (currentPaddleSoundChannel + 1) % MAX_SOUND_CHANNELS;
                        }

                        // Play sound on current channel (will interrupt if all channels busy)
                        paddleHitSounds[currentPaddleSoundChannel].play();
                        currentPaddleSoundChannel = (currentPaddleSoundChannel + 1) % MAX_SOUND_CHANNELS;
                    }
                }

                int hitBrick = checkBrickCollision(ballX, ballY, BALL_RADIUS, bricks);
                if (hitBrick != -1) {
                    // Always handle collision (even for indestructible bricks)
                    handleBrickCollision(ballX, ballY, hitBrick, ballVelX, ballVelY);

                    // Only damage/destroy destructible bricks
                    if (bricks[hitBrick] > 0) {
                        int row = hitBrick / GRID_WIDTH;
                        int col = hitBrick % GRID_WIDTH;
                        float brickX = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING) + BRICK_WIDTH / 2;
                        float brickY = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING) + BRICK_HEIGHT / 2;

                        score += calculateBrickScore(bricks[hitBrick]);
                        bricks[hitBrick]--;

                        // Play brick break sound using round-robin channel selection
                        if (hasBrickSound) {
                            // Find the next available or oldest sound channel
                            bool foundChannel = false;
                            for (int i = 0; i < MAX_SOUND_CHANNELS; i++) {
                                if (brickBreakSounds[currentBrickSoundChannel].getStatus() != Sound::Playing) {
                                    foundChannel = true;
                                    break;
                                }
                                currentBrickSoundChannel = (currentBrickSoundChannel + 1) % MAX_SOUND_CHANNELS;
                            }

                            // Play sound on current channel (will interrupt if all channels busy)
                            brickBreakSounds[currentBrickSoundChannel].play();
                            currentBrickSoundChannel = (currentBrickSoundChannel + 1) % MAX_SOUND_CHANNELS;
                        }

                        if (bricks[hitBrick] == 0) {
                            spawnPowerUp(brickX, brickY, powerUpX, powerUpY, powerUpType, powerUpActive);
                            createParticles(brickX, brickY, particleX, particleY,
                                particleVelX, particleVelY, particleLife, particleActive);
                        }
                    }
                    // For indestructible bricks (bricks[hitBrick] == -1), just bounce off
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
                        gameState = STATE_GAME_OVER;
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
                PADDLE_HEIGHT, powerUpX, powerUpY, powerUpActive);
            if (hitPowerUp != -1) {
                int powerUpTypeCollected = powerUpType[hitPowerUp];
                applyPowerUp(powerUpTypeCollected, lives, currentPaddleWidth, ballSpeedMultiplier,
                    getDifficultyPaddleWidth(difficulty), powerUpTimer, activePowerUpType, score);
                // Don't add SCORE_POWERUP here since BONUS_SCORE already adds to score
                if (powerUpTypeCollected != POWERUP_BONUS_SCORE) {
                    score += SCORE_POWERUP;
                }
                powerUpActive[hitPowerUp] = false;
            }
        }

        // Rendering
        if (gameState == STATE_MAIN_MENU || gameState == STATE_SETTINGS ||
            gameState == STATE_HIGH_SCORES || gameState == STATE_GAME_OVER) {
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

        if (gameState == STATE_MAIN_MENU) {
            drawMainMenuSimple(window, font, selectedMenuOption, titleFont, hasTitleFont, menuFont, hasMenuFont);
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
                heartTexture, starTexture, skullTexture, coinTexture, hasHeartTexture || hasStarTexture || hasSkullTexture || hasCoinTexture);
            drawSimpleParticles(window, particleX, particleY, particleActive);
            drawHUD(window, font, score, lives, level);

            if (gameState == STATE_PAUSED) {
                drawPauseMenu(window, font, selectedPauseOption, menuFont, hasMenuFont);
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
            drawSettings(window, font, difficulty, musicVolume, gameVolume, selectedSetting);
        }
        else if (gameState == STATE_HIGH_SCORES) {
            drawHighScores(window, font, highScores, highScoreNames);
        }

        window.display();
    }

    return 0;
}
