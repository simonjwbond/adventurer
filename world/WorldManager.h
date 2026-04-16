#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <string>
#include <unordered_map>
#include <thread>
#include <mutex>
#include <future>
#include <atomic>
#include <condition_variable>
#include <queue>
#include <functional>
#include "world/TileMap.h"

class ProceduralArt;

/**
 * WorldManager - Manages multiple world areas with lazy loading
 * 
 * Features:
 * - Lazy loading of areas (only load when player approaches)
 * - Background loading with thread pool (non-blocking)
 * - Unload distant areas to save memory
 * - Seamless transitions between loaded areas
 */
class WorldManager {
public:
    enum class AreaType {
        GRASSLAND,
        DESERT,
        ICE,
        FOREST,
        MOUNTAIN,
        CAVE,
        INDOOR
    };
    
    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };
    
    /**
     * Loading state for an area
     */
    enum class LoadState {
        NOT_LOADED,      // Area not in memory
        LOADING,         // Currently loading in background
        LOADED,          // Fully loaded and ready
        LOADING_FAILED   // Loading failed
    };
    
    /**
     * Structure representing a world area
     */
    struct WorldArea {
        AreaType type;
        std::string name;
        TileMap* tileMap;
        
        // Connection pointers
        WorldArea* connectedNorth = nullptr;
        WorldArea* connectedSouth = nullptr;
        WorldArea* connectedEast  = nullptr;
        WorldArea* connectedWest  = nullptr;
        
        // Entry position when transitioning into this area
        float entryX = 0.0f;
        float entryY = 0.0f;
        
        // Loading state
        std::atomic<LoadState> loadState{LoadState::NOT_LOADED};
        
        // Dimensions
        int widthTiles = 0;
        int heightTiles = 0;
        
        // World bounds in pixels
        int startX = 0;
        int startY = 0;
        
        WorldArea() : tileMap(nullptr) {}
        ~WorldArea() {
            if (tileMap != nullptr) {
                delete tileMap;
                tileMap = nullptr;
            }
        }
    };
    
    /**
     * Task for background loading
     */
    struct LoadTask {
        WorldArea* area;
        SDL_Renderer* renderer;
        ProceduralArt* art;
        std::promise<void> promise;
    };
    
    /**
     * Constructor
     */
    WorldManager(SDL_Renderer* renderer, ProceduralArt* art);
    
    /**
     * Destructor
     */
    ~WorldManager();
    
    /**
     * Initialize the world manager (create area definitions, but don't load yet)
     */
    void initialize();
    
    /**
     * Ensure current area is loaded (blocking if needed)
     */
    void ensureCurrentAreaLoaded();
    
    /**
     * Start loading adjacent areas in background (non-blocking)
     */
    void preloadAdjacentAreas();
    
    /**
     * Check if an area is ready to use
     */
    bool isAreaLoaded(AreaType type) const;
    
    /**
     * Get the current TileMap (assumes current area is loaded)
     */
    TileMap* getCurrentTileMap() const;
    
    /**
     * Check if player has crossed a boundary and needs to transition
     */
    bool checkBoundaryTransition(float& playerX, float& playerY);
    
    /**
     * Render the current area
     */
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);
    
    /**
     * Get world bounds for current area
     */
    void getWorldBounds(int& minX, int& maxX, int& minY, int& maxY) const;
    
    /**
     * Get current area type
     */
    AreaType getCurrentAreaType() const;
    
    /**
     * Get current area (for initialization)
     */
    WorldArea* getCurrentArea() const;
    
    /**
     * Get area by type
     */
    WorldArea* getArea(AreaType type) const;
    
private:
    /**
     * Worker thread function
     */
    void workerThread();
    
    /**
     * Load an area (blocking)
     */
    void loadAreaSync(WorldArea* area);
    
    /**
     * Start async loading of an area
     */
    void loadAreaAsync(WorldArea* area);
    
    /**
     * Unload an area to free memory
     */
    void unloadArea(WorldArea* area);
    
    /**
     * Get area type from string name
     */
    AreaType getAreaTypeFromString(const std::string& name);
    
    /**
     * Get string name for area type
     */
    std::string getAreaTypeString(AreaType type);
    
    /**
     * Create a new world area definition (not loaded yet)
     */
    WorldArea* createArea(AreaType type, const std::string& name);
    
    /**
     * Connect two areas in a specific direction
     */
    void connectAreas(WorldArea* from, WorldArea* to, Direction dir);
    
    /**
     * Load the layout for a specific area type
     */
    void loadAreaLayout(WorldArea* area);
    
    // Renderer and art generator
    SDL_Renderer* renderer;
    ProceduralArt* proceduralArt;
    
    // All defined areas
    std::unordered_map<AreaType, WorldArea*> areas;
    
    // Current active area
    WorldArea* currentArea = nullptr;
    
    // Thread pool for background loading
    std::vector<std::thread> workerThreads;
    std::atomic<bool> shutdown{false};
    std::queue<LoadTask*> loadQueue;
    std::mutex queueMutex;
    std::condition_variable queueCV;
    
    // Debug logging
    static constexpr bool DEBUG_LOGGING = false;
};

#endif // WORLD_MANAGER_H
