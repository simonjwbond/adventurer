#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "Game.h"
#include "utils/Constants.h"

/**
 * Adventurer - Top-Down Action Adventure Game
 * Main entry point
 */
int main([[maybe_unused]] int argc, [[maybe_unused]] char* argv[]) {
    DEBUG_LOG("========================================");
    DEBUG_LOG("Adventurer Starting...");
    DEBUG_LOG("Resolution: %dx%d", SCREEN_WIDTH, SCREEN_HEIGHT);
    DEBUG_LOG("Target FPS: %d", TARGET_FPS);
    DEBUG_LOG("========================================");
    
    // Initialize SDL subsystems
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER) < 0) {
        DEBUG_LOG("SDL_Init failed: %s", SDL_GetError());
        return 1;
    }
    
    DEBUG_LOG("SDL initialized successfully");
    
    // Create and run the game
    Game game;
    int result = game.run();
    
    // Cleanup SDL
    SDL_Quit();
    
    DEBUG_LOG("Adventurer exited with code: %d", result);
    return result;
}
