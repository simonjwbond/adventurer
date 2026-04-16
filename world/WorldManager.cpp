#include "WorldManager.h"
#include <cstdio>
#include <cstring>
#include <cmath>

#define DEBUG_LOG(fmt, ...) printf("[WorldManager] " fmt "\n", ##__VA_ARGS__)

/**
 * WorldManager Constructor
 */
WorldManager::WorldManager(SDL_Renderer* renderer, ProceduralArt* art)
    : renderer(renderer)
    , proceduralArt(art)
{
}

/**
 * WorldManager Destructor
 */
WorldManager::~WorldManager() {
    // Clean up all areas
    for (auto& pair : areas) {
        if (pair.second != nullptr) {
            if (pair.second->tileMap != nullptr) {
                delete pair.second->tileMap;
            }
            delete pair.second;
        }
    }
    areas.clear();
    currentArea = nullptr;
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
 * Create a new world area
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
    DEBUG_LOG("Created area: %s (%dx%d tiles)", name.c_str(), area->widthTiles, area->heightTiles);
    
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
    
    DEBUG_LOG("Loaded layout: %s", area->name.c_str());
}

/**
 * Initialize the world with all areas and connections
 */
void WorldManager::initialize() {
    DEBUG_LOG("Initializing World Manager...");
    
    // Create all world areas
    WorldArea* grass = createArea(AreaType::GRASSLAND, "grassland");
    WorldArea* desert = createArea(AreaType::DESERT, "desert");
    WorldArea* ice = createArea(AreaType::ICE, "ice");
    
    // Connect areas:
    // Ice (West) <--> Grass (Center) <--> Desert (East)
    connectAreas(grass, ice, Direction::WEST);   // From grass, go west to ice
    connectAreas(ice, grass, Direction::EAST);   // From ice, go east to grass
    
    connectAreas(grass, desert, Direction::EAST); // From grass, go east to desert
    connectAreas(desert, grass, Direction::WEST); // From desert, go west to grass
    
    // Load all area layouts (pre-load for seamless transitions)
    loadAreaLayout(grass);
    loadAreaLayout(desert);
    loadAreaLayout(ice);
    
    // Set current area to grassland (starting area)
    currentArea = grass;
    
    DEBUG_LOG("World Manager initialized with %zu areas", areas.size());
}

/**
 * Get the current TileMap
 */
TileMap* WorldManager::getCurrentTileMap() const {
    return (currentArea != nullptr) ? currentArea->tileMap : nullptr;
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
        
        // Switch to new area
        currentArea = targetArea;
        
        // Set player position to entry point of new area
        playerX = currentArea->entryX;
        playerY = currentArea->entryY;
        
        // Regenerate background for new area
        currentArea->tileMap->generateStaticBackground(renderer);
        
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
    currentArea->tileMap->render(renderer, cameraX, cameraY);
    
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
