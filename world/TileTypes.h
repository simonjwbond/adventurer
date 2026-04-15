#ifndef TILETYPES_H
#define TILETYPES_H

/**
 * TileType - Enum for different tile types in the game world
 */
enum class TileType {
    EMPTY = 0,          // Empty space (air)
    
    // Floor tiles
    FLOOR_WOOD,         // Wooden floor
    GRASS,              // Outdoor grass
    
    // Wall tiles
    WALL_STONE,         // Basic stone wall
    WALL_CORNER_TL,     // Top-left corner
    WALL_CORNER_TR,     // Top-right corner
    WALL_CORNER_BL,     // Bottom-left corner
    WALL_CORNER_BR,     // Bottom-right corner
    WALL_EDGE_H,        // Horizontal edge
    WALL_EDGE_V,        // Vertical edge
    
    // Interactive tiles
    DOOR,               // House door (transition point)
    
    // Outdoor decorations (non-colliding sprites)
    TREE,               // Tree sprite
    BUSH,               // Bush sprite
    FENCE,              // Fence segment
    
    // Count for array sizing
    COUNT
};

#endif // TILETYPES_H
