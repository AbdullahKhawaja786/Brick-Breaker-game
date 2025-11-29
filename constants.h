#ifndef CONSTANTS_H
#define CONSTANTS_H
// Window settings
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const char WINDOW_TITLE[] = "Brick Breaker Game";
// Paddle settings
const float PADDLE_WIDTH = 100.0f;
const float PADDLE_HEIGHT = 20.0f;
const float PADDLE_SPEED = 400.0f;
const float PADDLE_Y = 550.0f;
// Ball settings
const float BALL_RADIUS = 8.0f;
const float BALL_SPEED = 300.0f;
const float BALL_SPEED_INCREMENT = 50.0f;
// Brick settings
const int GRID_WIDTH = 10;
const int GRID_HEIGHT = 8;
const int TOTAL_BRICKS = GRID_WIDTH * GRID_HEIGHT;
const float BRICK_WIDTH = 75.0f;
const float BRICK_HEIGHT = 25.0f;
const float BRICK_OFFSET_X = 2.0f;
const float BRICK_OFFSET_Y = 50.0f;
const float BRICK_SPACING = 5.0f;
// Power-up settings
const int MAX_POWERUPS = 10;
const float POWERUP_SIZE = 20.0f;
const float POWERUP_FALL_SPEED = 100.0f;
const int POWERUP_CHANCE = 20;
// Particle settings
const int MAX_PARTICLES = 100;
const float PARTICLE_LIFETIME = 1.0f;
// Background animation
const int MAX_BG_STARS = 80;
// Game states
const int STATE_MAIN_MENU = 0;
const int STATE_PLAYING = 1;
const int STATE_PAUSED = 2;
const int STATE_GAME_OVER = 3;
const int STATE_SETTINGS = 4;
const int STATE_HIGH_SCORES = 5;
// Menu options
const int MENU_START_GAME = 0;
const int MENU_LOAD_GAME = 1;
const int MENU_HIGH_SCORES = 2;
const int MENU_SETTINGS = 3;
const int MENU_EXIT = 4;
const int MENU_OPTIONS_COUNT = 5;
// Pause menu options
const int PAUSE_RESUME = 0;
const int PAUSE_SAVE = 1;
const int PAUSE_SETTINGS = 2;
const int PAUSE_MAIN_MENU = 3;
const int PAUSE_OPTIONS_COUNT = 4;
// Power-up types
const int POWERUP_NONE = 0;
const int POWERUP_MULTIBALL = 1;
const int POWERUP_LARGER_PADDLE = 2;
const int POWERUP_SLOWER_BALL = 3;
const int POWERUP_EXTRA_LIFE = 4;
// Scoring
const int SCORE_BRICK_1HIT = 10;
const int SCORE_BRICK_2HIT = 25;
const int SCORE_BRICK_3HIT = 50;
const int SCORE_POWERUP = 100;
const int SCORE_LEVEL_COMPLETE = 1000;
// File names
const char SAVE_FILE[] = "savegame.txt";
const char HIGHSCORE_FILE[] = "highscores.txt";
const char SETTINGS_FILE[] = "settings.txt";
// High scores
const int MAX_HIGH_SCORES = 10;
const int MAX_NAME_LENGTH = 50;
// Initial lives
const int INITIAL_LIVES = 3;
const int MAX_LEVELS = 5;
#endif
