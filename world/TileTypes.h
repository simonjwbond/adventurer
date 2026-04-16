#ifndef TILETYPES_H
#define TILETYPES_H

/**
 * TileType - Enum for different tile types in the game world
 */
enum class TileType {
    EMPTY = 0,          // Empty space (air)
    
    // Floor tiles
    FLOOR_WOOD,         // Wooden floor (indoor)
    GRASS,              // Outdoor grass
    ROOF,               // House roof (visible when outside)
    
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
    TREE,               // Standard tree sprite
    TREE_LEFT,          // Left border tree
    TREE_RIGHT,         // Right border tree
    TREE_TOP,           // Top border tree
    TREE_BOTTOM,        // Bottom border tree
    TREE_CORNER_TL,     // Top-left corner tree
    TREE_CORNER_TR,     // Top-right corner tree
    TREE_CORNER_BL,     // Bottom-left corner tree
    TREE_CORNER_BR,     // Bottom-right corner tree
    BUSH,               // Bush sprite
    FENCE,              // Fence segment
    
    // Ice/Snow biome tiles (9 variations for the right side of the map)
    ICE_1,              // Light ice
    ICE_2,              // Medium ice
    ICE_3,              // Dark ice
    ICE_4,              // Cracked ice
    ICE_5,              // Smooth ice
    ICE_6,              // Frosty ice
    ICE_7,              // Deep ice
    ICE_8,              // Glacial ice
    ICE_9,              // Arctic ice
    SNOW,               // Snow tile
    
    // Count for array sizing
    COUNT
};

#endif // TILETYPES_H
