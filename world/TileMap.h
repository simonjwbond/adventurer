#ifndef TILEMAP_H
#define TILEMAP_H

#include <SDL2/SDL.h>
#include "TileTypes.h"
#include "graphics/ProceduralArt.h"

/**
 * TileMap - Single room tile map with collision detection
 * 
 * Responsibilities:
 * - Store tile data for a single room (16x8 tiles at 32x32 pixels)
 * - Render the room using procedural textures
 * - Provide collision detection for entities
 */
class TileMap {
public:
    TileMap(SDL_Renderer* renderer, ProceduralArt* art);
    ~TileMap();
    
    /**
     * Initialize the tile map with a room layout
     * Creates an enclosed room with walls around the perimeter
     */
    void initialize();
    
    /**
     * Render the entire tile map
     */
    void render(SDL_Renderer* renderer, int offsetX, int offsetY);
    
    /**
     * Check if a position collides with any solid tiles
     * @param x X position in world coordinates
     * @param y Y position in world coordinates
     * @return true if collision detected
     */
    bool checkCollision(float x, float y) const;
    
    /**
     * Check if a bounding box collides with any solid tiles
     * Checks all four corners of the box
     * @param x X position of top-left corner in world coordinates
     * @param y Y position of top-left corner in world coordinates
     * @param width Width of the bounding box
     * @param height Height of the bounding box
     * @return true if collision detected
     */
    bool checkCollisionBox(float x, float y, int width, int height) const;
    
    /**
     * Get tile at specific grid position
     */
    TileType getTile(int gridX, int gridY) const;
    
    /**
     * Set tile at specific grid position
     */
    void setTile(int gridX, int gridY, TileType type);
    
    /**
     * Generate static background texture for optimized rendering
     * Reduces draw calls from 112 per frame to 1 per frame
     */
    void generateStaticBackground(SDL_Renderer* renderer);
    
    /**
     * Get the door position for world transitions
     * @return True if door exists, and outputs door position
     */
    bool getDoorPosition(float& doorX, float& doorY) const;
    
    /**
     * Check if a tile is a door
     */
    bool isDoorTile(int gridX, int gridY) const;
    
    /**
     * Set the map layout type (INDOOR or OUTDOOR)
     */
    void setMapLayout(const char* layoutName);
    
    /**
     * Load indoor house layout
     */
    void loadIndoorHouseLayout();
    
    /**
     * Load outdoor world layout with house positioned in it
     */
    void loadOutdoorWorldLayout();
    
    /**
     * Load desert biome world layout
     */
    void loadDesertWorldLayout();
    
private:
    static const int TILE_SIZE = 48;               // HD tiles for better display (48x48 pixels)
    static const int DEFAULT_ROOM_WIDTH_TILES = 16;
    static const int DEFAULT_ROOM_HEIGHT_TILES = 8;
    
    // For larger outdoor maps - dynamically allocated
    static const int OUTDOOR_WIDTH_TILES = 160;    // 10x wider than indoor
    static const int OUTDOOR_HEIGHT_TILES = 80;    // 10x taller than indoor
    
    // Tile data - use larger buffer that can store both indoor and outdoor
    TileType tiles[OUTDOOR_HEIGHT_TILES][OUTDOOR_WIDTH_TILES];
    
    // Current map dimensions
    int currentWidthTiles;
    int currentHeightTiles;
    
    // Procedural art textures
    ProceduralArt* proceduralArt;
    
    // Static background texture for optimized rendering
    SDL_Texture* staticBackgroundTexture = nullptr;
    
    /**
     * Draw a single tile at screen position
     */
    void drawTile(SDL_Renderer* renderer, int gridX, int gridY, int offsetX, int offsetY);
};

#endif // TILEMAP_H
