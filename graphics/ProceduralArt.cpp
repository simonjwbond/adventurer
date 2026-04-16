#include "ProceduralArt.h"
#include <cstring>
#include <cmath>
#include <algorithm>

/**
 * ProceduralArt Constructor - Initialize with renderer reference
 */
ProceduralArt::ProceduralArt(SDL_Renderer* ren)
    : renderer(ren)
{
    createWoodFloorTexture();
    createStoneWallTexture();
    createRoofTexture();
    createGrassVariations();  // Create 9 grass variations
    createGrassTexture();
    createIceVariations();    // Create 9 ice variations
    createSnowTexture();
    createTreeTexture();
    createTreeBorderVariations();  // Create border tree variations
    createBushTexture();
    createFenceTexture();
    
    // Reuse wall textures for corners and edges (simple approach)
    wallCornerTopLeft = stoneWallTexture;
    wallCornerTopRight = stoneWallTexture;
    wallCornerBottomLeft = stoneWallTexture;
    wallCornerBottomRight = stoneWallTexture;
    wallEdgeHorizontal = stoneWallTexture;
    wallEdgeVertical = stoneWallTexture;
    
    // Create all 8 directional player sprites
    createPlayerSprites();
}

/**
 * Modular "Paper Doll" Character Assembly
 * Creates a custom sprite by layering body, clothes, and equipment
 * Direction: 0=Down, 1=Up, 2=Left, 3=Right, 4=DownLeft, 5=DownRight, 6=UpLeft, 7=UpRight
 */
SDL_Texture* ProceduralArt::assembleCharacter(const CharacterLoadout& loadout, int frame, int direction) {
    const int SPRITE_WIDTH = 48;
    const int SPRITE_HEIGHT = 64;
    Uint8 pixels[48 * 64 * 4];
    memset(pixels, 0, sizeof(pixels)); // Start transparent

    // Palette Colors (RGBA packed as Uint32, little-endian: ABGR)
    const Uint32 SKIN_PEACH = 0xFFDAB9;
    const Uint32 SKIN_SHADOW = 0xE5B899;
    const Uint32 HAIR_BROWN = 0x6B4423;
    const Uint32 HAIR_SHADOW = 0x4A3218;
    const Uint32 TUNIC_GREEN = 0x1B5E20;
    const Uint32 TUNIC_LIGHT = 0x4CAF50;
    const Uint32 BELT_BROWN = 0x8B4513;
    const Uint32 BOOT_BROWN = 0x5D4037;
    const Uint32 BLACK = 0x000000;
    const Uint32 WHITE = 0xFFFFFF;
    const Uint32 EYE_BROWN = 0x5D4037;
    const Uint32 HOOD_GREEN = 0x2E7D32;

    // Helper function to draw a simple character shape
    auto drawSprite = [&](const int* data, const Uint32* pal) {
        for (int y = 0; y < SPRITE_HEIGHT; y++) {
            for (int x = 0; x < SPRITE_WIDTH; x++) {
                int idx = y * SPRITE_WIDTH + x;
                int colorIndex = data[idx];
                
                if (colorIndex == 0) continue; // Transparent
                if (colorIndex >= 16) colorIndex = 0; // Safety check
                
                Uint32 color = pal[colorIndex];
                int pixelIdx = (y * SPRITE_WIDTH + x) * 4;
                pixels[pixelIdx + 0] = (color >> 16) & 0xFF; // R
                pixels[pixelIdx + 1] = (color >> 8) & 0xFF;  // G
                pixels[pixelIdx + 2] = color & 0xFF;         // B
                pixels[pixelIdx + 3] = 0xFF;                 // A
            }
        }
    };

    // ============================================================
    // GENERATE SPRITE DATA DYNAMICALLY BASED ON DIRECTION
    // ============================================================
    
    // Common sprite data arrays - each is exactly 48*64 = 3072 values
    // Using a simpler, cleaner design for each direction
    
    if (direction == 0) { // DOWN - Front facing with face visible
        // Build a simple hero design: hat, face, green tunic
        static int spriteData[48*64];
        memset(spriteData, 0, sizeof(spriteData));
        
        // Hat/Hair top (rows 0-4)
        for (int y = 0; y < 5; y++) {
            for (int x = 14; x < 34; x++) {
                spriteData[y * 48 + x] = 2; // Hair brown
            }
        }
        
        // Face (rows 5-10)
        for (int y = 5; y < 12; y++) {
            for (int x = 16; x < 32; x++) {
                spriteData[y * 48 + x] = 1; // Skin
            }
        }
        
        // Eyes (row 7)
        spriteData[7 * 48 + 20] = 7; // Eye white left
        spriteData[7 * 48 + 21] = 8; // Pupil left
        spriteData[7 * 48 + 26] = 7; // Eye white right
        spriteData[7 * 48 + 27] = 8; // Pupil right
        
        // Hair framing face (rows 5-8, sides)
        for (int y = 5; y < 9; y++) {
            spriteData[y * 48 + 14] = 2;
            spriteData[y * 48 + 15] = 2;
            spriteData[y * 48 + 33] = 2;
            spriteData[y * 48 + 34] = 2;
        }
        
        // Neck (rows 11-12)
        for (int y = 11; y < 13; y++) {
            for (int x = 21; x < 27; x++) {
                spriteData[y * 48 + x] = 1; // Skin
            }
        }
        
        // Shoulders/Tunic top (rows 13-20)
        for (int y = 13; y < 22; y++) {
            for (int x = 12; x < 36; x++) {
                if (y < 16) {
                    spriteData[y * 48 + x] = 4; // Tunic green
                } else {
                    // Add some variation
                    spriteData[y * 48 + x] = (x % 3 == 0) ? 5 : 4;
                }
            }
        }
        
        // Belt (rows 22-24)
        for (int y = 22; y < 25; y++) {
            for (int x = 14; x < 34; x++) {
                spriteData[y * 48 + x] = 6; // Belt brown
            }
        }
        
        // Lower tunic/legs (rows 25-45)
        for (int y = 25; y < 46; y++) {
            for (int x = 14; x < 34; x++) {
                spriteData[y * 48 + x] = 4; // Tunic green
            }
        }
        
        // Boots/feet (rows 46-52)
        for (int y = 46; y < 53; y++) {
            for (int x = 16; x < 32; x++) {
                spriteData[y * 48 + x] = 6; // Boot brown
            }
        }
        
        // Shadow (rows 53-60)
        for (int y = 53; y < 61; y++) {
            int shadowWidth = 20 - (y - 53);
            int startX = 14 + (y - 53) / 2;
            int endX = 34 - (y - 53) / 2;
            for (int x = startX; x < endX; x++) {
                if (x >= 0 && x < 48) {
                    spriteData[y * 48 + x] = 6;
                }
            }
        }
        
        // Build palette
        static Uint32 palette[16];
        memset(palette, 0, sizeof(palette));
        palette[1] = SKIN_PEACH;
        palette[2] = HAIR_BROWN;
        palette[3] = BLACK;
        palette[4] = TUNIC_GREEN;
        palette[5] = TUNIC_LIGHT;
        palette[6] = BELT_BROWN;
        palette[7] = WHITE;
        palette[8] = EYE_BROWN;
        palette[9] = SKIN_SHADOW;
        
        drawSprite(spriteData, palette);
    }
    else if (direction == 1) { // UP - Back view with hood
        static int spriteData[48*64];
        memset(spriteData, 0, sizeof(spriteData));
        
        // Hood top (rows 0-5)
        for (int y = 0; y < 6; y++) {
            for (int x = 14 + y/2; x < 34 - y/2; x++) {
                spriteData[y * 48 + x] = 1; // Hood green
            }
        }
        
        // Hair peeking out back (rows 2-5)
        for (int y = 2; y < 6; y++) {
            spriteData[y * 48 + 23] = 3; // Hair
            spriteData[y * 48 + 24] = 3;
            spriteData[y * 48 + 25] = 3;
        }
        
        // Back/Shoulders (rows 6-20)
        for (int y = 6; y < 22; y++) {
            for (int x = 12; x < 36; x++) {
                spriteData[y * 48 + x] = 1; // Hood green
            }
        }
        
        // Belt (rows 22-24)
        for (int y = 22; y < 25; y++) {
            for (int x = 14; x < 34; x++) {
                spriteData[y * 48 + x] = 4; // Belt
            }
        }
        
        // Lower body (rows 25-45)
        for (int y = 25; y < 46; y++) {
            for (int x = 14; x < 34; x++) {
                spriteData[y * 48 + x] = 5; // Tunic
            }
        }
        
        // Feet (rows 46-52)
        for (int y = 46; y < 53; y++) {
            for (int x = 16; x < 32; x++) {
                spriteData[y * 48 + x] = 4; // Boots
            }
        }
        
        // Shadow
        for (int y = 53; y < 61; y++) {
            int shadowWidth = 20 - (y - 53);
            int startX = 14 + (y - 53) / 2;
            int endX = 34 - (y - 53) / 2;
            for (int x = startX; x < endX; x++) {
                if (x >= 0 && x < 48) {
                    spriteData[y * 48 + x] = 4;
                }
            }
        }
        
        // Build palette for up direction
        static Uint32 palette[16];
        memset(palette, 0, sizeof(palette));
        palette[1] = HOOD_GREEN;
        palette[2] = HOOD_GREEN;
        palette[3] = HAIR_BROWN;
        palette[4] = BELT_BROWN;
        palette[5] = TUNIC_GREEN;
        palette[6] = TUNIC_LIGHT;
        palette[7] = HAIR_SHADOW;
        
        drawSprite(spriteData, palette);
    }
    else { // SIDE VIEWS (Left/Right and diagonals)
        static int spriteData[48*64];
        memset(spriteData, 0, sizeof(spriteData));
        
        // Determine if we should flip (for right-facing directions)
        bool flip = (direction == 3 || direction == 5 || direction == 7);
        
        // Head position (side profile)
        int headX = flip ? 28 : 18;
        
        // Hair/Hat (rows 0-4)
        for (int y = 0; y < 5; y++) {
            for (int x = headX; x < headX + 12; x++) {
                if (x >= 0 && x < 48) spriteData[y * 48 + x] = 2;
            }
        }
        
        // Face (rows 5-10)
        for (int y = 5; y < 11; y++) {
            for (int x = headX; x < headX + 10; x++) {
                if (x >= 0 && x < 48) spriteData[y * 48 + x] = 1;
            }
        }
        
        // Eye (row 7)
        int eyeX = flip ? headX + 6 : headX + 4;
        spriteData[7 * 48 + eyeX] = 7; // Eye white
        spriteData[7 * 48 + eyeX + 1] = 8; // Pupil
        
        // Neck (rows 11-12)
        for (int y = 11; y < 13; y++) {
            for (int x = headX + 4; x < headX + 10; x++) {
                if (x >= 0 && x < 48) spriteData[y * 48 + x] = 1;
            }
        }
        
        // Shoulders/Tunic (rows 13-21)
        for (int y = 13; y < 22; y++) {
            for (int x = headX - 4; x < headX + 16; x++) {
                if (x >= 0 && x < 48) {
                    spriteData[y * 48 + x] = (x % 3 == 0) ? 5 : 4;
                }
            }
        }
        
        // Belt (rows 22-24)
        for (int y = 22; y < 25; y++) {
            for (int x = headX - 2; x < headX + 16; x++) {
                if (x >= 0 && x < 48) spriteData[y * 48 + x] = 6;
            }
        }
        
        // Lower body (rows 25-45)
        for (int y = 25; y < 46; y++) {
            for (int x = headX - 2; x < headX + 16; x++) {
                if (x >= 0 && x < 48) spriteData[y * 48 + x] = 4;
            }
        }
        
        // Boots (rows 46-52)
        for (int y = 46; y < 53; y++) {
            for (int x = headX; x < headX + 12; x++) {
                if (x >= 0 && x < 48) spriteData[y * 48 + x] = 6;
            }
        }
        
        // Shadow
        for (int y = 53; y < 61; y++) {
            int startX = headX - 4 + (y - 53) / 2;
            int endX = headX + 18 - (y - 53) / 2;
            for (int x = startX; x < endX; x++) {
                if (x >= 0 && x < 48) spriteData[y * 48 + x] = 6;
            }
        }
        
        // Build palette
        static Uint32 palette[16];
        memset(palette, 0, sizeof(palette));
        palette[1] = SKIN_PEACH;
        palette[2] = HAIR_BROWN;
        palette[3] = BLACK;
        palette[4] = TUNIC_GREEN;
        palette[5] = TUNIC_LIGHT;
        palette[6] = BELT_BROWN;
        palette[7] = WHITE;
        palette[8] = EYE_BROWN;
        
        drawSprite(spriteData, palette);
    }
    
    return createTextureFromPixels(pixels, SPRITE_WIDTH, SPRITE_HEIGHT);
}

/**
 * ProceduralArt Destructor - Clean up textures
 */
ProceduralArt::~ProceduralArt() {
    cleanup();
}

/**
 * Create wooden floor texture
 */
void ProceduralArt::createWoodFloorTexture() {
    Uint8 pixels[32 * 32 * 4];
    
    const int width = 32;
    const int height = 32;
    const Uint8 baseR = 139, baseG = 69, baseB = 19;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4;
            
            Uint8 r, g, b;
            
            if ((y % 5) == 0 || (y % 6) == 0) {
                r = 101 + (rand() % 20 - 10);
                g = 46 + (rand() % 20 - 10);
                b = 13 + (rand() % 20 - 10);
            } else if ((x % 4) == 0 || (x % 5) == 0) {
                r = baseR + (rand() % 15 - 7);
                g = baseG + (rand() % 15 - 7);
                b = baseB + (rand() % 15 - 7);
            } else {
                r = baseR + (rand() % 30 - 15);
                g = baseG + (rand() % 30 - 15);
                b = baseB + (rand() % 30 - 15);
            }
            
            pixels[index + 0] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = 255;
        }
    }
    
    woodFloorTexture = createTextureFromPixels(pixels, 32, 32);
}

/**
 * Create roof texture - shingled pattern for house exterior
 */
void ProceduralArt::createRoofTexture() {
    Uint8 pixels[32 * 32 * 4];
    
    const int width = 32;
    const int height = 32;
    const Uint8 baseR = 101, baseG = 52, baseB = 26;  // Reddish-brown shingles
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4;
            
            Uint8 r, g, b;
            
            // Create shingle pattern - overlapping rows
            bool isShingleEdge = (y % 6) == 0 || (y % 6) == 5;
            bool isShingleVertical = (x % 8) == 0 || (x % 8) == 7;
            
            // Add offset for alternating rows to create staggered shingles
            int rowOffset = (y / 6) % 2 * 4;
            bool isStaggeredEdge = ((x + rowOffset) % 8) == 0 || ((x + rowOffset) % 8) == 7;
            
            if (isShingleEdge || isStaggeredEdge) {
                // Shingle edges - darker
                r = baseR - 20 + (rand() % 15 - 7);
                g = baseG - 15 + (rand() % 15 - 7);
                b = baseB - 10 + (rand() % 15 - 7);
            } else {
                // Shingle body - add variation for texture
                r = baseR + (rand() % 25 - 12);
                g = baseG + (rand() % 20 - 10);
                b = baseB + (rand() % 15 - 7);
            }
            
            pixels[index + 0] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = 255;
        }
    }
    
    roofTexture = createTextureFromPixels(pixels, 32, 32);
}

/**
 * Create 9 grass texture variations at 3x scale (96x96 pixels)
 * Each variation has different color palettes and noise patterns for natural variety
 */
void ProceduralArt::createGrassVariations() {
    const int TILE_SIZE = 96;  // 3x the original 32px
    
    // Define 9 different grass color palettes
    struct GrassPalette {
        Uint8 darkR, darkG, darkB;
        Uint8 mediumR, mediumG, mediumB;
        Uint8 lightR, lightG, lightB;
        Uint8 brownR, brownG, brownB;
    };
    
    GrassPalette palettes[9] = {
        // Variation 0: Standard green
        {20, 100, 20, 50, 168, 50, 100, 200, 100, 139, 90, 43},
        // Variation 1: Darker forest green
        {10, 80, 10, 30, 140, 30, 80, 180, 80, 100, 70, 30},
        // Variation 2: Bright spring green
        {40, 140, 40, 70, 190, 70, 120, 220, 120, 150, 100, 50},
        // Variation 3: Olive green
        {50, 100, 30, 80, 140, 60, 130, 180, 100, 120, 100, 50},
        // Variation 4: Yellow-green
        {60, 130, 40, 90, 170, 60, 140, 210, 100, 140, 110, 60},
        // Variation 5: Blue-green (teal tint)
        {30, 110, 50, 60, 150, 70, 100, 190, 110, 110, 90, 50},
        // Variation 6: Autumn green (more brown)
        {70, 120, 50, 100, 150, 70, 140, 180, 100, 160, 120, 80},
        // Variation 7: Deep emerald
        {15, 90, 30, 40, 130, 50, 90, 170, 90, 100, 80, 40},
        // Variation 8: Meadow mix
        {45, 125, 35, 75, 165, 55, 115, 205, 95, 135, 95, 45}
    };
    
    for (int v = 0; v < 9; v++) {
        Uint8 pixels[TILE_SIZE * TILE_SIZE * 4];
        const GrassPalette& pal = palettes[v];
        
        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                int index = (y * TILE_SIZE + x) * 4;
                
                // Create varied noise pattern - different seed per variation
                int noise = ((x * (7 + v) + y * (13 + v)) * (17 + v * 3)) % 100;
                
                Uint8 r, g, b;
                
                // Determine pixel type based on noise value
                if (noise < 35) {
                    // Dark green (shadow areas)
                    r = pal.darkR + (rand() % 15 - 7);
                    g = pal.darkG + (rand() % 15 - 7);
                    b = pal.darkB + (rand() % 15 - 7);
                } else if (noise < 65) {
                    // Medium green (base grass)
                    r = pal.mediumR + (rand() % 18 - 8);
                    g = pal.mediumG + (rand() % 18 - 8);
                    b = pal.mediumB + (rand() % 18 - 8);
                } else if (noise < 88) {
                    // Light green (sunlit areas)
                    r = pal.lightR + (rand() % 18 - 8);
                    g = pal.lightG + (rand() % 18 - 8);
                    b = pal.lightB + (rand() % 18 - 8);
                } else {
                    // Brown patches (dirt showing through)
                    r = pal.brownR + (rand() % 12 - 6);
                    g = pal.brownG + (rand() % 12 - 6);
                    b = pal.brownB + (rand() % 12 - 6);
                }
                
                // Add blade-like details - vertical streaks (more prominent at larger scale)
                if ((x % 6 == 0 || x % 6 == 1 || x % 6 == 2) && noise > 45) {
                    r = std::min(255, r + 25);
                    g = std::min(255, g + 25);
                    b = std::min(255, b + 8);
                }
                
                // Add some random bright spots (flowers/dew) - rare
                if (noise > 93) {
                    // Different flower colors per variation
                    int flowerType = v % 3;
                    if (flowerType == 0) {  // Yellow
                        r = 255; g = 255; b = 100;
                    } else if (flowerType == 1) {  // White
                        r = 255; g = 255; b = 240;
                    } else {  // Pink
                        r = 255; g = 180; b = 220;
                    }
                }
                
                pixels[index + 0] = r;
                pixels[index + 1] = g;
                pixels[index + 2] = b;
                pixels[index + 3] = 255;
            }
        }
        
        grassVariation[v] = createTextureFromPixels(pixels, TILE_SIZE, TILE_SIZE);
    }
}

/**
 * Create stone wall texture
 */
void ProceduralArt::createStoneWallTexture() {
    Uint8 pixels[32 * 32 * 4];
    
    const int width = 32;
    const int height = 32;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4;
            
            bool isMortar = false;
            if ((y % 8) == 7) isMortar = true;
            
            int brickRow = y / 8;
            int offset = (brickRow % 2) * 8;
            if ((x % 16) == 15 || (x % 16) == offset) isMortar = true;
            
            Uint8 r, g, b;
            
            if (isMortar) {
                r = 60 + (rand() % 10 - 5);
                g = 60 + (rand() % 10 - 5);
                b = 60 + (rand() % 10 - 5);
            } else {
                int brickY = y % 8;
                int brickX = (x - offset) % 16;
                
                if ((brickX < 2 || brickX > 13) || (brickY < 1 || brickY > 6)) {
                    r = 70 + (rand() % 15 - 7);
                    g = 70 + (rand() % 15 - 7);
                    b = 70 + (rand() % 15 - 7);
                } else {
                    r = 105 + (rand() % 20 - 10);
                    g = 105 + (rand() % 20 - 10);
                    b = 105 + (rand() % 20 - 10);
                }
            }
            
            pixels[index + 0] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = 255;
        }
    }
    
    stoneWallTexture = createTextureFromPixels(pixels, 32, 32);
}

/**
 * Create grass texture - seamless 32x32 tile for outdoor ground
 * Uses layered noise with multiple shades of green for natural variation
 */
void ProceduralArt::createGrassTexture() {
    Uint8 pixels[32 * 32 * 4];
    
    const int width = 32;
    const int height = 32;
    
    // Base grass color palette (Zelda-style earthy greens)
    const Uint8 DARK_GREEN_R = 34,   DARK_GREEN_G = 139,  DARK_GREEN_B = 34;
    const Uint8 MEDIUM_GREEN_R = 50, MEDIUM_GREEN_G = 168, MEDIUM_GREEN_B = 82;
    const Uint8 LIGHT_GREEN_R = 107, LIGHT_GREEN_G = 196, LIGHT_GREEN_B = 107;
    const Uint8 BROWN_R = 139, BROWN_G = 90, BROWN_B = 43;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            int index = (y * width + x) * 4;
            
            // Create a noise-like pattern using sine waves and modulo
            int noise = ((x * 7 + y * 13) * 17) % 100;
            
            Uint8 r, g, b;
            
            // Determine pixel type based on noise value
            if (noise < 40) {
                // Dark green (shadow areas)
                r = DARK_GREEN_R + (rand() % 10 - 5);
                g = DARK_GREEN_G + (rand() % 10 - 5);
                b = DARK_GREEN_B + (rand() % 10 - 5);
            } else if (noise < 70) {
                // Medium green (base grass)
                r = MEDIUM_GREEN_R + (rand() % 15 - 7);
                g = MEDIUM_GREEN_G + (rand() % 15 - 7);
                b = MEDIUM_GREEN_B + (rand() % 15 - 7);
            } else if (noise < 90) {
                // Light green (sunlit areas)
                r = LIGHT_GREEN_R + (rand() % 15 - 7);
                g = LIGHT_GREEN_G + (rand() % 15 - 7);
                b = LIGHT_GREEN_B + (rand() % 15 - 7);
            } else {
                // Brown patches (dirt showing through)
                r = BROWN_R + (rand() % 10 - 5);
                g = BROWN_G + (rand() % 10 - 5);
                b = BROWN_B + (rand() % 10 - 5);
            }
            
            // Add blade-like details - vertical streaks
            if ((x % 4 == 0 || x % 4 == 1) && noise > 50) {
                // Lighter vertical blade
                r = std::min(255, r + 20);
                g = std::min(255, g + 20);
                b = std::min(255, b + 5);
            }
            
            // Add some random bright spots (flowers/dew) - rare
            if (noise > 95) {
                r = 255;
                g = 255;
                b = 200;
            }
            
            pixels[index + 0] = r;
            pixels[index + 1] = g;
            pixels[index + 2] = b;
            pixels[index + 3] = 255;
        }
    }
    
    grassTexture = createTextureFromPixels(pixels, 32, 32);
}

/**
 * Create 9 ice texture variations at 32x32 for the ice biome
 * Each variation has different ice patterns and color palettes
 */
void ProceduralArt::createIceVariations() {
    const int TILE_SIZE = 32;
    
    // Define 9 different ice color palettes
    struct IcePalette {
        Uint8 baseR, baseG, baseB;
        Uint8 highlightR, highlightG, highlightB;
        Uint8 shadowR, shadowG, shadowB;
        Uint8 accentR, accentG, accentB;
    };
    
    IcePalette palettes[9] = {
        // Ice 1: Light blue ice
        {200, 230, 255, 240, 250, 255, 160, 200, 240, 180, 220, 250},
        // Ice 2: Medium cyan ice
        {100, 200, 220, 180, 230, 255, 70, 160, 190, 120, 190, 220},
        // Ice 3: Dark blue ice
        {60, 140, 180, 120, 190, 230, 40, 100, 140, 80, 160, 200},
        // Ice 4: Cracked ice (gray-blue)
        {140, 160, 180, 200, 210, 220, 100, 120, 140, 160, 170, 190},
        // Ice 5: Smooth ice (pure white-blue)
        {220, 240, 255, 255, 255, 255, 180, 210, 240, 240, 245, 255},
        // Ice 6: Frosty ice (white with blue tint)
        {230, 245, 255, 255, 255, 255, 190, 220, 245, 210, 235, 255},
        // Ice 7: Deep ice (dark teal)
        {40, 120, 140, 100, 170, 190, 20, 80, 100, 60, 140, 160},
        // Ice 8: Glacial ice (muted blue-gray)
        {150, 170, 190, 210, 225, 240, 110, 130, 150, 170, 190, 210},
        // Ice 9: Arctic ice (pale blue-white)
        {210, 235, 250, 245, 250, 255, 170, 200, 230, 220, 240, 255}
    };
    
    for (int v = 0; v < 9; v++) {
        Uint8 pixels[TILE_SIZE * TILE_SIZE * 4];
        const IcePalette& pal = palettes[v];
        
        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                int index = (y * TILE_SIZE + x) * 4;
                
                // Create ice pattern - crystalline structure
                int noise = ((x * 17 + y * 23 + v * 31) * 37) % 100;
                
                // Ice crystal pattern - diagonal streaks
                bool isCrystal = (x + y + v) % 7 == 0 || (x - y + v) % 9 == 0;
                
                Uint8 r, g, b;
                
                if (isCrystal) {
                    // Crystal highlights - bright streaks
                    r = pal.highlightR + (rand() % 15 - 7);
                    g = pal.highlightG + (rand() % 15 - 7);
                    b = pal.highlightB + (rand() % 15 - 7);
                } else if (noise < 30) {
                    // Shadow areas
                    r = pal.shadowR + (rand() % 12 - 6);
                    g = pal.shadowG + (rand() % 12 - 6);
                    b = pal.shadowB + (rand() % 12 - 6);
                } else if (noise < 70) {
                    // Base ice color
                    r = pal.baseR + (rand() % 20 - 10);
                    g = pal.baseG + (rand() % 20 - 10);
                    b = pal.baseB + (rand() % 20 - 10);
                } else {
                    // Accent/highlight
                    r = pal.accentR + (rand() % 18 - 9);
                    g = pal.accentG + (rand() % 18 - 9);
                    b = pal.accentB + (rand() % 18 - 9);
                }
                
                // Add subtle gradient for depth
                float gradient = (float)y / TILE_SIZE;
                r = (Uint8)(r * (0.9f + 0.1f * gradient));
                g = (Uint8)(g * (0.9f + 0.1f * gradient));
                b = (Uint8)(b * (0.9f + 0.1f * gradient));
                
                pixels[index + 0] = r;
                pixels[index + 1] = g;
                pixels[index + 2] = b;
                pixels[index + 3] = 255;
            }
        }
        
        iceVariation[v] = createTextureFromPixels(pixels, TILE_SIZE, TILE_SIZE);
    }
}

/**
 * Create snow texture - white fluffy snow tile
 */
void ProceduralArt::createSnowTexture() {
    const int TILE_SIZE = 32;
    Uint8 pixels[TILE_SIZE * TILE_SIZE * 4];
    
    for (int y = 0; y < TILE_SIZE; y++) {
        for (int x = 0; x < TILE_SIZE; x++) {
            int index = (y * TILE_SIZE + x) * 4;
            
            // Snow noise pattern
            int noise = ((x * 13 + y * 17) * 29) % 100;
            
            Uint8 brightness;
            if (noise < 20) {
                brightness = 220 + (rand() % 15);  // Slightly darker snow
            } else if (noise < 60) {
                brightness = 240 + (rand() % 15);  // Normal snow
            } else {
                brightness = 255;  // Bright snow highlights
            }
            
            // Add subtle blue tint for cold appearance
            pixels[index + 0] = brightness;
            pixels[index + 1] = (Uint8)(brightness - 5);
            pixels[index + 2] = (Uint8)(brightness - 2);
            pixels[index + 3] = 255;
        }
    }
    
    snowTexture = createTextureFromPixels(pixels, TILE_SIZE, TILE_SIZE);
}

/**
 * Create tree texture - 48x64 full sprite for outdoor decoration
 * Top-down compatible: can be used as overhead tree canopy or side view
 */
void ProceduralArt::createTreeTexture() {
    const int width = 48;
    const int height = 64;
    Uint8 pixels[48 * 64 * 4];
    memset(pixels, 0, sizeof(pixels)); // Start transparent
    
    // Color palette
    const Uint32 DARK_LEAF = 0xFF2D5220;      // Dark green
    const Uint32 MEDIUM_LEAF = 0xFF3CB358;    // Medium green
    const Uint32 LIGHT_LEAF = 0xFF5CB85C;     // Light green
    const Uint32 BRIGHT_LEAF = 0xFF7CDB7C;    // Highlight
    const Uint32 TRUNK_DARK = 0xFF4A3520;     // Dark brown
    const Uint32 TRUNK_LIGHT = 0xFF6B4E38;    // Light brown
    const Uint32 SHADOW = 0xFF1A1A1A;         // Shadow
    
    // Helper to set pixel with alpha
    auto setPixel = [&](int x, int y, Uint32 color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int idx = (y * width + x) * 4;
            pixels[idx + 0] = (color >> 16) & 0xFF;
            pixels[idx + 1] = (color >> 8) & 0xFF;
            pixels[idx + 2] = color & 0xFF;
            pixels[idx + 3] = 0xFF;
        }
    };
    
    // Draw trunk (bottom center, rows 40-60)
    for (int y = 40; y < 62; y++) {
        int trunkWidth = 4 + (y > 50 ? 1 : 0);
        int center = width / 2;
        for (int x = center - trunkWidth; x <= center + trunkWidth; x++) {
            if (x >= 0 && x < width) {
                // Add texture to trunk
                Uint32 trunkColor = (x % 2 == 0) ? TRUNK_DARK : TRUNK_LIGHT;
                if (y > 55) trunkColor = TRUNK_DARK; // Root shadow
                setPixel(x, y, trunkColor);
            }
        }
    }
    
    // Draw canopy (oval shape, rows 8-50)
    for (int y = 8; y < 52; y++) {
        // Calculate oval width at this height
        float normalizedY = (y - 30) / 22.0f;  // -1 at top, 1 at bottom
        float ovalWidth = 18.0f * sqrtf(1.0f - normalizedY * normalizedY * 0.7f);
        
        int centerX = width / 2;
        int halfWidth = (int)ovalWidth;
        
        for (int x = centerX - halfWidth; x <= centerX + halfWidth; x++) {
            if (x >= 0 && x < width) {
                // Determine leaf color based on position and noise
                int noise = ((x * 11 + y * 17) * 23) % 100;
                Uint32 leafColor;
                
                if (noise < 20) {
                    leafColor = DARK_LEAF;
                } else if (noise < 50) {
                    leafColor = MEDIUM_LEAF;
                } else if (noise < 75) {
                    leafColor = LIGHT_LEAF;
                } else {
                    leafColor = BRIGHT_LEAF;
                }
                
                // Add edge shadow
                if (x == centerX - halfWidth || x == centerX + halfWidth || 
                    y == 8 || y == 51) {
                    // Darken edge pixels slightly
                    Uint8 r = (leafColor >> 16) & 0xFF;
                    Uint8 g = (leafColor >> 8) & 0xFF;
                    Uint8 b = leafColor & 0xFF;
                    r = (Uint8)(r * 0.85);
                    g = (Uint8)(g * 0.85);
                    b = (Uint8)(b * 0.85);
                    leafColor = (0xFF << 24) | (r << 16) | (g << 8) | b;
                }
                
                setPixel(x, y, leafColor);
            }
        }
    }
    
    // Add some random leaves sticking out for organic look
    for (int i = 0; i < 30; i++) {
        int leafX = (rand() % 40) + 4;
        int leafY = (rand() % 36) + 12;
        Uint32 randomLeaf = (rand() % 2 == 0) ? LIGHT_LEAF : BRIGHT_LEAF;
        setPixel(leafX, leafY, randomLeaf);
    }
    
    // Add shadow under tree (bottom, semi-transparent)
    for (int y = 58; y < 64; y++) {
        int shadowWidth = 12 - (y - 58);
        int center = width / 2;
        for (int x = center - shadowWidth; x <= center + shadowWidth; x++) {
            if (x >= 0 && x < width) {
                int idx = (y * width + x) * 4;
                pixels[idx + 3] = 100; // Semi-transparent
            }
        }
    }
    
    treeTexture = createTextureFromPixels(pixels, width, height);
}

/**
 * Create 8 tree border variations for map edges
 * Each variation is positioned to face inward from the border
 */
void ProceduralArt::createTreeBorderVariations() {
    const int width = 48;
    const int height = 64;
    
    // Border tree types: 0=LEFT, 1=RIGHT, 2=TOP, 3=BOTTOM, 4=TL, 5=TR, 6=BL, 7=BR
    for (int borderType = 0; borderType < 8; borderType++) {
        Uint8 pixels[48 * 64 * 4];
        memset(pixels, 0, sizeof(pixels)); // Start transparent
        
        // Color palette - slightly different for border trees
        const Uint32 DARK_LEAF = 0xFF2D5220;
        const Uint32 MEDIUM_LEAF = 0xFF3CB358;
        const Uint32 LIGHT_LEAF = 0xFF5CB85C;
        const Uint32 BRIGHT_LEAF = 0xFF7CDB7C;
        const Uint32 TRUNK_DARK = 0xFF4A3520;
        const Uint32 TRUNK_LIGHT = 0xFF6B4E38;
        
        // Helper to set pixel
        auto setPixel = [&](int x, int y, Uint32 color) {
            if (x >= 0 && x < width && y >= 0 && y < height) {
                int idx = (y * width + x) * 4;
                pixels[idx + 0] = (color >> 16) & 0xFF;
                pixels[idx + 1] = (color >> 8) & 0xFF;
                pixels[idx + 2] = color & 0xFF;
                pixels[idx + 3] = 0xFF;
            }
        };
        
        // Adjust tree position based on border type
        int trunkX = width / 2;
        int trunkY = 40;
        float canopyOffsetX = 0;
        float canopyOffsetY = 0;
        
        // Position tree to lean/face inward from the border
        switch (borderType) {
            case 0: // LEFT - tree leans right (inward)
                canopyOffsetX = 4;
                break;
            case 1: // RIGHT - tree leans left (inward)
                canopyOffsetX = -4;
                break;
            case 2: // TOP - tree leans down (inward)
                canopyOffsetY = 4;
                break;
            case 3: // BOTTOM - tree leans up (inward)
                canopyOffsetY = -4;
                break;
            case 4: // TOP-LEFT - tree leans down-right
                canopyOffsetX = 3;
                canopyOffsetY = 3;
                break;
            case 5: // TOP-RIGHT - tree leans down-left
                canopyOffsetX = -3;
                canopyOffsetY = 3;
                break;
            case 6: // BOTTOM-LEFT - tree leans up-right
                canopyOffsetX = 3;
                canopyOffsetY = -3;
                break;
            case 7: // BOTTOM-RIGHT - tree leans up-left
                canopyOffsetX = -3;
                canopyOffsetY = -3;
                break;
        }
        
        // Draw trunk (bottom center)
        for (int y = trunkY; y < 62; y++) {
            int trunkWidth = 4 + (y > 50 ? 1 : 0);
            int centerX = static_cast<int>(trunkX + canopyOffsetX * 0.3f);
            for (int x = centerX - trunkWidth; x <= centerX + trunkWidth; x++) {
                if (x >= 0 && x < width) {
                    Uint32 trunkColor = (x % 2 == 0) ? TRUNK_DARK : TRUNK_LIGHT;
                    if (y > 55) trunkColor = TRUNK_DARK;
                    setPixel(x, y, trunkColor);
                }
            }
        }
        
        // Draw canopy (oval shape, shifted based on border type)
        for (int y = 8; y < 52; y++) {
            float normalizedY = (y - 30) / 22.0f;
            float ovalWidth = 18.0f * sqrtf(1.0f - normalizedY * normalizedY * 0.7f);
            
            int centerX = static_cast<int>(width / 2 + canopyOffsetX);
            int centerY = static_cast<int>(30 + canopyOffsetY);
            int halfWidth = (int)ovalWidth;
            
            for (int x = centerX - halfWidth; x <= centerX + halfWidth; x++) {
                if (x >= 0 && x < width) {
                    int noise = ((x * 11 + y * 17) * 23 + borderType * 7) % 100;
                    Uint32 leafColor;
                    
                    if (noise < 20) {
                        leafColor = DARK_LEAF;
                    } else if (noise < 50) {
                        leafColor = MEDIUM_LEAF;
                    } else if (noise < 75) {
                        leafColor = LIGHT_LEAF;
                    } else {
                        leafColor = BRIGHT_LEAF;
                    }
                    
                    // Edge shadow
                    if (x == centerX - halfWidth || x == centerX + halfWidth || 
                        y == 8 || y == 51) {
                        Uint8 r = (leafColor >> 16) & 0xFF;
                        Uint8 g = (leafColor >> 8) & 0xFF;
                        Uint8 b = leafColor & 0xFF;
                        r = (Uint8)(r * 0.85);
                        g = (Uint8)(g * 0.85);
                        b = (Uint8)(b * 0.85);
                        leafColor = (0xFF << 24) | (r << 16) | (g << 8) | b;
                    }
                    
                    setPixel(x, y, leafColor);
                }
            }
        }
        
        // Add random leaves for organic look
        for (int i = 0; i < 25; i++) {
            int leafX = (rand() % 40) + 4;
            int leafY = (rand() % 36) + 12;
            Uint32 randomLeaf = (rand() % 2 == 0) ? LIGHT_LEAF : BRIGHT_LEAF;
            setPixel(leafX, leafY, randomLeaf);
        }
        
        treeBorder[borderType] = createTextureFromPixels(pixels, width, height);
    }
}

/**
 * Create bush texture - 32x32 compact shrub for decoration
 */
void ProceduralArt::createBushTexture() {
    const int width = 32;
    const int height = 32;
    Uint8 pixels[32 * 32 * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Color palette for bush
    const Uint32 DARK_BUSH = 0xFF1B5E20;
    const Uint32 MEDIUM_BUSH = 0xFF2E7D32;
    const Uint32 LIGHT_BUSH = 0xFF4CAF50;
    const Uint32 HIGHLIGHT = 0xFF81C784;
    
    auto setPixel = [&](int x, int y, Uint32 color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int idx = (y * width + x) * 4;
            pixels[idx + 0] = (color >> 16) & 0xFF;
            pixels[idx + 1] = (color >> 8) & 0xFF;
            pixels[idx + 2] = color & 0xFF;
            pixels[idx + 3] = 0xFF;
        }
    };
    
    // Draw rounded bush shape
    int centerX = width / 2;
    int centerY = height / 2 + 2;
    int radius = 12;
    
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            // Calculate distance from center
            int dx = x - centerX;
            int dy = y - centerY;
            int dist = (int)(sqrtf(dx * dx + dy * dy));
            
            // Elliptical shape (wider than tall)
            int adjustedDist = (int)sqrtf((dx * dx) / 1.4f + dy * dy);
            
            if (adjustedDist < radius) {
                // Noise-based color variation
                int noise = ((x * 13 + y * 19) * 31) % 100;
                Uint32 bushColor;
                
                if (noise < 25) {
                    bushColor = DARK_BUSH;
                } else if (noise < 55) {
                    bushColor = MEDIUM_BUSH;
                } else if (noise < 80) {
                    bushColor = LIGHT_BUSH;
                } else {
                    bushColor = HIGHLIGHT;
                }
                
                // Edge darkening
                if (adjustedDist > radius - 3) {
                    Uint8 r = (bushColor >> 16) & 0xFF;
                    Uint8 g = (bushColor >> 8) & 0xFF;
                    Uint8 b = bushColor & 0xFF;
                    r = (Uint8)(r * 0.8);
                    g = (Uint8)(g * 0.8);
                    b = (Uint8)(b * 0.8);
                    bushColor = (0xFF << 24) | (r << 16) | (g << 8) | b;
                }
                
                setPixel(x, y, bushColor);
            }
        }
    }
    
    // Add some random bright spots (berries or highlights)
    for (int i = 0; i < 8; i++) {
        int bx = (rand() % 24) + 4;
        int by = (rand() % 20) + 6;
        if (pixels[(by * width + bx) * 4 + 3] > 0) {  // Only if already drawn
            setPixel(bx, by, HIGHLIGHT);
        }
    }
    
    bushTexture = createTextureFromPixels(pixels, width, height);
}

/**
 * Create fence texture - 32x32 wooden fence segment
 */
void ProceduralArt::createFenceTexture() {
    const int width = 32;
    const int height = 32;
    Uint8 pixels[32 * 32 * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Color palette for weathered wood fence
    const Uint32 WOOD_DARK = 0xFF4A3520;
    const Uint32 WOOD_MEDIUM = 0xFF6B4E38;
    const Uint32 WOOD_LIGHT = 0xFF8B6B4D;
    const Uint32 WOOD_WEATHERED = 0xFF8B7355;
    const Uint32 SHADOW = 0xFF2D2318;
    
    auto setPixel = [&](int x, int y, Uint32 color) {
        if (x >= 0 && x < width && y >= 0 && y < height) {
            int idx = (y * width + x) * 4;
            pixels[idx + 0] = (color >> 16) & 0xFF;
            pixels[idx + 1] = (color >> 8) & 0xFF;
            pixels[idx + 2] = color & 0xFF;
            pixels[idx + 3] = 0xFF;
        }
    };
    
    // Draw fence post (vertical, left side)
    int postX = 2;
    int postWidth = 5;
    for (int y = 0; y < height; y++) {
        for (int x = postX; x < postX + postWidth; x++) {
            // Add wood grain texture
            int noise = ((x * 3 + y * 7) * 13) % 100;
            Uint32 woodColor;
            if (noise < 30) {
                woodColor = WOOD_DARK;
            } else if (noise < 70) {
                woodColor = WOOD_MEDIUM;
            } else {
                woodColor = WOOD_LIGHT;
            }
            setPixel(x, y, woodColor);
        }
    }
    
    // Draw horizontal rails (two rails)
    // Top rail
    int railY1 = 8;
    int railHeight = 4;
    for (int y = railY1; y < railY1 + railHeight; y++) {
        for (int x = postX + postWidth; x < width - 2; x++) {
            int noise = ((x * 5 + y * 11) * 17) % 100;
            Uint32 railColor;
            if (noise < 25) {
                railColor = WOOD_WEATHERED;
            } else if (noise < 60) {
                railColor = WOOD_MEDIUM;
            } else {
                railColor = WOOD_LIGHT;
            }
            setPixel(x, y, railColor);
        }
    }
    
    // Bottom rail
    int railY2 = 20;
    for (int y = railY2; y < railY2 + railHeight; y++) {
        for (int x = postX + postWidth; x < width - 2; x++) {
            int noise = ((x * 5 + y * 11) * 17) % 100;
            Uint32 railColor;
            if (noise < 25) {
                railColor = WOOD_WEATHERED;
            } else if (noise < 60) {
                railColor = WOOD_MEDIUM;
            } else {
                railColor = WOOD_LIGHT;
            }
            setPixel(x, y, railColor);
        }
    }
    
    // Add shadow on right side of post and below rails
    for (int y = 0; y < height; y++) {
        // Post shadow
        setPixel(postX + postWidth, y, SHADOW);
        // Rail shadows
        if (y == railY1 + railHeight || y == railY2 + railHeight) {
            for (int x = postX + postWidth; x < width - 2; x++) {
                setPixel(x, y, SHADOW);
            }
        }
    }
    
    // Add some weathering marks (random dark spots)
    for (int i = 0; i < 15; i++) {
        int wx = (rand() % 20) + 8;
        int wy = (rand() % 28) + 2;
        if (pixels[(wy * width + wx) * 4 + 3] > 0) {
            setPixel(wx, wy, WOOD_DARK);
        }
    }
    
    fenceTexture = createTextureFromPixels(pixels, width, height);
}

SDL_Texture* ProceduralArt::createTextureFromPixels(Uint8* pixels, int width, int height) {
    if (renderer == nullptr || pixels == nullptr) {
        return nullptr;
    }

    SDL_Surface* surface = SDL_CreateRGBSurfaceWithFormat(0, width, height, 32, SDL_PIXELFORMAT_RGBA32);
    
    if (surface == nullptr) {
        return nullptr;
    }

    memcpy(surface->pixels, pixels, width * height * 4);

    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    
    SDL_FreeSurface(surface);

    if (texture == nullptr) {
        return nullptr;
    }

    SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

    return texture;
}

/**
 * Get wooden floor texture
 */
SDL_Texture* ProceduralArt::getWoodFloorTexture() {
    return woodFloorTexture;
}

/**
 * Get stone wall texture
 */
SDL_Texture* ProceduralArt::getStoneWallTexture() {
    return stoneWallTexture;
}

/**
 * Get roof texture
 */
SDL_Texture* ProceduralArt::getRoofTexture() {
    return roofTexture;
}

/**
 * Get grass variation texture (0-8 for 3x3 grid)
 */
SDL_Texture* ProceduralArt::getGrassVariation(int index) {
    if (index < 0 || index > 8) index = 0;  // Safety check
    return grassVariation[index];
}

/**
 * Get ice variation texture (0-8 for ice grid)
 */
SDL_Texture* ProceduralArt::getIceVariation(int index) {
    if (index < 0 || index > 8) index = 0;  // Safety check
    return iceVariation[index];
}

/**
 * Get snow texture
 */
SDL_Texture* ProceduralArt::getSnowTexture() {
    return snowTexture;
}

/**
 * Get grass texture (original 32x32 - deprecated)
 */
SDL_Texture* ProceduralArt::getGrassTexture() {
    return grassTexture;
}

/**
 * Get tree texture
 */
SDL_Texture* ProceduralArt::getTreeTexture() {
    return treeTexture;
}

/**
 * Get tree border texture (0-7 for different border positions)
 */
SDL_Texture* ProceduralArt::getTreeBorder(int index) {
    if (index < 0 || index > 7) index = 0;  // Safety check
    return treeBorder[index];
}

/**
 * Get bush texture
 */
SDL_Texture* ProceduralArt::getBushTexture() {
    return bushTexture;
}

/**
 * Get fence texture
 */
SDL_Texture* ProceduralArt::getFenceTexture() {
    return fenceTexture;
}

/**
 * Get corner wall textures
 */
SDL_Texture* ProceduralArt::getWallCornerTopLeft() {
    return wallCornerTopLeft;
}
SDL_Texture* ProceduralArt::getWallCornerTopRight() {
    return wallCornerTopRight;
}
SDL_Texture* ProceduralArt::getWallCornerBottomLeft() {
    return wallCornerBottomLeft;
}
SDL_Texture* ProceduralArt::getWallCornerBottomRight() {
    return wallCornerBottomRight;
}

/**
 * Get edge wall textures
 */
SDL_Texture* ProceduralArt::getWallEdgeHorizontal() {
    return wallEdgeHorizontal;
}
SDL_Texture* ProceduralArt::getWallEdgeVertical() {
    return wallEdgeVertical;
}

/**
 * Create all 8 directional player sprites (48x64 HD)
 */
void ProceduralArt::createPlayerSprites() {
    CharacterLoadout defaultLoadout;
    defaultLoadout.clothingType = 0;
    defaultLoadout.clothingColor = 0x2B5C9E;
    defaultLoadout.armorType = 0;
    defaultLoadout.weaponType = 0;
    
    for(int f = 0; f < MAX_ANIM_FRAMES; f++) {
        playerSpriteDown[f]     = assembleCharacter(defaultLoadout, f, 0);
        playerSpriteUp[f]       = assembleCharacter(defaultLoadout, f, 1);
        playerSpriteLeft[f]     = assembleCharacter(defaultLoadout, f, 2);
        playerSpriteRight[f]    = assembleCharacter(defaultLoadout, f, 3);
        playerSpriteDownLeft[f] = assembleCharacter(defaultLoadout, f, 4);
        playerSpriteDownRight[f]= assembleCharacter(defaultLoadout, f, 5);
        playerSpriteUpLeft[f]   = assembleCharacter(defaultLoadout, f, 6);
        playerSpriteUpRight[f]  = assembleCharacter(defaultLoadout, f, 7);
    }
}

/**
 * Get player sprite (facing down)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteDown(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteDown[frame];
}

/**
 * Get player sprite (facing up - back view)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteUp(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteUp[frame];
}

/**
 * Get player sprite (facing left)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteLeft(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteLeft[frame];
}

/**
 * Get player sprite (facing right)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteRight(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteRight[frame];
}

/**
 * Get player sprite (facing down-left diagonal)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteDownLeft(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteDownLeft[frame];
}

/**
 * Get player sprite (facing down-right diagonal)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteDownRight(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteDownRight[frame];
}

/**
 * Get player sprite (facing up-left diagonal)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteUpLeft(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteUpLeft[frame];
}

/**
 * Get player sprite (facing up-right diagonal)
 */
SDL_Texture* ProceduralArt::getPlayerSpriteUpRight(int frame) {
    if(frame < 0 || frame >= MAX_ANIM_FRAMES) frame = 0;
    return playerSpriteUpRight[frame];
}

/**
 * Cleanup all generated textures
 */
void ProceduralArt::cleanup() {
    if (woodFloorTexture != nullptr) {
        SDL_DestroyTexture(woodFloorTexture);
        woodFloorTexture = nullptr;
    }
    
    if (stoneWallTexture != nullptr && wallCornerTopLeft != stoneWallTexture) {
        SDL_DestroyTexture(stoneWallTexture);
        stoneWallTexture = nullptr;
    }
    
    if (grassTexture != nullptr) {
        SDL_DestroyTexture(grassTexture);
        grassTexture = nullptr;
    }
    
    // Clean up grass variations (9 textures)
    for (int i = 0; i < 9; i++) {
        if (grassVariation[i] != nullptr) {
            SDL_DestroyTexture(grassVariation[i]);
            grassVariation[i] = nullptr;
        }
    }
    
    // Clean up ice variations (9 textures)
    for (int i = 0; i < 9; i++) {
        if (iceVariation[i] != nullptr) {
            SDL_DestroyTexture(iceVariation[i]);
            iceVariation[i] = nullptr;
        }
    }
    
    if (snowTexture != nullptr) {
        SDL_DestroyTexture(snowTexture);
        snowTexture = nullptr;
    }
    
    if (treeTexture != nullptr) {
        SDL_DestroyTexture(treeTexture);
        treeTexture = nullptr;
    }
    
    // Clean up tree border variations (8 textures)
    for (int i = 0; i < 8; i++) {
        if (treeBorder[i] != nullptr) {
            SDL_DestroyTexture(treeBorder[i]);
            treeBorder[i] = nullptr;
        }
    }
    
    if (bushTexture != nullptr) {
        SDL_DestroyTexture(bushTexture);
        bushTexture = nullptr;
    }
    
    if (fenceTexture != nullptr) {
        SDL_DestroyTexture(fenceTexture);
        fenceTexture = nullptr;
    }
    
    for(int f = 0; f < MAX_ANIM_FRAMES; f++) {
        if (playerSpriteDown[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteDown[f]);
            playerSpriteDown[f] = nullptr;
        }
        if (playerSpriteUp[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteUp[f]);
            playerSpriteUp[f] = nullptr;
        }
        if (playerSpriteLeft[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteLeft[f]);
            playerSpriteLeft[f] = nullptr;
        }
        if (playerSpriteRight[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteRight[f]);
            playerSpriteRight[f] = nullptr;
        }
        if (playerSpriteDownLeft[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteDownLeft[f]);
            playerSpriteDownLeft[f] = nullptr;
        }
        if (playerSpriteDownRight[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteDownRight[f]);
            playerSpriteDownRight[f] = nullptr;
        }
        if (playerSpriteUpLeft[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteUpLeft[f]);
            playerSpriteUpLeft[f] = nullptr;
        }
        if (playerSpriteUpRight[f] != nullptr) {
            SDL_DestroyTexture(playerSpriteUpRight[f]);
            playerSpriteUpRight[f] = nullptr;
        }
    }
}
