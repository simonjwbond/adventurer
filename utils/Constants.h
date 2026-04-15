#ifndef CONSTANTS_H
#define CONSTANTS_H

// ============================================================================
// Display & Resolution Constants
// ============================================================================
const int SCREEN_WIDTH = 512;           // Doubled NES width for top-down visibility
const int SCREEN_HEIGHT = 240;          // Standard NES height
const int WINDOW_SCALE = 3;             // Integer scaling factor (512*3 = 1536px window)
const int TARGET_FPS = 60;
const float FRAME_DURATION_MS = 1000.0f / TARGET_FPS;  // ~16.67ms per frame

// ============================================================================
// Tile & Room Constants
// ============================================================================
const int TILE_SIZE = 48;               // HD tiles for better display (48x48 pixels)
const int ROOM_WIDTH_TILES = SCREEN_WIDTH / TILE_SIZE;   // 16 tiles wide
const int ROOM_HEIGHT_TILES = SCREEN_HEIGHT / TILE_SIZE; // 7 tiles tall (with margins)

// ============================================================================
// Player Physics Constants (Top-Down Movement)
// ============================================================================
const float PLAYER_SPEED = 2.5f;            // Pixels per frame max speed
const float PLAYER_ACCELERATION = 0.4f;     // Speed increase per frame
const float PLAYER_FRICTION = 0.85f;        // Slide factor when no input (0-1)
const float DIAGONAL_NORMALIZE = 0.7071f;   // 1/sqrt(2) for equal diagonal speed

// Player dimensions (in pixels, 48x64 HD sprite with modular armor/clothes)
const int PLAYER_WIDTH = 48;
const int PLAYER_HEIGHT = 64;

// ============================================================================
// Combat Constants
// ============================================================================
const int MAX_HEALTH = 3;                   // Classic Zelda: 3 heart containers start
const float ATTACK_COOLDOWN_FRAMES = 20.0f; // Frames between attacks (~0.33s)
const float ATTACK_ACTIVE_FRAMES = 12.0f;   // Frames current attack is active
const float PUSHBACK_FORCE = 8.0f;          // Pushback on taking damage

// Attack hitbox dimensions (scaled for 48x64 player)
const int ATTACK_RANGE = 56;                // Pixels in front of player (extends beyond character)
const int ATTACK_WIDTH = 32;
const int ATTACK_HEIGHT = 24;

// ============================================================================
// Enemy Constants
// ============================================================================
const float ENEMY_SPEED = 1.5f;             // Slower than player (scaled for HD)
const float ENEMY_CHASE_RANGE = 192.0f;     // Pixels - when to start chasing (scaled up)
const int ENEMY_DAMAGE = 1;                 // Hearts per hit (half heart)
const int ENEMY_WIDTH = 42;                 // Scaled for HD (was 28)
const int ENEMY_HEIGHT = 42;                // Scaled for HD (was 28)

// ============================================================================
// Input Constants
// ============================================================================
enum KeyAction {
    MOVE_LEFT,
    MOVE_RIGHT,
    MOVE_UP,
    MOVE_DOWN,
    ATTACK,
    PAUSE,
    QUIT
};

// Note: Key bindings are handled directly in InputManager using SDL_SCANCODE_* constants
// This avoids header dependency issues with SDL2/SDL.h

// ============================================================================
// Color Palette (NES-inspired, top-down appropriate)
// ============================================================================
struct Colors {
    // Background tiles
    static const unsigned int GRASS_LIGHT;      // Primary ground color
    static const unsigned int GRASS_DARK;       // Secondary for patterns
    static const unsigned int STONE_LIGHT;      // Wall/obstacle light
    static const unsigned int STONE_DARK;       // Wall/obstacle dark
    static const unsigned int WATER;            // Water tiles
    static const unsigned int SAND;             // Desert/sand areas
    
    // Player colors
    static const unsigned int PLAYER_TUNE;      // Hat color (green like Link)
    static const unsigned int PLAYER_SKIN;
    static const unsigned int PLAYER_SHIRT;
    
    // Enemy colors
    static const unsigned int ENEMY_OCTOROK_RED;
    static const unsigned int ENEMY_OCTOROK_EYES;
    
    // Items
    static const unsigned int RUPEE_GREEN;
    static const unsigned int RUPEE_BLUE;
    static const unsigned int RUPEE_RED;
    static const unsigned int HEART_RED;
    static const unsigned int HEART_OUTLINE;
    
    // UI
    static const unsigned int TEXT_COLOR;
    static const unsigned int BACKGROUND_DARK;
};

// Color initializations (0xRRGGBB format) - inline for header-only usage
inline const unsigned int Colors::GRASS_LIGHT = 0x4A7C23;     // Dark green
inline const unsigned int Colors::GRASS_DARK = 0x3D661A;      // Darker green
inline const unsigned int Colors::STONE_LIGHT = 0x8B8B8B;     // Light gray
inline const unsigned int Colors::STONE_DARK = 0x5C5C5C;      // Dark gray
inline const unsigned int Colors::WATER = 0x234A9E;           // Blue
inline const unsigned int Colors::SAND = 0xE6D7B8;            // Sandy beige

inline const unsigned int Colors::PLAYER_TUNE = 0x2E8B57;     // Sea green (Link's hat)
inline const unsigned int Colors::PLAYER_SKIN = 0xFFDAB9;     // Peach puff
inline const unsigned int Colors::PLAYER_SHIRT = 0x3CB371;    // Medium sea green

inline const unsigned int Colors::ENEMY_OCTOROK_RED = 0xCD5C5C;
inline const unsigned int Colors::ENEMY_OCTOROK_EYES = 0xFFFFFF;

inline const unsigned int Colors::RUPEE_GREEN = 0x32CD32;     // Lime green
inline const unsigned int Colors::RUPEE_BLUE = 0x4169E1;      // Royal blue
inline const unsigned int Colors::RUPEE_RED = 0xDC143C;       // Crimson
inline const unsigned int Colors::HEART_RED = 0xFF0000;
inline const unsigned int Colors::HEART_OUTLINE = 0x8B0000;

inline const unsigned int Colors::TEXT_COLOR = 0xFFFFFF;
inline const unsigned int Colors::BACKGROUND_DARK = 0x1A1A2E;

// ============================================================================
// Debug Logging Macro
// ============================================================================
#include <cstdio>
#include <ctime>

#define DEBUG_LOG(format, ...) do { \
    time_t now = time(nullptr); \
    struct tm* t = localtime(&now); \
    fprintf(stderr, "[%02d:%02d:%02d] " format "\n", \
            t->tm_hour, t->tm_min, t->tm_sec, ##__VA_ARGS__); \
} while(0)

#endif // CONSTANTS_H
