#ifndef GAME_H
#define GAME_H

#include <SDL2/SDL.h>
#include "utils/Constants.h"
#include "utils/MathUtils.h"

// Forward declarations
class InputManager;
class Camera;
class ProceduralArt;
class TileMap;

/**
 * Game - Main game class managing the game loop, window, and renderer
 * 
 * Responsibilities:
 * - SDL window and renderer creation/destruction
 * - Main game loop with 60 FPS timing
 * - State management (running, paused, quit)
 * - Frame timing and delta time calculation
 */
class Game {
public:
    Game();
    ~Game();
    
    /**
     * Run the main game loop
     * @return 0 on success, non-zero on error
     */
    int run();
    
private:
    // SDL resources
    SDL_Window* window;
    SDL_Renderer* renderer;
    
    // Game state
    bool running;
    bool paused;
    bool shouldQuit;
    
    // Timing
    Uint32 lastFrameTime;
    int frameCount;
    float secondsSinceLastHeartbeat;
    
    // Subsystems
    InputManager* inputManager;
    Camera* camera;
    ProceduralArt* proceduralArt;
    TileMap* tileMap;
    
    // Player position for testing (will move to Player class in Phase 3)
    Vector2D playerPosition;
    
    // Player facing direction for sprite selection
    enum FacingDirection {
        FACING_DOWN,
        FACING_UP,
        FACING_LEFT,
        FACING_RIGHT,
        FACING_DOWN_LEFT,
        FACING_DOWN_RIGHT,
        FACING_UP_LEFT,
        FACING_UP_RIGHT
    };
    FacingDirection playerFacing;
    
    // Animation timing for walk cycle
    Uint32 animationTimer = 0;  // Tracks time for frame cycling
    int currentAnimFrame = 0;   // Current animation frame (0-2)
    bool isMoving = false;      // Track if player is currently moving
    
    // World/Room tracking
    enum class WorldType {
        INDOOR_HOUSE,
        OUTDOOR_WORLD
    };
    WorldType currentWorld = WorldType::INDOOR_HOUSE;
    bool worldTransitionPending = false;  // Prevent rapid transition spam
    
    /**
     * Initialize SDL window and renderer
     * @return true on success, false on failure
     */
    bool initialize();
    
    /**
     * Cleanup all resources
     */
    void cleanup();
    
    /**
     * Process input events
     */
    void handleInput();
    
    /**
     * Update game state (placeholder for now)
     */
    void update(float deltaTime);
    
    /**
     * Render frame (placeholder for now)
     */
    void render();
    
    /**
     * Log heartbeat with frame timing info
     */
    void logHeartbeat();
    
    /**
     * Handle world transition through door
     */
    void checkWorldTransition();
    
    /**
     * Switch to a different world/room
     */
    void switchWorld(WorldType newWorld);
};

#endif // GAME_H
