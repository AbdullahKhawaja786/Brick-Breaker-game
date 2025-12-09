#ifndef RENDERING_H
#define RENDERING_H
#include <SFML/Graphics.hpp>
#include <cstring>
#include "constants.h"

using namespace std;
using namespace sf;

int getLength(char arr[]) {
    int len = 0;
    while (arr[len] != '\0') {
        len++;
    }
    return len;
}

void safeStringCopy(char dest[], const char src[]) {
    int i = 0;
    while (src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}

void intToString(int num, char result[]) {
    int count = 0, temp = num;
    int isNegative = 0;

    if (num == 0) {
        result[0] = '0';
        result[1] = '\0';
        return;
    }

    if (num < 0) {
        isNegative = 1;
        num = -num;
        temp = num;
    }

    while (temp > 0) {
        temp /= 10;
        count++;
    }

    if (isNegative) {
        result[0] = '-';
        result[count + 1] = '\0';
        count++;
    }
    else {
        result[count] = '\0';
    }

    while (num > 0) {
        result[--count] = (num % 10) + '0';
        num /= 10;
    }
}

void concatStrings(char dest[], const char src[]) {
    int destLen = getLength(dest);
    int i = 0;
    while (src[i] != '\0') {
        dest[destLen + i] = src[i];
        i++;
    }
    dest[destLen + i] = '\0';
}
void drawIntroscreen(RenderWindow& window, Sprite& logoSprite, bool hasLogo, Font& font, Font& titleFont, bool hasTitleFont) {
    Text title("BRICK BREAKER", hasTitleFont ? titleFont : font, 70);
    title.setFillColor(Color(0, 0, 0));
    title.setStyle(Text::Style::Bold);
    title.setOutlineColor(Color(0, 100, 100));
    title.setOutlineThickness(3);
    FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition(WINDOW_WIDTH / 2.0f - titleBounds.width / 2.0f, 80);
    window.draw(title);
    Text instruction("Press SPACE to Start", font, 24);
    instruction.setFillColor(Color::White);
    FloatRect instrBounds = instruction.getGlobalBounds();
    instruction.setPosition(WINDOW_WIDTH / 2.0f - instrBounds.width / 2.0f, 520);
    window.draw(instruction);
}

void drawMainMenuSimple(RenderWindow& window, Font& font, int selectedOption,
    Font& titleFont, bool hasTitleFont, Font& menuFont, bool hasMenuFont) {
    Text title("BRICK BREAKER", hasTitleFont ? titleFont : font, 65);
    title.setStyle(Text::Style::Bold);
    title.setFillColor(Color(64, 224, 208));
    title.setOutlineColor(Color(0, 100, 100));
    title.setOutlineThickness(3);
    FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition(WINDOW_WIDTH / 2.0f - titleBounds.width / 2.0f, 80);
    window.draw(title);
    char options[][50] = {
    "Start New Game",
    "Load Game",
    "High Scores",
    "Settings",
    "Exit"
    };
    for (int i = 0; i < MENU_OPTIONS_COUNT; i++) {
        Text text(options[i], hasMenuFont ? menuFont : font, 23);
        if (i == selectedOption) {
            text.setFillColor(Color(64, 224, 208));
            text.setStyle(Text::Bold);
            text.setScale(1.1f, 1.1f);
        }
        else {
            text.setFillColor(Color::Black);
        }
        FloatRect textBounds = text.getGlobalBounds();
        text.setPosition(WINDOW_WIDTH / 2.0f - textBounds.width / 2.0f, 230.0f + i * 50.0f);
        window.draw(text);
    }
    Text instructions("Use Arrow Keys to Navigate, Enter to Select", hasMenuFont ? menuFont : font, 16);
    instructions.setFillColor(Color(200, 200, 200));
    FloatRect instrBounds = instructions.getGlobalBounds();
    instructions.setPosition(WINDOW_WIDTH / 2.0f - instrBounds.width / 2.0f, 550);
    window.draw(instructions);
}

void drawPauseMenu(RenderWindow& window, Font& font, int selectedOption,
    Font& menuFont, bool hasMenuFont) {
    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));
    window.draw(overlay);
    Text title("PAUSED", hasMenuFont ? menuFont : font, 50);
    title.setFillColor(Color(64, 224, 208));
    FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition(WINDOW_WIDTH / 2.0f - titleBounds.width / 2.0f, 150);
    window.draw(title);
    char options[][50] = {"Resume", "Save Game", "Main Menu"};
    for (int i = 0; i < PAUSE_OPTIONS_COUNT; i++) {
        Text text(options[i], hasMenuFont ? menuFont : font, 23);
        if (i == selectedOption) {
            text.setFillColor(Color(64, 224, 208));
            text.setStyle(Text::Bold);
        }
        else {
            text.setFillColor(Color::White);
        }
        FloatRect textBounds = text.getGlobalBounds();
        text.setPosition(WINDOW_WIDTH / 2.0f - textBounds.width / 2.0f, 270.0f + i * 50.0f);
        window.draw(text);
    }
}

void drawBricksWithTypes(RenderWindow& window, int bricks[], int brickTypes[],
    Texture& greenIntact,
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
                brickSprite.setTexture(greenIntact);
            }
            else if (originalType == 2) {
                brickSprite.setTexture((currentHealth == 2) ? yellowIntact : yellowCracked);
            }
            else if (originalType == 3) {
                brickSprite.setTexture((currentHealth == 3) ? redIntact : redCracked);
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
            if (bricks[i] == -1) brick.setFillColor(Color(80, 80, 80));
            else if (bricks[i] == 1) brick.setFillColor(Color(50, 255, 50));
            else if (bricks[i] == 2) brick.setFillColor(Color(255, 255, 50));
            else brick.setFillColor(Color(255, 50, 50));
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
    Texture& heartTex, Texture& starTex, Texture& skullTex, Texture& coinTex, bool hasTextures) {
    for (int i = 0; i < MAX_POWERUPS; i++) {
        if (!powerUpActive[i]) continue;
        // Use larger size for coin power-up
        float powerUpSize = (powerUpType[i] == POWERUP_BONUS_SCORE) ? POWERUP_COIN_SIZE : POWERUP_SIZE;
        if (hasTextures) {
            Sprite powerUpSprite;
            if (powerUpType[i] == POWERUP_EXTRA_LIFE) {
                powerUpSprite.setTexture(heartTex);
            }
            else if (powerUpType[i] == POWERUP_LARGER_PADDLE) {
                powerUpSprite.setTexture(starTex);
            }
            else if (powerUpType[i] == POWERUP_SMALLER_PADDLE) {
                powerUpSprite.setTexture(skullTex);
            }
            else if (powerUpType[i] == POWERUP_BONUS_SCORE) {
                powerUpSprite.setTexture(coinTex);
            }
            float scale = powerUpSize / powerUpSprite.getLocalBounds().width;
            powerUpSprite.setScale(scale, scale);
            powerUpSprite.setPosition(powerUpX[i], powerUpY[i]);
            window.draw(powerUpSprite);
        }
        else {
            RectangleShape powerUp(Vector2f(powerUpSize, powerUpSize));
            powerUp.setPosition(powerUpX[i], powerUpY[i]);
            if (powerUpType[i] == POWERUP_LARGER_PADDLE) {
                powerUp.setFillColor(Color::Green);
            }
            else if (powerUpType[i] == POWERUP_EXTRA_LIFE) {
                powerUp.setFillColor(Color::Red);
            }
            else if (powerUpType[i] == POWERUP_SMALLER_PADDLE) {
                powerUp.setFillColor(Color(50, 50, 50));
            }
            else if (powerUpType[i] == POWERUP_BONUS_SCORE) {
                powerUp.setFillColor(Color(255, 215, 0));
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
    livesText.setPosition(700, 10);
    window.draw(livesText);
    safeStringCopy(buffer, "Level: ");
    char levelStr[20];
    intToString(level, levelStr);
    concatStrings(buffer, levelStr);
    Text levelText(buffer, font, 20);
    levelText.setFillColor(Color::White);
    levelText.setOutlineColor(Color::Black);
    levelText.setOutlineThickness(2);
    levelText.setPosition(360, 10);
    window.draw(levelText);
}

void drawGameOver(RenderWindow& window, Font& font, int score) {
    window.clear(Color(20, 20, 40));
    Text title("GAME OVER", font, 60);
    title.setFillColor(Color::Red);
    title.setOutlineColor(Color(100, 0, 0));
    title.setOutlineThickness(3);
    FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition(WINDOW_WIDTH / 2.0f - titleBounds.width / 2.0f, 180);
    window.draw(title);
    char buffer[100];
    safeStringCopy(buffer, "Final Score: ");
    char scoreStr[20];
    intToString(score, scoreStr);
    concatStrings(buffer, scoreStr);
    Text scoreText(buffer, font, 32);
    scoreText.setFillColor(Color::White);
    FloatRect scoreBounds = scoreText.getGlobalBounds();
    scoreText.setPosition(WINDOW_WIDTH / 2.0f - scoreBounds.width / 2.0f, 280);
    window.draw(scoreText);
    Text instruction("Press Enter to Continue", font, 20);
    instruction.setFillColor(Color(150, 150, 150));
    FloatRect instrBounds = instruction.getGlobalBounds();
    instruction.setPosition(WINDOW_WIDTH / 2.0f - instrBounds.width / 2.0f, 370);
    window.draw(instruction);
}

void drawSettings(RenderWindow& window, Font& font, int difficulty, int musicVolume, int gameVolume, int selectedSetting) {
    window.clear(Color(20, 20, 40));
    Text title("SETTINGS", font, 50);
    title.setFillColor(Color(64, 224, 208));
    FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition(WINDOW_WIDTH / 2.0f - titleBounds.width / 2.0f, 60);
    window.draw(title);
    char buffer[100];
    // Difficulty
    safeStringCopy(buffer, "Difficulty: ");
    char diffStr[20];
    intToString(difficulty, diffStr);
    concatStrings(buffer, diffStr);
    const char* diffName;
    if (difficulty == 1) {
        diffName = " (Easy)";
    }
    else if (difficulty == 2) {
        diffName = " (Normal)";
    }
    else {
        diffName = " (Hard)";
    }
    concatStrings(buffer, diffName);
    Text diffText(buffer, font, 26);
    diffText.setFillColor((selectedSetting == 0) ? Color(64, 224, 208) : Color::White);
    if (selectedSetting == 0) diffText.setStyle(Text::Bold);
    FloatRect diffBounds = diffText.getGlobalBounds();
    diffText.setPosition(WINDOW_WIDTH / 2.0f - diffBounds.width / 2.0f, 160);
    window.draw(diffText);
    // Music Volume
    safeStringCopy(buffer, "Music Volume: ");
    char volStr[20];
    intToString(musicVolume, volStr);
    concatStrings(buffer, volStr);
    concatStrings(buffer, "%");
    Text musicVolumeText(buffer, font, 26);
    musicVolumeText.setFillColor((selectedSetting == 1) ? Color(64, 224, 208) : Color::White);
    if (selectedSetting == 1) musicVolumeText.setStyle(Text::Bold);
    FloatRect musicBounds = musicVolumeText.getGlobalBounds();
    musicVolumeText.setPosition(WINDOW_WIDTH / 2.0f - musicBounds.width / 2.0f, 240);
    window.draw(musicVolumeText);
    // Music volume bar
    float barWidth = 300.0f;
    float barHeight = 18.0f;
    float barX = WINDOW_WIDTH / 2.0f - barWidth / 2.0f;
    RectangleShape musicBarBg(Vector2f(barWidth, barHeight));
    musicBarBg.setPosition(barX, 275);
    musicBarBg.setFillColor(Color(50, 50, 50));
    musicBarBg.setOutlineColor(Color::White);
    musicBarBg.setOutlineThickness(2);
    window.draw(musicBarBg);
    RectangleShape musicBarFill(Vector2f((barWidth * musicVolume) / 100.0f, barHeight));
    musicBarFill.setPosition(barX, 275);
    musicBarFill.setFillColor(Color(64, 224, 208));
    window.draw(musicBarFill);
    // Game Volume
    safeStringCopy(buffer, "Game Volume: ");
    char gameVolStr[20];
    intToString(gameVolume, gameVolStr);
    concatStrings(buffer, gameVolStr);
    concatStrings(buffer, "%");
    Text gameVolumeText(buffer, font, 26);
    gameVolumeText.setFillColor((selectedSetting == 2) ? Color(64, 224, 208) : Color::White);
    if (selectedSetting == 2) gameVolumeText.setStyle(Text::Bold);
    FloatRect gameBounds = gameVolumeText.getGlobalBounds();
    gameVolumeText.setPosition(WINDOW_WIDTH / 2.0f - gameBounds.width / 2.0f, 330);
    window.draw(gameVolumeText);
    // Game volume bar
    RectangleShape gameBarBg(Vector2f(barWidth, barHeight));
    gameBarBg.setPosition(barX, 365);
    gameBarBg.setFillColor(Color(50, 50, 50));
    gameBarBg.setOutlineColor(Color::White);
    gameBarBg.setOutlineThickness(2);
    window.draw(gameBarBg);
    RectangleShape gameBarFill(Vector2f((barWidth * gameVolume) / 100.0f, barHeight));
    gameBarFill.setPosition(barX, 365);
    gameBarFill.setFillColor(Color(255, 165, 0));
    window.draw(gameBarFill);
    // Instructions
    Text navInstruct("UP/DOWN: Select Setting", font, 16);
    navInstruct.setFillColor(Color(150, 150, 150));
    FloatRect navBounds = navInstruct.getGlobalBounds();
    navInstruct.setPosition(WINDOW_WIDTH / 2.0f - navBounds.width / 2.0f, 450);
    window.draw(navInstruct);
    Text adjustInstruct("LEFT/RIGHT: Adjust Value", font, 16);
    adjustInstruct.setFillColor(Color(150, 150, 150));
    FloatRect adjustBounds = adjustInstruct.getGlobalBounds();
    adjustInstruct.setPosition(WINDOW_WIDTH / 2.0f - adjustBounds.width / 2.0f, 475);
    window.draw(adjustInstruct);
    Text exitInstruct("ENTER/ESC: Save and Exit", font, 16);
    exitInstruct.setFillColor(Color(150, 150, 150));
    FloatRect exitBounds = exitInstruct.getGlobalBounds();
    exitInstruct.setPosition(WINDOW_WIDTH / 2.0f - exitBounds.width / 2.0f, 500);
    window.draw(exitInstruct);
}

void drawHighScores(RenderWindow& window, Font& font, int scores[], char names[][MAX_NAME_LENGTH]) {
    window.clear(Color(20, 20, 40));
    Text title("HIGH SCORES", font, 48);
    title.setFillColor(Color::Yellow);
    FloatRect titleBounds = title.getGlobalBounds();
    title.setPosition(WINDOW_WIDTH / 2.0f - titleBounds.width / 2.0f, 50);
    window.draw(title);
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        char buffer[200];
        char numStr[10];
        char scoreStr[20];
        intToString(i + 1, numStr);
        safeStringCopy(buffer, numStr);
        concatStrings(buffer, ". ");
        concatStrings(buffer, names[i]);
        int nameLen = getLength(names[i]);
        for (int j = nameLen; j < 20; j++) {
            concatStrings(buffer, " ");
        }
        intToString(scores[i], scoreStr);
        concatStrings(buffer, scoreStr);
        Text scoreText(buffer, font, 22);
        scoreText.setFillColor(Color::White);
        scoreText.setPosition(180, 140.0f + i * 36.0f);
        window.draw(scoreText);
    }
    Text instruction("Press Enter or ESC to Return", font, 18);
    instruction.setFillColor(Color(150, 150, 150));
    FloatRect instrBounds = instruction.getGlobalBounds();
    instruction.setPosition(WINDOW_WIDTH / 2.0f - instrBounds.width / 2.0f, 550);
    window.draw(instruction);
}

void drawNameInput(RenderWindow& window, Font& font, char playerName[]) {
    RectangleShape overlay(Vector2f(WINDOW_WIDTH, WINDOW_HEIGHT));
    overlay.setFillColor(Color(0, 0, 0, 180));
    window.draw(overlay);
    Text prompt("Enter Your Name:", font, 38);
    prompt.setFillColor(Color::Yellow);
    FloatRect promptBounds = prompt.getGlobalBounds();
    prompt.setPosition(WINDOW_WIDTH / 2.0f - promptBounds.width / 2.0f, 200);
    window.draw(prompt);
    Text nameText(playerName, font, 35);
    nameText.setFillColor(Color::White);
    FloatRect nameBounds = nameText.getGlobalBounds();
    nameText.setPosition(WINDOW_WIDTH / 2.0f - nameBounds.width / 2.0f, 280);
    window.draw(nameText);
    RectangleShape cursor(Vector2f(3, 35));
    cursor.setPosition(WINDOW_WIDTH / 2.0f + nameBounds.width / 2.0f + 8, 280);
    cursor.setFillColor(Color::White);
    window.draw(cursor);
    Text instruction("Press Enter when done", font, 20);
    instruction.setFillColor(Color(150, 150, 150));
    FloatRect instrBounds = instruction.getGlobalBounds();
    instruction.setPosition(WINDOW_WIDTH / 2.0f - instrBounds.width / 2.0f, 360);
    window.draw(instruction);
}

#endif
