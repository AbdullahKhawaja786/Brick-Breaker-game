#ifndef RENDERING_H
#define RENDERING_H

#include <SFML/Graphics.hpp>
#include <cstring>
#include "constants.h"

// Helper function to convert int to string
void intToString(int num, char str[]) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }

    int i = 0;
    int temp = num;
    bool isNegative = false;

    if (num < 0) {
        isNegative = true;
        temp = -temp;
    }

    while (temp > 0) {
        i++;
        temp /= 10;
    }

    int len = i;
    if (isNegative) len++;

    str[len] = '\0';
    temp = (num < 0) ? -num : num;

    while (temp > 0) {
        str[--len] = (temp % 10) + '0';
        temp /= 10;
    }

    if (isNegative) {
        str[0] = '-';
    }
}

// Helper function to concatenate strings
void concatStrings(char dest[], const char src[]) {
    int destLen = strlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
}

// Draw animated star background
void drawAnimatedBackground(sf::RenderWindow& window, float starX[], float starY[], float starSize[]) {
    for (int i = 0; i < MAX_BG_STARS; i++) {
        sf::CircleShape star(starSize[i]);
        star.setPosition(starX[i], starY[i]);

        int brightness = 150 + (i % 100);
        star.setFillColor(sf::Color(brightness, brightness, 255));
        window.draw(star);
    }
}

// Draw main menu
void drawMainMenu(sf::RenderWindow& window, sf::Font& font, int selectedOption,
    float starX[], float starY[], float starSize[]) {
    window.clear(sf::Color(10, 10, 30));

    drawAnimatedBackground(window, starX, starY, starSize);

    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);

    // Simple title without glow effect
    sf::Text title("BRICK BREAKER", font, 60);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 80);
    window.draw(title);

    sf::CircleShape circle1(80);
    circle1.setPosition(50, 100);
    circle1.setFillColor(sf::Color(50, 100, 200, 50));
    window.draw(circle1);

    sf::CircleShape circle2(60);
    circle2.setPosition(650, 400);
    circle2.setFillColor(sf::Color(200, 50, 100, 50));
    window.draw(circle2);

    const char* options[] = { "Start New Game", "Load Game", "High Scores", "Settings", "Exit" };
    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        sf::Text text(options[i], font, 30);
        if (i == selectedOption) {
            text.setFillColor(sf::Color::Yellow);
            text.setStyle(sf::Text::Bold);
            text.setScale(1.1f, 1.1f);
        }
        else {
            text.setFillColor(sf::Color::White);
        }
        text.setPosition(WINDOW_WIDTH / 2 - text.getGlobalBounds().width / 2, 250 + i * 50);
        window.draw(text);
    }

    sf::Text instructions("Use Arrow Keys to Navigate, Enter to Select", font, 16);
    instructions.setFillColor(sf::Color(200, 200, 200));
    instructions.setPosition(WINDOW_WIDTH / 2 - instructions.getGlobalBounds().width / 2, 550);
    window.draw(instructions);
}

// Draw pause menu
void drawPauseMenu(sf::RenderWindow& window, sf::Font& font, int selectedOption) {
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text title("PAUSED", font, 50);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 150);
    window.draw(title);

    const char* options[] = { "Resume", "Save Game", "Settings", "Main Menu" };
    for (int i = 0; i < PAUSE_OPTIONS_COUNT; i++) {
        sf::Text text(options[i], font, 30);
        if (i == selectedOption) {
            text.setFillColor(sf::Color::Yellow);
            text.setStyle(sf::Text::Bold);
        }
        else {
            text.setFillColor(sf::Color::White);
        }
        text.setPosition(WINDOW_WIDTH / 2 - text.getGlobalBounds().width / 2, 280 + i * 50);
        window.draw(text);
    }
}

// Draw bricks - UPDATED COORDINATES
void drawBricks(sf::RenderWindow& window, int bricks[], sf::Texture& spriteSheet, bool useSpriteSheet) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (bricks[i] <= 0 && bricks[i] != -1) continue;

        int row = i / GRID_WIDTH;
        int col = i % GRID_WIDTH;

        float x = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING);
        float y = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING);

        if (useSpriteSheet) {
            sf::Sprite brickSprite(spriteSheet);

            // Updated sprite coordinates based on your spritesheet
            if (bricks[i] == -1) {
                // Unbreakable brick (gray brick)
                brickSprite.setTextureRect(sf::IntRect(610, 500, 270, 95));
            }
            else if (bricks[i] == 1) {
                // 1-hit brick (green brick with cracks)
                brickSprite.setTextureRect(sf::IntRect(10, 215, 280, 95));
            }
            else if (bricks[i] == 2) {
                // 2-hit brick (yellow brick)
                brickSprite.setTextureRect(sf::IntRect(300, 215, 280, 95));
            }
            else if (bricks[i] >= 3) {
                // 3-hit brick (red brick)
                brickSprite.setTextureRect(sf::IntRect(590, 215, 280, 95));
            }

            float scaleX = BRICK_WIDTH / brickSprite.getTextureRect().width;
            float scaleY = BRICK_HEIGHT / brickSprite.getTextureRect().height;
            brickSprite.setScale(scaleX, scaleY);
            brickSprite.setPosition(x, y);

            window.draw(brickSprite);
        }
        else {
            sf::RectangleShape brick(sf::Vector2f(BRICK_WIDTH, BRICK_HEIGHT));
            brick.setPosition(x, y);

            if (bricks[i] == -1) {
                brick.setFillColor(sf::Color(80, 80, 80));
            }
            else if (bricks[i] == 1) {
                brick.setFillColor(sf::Color(50, 255, 50));
            }
            else if (bricks[i] == 2) {
                brick.setFillColor(sf::Color(255, 255, 50));
            }
            else {
                brick.setFillColor(sf::Color(255, 50, 50));
            }

            brick.setOutlineColor(sf::Color::Black);
            brick.setOutlineThickness(2);
            window.draw(brick);
        }
    }
}

// Draw paddle - UPDATED COORDINATES
void drawPaddle(sf::RenderWindow& window, float paddleX, float paddleWidth, sf::Texture& spriteSheet, bool useSpriteSheet) {
    if (useSpriteSheet) {
        sf::Sprite paddleSprite(spriteSheet);
        // Medium paddle from spritesheet
        paddleSprite.setTextureRect(sf::IntRect(10, 710, 230, 50));

        float scaleX = paddleWidth / paddleSprite.getTextureRect().width;
        float scaleY = PADDLE_HEIGHT / paddleSprite.getTextureRect().height;
        paddleSprite.setScale(scaleX, scaleY);
        paddleSprite.setPosition(paddleX, PADDLE_Y);

        window.draw(paddleSprite);
    }
    else {
        sf::RectangleShape paddle(sf::Vector2f(paddleWidth, PADDLE_HEIGHT));
        paddle.setPosition(paddleX, PADDLE_Y);
        paddle.setFillColor(sf::Color::Cyan);
        paddle.setOutlineColor(sf::Color::White);
        paddle.setOutlineThickness(2);
        window.draw(paddle);
    }
}

// Draw ball - UPDATED COORDINATES
void drawBall(sf::RenderWindow& window, float ballX, float ballY, sf::Texture& spriteSheet, bool useSpriteSheet) {
    if (useSpriteSheet) {
        sf::Sprite ballSprite(spriteSheet);
        // Blue ball from spritesheet
        ballSprite.setTextureRect(sf::IntRect(1030, 510, 50, 50));

        float scale = (BALL_RADIUS * 2) / ballSprite.getTextureRect().width;
        ballSprite.setScale(scale, scale);
        ballSprite.setPosition(ballX - BALL_RADIUS, ballY - BALL_RADIUS);

        window.draw(ballSprite);
    }
    else {
        sf::CircleShape ball(BALL_RADIUS);
        ball.setPosition(ballX - BALL_RADIUS, ballY - BALL_RADIUS);
        ball.setFillColor(sf::Color::White);
        ball.setOutlineColor(sf::Color(200, 200, 255));
        ball.setOutlineThickness(2);
        window.draw(ball);
    }
}

// Draw power-ups - UPDATED COORDINATES
void drawPowerUps(sf::RenderWindow& window, float powerUpX[], float powerUpY[],
    int powerUpType[], bool powerUpActive[], sf::Texture& spriteSheet, bool useSpriteSheet) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUpActive[i]) continue;

        if (useSpriteSheet) {
            sf::Sprite powerUpSprite(spriteSheet);

            if (powerUpType[i] == POWERUP_MULTIBALL) {
                // Multi-ball powerup (three balls icon)
                powerUpSprite.setTextureRect(sf::IntRect(490, 710, 110, 60));
            }
            else if (powerUpType[i] == POWERUP_LARGER_PADDLE) {
                // Larger paddle powerup (arrows icon)
                powerUpSprite.setTextureRect(sf::IntRect(910, 170, 140, 50));
            }
            else if (powerUpType[i] == POWERUP_SLOWER_BALL) {
                // Slower ball powerup (slow text)
                powerUpSprite.setTextureRect(sf::IntRect(910, 60, 140, 50));
            }
            else if (powerUpType[i] == POWERUP_EXTRA_LIFE) {
                // Extra life powerup (heart icon)
                powerUpSprite.setTextureRect(sf::IntRect(1260, 510, 50, 50));
            }

            float scale = POWERUP_SIZE / powerUpSprite.getTextureRect().width;
            powerUpSprite.setScale(scale, scale);
            powerUpSprite.setPosition(powerUpX[i], powerUpY[i]);

            window.draw(powerUpSprite);
        }
        else {
            sf::RectangleShape powerUp(sf::Vector2f(POWERUP_SIZE, POWERUP_SIZE));
            powerUp.setPosition(powerUpX[i], powerUpY[i]);

            if (powerUpType[i] == POWERUP_MULTIBALL) {
                powerUp.setFillColor(sf::Color::Blue);
            }
            else if (powerUpType[i] == POWERUP_LARGER_PADDLE) {
                powerUp.setFillColor(sf::Color::Green);
            }
            else if (powerUpType[i] == POWERUP_SLOWER_BALL) {
                powerUp.setFillColor(sf::Color::Yellow);
            }
            else if (powerUpType[i] == POWERUP_EXTRA_LIFE) {
                powerUp.setFillColor(sf::Color::Red);
            }

            powerUp.setOutlineColor(sf::Color::White);
            powerUp.setOutlineThickness(1);
            window.draw(powerUp);
        }
    }
}

// Draw particles
void drawParticles(sf::RenderWindow& window, float particleX[], float particleY[],
    float particleLife[], bool particleActive[]) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particleActive[i]) continue;

        sf::CircleShape particle(2);
        particle.setPosition(particleX[i], particleY[i]);

        int alpha = static_cast<int>(255 * (particleLife[i] / PARTICLE_LIFETIME));
        particle.setFillColor(sf::Color(255, 255, 255, alpha));
        window.draw(particle);
    }
}

// Draw HUD
void drawHUD(sf::RenderWindow& window, sf::Font& font, int score, int lives, int level) {
    char buffer[100];

    // Score
    strcpy_s(buffer, "Score: ");
    char scoreStr[20];
    intToString(score, scoreStr);
    concatStrings(buffer, scoreStr);

    sf::Text scoreText(buffer, font, 20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition(10, 10);
    window.draw(scoreText);

    // Lives
    strcpy_s(buffer, "Lives: ");
    char livesStr[20];
    intToString(lives, livesStr);
    concatStrings(buffer, livesStr);

    sf::Text livesText(buffer, font, 20);
    livesText.setFillColor(sf::Color::White);
    livesText.setOutlineColor(sf::Color::Black);
    livesText.setOutlineThickness(2);
    livesText.setPosition(WINDOW_WIDTH - 120, 10);
    window.draw(livesText);

    // Level
    strcpy_s(buffer, "Level: ");
    char levelStr[20];
    intToString(level, levelStr);
    concatStrings(buffer, levelStr);

    sf::Text levelText(buffer, font, 20);
    levelText.setFillColor(sf::Color::White);
    levelText.setOutlineColor(sf::Color::Black);
    levelText.setOutlineThickness(2);
    levelText.setPosition(WINDOW_WIDTH / 2 - 40, 10);
    window.draw(levelText);
}

// Draw game over screen
void drawGameOver(sf::RenderWindow& window, sf::Font& font, int score) {
    window.clear(sf::Color(20, 20, 40));

    sf::Text title("GAME OVER", font, 60);
    title.setFillColor(sf::Color::Red);
    title.setOutlineColor(sf::Color(100, 0, 0));
    title.setOutlineThickness(3);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 200);
    window.draw(title);

    char buffer[100];
    strcpy_s(buffer, "Final Score: ");
    char scoreStr[20];
    intToString(score, scoreStr);
    concatStrings(buffer, scoreStr);

    sf::Text scoreText(buffer, font, 30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(WINDOW_WIDTH / 2 - scoreText.getGlobalBounds().width / 2, 300);
    window.draw(scoreText);

    sf::Text instruction("Press Enter to Continue", font, 20);
    instruction.setFillColor(sf::Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 400);
    window.draw(instruction);
}

// Draw settings screen
void drawSettings(sf::RenderWindow& window, sf::Font& font, float volume, int difficulty) {
    window.clear(sf::Color(20, 20, 40));

    sf::Text title("SETTINGS", font, 50);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 100);
    window.draw(title);

    char buffer[100];

    // Difficulty
    strcpy_s(buffer, "Difficulty: ");
    char diffStr[20];
    intToString(difficulty, diffStr);
    concatStrings(buffer, diffStr);

    sf::Text diffText(buffer, font, 30);
    diffText.setFillColor(sf::Color::White);
    diffText.setPosition(WINDOW_WIDTH / 2 - diffText.getGlobalBounds().width / 2, 250);
    window.draw(diffText);

    sf::Text instructions("Use Arrow Keys to Adjust, Enter/ESC to Exit", font, 18);
    instructions.setFillColor(sf::Color(150, 150, 150));
    instructions.setPosition(WINDOW_WIDTH / 2 - instructions.getGlobalBounds().width / 2, 500);
    window.draw(instructions);
}

// Draw high scores screen
void drawHighScores(sf::RenderWindow& window, sf::Font& font, int scores[], char names[][MAX_NAME_LENGTH]) {
    window.clear(sf::Color(20, 20, 40));

    sf::Text title("HIGH SCORES", font, 50);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);
    window.draw(title);

    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        char buffer[200];
        char numStr[10];
        char scoreStr[20];

        // Build the line: "1. Name        Score"
        intToString(i + 1, numStr);
        strcpy_s(buffer, numStr);
        concatStrings(buffer, ". ");
        concatStrings(buffer, names[i]);

        // Add spacing
        int nameLen = strlen(names[i]);
        for (int j = nameLen; j < 25; j++) {
            concatStrings(buffer, " ");
        }

        intToString(scores[i], scoreStr);
        concatStrings(buffer, scoreStr);

        sf::Text scoreText(buffer, font, 24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(150, 150 + i * 35);
        window.draw(scoreText);
    }

    sf::Text instruction("Press Enter or ESC to Return", font, 18);
    instruction.setFillColor(sf::Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 550);
    window.draw(instruction);
}

// Draw name input for high score
void drawNameInput(sf::RenderWindow& window, sf::Font& font, char playerName[]) {
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 180));
    window.draw(overlay);

    sf::Text prompt("Enter Your Name:", font, 40);
    prompt.setFillColor(sf::Color::Yellow);
    prompt.setPosition(WINDOW_WIDTH / 2 - prompt.getGlobalBounds().width / 2, 200);
    window.draw(prompt);

    sf::Text nameText(playerName, font, 35);
    nameText.setFillColor(sf::Color::White);
    nameText.setPosition(WINDOW_WIDTH / 2 - nameText.getGlobalBounds().width / 2, 280);
    window.draw(nameText);

    sf::RectangleShape cursor(sf::Vector2f(2, 35));
    cursor.setPosition(WINDOW_WIDTH / 2 + nameText.getGlobalBounds().width / 2 + 5, 280);
    cursor.setFillColor(sf::Color::White);
    window.draw(cursor);

    sf::Text instruction("Press Enter when done", font, 20);
    instruction.setFillColor(sf::Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 360);
    window.draw(instruction);
}

#endif
