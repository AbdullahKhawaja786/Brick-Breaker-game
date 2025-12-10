#ifndef COLLISION_H
#define COLLISION_H
#include <cmath>
#include "constants.h"
float absoluteValue(float value) {
    if (value < 0) {
        return -value;
    }
    return value;
}

bool checkPaddleCollision(float ballX, float ballY, float ballRadius,
    float paddleX, float paddleY, float paddleWidth, float paddleHeight) {
    return ballX + ballRadius > paddleX &&
        ballX - ballRadius < paddleX + paddleWidth &&
        ballY + ballRadius > paddleY &&
        ballY - ballRadius < paddleY + paddleHeight;
}

void handlePaddleCollision(float ballX, float paddleX, float paddleWidth,
    float& ballVelX, float& ballVelY) {
    ballVelY = -absoluteValue(ballVelY);
    float hitPos = (ballX - paddleX) / paddleWidth;
    float angle = (hitPos - 0.5f) * 2.0f; //-1 to 1
    ballVelX = angle * BALL_SPEED * 0.7f;
    float speed = sqrt(ballVelX * ballVelX + ballVelY * ballVelY);
    if (speed < BALL_SPEED * 0.8f) {
        float ratio = BALL_SPEED * 0.8f / speed;
        ballVelX *= ratio;
        ballVelY *= ratio;
    }
}

int checkBrickCollision(float ballX, float ballY, float ballRadius, int bricks[]) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (bricks[i] == 0) continue;

        int row = i / GRID_WIDTH;
        int col = i % GRID_WIDTH;
        float brickX = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING);
        float brickY = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING);

        if (ballX + ballRadius > brickX &&
            ballX - ballRadius < brickX + BRICK_WIDTH &&
            ballY + ballRadius > brickY &&
            ballY - ballRadius < brickY + BRICK_HEIGHT) {
            return i;
        }
    }
    return -1;
}

void handleBrickCollision(float& ballX, float& ballY, int brickIndex,
    float& ballVelX, float& ballVelY) {
    int row = brickIndex / GRID_WIDTH;
    int col = brickIndex % GRID_WIDTH;
    float brickX = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING);
    float brickY = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING);
    float brickCenterX = brickX + BRICK_WIDTH / 2.0f;
    float brickCenterY = brickY + BRICK_HEIGHT / 2.0f;
    float dx = ballX - brickCenterX;
    float dy = ballY - brickCenterY;
    float overlapX = (BRICK_WIDTH / 2.0f + BALL_RADIUS) - absoluteValue(dx);
    float overlapY = (BRICK_HEIGHT / 2.0f + BALL_RADIUS) - absoluteValue(dy);

    // Push ball out of brick before bouncing
    if (overlapX < overlapY) {
        // Horizontal collision 
        if (dx < 0) {
            ballX = brickX - BALL_RADIUS - 1.0f;
        }
        else {
            ballX = brickX + BRICK_WIDTH + BALL_RADIUS + 1.0f;
        }
        ballVelX = -ballVelX;
    }
    else {
        // Vertical collision
        if (dy < 0) {
            ballY = brickY - BALL_RADIUS - 1.0f;
        }
        else {
            ballY = brickY + BRICK_HEIGHT + BALL_RADIUS + 1.0f;
        }
        ballVelY = -ballVelY;
    }
}

void checkWallCollisions(float& ballX, float& ballY, float ballRadius,
    float& ballVelX, float& ballVelY) {
    if (ballX - ballRadius < 0) {
        ballX = ballRadius;
        ballVelX = absoluteValue(ballVelX);
    }
    if (ballX + ballRadius > window_width) {
        ballX = window_width - ballRadius;
        ballVelX = -absoluteValue(ballVelX);
    }
    if (ballY - ballRadius < 0) {
        ballY = ballRadius;
        ballVelY = absoluteValue(ballVelY);
    }
}

bool checkBallLost(float ballY, float ballRadius) {
    return ballY - ballRadius > window_height;
}

int checkPowerUpCollision(float paddleX, float paddleY, float paddleWidth, float paddleHeight,
    float powerUpX[], float powerUpY[], bool powerUpActive[], int powerUpType[]) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUpActive[i]) continue;
        float powerUpSize = (powerUpType[i] == POWERUP_BONUS_SCORE) ? POWERUP_COIN_SIZE : POWERUP_SIZE;
        if (powerUpX[i] + powerUpSize > paddleX &&
            powerUpX[i] < paddleX + paddleWidth &&
            powerUpY[i] + powerUpSize > paddleY &&
            powerUpY[i] < paddleY + paddleHeight) {
            return i;
        }
    }
    return -1;
}

#endif
