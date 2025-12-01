#ifndef MENU_SYSTEM_H
#define MENU_SYSTEM_H
#include <SFML/Graphics.hpp>
#include "constants.h"
// Handle main menu input and return selected option
int handleMainMenuInput(sf::Event& event, int& selectedOption) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption--;
            if (selectedOption < 0) selectedOption = MENU_OPTIONS_COUNT - 1;
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedOption++;
            if (selectedOption >= MENU_OPTIONS_COUNT) selectedOption = 0;
        }
        else if (event.key.code == sf::Keyboard::Return) {
            return selectedOption;
        }
    }
    return -1;
}
// Handle pause menu input
int handlePauseMenuInput(sf::Event& event, int& selectedOption) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            selectedOption--;
            if (selectedOption < 0) selectedOption = PAUSE_OPTIONS_COUNT - 1;
        }
        else if (event.key.code == sf::Keyboard::Down) {
            selectedOption++;
            if (selectedOption >= PAUSE_OPTIONS_COUNT) selectedOption = 0;
        }
        else if (event.key.code == sf::Keyboard::Return) {
            return selectedOption;
        }
    }
    return -1;
}
// Handle settings input - VOLUME REMOVED, LEFT/RIGHT KEYS REMOVED
void handleSettingsInput(sf::Event& event, int& difficulty, bool& exitSettings) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Up) {
            difficulty--;
            if (difficulty < 1) difficulty = 1;
        }
        else if (event.key.code == sf::Keyboard::Down) {
            difficulty++;
            if (difficulty > 3) difficulty = 3;
        }
        else if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::Return) {
            exitSettings = true;
        }
    }
}
// Get player name input for high score
bool getPlayerNameInput(sf::Event& event, char playerName[], int& nameLength, bool& finishedInput) {
    if (event.type == sf::Event::TextEntered) {
        if (event.text.unicode == '\b') { // Backspace
            if (nameLength > 0) {
                nameLength--;
                playerName[nameLength] = '\0';
            }
        }
        else if (event.text.unicode == '\r' || event.text.unicode == '\n') { // Enter
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
// Navigate high scores screen
bool handleHighScoresInput(sf::Event& event) {
    if (event.type == sf::Event::KeyPressed) {
        if (event.key.code == sf::Keyboard::Escape ||
            event.key.code == sf::Keyboard::Return) {
            return true;
        }
    }
    return false;
}
#endif
