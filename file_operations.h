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

    // Check if file is empty
    file.seekg(0, ios::end);
    if (file.tellg() == 0) {
        file.close();
        return false;
    }
    file.seekg(0, ios::beg);

    // Try to read data and validate
    if (!(file >> level)) {
        file.close();
        return false;
    }
    if (!(file >> score)) {
        file.close();
        return false;
    }
    if (!(file >> lives)) {
        file.close();
        return false;
    }
    if (!(file >> ballX)) {
        file.close();
        return false;
    }
    if (!(file >> ballY)) {
        file.close();
        return false;
    }
    if (!(file >> ballVelX)) {
        file.close();
        return false;
    }
    if (!(file >> ballVelY)) {
        file.close();
        return false;
    }
    if (!(file >> paddleX)) {
        file.close();
        return false;
    }
    if (!(file >> ballLaunched)) {
        file.close();
        return false;
    }

    for (int i = 0; i < TOTAL_BRICKS; i++) {
        if (!(file >> bricks[i])) {
            file.close();
            return false;
        }
    }

    file.close();

    // Validate the loaded data
    if (level < 1 || level > MAX_LEVELS || lives < 0 || score < 0) {
        return false;
    }

    return true;
}

// Save high score
void saveHighScore(const char filename[], int newScore, const char playerName[],
    int scores[], char names[][maximun_name_length]) {
    // Insert new score in sorted position
    int insertPos = maximum_scores;
    for (int i = 0; i < maximum_scores; i++) {
        if (newScore > scores[i]) {
            insertPos = i;
            break;
        }
    }
    if (insertPos < maximum_scores) {
        // Shift scores down
        for (int i = maximum_scores - 1; i > insertPos; i--) {
            scores[i] = scores[i - 1];
            copyString(names[i], names[i - 1], maximun_name_length);
        }
        scores[insertPos] = newScore;
        copyString(names[insertPos], playerName, maximun_name_length);
    }
    // Save to file
    ofstream file(filename);
    if (file.is_open()) {
        for (int i = 0; i < maximum_scores; i++) {
            file << scores[i] << "\n";
            file << names[i] << "\n";
        }
        file.close();
    }
}

// Load high scores
void loadHighScores(const char filename[], int scores[], char names[][maximun_name_length]) {
    // Initialize with default values first
    for (int i = 0; i < maximum_scores; i++) {
        scores[i] = 0;
        copyString(names[i], "---", maximun_name_length);
    }

    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    // Check if file is empty
    file.seekg(0, ios::end);
    if (file.tellg() == 0) {
        file.close();
        return;
    }
    file.seekg(0, ios::beg);

    // Try to read high scores with validation
    bool validData = true;
    for (int i = 0; i < maximum_scores; i++) {
        if (!(file >> scores[i])) {
            validData = false;
            break;
        }
        file.ignore();
        if (!file.getline(names[i], maximun_name_length)) {
            validData = false;
            break;
        }

        // Validate score (should be non-negative)
        if (scores[i] < 0) {
            validData = false;
            break;
        }
    }

    file.close();

    // If data was invalid, reset to defaults
    if (!validData) {
        for (int i = 0; i < maximum_scores; i++) {
            scores[i] = 0;
            copyString(names[i], "---", maximun_name_length);
        }
    }
}

void saveSettings(const char filename[], int difficulty, int musicVolume, int gameVolume) {
    ofstream file(filename);
    if (file.is_open()) {
        file << difficulty << "\n";
        file << musicVolume << "\n";
        file << gameVolume << "\n";
        file.close();
    }
}

void loadSettings(const char filename[], int& difficulty, int& musicVolume, int& gameVolume) {
    // Set defaults first
    difficulty = 1;
    musicVolume = default_music;
    gameVolume = default_gamevolume;

    ifstream file(filename);
    if (!file.is_open()) {
        return;
    }

    // Check if file is empty
    file.seekg(0, ios::end);
    if (file.tellg() == 0) {
        file.close();
        return;
    }
    file.seekg(0, ios::beg);

    // Try to read settings with validation
    int tempDifficulty, tempMusicVolume, tempGameVolume;

    if (!(file >> tempDifficulty)) {
        file.close();
        return;
    }

    if (!(file >> tempMusicVolume)) {
        file.close();
        return;
    }

    if (!(file >> tempGameVolume)) {
        file.close();
        return;
    }

    file.close();

    // Validate the loaded values
    if (tempDifficulty >= 1 && tempDifficulty <= 3) {
        difficulty = tempDifficulty;
    }

    if (tempMusicVolume >= MIN_VOLUME && tempMusicVolume <= MAX_VOLUME) {
        musicVolume = tempMusicVolume;
    }

    if (tempGameVolume >= MIN_VOLUME && tempGameVolume <= MAX_VOLUME) {
        gameVolume = tempGameVolume;
    }
}

#endif
