#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H
#include <fstream>
#include "constants.h"

using namespace std;

void copyString(char dest[], const char src[], int maxLength) {
    int i = 0;
    while (i < maxLength - 1 && src[i] != '\0') {
        dest[i] = src[i];
        i++;
    }
    dest[i] = '\0';
}
// Save game state to file
bool saveGameState(const char filename[], int level, int score, int lives,
    float ballX, float ballY, float ballVelX, float ballVelY,
    float paddleX, int bricks[], bool ballLaunched) {
    ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file << level << "\n";
    file << score << "\n";
    file << lives << "\n";
    file << ballX << "\n";
    file << ballY << "\n";
    file << ballVelX << "\n";
    file << ballVelY << "\n";
    file << paddleX << "\n";
    file << ballLaunched << "\n";
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        file << bricks[i] << " ";
    }
    file.close();
    return true;
}
// Load game state from file
bool loadGameState(const char filename[], int& level, int& score, int& lives,
    float& ballX, float& ballY, float& ballVelX, float& ballVelY,
    float& paddleX, int bricks[], bool& ballLaunched) {
    ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }
    file >> level;
    file >> score;
    file >> lives;
    file >> ballX;
    file >> ballY;
    file >> ballVelX;
    file >> ballVelY;
    file >> paddleX;
    file >> ballLaunched;
    for (int i = 0; i < TOTAL_BRICKS; i++) {
        file >> bricks[i];
    }
    file.close();
    return true;
}
// Save high score
void saveHighScore(const char filename[], int newScore, const char playerName[],
    int scores[], char names[][MAX_NAME_LENGTH]) {
    // Insert new score in sorted position
    int insertPos = MAX_HIGH_SCORES;
    for (int i = 0; i < MAX_HIGH_SCORES; i++) {
        if (newScore > scores[i]) {
            insertPos = i;
            break;
        }
    }
    if (insertPos < MAX_HIGH_SCORES) {
        // Shift scores down
        for (int i = MAX_HIGH_SCORES - 1; i > insertPos; i--) {
            scores[i] = scores[i - 1];
            copyString(names[i], names[i - 1], MAX_NAME_LENGTH);
        }
        scores[insertPos] = newScore;
        copyString(names[insertPos], playerName, MAX_NAME_LENGTH);
    }
    // Save to file
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            file << scores[i] << "\n";
            file << names[i] << "\n";
        }
        file.close();
    }
}
// Load high scores
void loadHighScores(const char filename[], int scores[], char names[][MAX_NAME_LENGTH]) {
    ifstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            file >> scores[i];
            file.ignore();
            file.getline(names[i], MAX_NAME_LENGTH);
        }
        file.close();
    }
    else {
        // Initialize with default values
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            scores[i] = 0;
            copyString(names[i], "---", MAX_NAME_LENGTH);
        }
    }
}
// Save settings 
void saveSettings(const char filename[], int difficulty) {
    ofstream file(filename);
    if (file.is_open()) {
        file << difficulty << "\n";
        file.close();
    }
}
// Load settings 
void loadSettings(const char filename[], int& difficulty) {
    ifstream file(filename);
    if (file.is_open()) {
        file >> difficulty;
        file.close();
    }
    else {
        difficulty = 1;
    }
}
#endif
