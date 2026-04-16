#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include <cstdlib>  // For atof
#include "Game.h"
#include "utils/Constants.h"

/**
 * Adventurer - Top-Down Action Adventure Game
 * Main entry point
 * 
 * Usage: adventurer [--autoclose SECONDS]
 *   --autoclose SECONDS  Automatically close after specified number of seconds
 */
int main(int argc, char* argv[]) {
    float autoCloseSeconds = 0.0f;
    
    // Parse command-line arguments
    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--autoclose") == 0 && i + 1 < argc) {
            autoCloseSeconds = atof(argv[++i]);
            if (autoCloseSeconds <= 0) {
                DEBUG_LOG("Warning: Invalid autoclose value, ignoring");
                autoCloseSeconds = 0.0f;
            }
        }
    }
    
    DEBUG_LOG("========================================");
    DEBUG_LOG("Adventurer Starting...");
    DEBUG_LOG("Resolution: %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
    DEBUG_LOG("Target FPS: %d", TARGET_FPS);
    if (autoCloseSeconds > 0) {
        DEBUG_LOG("Auto-close enabled: %g seconds", autoCloseSeconds);
    }
    DEBUG_LOG("========================================");
    
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        DEBUG_LOG("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    
    DEBUG_LOG("SDL initialized successfully");
    
    // Create and run the game
    Game game;
    
    // Set auto-close timer if specified
    if (autoCloseSeconds > 0) {
        game.setAutoClose(autoCloseSeconds);
    }
    
    int result = game.run();
    
    // Cleanup SDL
    SDL_Quit();
    
    DEBUG_LOG("Adventurer exited with code: %d", result);
    return result;
}
