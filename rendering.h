#ifndef RENDERING_H
#define RENDERING_H
#include <SFML/Graphics.hpp>
#include <cstring>
#include "constants.h"

using namespace std;
using namespace sf;

void safeStringCopy(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

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

void concatStrings(char dest[], const char src[]) {
    int destLen = strlen(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
}

// Updated function with custom title font and menu font parameters
void drawMainMenuSimple(RenderWindow& window, Font& font, int selectedOption,
    Font& titleFont, bool hasTitleFont, Font& menuFont, bool hasMenuFont) {
    // Use custom font if available, otherwise use default font
    Text title("BRICK BREAKER", hasTitleFont ? titleFont : font, 65);
    title.setStyle(Text::Style::Bold);
    title.setFillColor(Color(64, 224, 208));
    // Add outline for more stylish look
    title.setOutlineColor(Color(0, 100, 100));
    title.setOutlineThickness(3);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 80);
    window.draw(title);
    const char* options[] = { "Start New Game", "Load Game", "High Scores", "Settings", "Exit" };
    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        // Use custom menu font if available
        Text text(options[i], hasMenuFont ? menuFont : font, 23);
        if (i == selectedOption) {
            text.setFillColor(Color(64, 224, 208));
            text.setStyle(Text::Bold);
            text.setScale(1.1f, 1.1f);
        }
        else {
            text.setFillColor(Color::Black);
        }
        text.setPosition(WINDOW_WIDTH / 2 - text.getGlobalBounds().width / 2, 230 + i * 50);
        window.draw(text);
    }
    // Use custom menu font for instructions too
    Text instructions("Use Arrow Keys to Navigate, Enter to Select", hasMenuFont ? menuFont : font, 16);
    instructions.setFillColor(Color(200, 200, 200));
    instructions.setPosition(WINDOW_WIDTH / 2 - instructions.getGlobalBounds().width / 2, 550);
    window.draw(instructions);
}

void drawPauseMenu(RenderWindow& window, Font& font, int selectedOption,
    Font& menuFont, bool hasMenuFont) {
    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));
    window.draw(overlay);
    // Use custom menu font for PAUSED title
    Text title("PAUSED", hasMenuFont ? menuFont : font, 50);
    title.setFillColor(Color(64, 224, 208));
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 150);
    window.draw(title);
    const char* options[] = { "Resume", "Save Game", "Main Menu" };
    for (int i = 0; i < PAUSE_OPTIONS_COUNT; i++) {
        // Use custom menu font for pause menu options
        Text text(options[i], hasMenuFont ? menuFont : font, 30);
        if (i == selectedOption) {
            text.setFillColor(Color(64, 224, 208));
            text.setStyle(Text::Bold);
        }
        else {
            text.setFillColor(Color::White);
        }
        text.setPosition(WINDOW_WIDTH / 2 - text.getGlobalBounds().width / 2, 270 + i * 50);
        window.draw(text);
    }
}

void drawBricksWithTypes(RenderWindow& window, int bricks[], int brickTypes[],
    Texture& greenIntact, Texture& greenCracked,
    Texture& yellowIntact, Texture& yellowCracked,
    Texture& redIntact, Texture& redCracked,
    Texture& gray, bool hasTextures) {
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (bricks[i] <= 0 && bricks[i] != -1) continue;
        int row = i / GRID_WIDTH;
        int col = i % GRID_WIDTH;
        float x = BRICK_OFFSET_X + col * (BRICK_WIDTH + BRICK_SPACING);
        float y = BRICK_OFFSET_Y + row * (BRICK_HEIGHT + BRICK_SPACING);
        if (hasTextures) {
            Sprite brickSprite;
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
            RectangleShape brick(Vector2f(BRICK_WIDTH, BRICK_HEIGHT));
            brick.setPosition(x, y);
            if (bricks[i] == -1) {
                brick.setFillColor(Color(80, 80, 80));
            }
            else if (bricks[i] == 1) {
                brick.setFillColor(Color(50, 255, 50));
            }
            else if (bricks[i] == 2) {
                brick.setFillColor(Color(255, 255, 50));
            }
            else {
                brick.setFillColor(Color(255, 50, 50));
            }
            brick.setOutlineColor(Color::Black);
            brick.setOutlineThickness(2);
            window.draw(brick);
        }
    }
}

void drawPaddle(RenderWindow& window, float paddleX, float paddleWidth,
    Texture& paddleTexture, bool hasTexture) {
    if (hasTexture) {
        Sprite paddleSprite(paddleTexture);
        float scaleX = paddleWidth / paddleSprite.getLocalBounds().width;
        float scaleY = PADDLE_HEIGHT / paddleSprite.getLocalBounds().height;
        paddleSprite.setScale(scaleX, scaleY);
        paddleSprite.setPosition(paddleX, PADDLE_Y);
        window.draw(paddleSprite);
    }
    else {
        RectangleShape paddle(Vector2f(paddleWidth, PADDLE_HEIGHT));
        paddle.setPosition(paddleX, PADDLE_Y);
        paddle.setFillColor(Color::Cyan);
        paddle.setOutlineColor(Color::White);
        paddle.setOutlineThickness(2);
        window.draw(paddle);
    }
}

void drawBall(RenderWindow& window, float ballX, float ballY,
    Texture& ballTexture, bool hasTexture) {
    if (hasTexture) {
        Sprite ballSprite(ballTexture);
        float scale = (BALL_RADIUS * 2) / ballSprite.getLocalBounds().width;
        ballSprite.setScale(scale, scale);
        ballSprite.setPosition(ballX - BALL_RADIUS, ballY - BALL_RADIUS);
        window.draw(ballSprite);
    }
    else {
        CircleShape ball(BALL_RADIUS);
        ball.setPosition(ballX - BALL_RADIUS, ballY - BALL_RADIUS);
        ball.setFillColor(Color::White);
        ball.setOutlineColor(Color(100, 150, 255));
        ball.setOutlineThickness(2);
        window.draw(ball);
    }
}

void drawPowerUpsSimple(RenderWindow& window, float powerUpX[], float powerUpY[],
    int powerUpType[], bool powerUpActive[],
    Texture& heartTex, Texture& starTex, bool hasTextures) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUpActive[i]) continue;
        if (hasTextures) {
            Sprite powerUpSprite;
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
            RectangleShape powerUp(Vector2f(POWERUP_SIZE, POWERUP_SIZE));
            powerUp.setPosition(powerUpX[i], powerUpY[i]);
            if (powerUpType[i] == POWERUP_MULTIBALL) {
                powerUp.setFillColor(Color::Blue);
            }
            else if (powerUpType[i] == POWERUP_LARGER_PADDLE) {
                powerUp.setFillColor(Color::Green);
            }
            else if (powerUpType[i] == POWERUP_SLOWER_BALL) {
                powerUp.setFillColor(Color::Yellow);
            }
            else if (powerUpType[i] == POWERUP_EXTRA_LIFE) {
                powerUp.setFillColor(Color::Red);
            }
            powerUp.setOutlineColor(Color::White);
            powerUp.setOutlineThickness(1);
            window.draw(powerUp);
        }
    }
}

void drawSimpleParticles(RenderWindow& window, float particleX[], float particleY[],
    bool particleActive[]) {
    for (int i = 0; i < MAX_PARTICLES; i++) {
        if (!particleActive[i]) continue;
        RectangleShape particle(Vector2f(4, 4));
        particle.setPosition(particleX[i], particleY[i]);
        particle.setFillColor(Color::White);
        window.draw(particle);
    }
}

void drawHUD(RenderWindow& window, Font& font, int score, int lives, int level) {
    char buffer[100];
    safeStringCopy(buffer, "Score: ");
    char scoreStr[20];
    intToString(score, scoreStr);
    concatStrings(buffer, scoreStr);
    Text scoreText(buffer, font, 20);
    scoreText.setFillColor(Color::White);
    scoreText.setOutlineColor(Color::Black);
    scoreText.setOutlineThickness(2);
    scoreText.setPosition(10, 10);
    window.draw(scoreText);
    safeStringCopy(buffer, "Lives: ");
    char livesStr[20];
    intToString(lives, livesStr);
    concatStrings(buffer, livesStr);
    Text livesText(buffer, font, 20);
    livesText.setFillColor(Color::White);
    livesText.setOutlineColor(Color::Black);
    livesText.setOutlineThickness(2);
    livesText.setPosition(WINDOW_WIDTH - 100, 10);
    window.draw(livesText);
    safeStringCopy(buffer, "Level: ");
    char levelStr[20];
    intToString(level, levelStr);
    concatStrings(buffer, levelStr);
    Text levelText(buffer, font, 20);
    levelText.setFillColor(Color::White);
    levelText.setOutlineColor(Color::Black);
    levelText.setOutlineThickness(2);
    levelText.setPosition(WINDOW_WIDTH / 2 - 40, 10);
    window.draw(levelText);
}

void drawGameOver(RenderWindow& window, Font& font, int score) {
    window.clear(Color(20, 20, 40));
    Text title("GAME OVER", font, 60);
    title.setFillColor(Color::Red);
    title.setOutlineColor(Color(100, 0, 0));
    title.setOutlineThickness(3);
    title.setPosition(WINDOW_WIDTH / 2 - title.getGlobalBounds().width / 2, 180);
    window.draw(title);
    char buffer[100];
    safeStringCopy(buffer, "Final Score: ");
    char scoreStr[20];
    intToString(score, scoreStr);
    concatStrings(buffer, scoreStr);
    Text scoreText(buffer, font, 32);
    scoreText.setFillColor(Color::White);
    scoreText.setPosition(WINDOW_WIDTH / 2 - scoreText.getGlobalBounds().width / 2, 280);
    window.draw(scoreText);
    Text instruction("Press Enter to Continue", font, 20);
    instruction.setFillColor(Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 370);
    window.draw(instruction);
}

void drawSettings(RenderWindow& window, Font& font, int difficulty) {
    window.clear(Color(20, 20, 40));
    Text title("SETTINGS", font, 50);
    title.setFillColor(Color(64, 224, 208));
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
    Text diffText(buffer, font, 30);
    diffText.setFillColor(Color::White);
    diffText.setPosition(WINDOW_WIDTH / 2 - diffText.getGlobalBounds().width / 2, 250);
    window.draw(diffText);
    Text instructions("Use UP/DOWN to Adjust, ESC to Exit", font, 18);
    instructions.setFillColor(Color(150, 150, 150));
    instructions.setPosition(WINDOW_WIDTH / 2 - instructions.getGlobalBounds().width / 2, 480);
    window.draw(instructions);
}

void drawHighScores(RenderWindow& window, Font& font, int scores[], char names[][MAX_NAME_LENGTH]) {
    window.clear(Color(20, 20, 40));
    Text title("HIGH SCORES", font, 48);
    title.setFillColor(Color::Yellow);
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
        for (int j = nameLen; j < 20; j++) {
            concatStrings(buffer, " ");
        }
        intToString(scores[i], scoreStr);
        concatStrings(buffer, scoreStr);
        Text scoreText(buffer, font, 22);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(180, 140 + i * 36);
        window.draw(scoreText);
    }
    Text instruction("Press Enter or ESC to Return", font, 18);
    instruction.setFillColor(Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 550);
    window.draw(instruction);
}

void drawNameInput(RenderWindow& window, Font& font, char playerName[]) {
    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));
    window.draw(overlay);
    Text prompt("Enter Your Name:", font, 38);
    prompt.setFillColor(Color::Yellow);
    prompt.setPosition(WINDOW_WIDTH / 2 - prompt.getGlobalBounds().width / 2, 200);
    window.draw(prompt);
    Text nameText(playerName, font, 35);
    nameText.setFillColor(Color::White);
    nameText.setPosition(WINDOW_WIDTH / 2 - nameText.getGlobalBounds().width / 2, 280);
    window.draw(nameText);
    RectangleShape cursor(Vector2f(3, 35));
    cursor.setPosition(WINDOW_WIDTH / 2 + nameText.getGlobalBounds().width / 2 + 8, 280);
    cursor.setFillColor(Color::White);
    window.draw(cursor);
    Text instruction("Press Enter when done", font, 20);
    instruction.setFillColor(Color(150, 150, 150));
    instruction.setPosition(WINDOW_WIDTH / 2 - instruction.getGlobalBounds().width / 2, 360);
    window.draw(instruction);
}
#endif
