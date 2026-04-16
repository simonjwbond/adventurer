#ifndef WORLD_MANAGER_H
#define WORLD_MANAGER_H

#include <SDL2/SDL.h>
#include <map>
#include <string>
#include "world/TileMap.h"

/**
 * WorldManager - Manages multiple world areas with seamless transitions
 * 
 * Responsibilities:
 * - Load and manage multiple TileMap instances (grass, desert, ice, etc.)
 * - Track world area connections (north, south, east, west)
 * - Handle seamless transitions when player crosses map boundaries
 * - Pre-load adjacent areas for smooth transitions
 * - Render only visible areas based on camera position
 */
class WorldManager {
public:
    /**
     * World area types
     */
    enum class AreaType {
        GRASSLAND,
        DESERT,
        ICE,
        FOREST,
        MOUNTAIN,
        CAVE,
        INDOOR
    };
    
    /**
     * Direction for area connections
     */
    enum class Direction {
        NORTH,
        SOUTH,
        EAST,
        WEST
    };
    
    /**
     * Represents a world area with its connections
     */
    struct WorldArea {
        AreaType type;
        std::string name;
        TileMap* tileMap;
        
        // Connections to adjacent areas
        WorldArea* connectedNorth = nullptr;
        WorldArea* connectedSouth = nullptr;
        WorldArea* connectedEast = nullptr;
        WorldArea* connectedWest = nullptr;
        
        // Transition positions (where player enters from each direction)
        float entryX = 0;
        float entryY = 0;
        
        // Bounds in world coordinates
        int widthTiles = 0;
        int heightTiles = 0;
        int startX = 0;
        int startY = 0;
    };
    
    WorldManager(SDL_Renderer* renderer, ProceduralArt* art);
    ~WorldManager();
    
    /**
     * Initialize the world with all areas and connections
     */
    void initialize();
    
    /**
     * Get the current active area
     */
    WorldArea* getCurrentArea() const { return currentArea; }
    
    /**
     * Get the current TileMap
     */
    TileMap* getCurrentTileMap() const;
    
    /**
     * Check if player has crossed a boundary and needs to transition
     * @param playerX Player X position in world coordinates
     * @param playerY Player Y position in world coordinates
     * @return true if transition occurred
     */
    bool checkBoundaryTransition(float& playerX, float& playerY);
    
    /**
     * Get area type by name
     */
    static AreaType getAreaTypeFromString(const std::string& name);
    
    /**
     * Get string name for area type
     */
    static std::string getAreaTypeString(AreaType type);
    
    /**
     * Render all visible areas (current + adjacent if needed)
     */
    void render(SDL_Renderer* renderer, int cameraX, int cameraY);
    
    /**
     * Get world bounds for current area
     */
    void getWorldBounds(int& minX, int& maxX, int& minY, int& maxY) const;
    
private:
    SDL_Renderer* renderer;
    ProceduralArt* proceduralArt;
    
    // All loaded areas
    std::map<AreaType, WorldArea*> areas;
    
    // Current active area
    WorldArea* currentArea = nullptr;
    
    /**
     * Create a new world area
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
};

#endif // WORLD_MANAGER_H
