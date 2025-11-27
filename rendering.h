// rendering.h
#ifndef RENDERING_H
#define RENDERING_H

#include <SFML/Graphics.hpp>
#include <sstream>
#include <iomanip>
#include "constants.h"

// Draw animated star background
void drawAnimatedBackground(sf::RenderWindow& window, float starX[], float starY[], float starSize[]) {
    for (int i = 0; i < MAX_BG_STARS; i++) {
        sf::CircleShape star(starSize[i]);
        star.setPosition(starX[i], starY[i]);

        // Color variation
        int brightness = 150 + (i % 100);
        star.setFillColor(sf::Color(brightness, brightness, 255));
        window.draw(star);
    }
}

// Draw main menu (sprite background logic removed)
void drawMainMenu(sf::RenderWindow& window, sf::Font& font, int selectedOption,
    sf::Sprite* backgroundSprite, float starX[], float starY[], float starSize[]) {
    window.clear(sf::Color(10, 10, 30));

    // Draw animated star background (now always used)
    drawAnimatedBackground(window, starX, starY, starSize);


    // Semi-transparent overlay for readability
    sf::RectangleShape overlay(sf::Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(sf::Color(0, 0, 0, 100));
    window.draw(overlay);

    // Title (Single, clean text - glow effect removed)
    sf::Text title("BRICK BREAKER", font, 60);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 80);
    window.draw(title);

    // Decorative shapes
    sf::CircleShape circle1(80);
    circle1.setPosition(50, 100);
    circle1.setFillColor(sf::Color(50, 100, 200, 50));
    window.draw(circle1);

    sf::CircleShape circle2(60);
    circle2.setPosition(650, 400);
    circle2.setFillColor(sf::Color(200, 50, 100, 50));
    window.draw(circle2);

    // Menu options
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

    // Instructions
    sf::Text instructions("Use Arrow Keys to Navigate, Enter to Select", font, 16);
    instructions.setFillColor(sf::Color(200, 200, 200));
    instructions.setPosition(WINDOW_WIDTH / 2 - instructions.getGlobalBounds().width / 2, 550);
    window.draw(instructions);
}

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
void drawBricks(sf::RenderWindow& window, int bricks[]) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (bricks[i] <= 0 && bricks[i] != -1) continue;
        int row = i / GRID_WIDTH;
        int col = i % GRID_WIDTH;

        float x = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING);
        float y = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING);

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

        brick.setOutlineColor(sf::Color(0, 0, 0));
        brick.setOutlineThickness(2);
        window.draw(brick);
    }
}
void drawPaddle(sf::RenderWindow& window, float paddleX, float paddleWidth) {
    sf::RectangleShape paddle(sf::Vector2f(paddleWidth, PADDLE_HEIGHT));
    paddle.setPosition(paddleX, PADDLE_Y);
    paddle.setFillColor(sf::Color::Cyan);
    paddle.setOutlineColor(sf::Color::White);
    paddle.setOutlineThickness(2);
    window.draw(paddle);
}
void drawBall(sf::RenderWindow& window, float ballX, float ballY) {
    sf::CircleShape ball(BALL_RADIUS);
    ball.setPosition(ballX - BALL_RADIUS, ballY - BALL_RADIUS);
    ball.setFillColor(sf::Color::White);
    ball.setOutlineColor(sf::Color(200, 200, 255));
    ball.setOutlineThickness(2);
    window.draw(ball);
}
void drawPowerUps(sf::RenderWindow& window, float powerUpX[], float powerUpY[],
    int powerUpType[], bool powerUpActive[]) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUpActive[i]) continue;
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
void drawHUD(sf::RenderWindow& window, sf::Font& font, int score, int lives, int level) {
    std::stringstream ss;
    ss << "Score: " << score;
    sf::Text scoreText(ss.str(), font, 20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10, 10);
    window.draw(scoreText);

    ss.str("");
    ss << "Lives: " << lives;
    sf::Text livesText(ss.str(), font, 20);
    livesText.setFillColor(sf::Color::White);
    livesText.setPosition(WINDOW_WIDTH - 120, 10);
    window.draw(livesText);

    ss.str("");
    ss << "Level: " << level;
    sf::Text levelText(ss.str(), font, 20);
    levelText.setFillColor(sf::Color::White);
    levelText.setPosition(WINDOW_WIDTH / 2 - 40, 10);
    window.draw(levelText);
}
void drawGameOver(sf::RenderWindow& window, sf::Font& font, int score) {
    window.clear(sf::Color(20, 20, 40));
    sf::Text title("GAME OVER", font, 60);
    title.setFillColor(sf::Color::Red);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 200);
    window.draw(title);

    std::stringstream ss;
    ss << "Final Score: " << score;
    sf::Text scoreText(ss.str(), font, 30);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(WINDOW_WIDTH / 2 - scoreText.getGlobalBounds().width / 2, 300);
    window.draw(scoreText);

    sf::Text instruction("Press Enter to Continue", font, 20);
    instruction.setFillColor(sf::Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 400);
    window.draw(instruction);
}
void drawSettings(sf::RenderWindow& window, sf::Font& font, float volume, int difficulty) {
    window.clear(sf::Color(20, 20, 40));
    sf::Text title("SETTINGS", font, 50);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 100);
    window.draw(title);

    std::stringstream ss;
    ss << "Volume: " << static_cast<int>(volume) << "%";
    sf::Text volumeText(ss.str(), font, 30);
    volumeText.setFillColor(sf::Color::White);
    volumeText.setPosition(200, 250);
    window.draw(volumeText);

    ss.str("");
    ss << "Difficulty: " << difficulty;
    sf::Text diffText(ss.str(), font, 30);
    diffText.setFillColor(sf::Color::White);
    diffText.setPosition(200, 320);
    window.draw(diffText);

    sf::Text instructions("Use Arrow Keys to Adjust, Enter/ESC to Exit", font, 18);
    instructions.setFillColor(sf::Color(150, 150, 150));
    instructions.setPosition(WINDOW_WIDTH / 2 - instructions.getGlobalBounds().width / 2, 500);
    window.draw(instructions);
}
void drawHighScores(sf::RenderWindow& window, sf::Font& font, int scores[], char names[][MAX_NAME_LENGTH]) {
    window.clear(sf::Color(20, 20, 40));
    sf::Text title("HIGH SCORES", font, 50);
    title.setFillColor(sf::Color::Yellow);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 50);
    window.draw(title);

    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        std::stringstream ss;
        ss << std::setw(2) << (i + 1) << ". " << std::setw(20) << std::left << names[i]
            << std::setw(8) << std::right << scores[i];

        sf::Text scoreText(ss.str(), font, 24);
        scoreText.setFillColor(sf::Color::White);
        scoreText.setPosition(150, 150 + i * 35);
        window.draw(scoreText);
    }

    sf::Text instruction("Press Enter or ESC to Return", font, 18);
    instruction.setFillColor(sf::Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 550);
    window.draw(instruction);
}
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

    // Cursor
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
