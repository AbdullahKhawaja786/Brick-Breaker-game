// file_operations.h
#ifndef FILE_OPERATIONS_H
#define FILE_OPERATIONS_H

#include <fstream>
#include <cstring>
#include "constants.h"

// Save game state to file
bool saveGameState(const char* filename, int level, int score, int lives,
    float ballX, float ballY, float ballVelX, float ballVelY,
    float paddleX, int bricks[], bool ballLaunched) {
    std::ofstream file(filename);
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
bool loadGameState(const char* filename, int& level, int& score, int& lives,
    float& ballX, float& ballY, float& ballVelX, float& ballVelY,
    float& paddleX, int bricks[], bool& ballLaunched) {
    std::ifstream file(filename);
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
void saveHighScore(const char* filename, int newScore, const char* playerName,
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
            strcpy_s(names[i], names[i - 1]);
        }

        scores[insertPos] = newScore;
        strcpy_s(names[insertPos], playerName);
    }

    // Save to file
    std::ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < MAX_HIGH_SCORES; i++) {
            file << scores[i] << "\n";
            file << names[i] << "\n";
        }
        file.close();
    }
}

// Load high scores
void loadHighScores(const char* filename, int scores[], char names[][MAX_NAME_LENGTH]) {
    std::ifstream file(filename);
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
            strcpy_s(names[i], "---");
        }
    }
}

// Save settings
void saveSettings(const char* filename, float volume, int difficulty) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << volume << "\n";
        file << difficulty << "\n";
        file.close();
    }
}

// Load settings
void loadSettings(const char* filename, float& volume, int& difficulty) {
    std::ifstream file(filename);
    if (file.is_open()) {
        file >> volume;
        file >> difficulty;
        file.close();
    }
    else {
        volume = 50.0f;
        difficulty = 1;
    }
}

#endif
