#ifndef GAME_LOGIC_H
#define GAME_LOGIC_H
#include <cstdlib>
#include <ctime>
#include <cmath>
#include "constants.h"
#include "collision.h"
// Get adjusted ball speed based on difficulty
float getDifficultyBallSpeed(int difficulty) {
    if (difficulty == 1) return BALL_SPEED * 0.8f;  // Easy: 80% speed
    if (difficulty == 2) return BALL_SPEED;         // Normal: 100% speed
    if (difficulty == 3) return BALL_SPEED * 1.3f;  // Hard: 130% speed
    return BALL_SPEED;
}
// Get initial lives based on difficulty
int getDifficultyLives(int difficulty) {
    if (difficulty == 1) return 5;  // Easy: 5 lives
    if (difficulty == 2) return 3;  // Normal: 3 lives
    if (difficulty == 3) return 2;  // Hard: 2 lives
    return INITIAL_LIVES;
}
// Get paddle width based on difficulty
float getDifficultyPaddleWidth(int difficulty) {
    if (difficulty == 1) return PADDLE_WIDTH * 1.3f;  // Easy: 30% wider
    if (difficulty == 2) return PADDLE_WIDTH;         // Normal: standard
    if (difficulty == 3) return PADDLE_WIDTH * 0.7f;  // Hard: 30% narrower
    return PADDLE_WIDTH;
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
                else if (row < 5) bricks[i] = 2;
                else bricks[i] = 1;
            }
            else {
                bricks[i] = 0;
            }
        }
        else if (level == 4) {
            if (row < 7) {
                bricks[i] = ((row + col) % 2 == 0) ? -1 : 2;
            }
            else {
                bricks[i] = 0;
            }
        }
        else {
            if (row < 2) bricks[i] = 3;
            else if (row < 4) bricks[i] = -1;
            else if (row < 6) bricks[i] = 2;
            else bricks[i] = 1;
        }
    }
}
void initializeGame(int& level, int& score, int& lives, float& ballX, float& ballY,
    float& ballVelX, float& ballVelY, float& paddleX,
    int bricks[], bool& ballLaunched, int difficulty, float& currentPaddleWidth, float& currentBallSpeed) {
    level = 1;
    score = 0;
    lives = getDifficultyLives(difficulty);
    currentPaddleWidth = getDifficultyPaddleWidth(difficulty);
    currentBallSpeed = getDifficultyBallSpeed(difficulty);

    paddleX = WINDOW_WIDTH / 2.0f - currentPaddleWidth / 2.0f;
    ballX = WINDOW_WIDTH / 2.0f;
    ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    ballVelX = 0.0f;
    ballVelY = 0.0f;
    ballLaunched = false;
    initializeBricks(bricks, level);
}
void resetBall(float& ballX, float& ballY, float& ballVelX, float& ballVelY,
    float paddleX, bool& ballLaunched, float paddleWidth) {
    ballX = paddleX + paddleWidth / 2.0f;
    ballY = PADDLE_Y - BALL_RADIUS - 5.0f;
    ballVelX = 0.0f;
    ballVelY = 0.0f;
    ballLaunched = false;
}
void launchBall(float& ballVelX, float& ballVelY, bool& ballLaunched, float ballSpeed) {
    if (!ballLaunched) {
        ballVelX = ballSpeed * 0.5f;
        ballVelY = -ballSpeed;
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
void updatePaddlePosition(float& paddleX, bool leftPressed, bool rightPressed, float deltaTime, float paddleWidth) {
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
void applyPowerUp(int type, int& lives, float& paddleWidth, float& ballSpeed, float basePaddleWidth) {
    if (type == POWERUP_EXTRA_LIFE) {
        lives++;
    }
    else if (type == POWERUP_LARGER_PADDLE) {
        paddleWidth = basePaddleWidth * 1.5f;
    }
    else if (type == POWERUP_SLOWER_BALL) {
        ballSpeed = ballSpeed * 0.7f;
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
    int bricks[], bool& ballLaunched, float paddleWidth) {
    level++;
    score += SCORE_LEVEL_COMPLETE;
    if (level > MAX_LEVELS) {
        level = MAX_LEVELS;
    }
    initializeBricks(bricks, level);
    resetBall(ballX, ballY, ballVelX, ballVelY, paddleX, ballLaunched, paddleWidth);
}
void createParticles(float x, float y, float particleX[], float particleY[],
    float particleVelX[], float particleVelY[],
    float particleLife[], bool particleActive[]) {
    int particlesCreated = 0;
    for (int i = 0; i < MAX_PARTICLES && particlesCreated < 8; i++) {
        if (!particleActive[i]) {
            particleActive[i] = true;
            particleX[i] = x;
            particleY[i] = y;
            float angle = (particlesCreated * 45.0f) * 3.14159f / 180.0f;
            particleVelX[i] = cos(angle) * 100.0f;
            particleVelY[i] = sin(angle) * 100.0f - 50.0f;
            particleLife[i] = PARTICLE_LIFETIME;
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
