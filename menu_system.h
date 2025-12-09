#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H
#include <SFML/Graphics.hpp>
#include "constants.h"

using namespace std;
using namespace sf;

int handleMainMenuInput(Event& event, int& selectedOption) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Up) {
            selectedOption--;
            if (selectedOption < 0) selectedOption = MENU_OPTIONS_COUNT - 1;
        }
        else if (event.key.code == Keyboard::Down) {
            selectedOption++;
            if (selectedOption >= MENU_OPTIONS_COUNT) selectedOption = 0;
        }
        else if (event.key.code == Keyboard::Return) {
            return selectedOption;
        }
    }
    return -1;
}

int handlePauseMenuInput(Event& event, int& selectedOption) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Up) {
            selectedOption--;
            if (selectedOption < 0) selectedOption = PAUSE_OPTIONS_COUNT - 1;
        }
        else if (event.key.code == Keyboard::Down) {
            selectedOption++;
            if (selectedOption >= PAUSE_OPTIONS_COUNT) selectedOption = 0;
        }
        else if (event.key.code == Keyboard::Return) {
            return selectedOption;
        }
    }
    return -1;
}

void handleSettingsInput(Event& event, int& difficulty, int& musicVolume, int& gameVolume,
    int& selectedSetting, bool& exitSettings, bool& volumeChanged) {
    volumeChanged = false;
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Up) {
            selectedSetting--;
            if (selectedSetting < 0) selectedSetting = 2;
        }
        else if (event.key.code == Keyboard::Down) {
            selectedSetting++;
            if (selectedSetting > 2) selectedSetting = 0;
        }
        else if (event.key.code == Keyboard::Left) {
            if (selectedSetting == 0) {
                difficulty--;
                if (difficulty < 1) difficulty = 1;
            }
            else if (selectedSetting == 1) {
                musicVolume -= VOLUME_STEP;
                if (musicVolume < MIN_VOLUME) musicVolume = MIN_VOLUME;
                volumeChanged = true;
            }
            else if (selectedSetting == 2) {
                gameVolume -= VOLUME_STEP;
                if (gameVolume < MIN_VOLUME) gameVolume = MIN_VOLUME;
                volumeChanged = true;
            }
        }
        else if (event.key.code == Keyboard::Right) {
            if (selectedSetting == 0) {
                difficulty++;
                if (difficulty > 3) difficulty = 3;
            }
            else if (selectedSetting == 1) {
                musicVolume += VOLUME_STEP;
                if (musicVolume > MAX_VOLUME) musicVolume = MAX_VOLUME;
                volumeChanged = true;
            }
            else if (selectedSetting == 2) {
                gameVolume += VOLUME_STEP;
                if (gameVolume > MAX_VOLUME) gameVolume = MAX_VOLUME;
                volumeChanged = true;
            }
        }
        else if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Return) {
            exitSettings = true;
        }
    }
}

void handlePauseSettingsInput(Event& event, int& musicVolume, int& gameVolume,
    int& selectedSetting, bool& exitSettings, bool& volumeChanged) {
    volumeChanged = false;
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Up) {
            selectedSetting--;
            if (selectedSetting < 0) selectedSetting = 1;
        }
        else if (event.key.code == Keyboard::Down) {
            selectedSetting++;
            if (selectedSetting > 1) selectedSetting = 0;
        }
        else if (event.key.code == Keyboard::Left) {
            if (selectedSetting == 0) {
                musicVolume -= VOLUME_STEP;
                if (musicVolume < MIN_VOLUME) musicVolume = MIN_VOLUME;
                volumeChanged = true;
            }
            else if (selectedSetting == 1) {
                gameVolume -= VOLUME_STEP;
                if (gameVolume < MIN_VOLUME) gameVolume = MIN_VOLUME;
                volumeChanged = true;
            }
        }
        else if (event.key.code == Keyboard::Right) {
            if (selectedSetting == 0) {
                musicVolume += VOLUME_STEP;
                if (musicVolume > MAX_VOLUME) musicVolume = MAX_VOLUME;
                volumeChanged = true;
            }
            else if (selectedSetting == 1) {
                gameVolume += VOLUME_STEP;
                if (gameVolume > MAX_VOLUME) gameVolume = MAX_VOLUME;
                volumeChanged = true;
            }
        }
        else if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Return) {
            exitSettings = true;
        }
    }
}

bool getPlayerNameInput(Event& event, char playerName[], int& nameLength, bool& finishedInput) {
    if (event.type == Event::TextEntered) {
        if (event.text.unicode == '\b') {
            if (nameLength > 0) {
                nameLength--;
                playerName[nameLength] = '\0';
            }
        }
        else if (event.text.unicode == '\r' || event.text.unicode == '\n') {
            if (nameLength > 0) {
                finishedInput = true;
                return true;
            }
        }
        else if (event.text.unicode < 128 && nameLength < MAX_NAME_LENGTH - 1) {
            playerName[nameLength] = static_cast<char>(event.text.unicode);
            nameLength++;
            playerName[nameLength] = '\0';
        }
    }
    return false;
}

bool handleHighScoresInput(Event& event) {
    if (event.type == Event::KeyPressed) {
        if (event.key.code == Keyboard::Escape || event.key.code == Keyboard::Return) {
            return true;
        }
    }
    return false;
}

#endif
