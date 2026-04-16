#include "WorldManager.h"
#include "graphics/ProceduralArt.h"
#include <cstdio>
#include <cstring>
#include <cmath>

#define DEBUG_LOG(fmt, ...) if (WorldManager::DEBUG_LOGGING) printf("[WorldManager] " fmt "\n", ##__VA_ARGS__)

/**
 * Worker thread function - processes loading tasks from queue
 */
void WorldManager::workerThread() {
    while (!shutdown) {
        LoadTask* task = nullptr;
        
        // Get task from queue
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            queueCV.wait(lock, [this] { 
                return shutdown || !loadQueue.empty(); 
            });
            
            if (shutdown && loadQueue.empty()) {
                break;
            }
            
            if (!loadQueue.empty()) {
                task = loadQueue.front();
                loadQueue.pop();
            }
        }
        
        if (task != nullptr) {
            // Execute the load task
            try {
                loadAreaSync(task->area);
                task->promise.set_value();
            } catch (const std::exception& e) {
                DEBUG_LOG("Loading failed: %s", e.what());
                task->area->loadState = LoadState::LOADING_FAILED;
                task->promise.set_exception(std::current_exception());
            }
            
            delete task;
        }
    }
}

/**
 * Constructor
 */
WorldManager::WorldManager(SDL_Renderer* renderer, ProceduralArt* art)
    : renderer(renderer)
    , proceduralArt(art)
{
    // Create worker threads for background loading
    // One thread per CPU core, up to 4 threads
    int numThreads = std::min(4u, std::thread::hardware_concurrency());
    if (numThreads == 0) numThreads = 2;
    
    DEBUG_LOG("Creating %d worker threads for background loading", numThreads);
    
    for (int i = 0; i < numThreads; i++) {
        workerThreads.emplace_back(&WorldManager::workerThread, this);
    }
    
    DEBUG_LOG("WorldManager created with %d worker threads", (int)workerThreads.size());
}

/**
 * Destructor
 */
WorldManager::~WorldManager() {
    // Signal shutdown
    shutdown = true;
    queueCV.notify_all();
    
    // Wait for all threads to finish
    for (auto& thread : workerThreads) {
        if (thread.joinable()) {
            thread.join();
        }
    }
    
    DEBUG_LOG("Worker threads stopped");
    
    // Clean up all areas
    for (auto& pair : areas) {
        if (pair.second != nullptr) {
            delete pair.second;
        }
    }
    areas.clear();
    currentArea = nullptr;
    
    DEBUG_LOG("WorldManager destroyed");
}

/**
 * Get area type from string name
 */
WorldManager::AreaType WorldManager::getAreaTypeFromString(const std::string& name) {
    if (name == "grass" || name == "grassland") return AreaType::GRASSLAND;
    if (name == "desert") return AreaType::DESERT;
    if (name == "ice") return AreaType::ICE;
    if (name == "forest") return AreaType::FOREST;
    if (name == "mountain") return AreaType::MOUNTAIN;
    if (name == "cave") return AreaType::CAVE;
    if (name == "indoor") return AreaType::INDOOR;
    return AreaType::GRASSLAND; // Default
}

/**
 * Get string name for area type
 */
std::string WorldManager::getAreaTypeString(AreaType type) {
    switch (type) {
        case AreaType::GRASSLAND: return "grassland";
        case AreaType::DESERT: return "desert";
        case AreaType::ICE: return "ice";
        case AreaType::FOREST: return "forest";
        case AreaType::MOUNTAIN: return "mountain";
        case AreaType::CAVE: return "cave";
        case AreaType::INDOOR: return "indoor";
        default: return "unknown";
    }
}

/**
 * Create a new world area definition (not loaded yet)
 */
WorldManager::WorldArea* WorldManager::createArea(AreaType type, const std::string& name) {
    WorldArea* area = new WorldArea();
    area->type = type;
    area->name = name;
    area->tileMap = new TileMap(renderer, proceduralArt);
    
    // Set default dimensions
    area->widthTiles = 160;  // Standard outdoor size
    area->heightTiles = 80;
    
    // Calculate world bounds (in pixels)
    const int TILE_SIZE = 48;
    area->startX = 0;
    area->startY = 0;
    
    areas[type] = area;
    DEBUG_LOG("Created area definition: %s (%dx%d tiles)", name.c_str(), area->widthTiles, area->heightTiles);
    
    return area;
}

/**
 * Connect two areas in a specific direction
 */
void WorldManager::connectAreas(WorldArea* from, WorldArea* to, Direction dir) {
    const int TILE_SIZE = 48;
    
    switch (dir) {
        case Direction::NORTH:
            from->connectedNorth = to;
            to->connectedSouth = from;
            // Entry position: bottom of northern area, top of southern area
            to->entryX = (to->widthTiles * TILE_SIZE) / 2.0f;
            to->entryY = TILE_SIZE * 2;  // Near top of new area
            from->entryY = (from->heightTiles - 2) * TILE_SIZE;
            break;
            
        case Direction::SOUTH:
            from->connectedSouth = to;
            to->connectedNorth = from;
            // Entry position: top of southern area, bottom of northern area
            to->entryX = (to->widthTiles * TILE_SIZE) / 2.0f;
            to->entryY = (to->heightTiles - 2) * TILE_SIZE;  // Near bottom of new area
            from->entryY = TILE_SIZE * 2;
            break;
            
        case Direction::EAST:
            from->connectedEast = to;
            to->connectedWest = from;
            // Entry position: left side of eastern area
            to->entryX = TILE_SIZE * 2;  // Near left edge of new area
            to->entryY = (to->heightTiles * TILE_SIZE) / 2.0f;
            from->entryX = (from->widthTiles - 2) * TILE_SIZE;
            break;
            
        case Direction::WEST:
            from->connectedWest = to;
            to->connectedEast = from;
            // Entry position: right side of western area
            to->entryX = (to->widthTiles - 2) * TILE_SIZE;  // Near right edge of new area
            to->entryY = (to->heightTiles * TILE_SIZE) / 2.0f;
            from->entryX = TILE_SIZE * 2;
            break;
    }
    
    DEBUG_LOG("Connected %s -> %s (%s)", 
              from->name.c_str(), to->name.c_str(),
              dir == Direction::NORTH ? "NORTH" : 
              dir == Direction::SOUTH ? "SOUTH" :
              dir == Direction::EAST ? "EAST" : "WEST");
}

/**
 * Load the layout for a specific area type
 */
void WorldManager::loadAreaLayout(WorldArea* area) {
    if (area == nullptr || area->tileMap == nullptr) return;
    
    DEBUG_LOG("Loading layout for: %s", area->name.c_str());
    
    switch (area->type) {
        case AreaType::GRASSLAND:
            area->tileMap->loadOutdoorWorldLayout();
            break;
            
        case AreaType::DESERT:
            area->tileMap->loadDesertWorldLayout();
            break;
            
        case AreaType::ICE:
            // For now, use grass layout as placeholder
            // TODO: Implement ice world layout
            area->tileMap->loadOutdoorWorldLayout();
            break;
            
        case AreaType::FOREST:
            area->tileMap->loadOutdoorWorldLayout();
            break;
            
        case AreaType::MOUNTAIN:
            area->tileMap->loadOutdoorWorldLayout();
            break;
            
        case AreaType::CAVE:
        case AreaType::INDOOR:
            area->tileMap->loadIndoorHouseLayout();
            break;
    }
    
    // Generate static background
    area->tileMap->generateStaticBackground(renderer);
    
    area->loadState = LoadState::LOADED;
    DEBUG_LOG("Loaded layout: %s", area->name.c_str());
}

/**
 * Load an area synchronously (blocking)
 */
void WorldManager::loadAreaSync(WorldArea* area) {
    if (area == nullptr) return;
    
    area->loadState = LoadState::LOADING;
    loadAreaLayout(area);
}

/**
 * Start async loading of an area
 */
void WorldManager::loadAreaAsync(WorldArea* area) {
    if (area == nullptr) return;
    
    // Check if already loading or loaded
    LoadState state = area->loadState.load();
    if (state == LoadState::LOADING || state == LoadState::LOADED) {
        return;  // Already being loaded or done
    }
    
    area->loadState = LoadState::LOADING;
    
    // Create loading task
    LoadTask* task = new LoadTask();
    task->area = area;
    task->renderer = renderer;
    task->art = proceduralArt;
    
    // Add to queue
    {
        std::unique_lock<std::mutex> lock(queueMutex);
        loadQueue.push(task);
    }
    queueCV.notify_one();
    
    DEBUG_LOG("Started async loading for: %s", area->name.c_str());
}

/**
 * Unload an area to free memory
 */
void WorldManager::unloadArea(WorldArea* area) {
    if (area == nullptr || area->tileMap == nullptr) return;
    
    DEBUG_LOG("Unloading area: %s", area->name.c_str());
    
    // Delete tile map to free memory
    delete area->tileMap;
    area->tileMap = nullptr;
    area->loadState = LoadState::NOT_LOADED;
}

/**
 * Initialize the world with all area definitions (but don't load yet)
 */
void WorldManager::initialize() {
    DEBUG_LOG("Initializing World Manager (lazy loading enabled)...");
    
    // Create all world area definitions (not loaded yet)
    WorldArea* grass = createArea(AreaType::GRASSLAND, "grassland");
    WorldArea* desert = createArea(AreaType::DESERT, "desert");
    WorldArea* ice = createArea(AreaType::ICE, "ice");
    
    // Connect areas:
    // Ice (West) <--> Grass (Center) <--> Desert (East)
    connectAreas(grass, ice, Direction::WEST);   // From grass, go west to ice
    connectAreas(ice, grass, Direction::EAST);   // From ice, go east to grass
    
    connectAreas(grass, desert, Direction::EAST); // From grass, go east to desert
    connectAreas(desert, grass, Direction::WEST); // From desert, go west to grass
    
    // Set current area to grassland (starting area)
    currentArea = grass;
    
    // Don't pre-load - let it load on demand
    DEBUG_LOG("World Manager initialized with %zu area definitions (lazy loading enabled)", areas.size());
}

/**
 * Ensure current area is loaded (blocking if needed)
 */
void WorldManager::ensureCurrentAreaLoaded() {
    if (currentArea == nullptr) return;
    
    if (currentArea->loadState.load() != LoadState::LOADED) {
        DEBUG_LOG("Ensuring current area is loaded: %s", currentArea->name.c_str());
        loadAreaSync(currentArea);
    }
}

/**
 * Start loading adjacent areas in background (non-blocking)
 */
void WorldManager::preloadAdjacentAreas() {
    if (currentArea == nullptr) return;
    
    DEBUG_LOG("Preloading adjacent areas for: %s", currentArea->name.c_str());
    
    // Start async loading for all adjacent areas
    if (currentArea->connectedNorth != nullptr) {
        loadAreaAsync(currentArea->connectedNorth);
    }
    if (currentArea->connectedSouth != nullptr) {
        loadAreaAsync(currentArea->connectedSouth);
    }
    if (currentArea->connectedEast != nullptr) {
        loadAreaAsync(currentArea->connectedEast);
    }
    if (currentArea->connectedWest != nullptr) {
        loadAreaAsync(currentArea->connectedWest);
    }
}

/**
 * Check if an area is ready to use
 */
bool WorldManager::isAreaLoaded(AreaType type) const {
    auto it = areas.find(type);
    if (it == areas.end() || it->second == nullptr) {
        return false;
    }
    return it->second->loadState.load() == LoadState::LOADED;
}

/**
 * Get the current TileMap (assumes current area is loaded)
 */
TileMap* WorldManager::getCurrentTileMap() const {
    return (currentArea != nullptr && currentArea->tileMap != nullptr) ? currentArea->tileMap : nullptr;
}

/**
 * Get current area type
 */
WorldManager::AreaType WorldManager::getCurrentAreaType() const {
    return (currentArea != nullptr) ? currentArea->type : AreaType::GRASSLAND;
}

/**
 * Get current area (for initialization)
 */
WorldManager::WorldArea* WorldManager::getCurrentArea() const {
    return currentArea;
}

/**
 * Get area by type
 */
WorldManager::WorldArea* WorldManager::getArea(AreaType type) const {
    auto it = areas.find(type);
    return (it != areas.end()) ? it->second : nullptr;
}

/**
 * Check if player has crossed a boundary and needs to transition
 */
bool WorldManager::checkBoundaryTransition(float& playerX, float& playerY) {
    if (currentArea == nullptr) return false;
    
    const int TILE_SIZE = 48;
    const int BOUNDARY_MARGIN = 32;  // Pixels from edge to trigger transition
    
    int currentWidth = currentArea->widthTiles * TILE_SIZE;
    int currentHeight = currentArea->heightTiles * TILE_SIZE;
    
    WorldArea* targetArea = nullptr;
    Direction exitDir;
    
    // Check each boundary
    if (playerX < BOUNDARY_MARGIN && currentArea->connectedWest != nullptr) {
        targetArea = currentArea->connectedWest;
        exitDir = Direction::WEST;
    }
    else if (playerX > currentWidth - BOUNDARY_MARGIN && currentArea->connectedEast != nullptr) {
        targetArea = currentArea->connectedEast;
        exitDir = Direction::EAST;
    }
    else if (playerY < BOUNDARY_MARGIN && currentArea->connectedNorth != nullptr) {
        targetArea = currentArea->connectedNorth;
        exitDir = Direction::NORTH;
    }
    else if (playerY > currentHeight - BOUNDARY_MARGIN && currentArea->connectedSouth != nullptr) {
        targetArea = currentArea->connectedSouth;
        exitDir = Direction::SOUTH;
    }
    
    if (targetArea != nullptr) {
        DEBUG_LOG("Boundary transition: %s -> %s", 
                  currentArea->name.c_str(), targetArea->name.c_str());
        
        // Ensure target area is loaded (blocking if needed)
        if (targetArea->loadState.load() != LoadState::LOADED) {
            DEBUG_LOG("Loading target area before transition: %s", targetArea->name.c_str());
            loadAreaSync(targetArea);
        }
        
        // Switch to new area
        currentArea = targetArea;
        
        // Set player position to entry point of new area
        playerX = currentArea->entryX;
        playerY = currentArea->entryY;
        
        // Generate background for new area (should already be done, but just in case)
        if (currentArea->tileMap != nullptr) {
            currentArea->tileMap->generateStaticBackground(renderer);
        }
        
        // Start preloading the new adjacent areas
        preloadAdjacentAreas();
        
        return true;
    }
    
    return false;
}

/**
 * Render all visible areas
 */
void WorldManager::render(SDL_Renderer* renderer, int cameraX, int cameraY) {
    if (currentArea == nullptr) return;
    
    // Render current area
    if (currentArea->tileMap != nullptr) {
        currentArea->tileMap->render(renderer, cameraX, cameraY);
    }
    
    // TODO: Render adjacent areas if they overlap with viewport
    // This would enable seamless transitions without black borders
}

/**
 * Get world bounds for current area
 */
void WorldManager::getWorldBounds(int& minX, int& maxX, int& minY, int& maxY) const {
    if (currentArea == nullptr) {
        minX = maxX = minY = maxY = 0;
        return;
    }
    
    const int TILE_SIZE = 48;
    minX = 0;
    minY = 0;
    maxX = currentArea->widthTiles * TILE_SIZE;
    maxY = currentArea->heightTiles * TILE_SIZE;
}
