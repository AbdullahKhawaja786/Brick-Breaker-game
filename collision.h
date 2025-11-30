#ifndef COLLISION_H
#define COLLISION_H

#include <cmath>
#include "constants.h"

// Check if ball collides with paddle
bool checkPaddleCollision(float ballX, float ballY, float ballRadius,
    float paddleX, float paddleY, float paddleWidth, float paddleHeight) {
    return ballX + ballRadius > paddleX &&
        ballX - ballRadius < paddleX + paddleWidth &&
        ballY + ballRadius > paddleY &&
        ballY - ballRadius < paddleY + paddleHeight;
}

// Handle paddle collision and update ball velocity
void handlePaddleCollision(float ballX, float paddleX, float paddleWidth,
    float& ballVelX, float& ballVelY) {
    // Reverse Y direction
    ballVelY = -fabs(ballVelY);

    // Add English based on where ball hits paddle
    float hitPos = (ballX - paddleX) / paddleWidth; // 0 to 1
    float angle = (hitPos - 0.5f) * 2.0f; // -1 to 1
    ballVelX = angle * BALL_SPEED * 0.7f;

    // Ensure minimum speed
    float speed = sqrt(ballVelX * ballVelX + ballVelY * ballVelY);
    if (speed < BALL_SPEED * 0.8f) {
        float ratio = BALL_SPEED * 0.8f / speed;
        ballVelX *= ratio;
        ballVelY *= ratio;
    }
}

// Check brick collision and return brick index (-1 if no collision)
int checkBrickCollision(float ballX, float ballY, float ballRadius, int bricks[]) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (bricks[i] <= 0) continue; // Skip destroyed bricks

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

// Determine collision side and update ball velocity
void handleBrickCollision(float ballX, float ballY, int brickIndex,
    float& ballVelX, float& ballVelY) {
    int row = brickIndex / GRID_WIDTH;
    int col = brickIndex % GRID_WIDTH;

    float brickX = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING);
    float brickY = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING);
    float brickCenterX = brickX + BRICK_WIDTH / 2.0f;
    float brickCenterY = brickY + BRICK_HEIGHT / 2.0f;

    float dx = ballX - brickCenterX;
    float dy = ballY - brickCenterY;

    // Determine which side was hit
    float overlapX = (BRICK_WIDTH / 2.0f + BALL_RADIUS) - fabs(dx);
    float overlapY = (BRICK_HEIGHT / 2.0f + BALL_RADIUS) - fabs(dy);

    if (overlapX < overlapY) {
        // Hit from left or right
        ballVelX = -ballVelX;
    }
    else {
        // Hit from top or bottom
        ballVelY = -ballVelY;
    }
}

// Check wall collisions
void checkWallCollisions(float& ballX, float& ballY, float ballRadius,
    float& ballVelX, float& ballVelY) {
    // Left wall
    if (ballX - ballRadius < 0) {
        ballX = ballRadius;
        ballVelX = fabs(ballVelX);
    }

    // Right wall
    if (ballX + ballRadius > WINDOW_WIDTH) {
        ballX = WINDOW_WIDTH - ballRadius;
        ballVelX = -fabs(ballVelX);
    }

    // Top wall
    if (ballY - ballRadius < 0) {
        ballY = ballRadius;
        ballVelY = fabs(ballVelY);
    }
}

// Check if ball fell below paddle (lose life)
bool checkBallLost(float ballY, float ballRadius) {
    return ballY - ballRadius > WINDOW_HEIGHT;
}

// Check power-up collision with paddle
int checkPowerUpCollision(float paddleX, float paddleY, float paddleWidth, float paddleHeight,
    float powerUpX[], float powerUpY[], bool powerUpActive[]) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUpActive[i]) continue;

        if (powerUpX[i] + POWERUP_SIZE > paddleX &&
            powerUpX[i] < paddleX + paddleWidth &&
            powerUpY[i] + POWERUP_SIZE > paddleY &&
            powerUpY[i] < paddleY + paddleHeight) {
            return i;
        }
    }
    return -1;
}

#endif
