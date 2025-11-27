// game_logic.h
#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H

#include <cstdlib>
#include <ctime>
#include <cmath>
#include "constants.h"
#include "collision.h"

void initializeBricks(int bricks[], int level) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        int row = i / GRID_WIDTH;
        int col = i % GRID_WIDTH;

        if (level == 1) {
            // 4 rows of 1-hit bricks
            bricks[i] = (row < 4) ? 1 : 0;

        }
        else if (level == 2) {
            // 6 rows, alternating 1-hit and 2-hit
            if (row < 6) {
                bricks[i] = (row % 2 == 0) ? 2 : 1;
            }
            else {
                bricks[i] = 0;
            }

        }
        else if (level == 3) {
            // 6 rows with harder bricks at top
            if (row < 6) {
                if (row < 2) bricks[i] = 3;
                else if (row < 5) bricks[i] = 2;
                else bricks[i] = 1;
            }
            else {
                bricks[i] = 0;
            }

        }
        else if (level == 4) {
            // 7 rows checkerboard with indestructible bricks
            if (row < 7) {
                bricks[i] = ((row + col) % 2 == 0) ? -1 : 2;
            }
            else {
                bricks[i] = 0;
            }

        }
        else {
            // Level 5: Full 8 rows with mixed difficulty
            if (row < 2) bricks[i] = 3;
            else if (row < 4) bricks[i] = -1;
            else if (row < 6) bricks[i] = 2;
            else bricks[i] = 1;
        }
    }
}

void initializeGame(int& level, int& score, int& lives, float& ballX, float& ballY,
    float& ballVelX, float& ballVelY, float& paddleX,
    int bricks[], bool& ballLaunched) {
    level = 1;
    score = 0;
    lives = INITIAL_LIVES;
    paddleX = WINDOW_WIDTH / 2.0f - PADDLE_WIDTH / 2.0f;
    ballX = WINDOW_WIDTH / 2.0f;
    ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    ballVelX = 0.0f;
    ballVelY = 0.0f;
    ballLaunched = false;

    initializeBricks(bricks, level);
}

void resetBall(float& ballX, float& ballY, float& ballVelX, float& ballVelY,
    float paddleX, bool& ballLaunched) {
    ballX = paddleX + PADDLE_WIDTH / 2.0f;
    ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    ballVelX = 0.0f;
    ballVelY = 0.0f;
    ballLaunched = false;
}

void launchBall(float& ballVelX, float& ballVelY, bool& ballLaunched) {
    if (!ballLaunched) {
        ballVelX = BALL_SPEED * 0.5f;
        ballVelY = -BALL_SPEED;
        ballLaunched = true;
    }
}

void updateBallPosition(float& ballX, float& ballY, float ballVelX, float ballVelY,
    float deltaTime, bool ballLaunched, float paddleX) {
    if (ballLaunched) {
        ballX += ballVelX * deltaTime;
        ballY += ballVelY * deltaTime;
    }
    else {
        ballX = paddleX + PADDLE_WIDTH / 2.0f;
        ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    }
}

void updatePaddlePosition(float& paddleX, bool leftPressed, bool rightPressed, float deltaTime) {
    if (leftPressed && paddleX > 0) {
        paddleX -= PADDLE_SPEED * deltaTime;
        if (paddleX < 0) paddleX = 0;
    }
    if (rightPressed && paddleX < WINDOW_WIDTH - PADDLE_WIDTH) {
        paddleX += PADDLE_SPEED * deltaTime;
        if (paddleX > WINDOW_WIDTH - PADDLE_WIDTH) {
            paddleX = WINDOW_WIDTH - PADDLE_WIDTH;
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

void applyPowerUp(int type, int& lives, float& paddleWidth, float& ballSpeed) {
    if (type == POWERUP_EXTRA_LIFE) {
        lives++;
    }
    else if (type == POWERUP_LARGER_PADDLE) {
        paddleWidth = PADDLE_WIDTH * 1.5f;
    }
    else if (type == POWERUP_SLOWER_BALL) {
        ballSpeed = BALL_SPEED * 0.7f;
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
    int bricks[], bool& ballLaunched) {
    level++;
    score += SCORE_LEVEL_COMPLETE;

    if (level > MAX_LEVELS) {
        level = MAX_LEVELS;
    }

    initializeBricks(bricks, level);
    resetBall(ballX, ballY, ballVelX, ballVelY, paddleX, ballLaunched);
}

void createParticles(float x, float y, float particleX[], float particleY[],
    float particleVelX[], float particleVelY[],
    float particleLife[], bool particleActive[]) {
    int particlesCreated = 0;
    for (int i = 0; i < MAX_PARTICLES && particlesCreated < 8; i++) {
        if (!particleActive[i]) {
            particleX[i] = x;
            particleY[i] = y;
            float angle = (rand() % 360) * 3.14159f / 180.0f;
            float speed = 50.0f + rand() % 100;
            particleVelX[i] = cos(angle) * speed;
            particleVelY[i] = sin(angle) * speed;
            particleLife[i] = PARTICLE_LIFETIME;
            particleActive[i] = true;
            particlesCreated++;
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
