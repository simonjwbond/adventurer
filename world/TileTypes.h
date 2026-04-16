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
    
    // Desert biome tiles (Arizona/Southwest style)
    // Ground tiles
    SAND,               // Light sand ground
    SAND_DARK,          // Darker sand (oasis/shadow)
    SAND_ROCKY,         // Sandy ground with rocks
    CRACKED_EARTH,      // Dry cracked mud
    ROCKY_GROUND,       // Rocky desert floor
    
    // Cacti
    CACTUS_SAGUARO,     // Tall Saguaro cactus
    CACTUS_SAGUARO_2ARMS, // Saguaro with 2 arms
    CACTUS_SAGUARO_3ARMS, // Saguaro with 3 arms
    CACTUS_PRICKLY_PEAR,  // Flat pad cactus
    CACTUS_CHOLLA,        // Cylindrical cholla cactus
    
    // Desert vegetation
    DESERT_BUSH,        // Sparse desert shrub
    DESERT_GRASS,       // Tuft of desert grass
    PALO_VERDE,         // Green-barked desert tree
    YUCCA,              // Spiky yucca plant
    AGAVE,              // Large agave succulent
    
    // Rocks and boulders
    ROCK_SMALL,         // Small rock/pebble
    ROCK_MEDIUM,        // Medium boulder
    ROCK_LARGE,         // Large boulder
    ROCK_FORMATION,     // Small cliff/mesa piece
    
    // Desert structures
    ADOBE_WALL,         // Adobe/mud-brick wall
    ADOBE_RUIN,         // Crumbling adobe structure
    DESERT_WELL,        // Stone well
    CAVE_ENTRANCE,      // Cave/mine entrance
    CAMPFIRE_SITE,      // Fire ring with stones
    
    // Decorative details
    BONES,              // Animal bones (skull/ribs)
    TUMBLEWEED,         // Dried rolling bush
    DEAD_TREE,          // Weathered driftwood
    SCORPION,           // Desert scorpion
    SNAKE,              // Rattlesnake
    
    // Count for array sizing
    COUNT
};

#endif // TILETYPES_H
