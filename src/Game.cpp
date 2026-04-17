#include "Game.h"
#include "InputManager.h"
#include "Camera.h"
#include "graphics/ProceduralArt.h"
#include "world/TileMap.h"
#include "world/WorldManager.h"
#include <cstdio>
#include <cstring>

/**
 * Game Constructor - Initialize member variables
 */
Game::Game() 
    : window(nullptr)
    , renderer(nullptr)
    , running(false)
    , paused(false)
    , shouldQuit(false)
    , lastFrameTime(0)
    , frameCount(0)
    , secondsSinceLastHeartbeat(0.0f)
    , autoCloseTimer(0.0f)
    , autoCloseDuration(0.0f)
    , inputManager(nullptr)
    , camera(nullptr)
    , proceduralArt(nullptr)
    , worldManager(nullptr)
    , settings(nullptr)
    , playerPosition(SCREEN_WIDTH / 2.0f, SCREEN_HEIGHT / 2.0f)  // Start in center of screen
    , playerFacing(FACING_DOWN)  // Default facing direction
{
}

/**
 * Game Destructor - Cleanup handled by cleanup() method
 */
Game::~Game() {
    cleanup();
}

/**
 * Initialize SDL window and renderer
 */
bool Game::initialize() {
    DEBUG_LOG("Creating window: %dx%d (scaled %dx%d)", 
              SCREEN_WIDTH, SCREEN_HEIGHT,
              SCREEN_WIDTH * WINDOW_SCALE, SCREEN_HEIGHT * WINDOW_SCALE);
    
    // Create window with scaling
    window = SDL_CreateWindow(
        "Adventurer",
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        SCREEN_WIDTH * WINDOW_SCALE,
        SCREEN_HEIGHT * WINDOW_SCALE,
        SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE
    );
    
    if (window == nullptr) {
        DEBUG_LOG("Failed to create window: %s", SDL_GetError());
        return false;
    }
    
    DEBUG_LOG("Window created successfully");
    
    // Create renderer with nearest-neighbor filtering for pixel art look
    renderer = SDL_CreateRenderer(window, -1,
                                  SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    
    if (renderer == nullptr) {
        DEBUG_LOG("Failed to create renderer: %s", SDL_GetError());
        return false;
    }
    
    DEBUG_LOG("Renderer created successfully");
    
    // Set rendering scale for integer scaling
    SDL_RenderSetIntegerScale(renderer, SDL_TRUE);
    
    // Set initial render target size (internal resolution)
    SDL_SetRenderDrawColor(renderer,
                           (Uint8)(Colors::BACKGROUND_DARK >> 16),
                           (Uint8)((Colors::BACKGROUND_DARK >> 8) & 0xFF),
                           (Uint8)(Colors::BACKGROUND_DARK & 0xFF),
                           255);
    SDL_RenderClear(renderer);
    SDL_RenderPresent(renderer);
    
    // Create subsystems
    inputManager = new InputManager();
    camera = new Camera();
    proceduralArt = new ProceduralArt();
    proceduralArt->initialize(renderer);
    worldManager = new WorldManager(renderer, proceduralArt);
    settings = new Settings();
    settings->loadFromFile(Settings::getDefaultPath());
    
    // Initialize the world manager with all areas
    worldManager->initialize();
    
    // Load the current area (blocking - must be ready before rendering)
    worldManager->ensureCurrentAreaLoaded();
    
    // Start preloading adjacent areas in background
    worldManager->preloadAdjacentAreas();
    
    // Start in fixed camera mode for indoor testing
    camera->setFixed(true);
    
    // Get starting position from current area
    WorldManager::WorldArea* currentArea = worldManager->getCurrentArea();
    if (currentArea != nullptr) {
        playerPosition.x = currentArea->entryX;
        playerPosition.y = currentArea->entryY;
        DEBUG_LOG("Player starting position from area: (%.1f, %.1f)", playerPosition.x, playerPosition.y);
    } else {
        playerPosition.x = SCREEN_WIDTH / 2.0f;
        playerPosition.y = SCREEN_HEIGHT / 2.0f;
        DEBUG_LOG("Player starting position (default): (%.1f, %.1f)", playerPosition.x, playerPosition.y);
    }
    
    running = true;
    lastFrameTime = SDL_GetTicks();
    
    DEBUG_LOG("InputManager created");
    DEBUG_LOG("Camera created (fixed, no panning)");
    DEBUG_LOG("ProceduralArt created");
    DEBUG_LOG("WorldManager initialized with multiple areas");
    DEBUG_LOG("Player starting position: (%.1f, %.1f)", playerPosition.x, playerPosition.y);
    DEBUG_LOG("Game initialized successfully");
    return true;
}

/**
 * Cleanup all resources
 */
void Game::cleanup() {
    // Delete subsystems
    if (worldManager != nullptr) {
        delete worldManager;
        worldManager = nullptr;
        DEBUG_LOG("WorldManager destroyed");
    }
    
    if (proceduralArt != nullptr) {
        delete proceduralArt;
        proceduralArt = nullptr;
        DEBUG_LOG("ProceduralArt destroyed");
    }
    
    if (inputManager != nullptr) {
        delete inputManager;
        inputManager = nullptr;
        DEBUG_LOG("InputManager destroyed");
    }
    
    if (camera != nullptr) {
        delete camera;
        camera = nullptr;
        DEBUG_LOG("Camera destroyed");
    }
    
    if (settings != nullptr) {
        delete settings;
        settings = nullptr;
        DEBUG_LOG("Settings destroyed");
    }
    
    // Delete SDL resources
    if (renderer != nullptr) {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
        DEBUG_LOG("Renderer destroyed");
    }
    
    if (window != nullptr) {
        SDL_DestroyWindow(window);
        window = nullptr;
        DEBUG_LOG("Window destroyed");
    }
}

/**
 * Handle input events - Centralized event polling per AGENTS.md protocol
 */
void Game::handleInput() {
    if (inputManager == nullptr) return;
    
    // Let InputManager handle all event polling and input state updates
    inputManager->handleEvents(shouldQuit);
    
    // Check for pause toggle
    if (inputManager->isPausePressed()) {
        paused = !paused;
        DEBUG_LOG("Game %s", paused ? "PAUSED" : "RESUMED");
    }
}

/**
 * Update game state - Placeholder for now
 */
void Game::update(float deltaTime) {
    if (inputManager == nullptr || camera == nullptr) return;
    
    // Get input values (-1.0 to +1.0)
    float horizontalInput = inputManager->getHorizontalInput();
    float verticalInput = inputManager->getVerticalInput();
    
    // Update player facing direction based on input (only when moving)
    if (horizontalInput != 0.0f || verticalInput != 0.0f) {
        if (verticalInput > 0.3f) { // Moving Down
            if (horizontalInput > 0.3f) playerFacing = FACING_DOWN_RIGHT;
            else if (horizontalInput < -0.3f) playerFacing = FACING_DOWN_LEFT;
            else playerFacing = FACING_DOWN;
        } else if (verticalInput < -0.3f) { // Moving Up
            if (horizontalInput > 0.3f) playerFacing = FACING_UP_RIGHT;
            else if (horizontalInput < -0.3f) playerFacing = FACING_UP_LEFT;
            else playerFacing = FACING_UP;
        } else { // Moving purely horizontally
            if (horizontalInput > 0.3f) playerFacing = FACING_RIGHT;
            else if (horizontalInput < -0.3f) playerFacing = FACING_LEFT;
        }
    }
    
    // Update animation timer and frame for walk cycle
    isMoving = (horizontalInput != 0.0f || verticalInput != 0.0f);
    if (isMoving) {
        animationTimer += 16; // Approximate ms per frame at 60 FPS
        // Cycle through frames every ~240ms (4 frames per second for walk cycle)
        currentAnimFrame = (animationTimer / 240) % 3;
    } else {
        animationTimer = 0;
        currentAnimFrame = 0; // Reset to idle frame when not moving
    }
    
    // Apply diagonal normalization for equal speed in all directions
    if (horizontalInput != 0.0f && verticalInput != 0.0f) {
        horizontalInput *= DIAGONAL_NORMALIZE;
        verticalInput *= DIAGONAL_NORMALIZE;
    }
    
    // Update player position based on input
    // Player speed is now configurable via game_settings.json
    float moveSpeed = settings ? settings->playerSpeed : PLAYER_SPEED;
    
    TileMap* tileMap = (worldManager != nullptr) ? worldManager->getCurrentTileMap() : nullptr;
    
    if (tileMap != nullptr) {
        // Check collision at new positions BEFORE applying movement
        float newX = playerPosition.x + horizontalInput * moveSpeed;
        float newY = playerPosition.y + verticalInput * moveSpeed;
        
        // Calculate bounding box for collision check
        float halfWidth = PLAYER_WIDTH / 2.0f;
        float halfHeight = PLAYER_HEIGHT / 2.0f;
        
        // Only update X if no collision at new X position
        if (!tileMap->checkCollisionBox(newX - halfWidth, playerPosition.y - halfHeight, 
                                        PLAYER_WIDTH, PLAYER_HEIGHT)) {
            playerPosition.x = newX;
        }
        
        // Only update Y if no collision at new Y position
        if (!tileMap->checkCollisionBox(playerPosition.x - halfWidth, newY - halfHeight,
                                        PLAYER_WIDTH, PLAYER_HEIGHT)) {
            playerPosition.y = newY;
        }
    } else {
        // Fallback: simple screen bounds clamping with wall margin
        const float WALL_MARGIN = 16.0f;
        float newX = playerPosition.x + horizontalInput * moveSpeed;
        float newY = playerPosition.y + verticalInput * moveSpeed;
        
        playerPosition.x = std::max(WALL_MARGIN, std::min(newX, static_cast<float>(SCREEN_WIDTH - WALL_MARGIN)));
        playerPosition.y = std::max(WALL_MARGIN, std::min(newY, static_cast<float>(SCREEN_HEIGHT - WALL_MARGIN)));
    }
    
    // Update camera to follow player
    camera->update(playerPosition);
    
    // Check for world transitions through door
    checkWorldTransition();
}

/**
 * Check if player has crossed a world boundary and needs to transition
 */
void Game::checkWorldTransition() {
    if (worldManager == nullptr) return;
    
    // Check for boundary transitions (ALttP style - walk off edge of map)
    bool transitioned = worldManager->checkBoundaryTransition(playerPosition.x, playerPosition.y);
    
    if (transitioned) {
        DEBUG_LOG("World boundary transition completed");
    }
}

/**
 * Render frame - Draw room with walls and floor
 */
void Game::render() {
    // Get camera offset (fixed at 0,0 for testing)
    Vector2D cameraOffset = camera->getOffset();
    
    // Clear screen to dark background (outside the room area)
    SDL_SetRenderDrawColor(renderer, 
                           (Uint8)(Colors::BACKGROUND_DARK >> 16),
                           (Uint8)((Colors::BACKGROUND_DARK >> 8) & 0xFF),
                           (Uint8)(Colors::BACKGROUND_DARK & 0xFF),
                           255);
    SDL_RenderClear(renderer);
    
    // Draw the tile map(s)
    if (worldManager != nullptr) {
        worldManager->render(renderer, static_cast<int>(cameraOffset.x), static_cast<int>(cameraOffset.y));
    }
    
    // Draw player sprite (Link-style, facing down)
    float screenX = playerPosition.x - cameraOffset.x;
    float screenY = playerPosition.y - cameraOffset.y;
    
    // Get the player sprite texture from ProceduralArt based on facing direction
    SDL_Texture* playerTexture = nullptr;
    switch (playerFacing) {
        case FACING_DOWN:
            playerTexture = proceduralArt->getPlayerSpriteDown(currentAnimFrame);
            break;
        case FACING_UP:
            playerTexture = proceduralArt->getPlayerSpriteUp(currentAnimFrame);
            break;
        case FACING_LEFT:
            playerTexture = proceduralArt->getPlayerSpriteLeft(currentAnimFrame);
            break;
        case FACING_RIGHT:
            playerTexture = proceduralArt->getPlayerSpriteRight(currentAnimFrame);
            break;
        case FACING_DOWN_LEFT:
            playerTexture = proceduralArt->getPlayerSpriteDownLeft(currentAnimFrame);
            break;
        case FACING_DOWN_RIGHT:
            playerTexture = proceduralArt->getPlayerSpriteDownRight(currentAnimFrame);
            break;
        case FACING_UP_LEFT:
            playerTexture = proceduralArt->getPlayerSpriteUpLeft(currentAnimFrame);
            break;
        case FACING_UP_RIGHT:
            playerTexture = proceduralArt->getPlayerSpriteUpRight(currentAnimFrame);
            break;
    }
    
    if (playerTexture != nullptr) {
        // Define source rectangle (full sprite)
        SDL_Rect srcRect;
        srcRect.x = 0;
        srcRect.y = 0;
        srcRect.w = PLAYER_WIDTH;   // 48 pixels (HD)
        srcRect.h = PLAYER_HEIGHT;  // 64 pixels (HD)
        
        // Define destination rectangle (centered on player position)
        SDL_Rect dstRect;
        dstRect.x = static_cast<int>(screenX - PLAYER_WIDTH / 2);
        dstRect.y = static_cast<int>(screenY - PLAYER_HEIGHT / 2);
        dstRect.w = PLAYER_WIDTH;
        dstRect.h = PLAYER_HEIGHT;
        
        // Render the sprite
        SDL_RenderCopy(renderer, playerTexture, &srcRect, &dstRect);
    } else {
        // Fallback: draw green rectangle if sprite fails to load
        SDL_Rect playerRect;
        playerRect.x = static_cast<int>(screenX - PLAYER_WIDTH / 2);
        playerRect.y = static_cast<int>(screenY - PLAYER_HEIGHT / 2);
        playerRect.w = PLAYER_WIDTH;
        playerRect.h = PLAYER_HEIGHT;
        
        SDL_SetRenderDrawColor(renderer,
                               (Uint8)(Colors::PLAYER_TUNE >> 16),
                               (Uint8)((Colors::PLAYER_TUNE >> 8) & 0xFF),
                               (Uint8)(Colors::PLAYER_TUNE & 0xFF),
                               255);
        SDL_RenderFillRect(renderer, &playerRect);
    }
    
    // Present the rendered frame
    SDL_RenderPresent(renderer);
}

/**
 * Log heartbeat with performance metrics
 */
void Game::logHeartbeat() {
    static bool firstHeartbeat = true;
    
    if (firstHeartbeat) {
        DEBUG_LOG("========================================");
        DEBUG_LOG("GAME LOOP STARTED - Performance Monitor");
        DEBUG_LOG("========================================");
        firstHeartbeat = false;
    }
    
    float fps = frameCount / secondsSinceLastHeartbeat;
    float avgFrameTime = (secondsSinceLastHeartbeat * 1000.0f) / frameCount;
    
    DEBUG_LOG("[HEARTBEAT] FPS: %.1f | Avg Frame Time: %.2fms | Frames: %d",
              fps, avgFrameTime, frameCount);
    
    // Check if we're maintaining target FPS
    if (fps < TARGET_FPS * 0.9f) {
        DEBUG_LOG("WARNING: Below target FPS (%d)", TARGET_FPS);
    }
}

/**
 * Set auto-close timer
 */
void Game::setAutoClose(float seconds) {
    if (seconds > 0) {
        autoCloseDuration = seconds;
        autoCloseTimer = 0.0f;
        DEBUG_LOG("Auto-close enabled: application will close in %.1f seconds", seconds);
    } else {
        autoCloseDuration = 0.0f;
        autoCloseTimer = 0.0f;
        DEBUG_LOG("Auto-close disabled");
    }
}

/**
 * Main game loop - Runs at 60 FPS with delta time calculation
 */
int Game::run() {
    // Initialize SDL resources
    if (!initialize()) {
        DEBUG_LOG("Failed to initialize game");
        return 1;
    }
    
    DEBUG_LOG("Entering main game loop...");
    
    while (running && !shouldQuit) {
        Uint32 currentFrameTime = SDL_GetTicks();
        
        // Calculate delta time in seconds
        float deltaTime = (currentFrameTime - lastFrameTime) / 1000.0f;
        lastFrameTime = currentFrameTime;
        
        // Frame timing clamp to prevent spiral of death
        if (deltaTime > 0.25f) {
            deltaTime = 0.25f;  // Cap at 250ms
        }
        
        // Handle input events
        handleInput();
        
        // Update and render if not paused
        if (!paused && !shouldQuit) {
            update(deltaTime);
            render();
        }
        
        // Frame counting for heartbeat
        frameCount++;
        secondsSinceLastHeartbeat += deltaTime;
        
        // Check auto-close timer
        if (autoCloseDuration > 0.0f) {
            autoCloseTimer += deltaTime;
            if (autoCloseTimer >= autoCloseDuration) {
                DEBUG_LOG("Auto-close timer expired (%.1f seconds). Closing application.", autoCloseDuration);
                shouldQuit = true;
            }
        }
        
        // Log performance heartbeat every ~5 seconds
        if (secondsSinceLastHeartbeat >= 5.0f) {
            logHeartbeat();
            frameCount = 0;
            secondsSinceLastHeartbeat = 0.0f;
        }
        
        // Frame rate limiting - maintain 60 FPS target
        Uint32 elapsed = SDL_GetTicks() - currentFrameTime;
        Uint32 targetElapsed = static_cast<Uint32>(FRAME_DURATION_MS);
        if (elapsed < targetElapsed) {
            SDL_Delay(targetElapsed - elapsed);
        }
    }
    
    DEBUG_LOG("Exiting game loop");
    cleanup();
    return 0;
}
