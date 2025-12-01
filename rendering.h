#ifndef RENDERING_H
#define RENDERING_H
#include <SFML/Graphics.hpp>
#include <cstring>
#include "constants.h"
// Helper function to safely copy strings
void safeStringCopy(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}
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
// Draw main menu
void drawMainMenuSimple(sf::RenderWindow& window, sf::Font& font, int selectedOption) {
    sf::Text title("BRICK BREAKER", font, 60);
    title.setStyle(sf::Text::Style::Bold);
    title.setFillColor(sf::Color(64, 224, 208)); // Turquoise
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 80);
    window.draw(title);
    const char* options[] = { "Start New Game", "Load Game", "High Scores", "Settings", "Exit" };
    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        sf::Text text(options[i], font, 30);
        if (i == selectedOption) {
            text.setFillColor(sf::Color(64, 224, 208)); // Turquoise for selected
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
    title.setFillColor(sf::Color(64, 224, 208)); // Turquoise
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 150);
    window.draw(title);
    const char* options[] = { "Resume", "Save Game", "Settings", "Main Menu" };
    for (int i = 0; i < PAUSE_OPTIONS_COUNT; i++) {
        sf::Text text(options[i], font, 30);
        if (i == selectedOption) {
            text.setFillColor(sf::Color(64, 224, 208)); // Turquoise for selected
            text.setStyle(sf::Text::Bold);
        }
        else {
            text.setFillColor(sf::Color::White);
        }
        text.setPosition(WINDOW_WIDTH / 2 - text.getGlobalBounds().width / 2, 280 + i * 50);
        window.draw(text);
    }
}
// Draw bricks with individual textures
void drawBricksWithTypes(sf::RenderWindow& window, int bricks[], int brickTypes[],
    sf::Texture& greenIntact, sf::Texture& greenCracked,
    sf::Texture& yellowIntact, sf::Texture& yellowCracked,
    sf::Texture& redIntact, sf::Texture& redCracked,
    sf::Texture& gray, bool hasTextures) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (bricks[i] <= 0 && bricks[i] != -1) continue;
        int row = i / GRID_WIDTH;
        int col = i % GRID_WIDTH;
        float x = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING);
        float y = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING);
        if (hasTextures) {
            sf::Sprite brickSprite;
            int currentHealth = bricks[i];
            int originalType = brickTypes[i];
            if (originalType == -1) {
                brickSprite.setTexture(gray);
            }
            else if (originalType == 1) {
                brickSprite.setTexture(greenCracked);
            }
            else if (originalType == 2) {
                if (currentHealth == 2) {
                    brickSprite.setTexture(yellowIntact);
                }
                else {
                    brickSprite.setTexture(yellowCracked);
                }
            }
            else if (originalType == 3) {
                if (currentHealth == 3) {
                    brickSprite.setTexture(redIntact);
                }
                else {
                    brickSprite.setTexture(redCracked);
                }
            }
            float scaleX = BRICK_WIDTH / brickSprite.getLocalBounds().width;
            float scaleY = BRICK_HEIGHT / brickSprite.getLocalBounds().height;
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
// Draw paddle
void drawPaddle(sf::RenderWindow& window, float paddleX, float paddleWidth,
    sf::Texture& paddleTexture, bool hasTexture) {
    if (hasTexture) {
        sf::Sprite paddleSprite(paddleTexture);
        float scaleX = paddleWidth / paddleSprite.getLocalBounds().width;
        float scaleY = PADDLE_HEIGHT / paddleSprite.getLocalBounds().height;
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
// Draw ball
void drawBall(sf::RenderWindow& window, float ballX, float ballY,
    sf::Texture& ballTexture, bool hasTexture) {
    if (hasTexture) {
        sf::Sprite ballSprite(ballTexture);
        float scale = (BALL_RADIUS * 2) / ballSprite.getLocalBounds().width;
        ballSprite.setScale(scale, scale);
        ballSprite.setPosition(ballX - BALL_RADIUS, ballY - BALL_RADIUS);
        window.draw(ballSprite);
    }
    else {
        sf::CircleShape ball(BALL_RADIUS);
        ball.setPosition(ballX - BALL_RADIUS, ballY - BALL_RADIUS);
        ball.setFillColor(sf::Color::White);
        ball.setOutlineColor(sf::Color(100, 150, 255));
        ball.setOutlineThickness(2);
        window.draw(ball);
    }
}
// Draw power-ups
void drawPowerUpsSimple(sf::RenderWindow& window, float powerUpX[], float powerUpY[],
    int powerUpType[], bool powerUpActive[],
    sf::Texture& heartTex, sf::Texture& starTex, bool hasTextures) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUpActive[i]) continue;
        if (hasTextures) {
            sf::Sprite powerUpSprite;
            if (powerUpType[i] == POWERUP_EXTRA_LIFE) {
                powerUpSprite.setTexture(heartTex);
            }
            else {
                powerUpSprite.setTexture(starTex);
            }
            float scale = POWERUP_SIZE / powerUpSprite.getLocalBounds().width;
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
    safeStringCopy(buffer, "Score: ");
    char scoreStr[20];
    intToString(score, scoreStr);
    concatStrings(buffer, scoreStr);
    sf::Text scoreText(buffer, font, 20);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setOutlineColor(sf::Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition(10, 10);
    window.draw(scoreText);
    safeStringCopy(buffer, "Lives: ");
    char livesStr[20];
    intToString(lives, livesStr);
    concatStrings(buffer, livesStr);
    sf::Text livesText(buffer, font, 20);
    livesText.setFillColor(sf::Color::White);
    livesText.setOutlineColor(sf::Color::Black);
    livesText.setOutlineThickness(2);
    livesText.setPosition(WINDOW_WIDTH - 120, 10);
    window.draw(livesText);
    safeStringCopy(buffer, "Level: ");
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
    safeStringCopy(buffer, "Final Score: ");
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
void drawSettings(sf::RenderWindow& window, sf::Font& font, int difficulty) {
    window.clear(sf::Color(20, 20, 40));
    sf::Text title("SETTINGS", font, 50);
    title.setFillColor(sf::Color(64, 224, 208)); // Turquoise
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 100);
    window.draw(title);
    char buffer[100];
    safeStringCopy(buffer, "Difficulty: ");
    char diffStr[20];
    intToString(difficulty, diffStr);
    concatStrings(buffer, diffStr);
    const char* diffName = "";
    if (difficulty == 1) diffName = " (Easy)";
    else if (difficulty == 2) diffName = " (Normal)";
    else if (difficulty == 3) diffName = " (Hard)";
    concatStrings(buffer, diffName);
    sf::Text diffText(buffer, font, 30);
    diffText.setFillColor(sf::Color::White);
    diffText.setPosition(WINDOW_WIDTH / 2 - diffText.getGlobalBounds().width / 2, 250);
    window.draw(diffText);
    sf::Text instructions("Use UP/DOWN to Adjust, ESC to Exit", font, 18);
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
        intToString(i + 1, numStr);
        safeStringCopy(buffer, numStr);
        concatStrings(buffer, ". ");
        concatStrings(buffer, names[i]);
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
