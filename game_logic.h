#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "constants.h"
#include "collision.h"

float getDifficultyBallSpeed(int difficulty) {
    if (difficulty == 1) return BALL_SPEED * 0.8f;
    if (difficulty == 2) return BALL_SPEED;
    if (difficulty == 3) return BALL_SPEED * 1.3f;
    return BALL_SPEED;
}

int getDifficultyLives(int difficulty) {
    if (difficulty == 1) return 5;
    if (difficulty == 2) return 3;
    if (difficulty == 3) return 2;
    return INITIAL_LIVES;
}

float getDifficultyPaddleWidth(int difficulty) {
    if (difficulty == 1) return PADDLE_WIDTH * 1.3f;
    if (difficulty == 2) return PADDLE_WIDTH;
    if (difficulty == 3) return PADDLE_WIDTH * 0.7f;
    return PADDLE_WIDTH;
}

float getLevelBallSpeed(int level, int difficulty) {
    float baseSpeed = getDifficultyBallSpeed(difficulty);
    return baseSpeed + (level - 1) * BALL_SPEED_INCREMENT;
}

void initializeBricks(int bricks[], int level) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        int row = i / GRID_WIDTH;
        int col = i % GRID_WIDTH;

        if (level == 1) {
            bricks[i] = (row < 4) ? 1 : 0;
        }
        else if (level == 2) {
            if (row < 6) {
                bricks[i] = (row % 2 == 0) ? 2 : 1;
            }
            else {
                bricks[i] = 0;
            }
        }
        else if (level == 3) {
            if (row < 6) {
                if (row < 2) bricks[i] = 3;
                else if (row < 4) bricks[i] = 2;
                else bricks[i] = 1;
            }
            else {
                bricks[i] = 0;
            }
        }
        else if (level == 4) {
            if (row < 7) {
                if (row == 2 && (col == 2 || col == 4 || col == 5 || col == 7)) {
                    bricks[i] = -1;
                }
                else if (row == 4 && (col == 1 || col == 3 || col == 6 || col == 8)) {
                    bricks[i] = -1;
                }
                else if (row < 3) {
                    bricks[i] = 3;
                }
                else {
                    bricks[i] = 2;
                }
            }
            else {
                bricks[i] = 0;
            }
        }
        else {
            if (row < 7) {
                if (row < 2) {
                    bricks[i] = 3;
                }
                else if (row == 3 && col % 2 == 0) {
                    bricks[i] = -1;
                }
                else if (row < 5) {
                    bricks[i] = 3;
                }
                else {
                    bricks[i] = 2;
                }
            }
            else {
                bricks[i] = 1;
            }
        }
    }
}

void initializeGame(int& level, int& score, int& lives, float& ballX, float& ballY,
    float& ballVelX, float& ballVelY, float& paddleX,
    int bricks[], bool& ballLaunched, int difficulty,
    float& currentPaddleWidth, float& currentBallSpeed) {
    level = 1;
    score = 0;
    lives = getDifficultyLives(difficulty);
    currentPaddleWidth = getDifficultyPaddleWidth(difficulty);
    currentBallSpeed = getLevelBallSpeed(level, difficulty);

    paddleX = WINDOW_WIDTH / 2.0f - currentPaddleWidth / 2.0f;
    ballX = WINDOW_WIDTH / 2.0f;
    ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    ballVelX = 0.0f;
    ballVelY = 0.0f;
    ballLaunched = false;

    initializeBricks(bricks, level);
}

void resetBall(float& ballX, float& ballY, float& ballVelX, float& ballVelY,
    float paddleX, bool& ballLaunched, float basePaddleWidth,
    float& currentBallSpeed, float& currentPaddleWidth,
    int difficulty, int level, bool resetPowerUps = true) {
    ballX = paddleX + currentPaddleWidth / 2.0f;
    ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    ballVelX = 0.0f;
    ballVelY = 0.0f;
    ballLaunched = false;

    if (resetPowerUps) {
        currentBallSpeed = getLevelBallSpeed(level, difficulty);
        currentPaddleWidth = basePaddleWidth;
    }
}

void launchBall(float& ballVelX, float& ballVelY, bool& ballLaunched, float ballSpeed, float speedMultiplier) {
    if (!ballLaunched) {
        float actualSpeed = ballSpeed * speedMultiplier;
        float angleVariation = (rand() % 100 - 50) / 100.0f * 0.3f;
        ballVelX = actualSpeed * angleVariation;
        ballVelY = -actualSpeed;
        ballLaunched = true;
    }
}

void updateBallPosition(float& ballX, float& ballY, float ballVelX, float ballVelY,
    float deltaTime, bool ballLaunched, float paddleX, float paddleWidth) {
    if (ballLaunched) {
        ballX += ballVelX * deltaTime;
        ballY += ballVelY * deltaTime;
    }
    else {
        ballX = paddleX + paddleWidth / 2.0f;
        ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    }
}

void updatePaddlePosition(float& paddleX, bool leftPressed, bool rightPressed,
    float deltaTime, float paddleWidth) {
    if (leftPressed && paddleX > 0) {
        paddleX -= PADDLE_SPEED * deltaTime;
        if (paddleX < 0) paddleX = 0;
    }
    if (rightPressed && paddleX < WINDOW_WIDTH - paddleWidth) {
        paddleX += PADDLE_SPEED * deltaTime;
        if (paddleX > WINDOW_WIDTH - paddleWidth) {
            paddleX = WINDOW_WIDTH - paddleWidth;
        }
    }
}

int calculateBrickScore(int brickType) {
    if (brickType == 1) return SCORE_BRICK_1HIT;
    if (brickType == 2) return SCORE_BRICK_2HIT;
    if (brickType >= 3) return SCORE_BRICK_3HIT;
    return 0;
}

void spawnPowerUp(float x, float y, float powerUpX[], float powerUpY[],
    int powerUpType[], bool powerUpActive[]) {
    if (rand() % 100 < POWERUP_CHANCE) {
        for (int i = 0; i < MAX_POWERUPS; i++) {
            if (!powerUpActive[i]) {
                powerUpX[i] = x;
                powerUpY[i] = y;
                powerUpType[i] = (rand() % 4) + 1;
                powerUpActive[i] = true;
                break;
            }
        }
    }
}

void updatePowerUps(float powerUpX[], float powerUpY[], bool powerUpActive[],
    float deltaTime) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (powerUpActive[i]) {
            powerUpY[i] += POWERUP_FALL_SPEED * deltaTime;
            if (powerUpY[i] > WINDOW_HEIGHT) {
                powerUpActive[i] = false;
            }
        }
    }
}

void applyPowerUp(int type, int& lives, float& paddleWidth, float& ballSpeedMultiplier,
    float basePaddleWidth, float& powerUpTimer, int& activePowerUpType, int& score) {
    if (type == POWERUP_EXTRA_LIFE) {
        lives++;
    }
    else if (type == POWERUP_LARGER_PADDLE) {
        paddleWidth = basePaddleWidth * 1.5f;
        powerUpTimer = POWERUP_DURATION;
        activePowerUpType = POWERUP_LARGER_PADDLE;
    }
    else if (type == POWERUP_SMALLER_PADDLE) {
        paddleWidth = basePaddleWidth * 0.5f;
        powerUpTimer = POWERUP_DURATION;
        activePowerUpType = POWERUP_SMALLER_PADDLE;
    }
    else if (type == POWERUP_BONUS_SCORE) {
        score += SCORE_BONUS_POWERUP;
    }
}

void updatePowerUpTimers(float& powerUpTimer, int& activePowerUpType,
    float& currentPaddleWidth, float& ballSpeedMultiplier,
    int difficulty, float deltaTime) {
    if (powerUpTimer > 0) {
        powerUpTimer -= deltaTime;
        if (powerUpTimer <= 0) {
            currentPaddleWidth = getDifficultyPaddleWidth(difficulty);
            activePowerUpType = POWERUP_NONE;
        }
    }
}

bool isLevelComplete(int bricks[]) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (bricks[i] > 0) return false;
    }
    return true;
}

void nextLevel(int& level, int& score, float& ballX, float& ballY,
    float& ballVelX, float& ballVelY, float& paddleX,
    int bricks[], bool& ballLaunched, float& currentPaddleWidth,
    float& currentBallSpeed, int difficulty,
    float& powerUpTimer, int& activePowerUpType, float& ballSpeedMultiplier) {
    level++;
    score += SCORE_LEVEL_COMPLETE;

    if (level > MAX_LEVELS) {
        level = MAX_LEVELS;
    }

    currentBallSpeed = getLevelBallSpeed(level, difficulty);
    float basePaddleWidth = getDifficultyPaddleWidth(difficulty);
    currentPaddleWidth = basePaddleWidth;

    powerUpTimer = 0;
    activePowerUpType = POWERUP_NONE;
    ballSpeedMultiplier = 1.0f;

    initializeBricks(bricks, level);
    resetBall(ballX, ballY, ballVelX, ballVelY, paddleX, ballLaunched,
        basePaddleWidth, currentBallSpeed, currentPaddleWidth,
        difficulty, level, false);
}

void createParticles(float x, float y, float particleX[], float particleY[],
    float particleVelX[], float particleVelY[],
    float particleLife[], bool particleActive[]) {

    float velX[8] = { 150, 106, 0, -106, -150, -106, 0, 106 };
    float velY[8] = { 0, 106, 150, 106, 0, -106, -150, -106 };

    for (int i = 0, n = 0; i < MAX_PARTICLES && n < 8; i++) {
        if (!particleActive[i]) {
            particleActive[i] = true;
            particleX[i] = x;
            particleY[i] = y;
            particleVelX[i] = velX[n];
            particleVelY[i] = velY[n];
            particleLife[i] = PARTICLE_LIFETIME;
            n++;
        }
    }
}
void updateParticles(float particleX[], float particleY[], float particleVelX[],
    float particleVelY[], float particleLife[], bool particleActive[],
    float deltaTime) {

    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (particleActive[i]) {
            particleX[i] += particleVelX[i] * deltaTime;
            particleY[i] += particleVelY[i] * deltaTime;
            particleLife[i] -= deltaTime;
            if (particleLife[i] <= 0) {
                particleActive[i] = false;
            }
        }
    }
}
#endif
