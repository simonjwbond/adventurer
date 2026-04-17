#include "Desert.h"
#include "../Core/PixelUtils.h"
#include <cstring>
#include <cmath>

Desert::Desert() {}

Desert::~Desert() {
    // Ground
    if (sandTexture) SDL_DestroyTexture(sandTexture);
    if (sandDarkTexture) SDL_DestroyTexture(sandDarkTexture);
    if (sandRockyTexture) SDL_DestroyTexture(sandRockyTexture);
    if (crackedEarthTexture) SDL_DestroyTexture(crackedEarthTexture);
    if (rockyGroundTexture) SDL_DestroyTexture(rockyGroundTexture);
    
    // Cacti
    if (cactusSaguaroTexture) SDL_DestroyTexture(cactusSaguaroTexture);
    if (cactusSaguaro2ArmsTexture) SDL_DestroyTexture(cactusSaguaro2ArmsTexture);
    if (cactusSaguaro3ArmsTexture) SDL_DestroyTexture(cactusSaguaro3ArmsTexture);
    if (cactusPricklyPearTexture) SDL_DestroyTexture(cactusPricklyPearTexture);
    if (cactusChollaTexture) SDL_DestroyTexture(cactusChollaTexture);
    
    // Vegetation
    if (desertBushTexture) SDL_DestroyTexture(desertBushTexture);
    if (desertGrassTexture) SDL_DestroyTexture(desertGrassTexture);
    if (paloVerdeTexture) SDL_DestroyTexture(paloVerdeTexture);
    if (yuccaTexture) SDL_DestroyTexture(yuccaTexture);
    if (agaveTexture) SDL_DestroyTexture(agaveTexture);
    
    // Rocks
    if (rockSmallTexture) SDL_DestroyTexture(rockSmallTexture);
    if (rockMediumTexture) SDL_DestroyTexture(rockMediumTexture);
    if (rockLargeTexture) SDL_DestroyTexture(rockLargeTexture);
    if (rockFormationTexture) SDL_DestroyTexture(rockFormationTexture);
    
    // Structures
    if (adobeWallTexture) SDL_DestroyTexture(adobeWallTexture);
    if (adobeRuinTexture) SDL_DestroyTexture(adobeRuinTexture);
    if (desertWellTexture) SDL_DestroyTexture(desertWellTexture);
    if (caveEntranceTexture) SDL_DestroyTexture(caveEntranceTexture);
    if (campfireSiteTexture) SDL_DestroyTexture(campfireSiteTexture);
    
    // Details
    if (bonesTexture) SDL_DestroyTexture(bonesTexture);
    if (tumbleweedTexture) SDL_DestroyTexture(tumbleweedTexture);
    if (deadTreeTexture) SDL_DestroyTexture(deadTreeTexture);
    if (scorpionTexture) SDL_DestroyTexture(scorpionTexture);
    if (snakeTexture) SDL_DestroyTexture(snakeTexture);
}

void Desert::initialize(SDL_Renderer* ren) {
    renderer = ren;
    createSandTextures();
    createCactusTextures();
    createDesertVegetationTextures();
    createRockTextures();
    createDesertStructureTextures();
    createDesertDetailTextures();
    initialized = true;
}

void Desert::createSandTextures() {
    const int SIZE = 32;
    
    // Sand texture
    {
        Uint8 pixels[SIZE * SIZE * 4];
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x * 3 + y * 5) % 13 - 6) * 8;
                int val = 237 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 10));
                pixels[idx + 2] = std::max(0, std::min(255, val - 20));
                pixels[idx + 3] = 255;
            }
        }
        sandTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Sand dark
    {
        Uint8 pixels[SIZE * SIZE * 4];
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x + y) % 11 - 5) * 6;
                int val = 210 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 8));
                pixels[idx + 2] = std::max(0, std::min(255, val - 15));
                pixels[idx + 3] = 255;
            }
        }
        sandDarkTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Sand rocky
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x * 3 + y * 5) % 13 - 6) * 8;
                int val = 237 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 10));
                pixels[idx + 2] = std::max(0, std::min(255, val - 20));
                pixels[idx + 3] = 255;
                
                // Add small rocks
                if ((x * y + x + y) % 47 == 0) {
                    pixels[idx + 0] = 139;
                    pixels[idx + 1] = 125;
                    pixels[idx + 2] = 116;
                }
            }
        }
        sandRockyTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Cracked earth
    {
        Uint8 pixels[SIZE * SIZE * 4];
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x + y) % 9 - 4) * 10;
                int val = 160 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 20));
                pixels[idx + 2] = std::max(0, std::min(255, val - 30));
                pixels[idx + 3] = 255;
                
                // Cracks
                if (x % 8 == 0 || y % 8 == 0) {
                    pixels[idx + 0] = 80;
                    pixels[idx + 1] = 60;
                    pixels[idx + 2] = 50;
                }
            }
        }
        crackedEarthTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Rocky ground
    {
        Uint8 pixels[SIZE * SIZE * 4];
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x * 5 + y * 7) % 15 - 7) * 6;
                int val = 180 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 15));
                pixels[idx + 2] = std::max(0, std::min(255, val - 25));
                pixels[idx + 3] = 255;
            }
        }
        rockyGroundTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

void Desert::createCactusTextures() {
    const int SIZE = 32;
    
    // Saguaro cactus
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        
        // Main stem
        for (int y = 8; y < SIZE - 4; y++) {
            for (int x = 12; x < 20; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        
        // Left arm
        for (int y = 16; y < 22; y++) {
            for (int x = 6; x < 12; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        for (int y = 12; y < 17; y++) {
            for (int x = 6; x < 8; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        
        // Right arm
        for (int y = 14; y < 20; y++) {
            for (int x = 20; x < 26; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        for (int y = 10; y < 15; y++) {
            for (int x = 24; x < 26; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        
        cactusSaguaro2ArmsTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Simple saguaro (no arms)
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 6; y < SIZE - 4; y++) {
            for (int x = 12; x < 20; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        cactusSaguaroTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Three-armed saguaro
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        
        // Main stem
        for (int y = 6; y < SIZE - 4; y++) {
            for (int x = 12; x < 20; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        
        // Three arms
        for (int y = 12; y < 18; y++) {
            for (int x = 6; x < 12; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        for (int y = 10; y < 15; y++) {
            for (int x = 20; x < 26; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        for (int y = 8; y < 12; y++) {
            for (int x = 24; x < 28; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        
        cactusSaguaro3ArmsTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Prickly pear
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Main pad
        for (int y = 14; y < 24; y++) {
            for (int x = 10; x < 22; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 50;
                pixels[idx + 1] = 160;
                pixels[idx + 2] = 50;
                pixels[idx + 3] = 255;
            }
        }
        // Top pad
        for (int y = 8; y < 16; y++) {
            for (int x = 12; x < 20; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 50;
                pixels[idx + 1] = 160;
                pixels[idx + 2] = 50;
                pixels[idx + 3] = 255;
            }
        }
        cactusPricklyPearTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Cholla
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Multiple segments
        for (int seg = 0; seg < 3; seg++) {
            int y = 10 + seg * 6;
            for (int dy = 0; dy < 5; dy++) {
                for (int dx = -3; dx <= 3; dx++) {
                    int x = 16 + dx;
                    if (x >= 0 && x < SIZE && y + dy < SIZE) {
                        int idx = ((y + dy) * SIZE + x) * 4;
                        pixels[idx + 0] = 60;
                        pixels[idx + 1] = 140;
                        pixels[idx + 2] = 60;
                        pixels[idx + 3] = 255;
                    }
                }
            }
        }
        cactusChollaTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

void Desert::createDesertVegetationTextures() {
    const int SIZE = 32;
    
    // Desert bush
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 16; y < SIZE; y++) {
            for (int x = 8; x < 24; x++) {
                if ((x + y) % 3 == 0 || (x * y) % 5 == 0) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = 85;
                    pixels[idx + 1] = 107;
                    pixels[idx + 2] = 54;
                    pixels[idx + 3] = 255;
                }
            }
        }
        desertBushTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Desert grass
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int x = 4; x < SIZE - 4; x++) {
            int height = 10 + (x % 8);
            for (int y = SIZE - height; y < SIZE; y++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 139;
                pixels[idx + 1] = 125;
                pixels[idx + 2] = 80;
                pixels[idx + 3] = 255;
            }
        }
        desertGrassTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Palo verde
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Trunk
        for (int y = 16; y < SIZE; y++) {
            for (int x = 14; x < 18; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 100;
                pixels[idx + 1] = 160;
                pixels[idx + 2] = 100;
                pixels[idx + 3] = 255;
            }
        }
        // Branches
        for (int y = 8; y < 18; y++) {
            for (int x = 8; x < 14; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 100;
                pixels[idx + 1] = 160;
                pixels[idx + 2] = 100;
                pixels[idx + 3] = 255;
            }
        }
        for (int y = 6; y < 16; y++) {
            for (int x = 18; x < 24; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 100;
                pixels[idx + 1] = 160;
                pixels[idx + 2] = 100;
                pixels[idx + 3] = 255;
            }
        }
        paloVerdeTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Yucca
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Spiky leaves
        for (int i = 0; i < 8; i++) {
            float angle = i * 45.0f * 3.14159f / 180.0f;
            for (int len = 0; len < 12; len++) {
                int x = 16 + (int)(cosf(angle) * len);
                int y = 16 - (int)(sinf(angle) * len);
                if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = 144;
                    pixels[idx + 1] = 200;
                    pixels[idx + 2] = 144;
                    pixels[idx + 3] = 255;
                }
            }
        }
        yuccaTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Agave
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Thick spiky leaves
        for (int i = 0; i < 10; i++) {
            float angle = i * 36.0f * 3.14159f / 180.0f;
            for (int len = 0; len < 14; len++) {
                int x = 16 + (int)(cosf(angle) * len);
                int y = 16 - (int)(sinf(angle) * len);
                if (x >= 0 && x < SIZE && y >= 0 && y < SIZE) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = 100;
                    pixels[idx + 1] = 180;
                    pixels[idx + 2] = 100;
                    pixels[idx + 3] = 255;
                }
            }
        }
        agaveTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

void Desert::createRockTextures() {
    const int SIZE = 32;
    
    // Small rock
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 18; y < 26; y++) {
            for (int x = 12; x < 20; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x + y) % 7 - 3) * 8;
                int val = 120 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 10));
                pixels[idx + 2] = std::max(0, std::min(255, val - 20));
                pixels[idx + 3] = 255;
            }
        }
        rockSmallTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Medium rock
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 12; y < 28; y++) {
            for (int x = 8; x < 24; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x * 3 + y * 5) % 11 - 5) * 10;
                int val = 130 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 15));
                pixels[idx + 2] = std::max(0, std::min(255, val - 25));
                pixels[idx + 3] = 255;
            }
        }
        rockMediumTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Large rock
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 8; y < 30; y++) {
            for (int x = 4; x < 28; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x * 5 + y * 7) % 13 - 6) * 12;
                int val = 140 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 20));
                pixels[idx + 2] = std::max(0, std::min(255, val - 30));
                pixels[idx + 3] = 255;
            }
        }
        rockLargeTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Rock formation
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Base
        for (int y = 20; y < 30; y++) {
            for (int x = 6; x < 26; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 150;
                pixels[idx + 1] = 130;
                pixels[idx + 2] = 110;
                pixels[idx + 3] = 255;
            }
        }
        // Top
        for (int y = 10; y < 22; y++) {
            for (int x = 10; x < 22; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 140;
                pixels[idx + 1] = 120;
                pixels[idx + 2] = 100;
                pixels[idx + 3] = 255;
            }
        }
        rockFormationTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

void Desert::createDesertStructureTextures() {
    const int SIZE = 32;
    
    // Adobe wall
    {
        Uint8 pixels[SIZE * SIZE * 4];
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x + y) % 9 - 4) * 6;
                int val = 205 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 30));
                pixels[idx + 2] = std::max(0, std::min(255, val - 50));
                pixels[idx + 3] = 255;
            }
        }
        adobeWallTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Adobe ruin
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 8; y < SIZE; y++) {
            for (int x = 4; x < 28; x++) {
                if ((x + y) % 4 != 0) continue;
                int idx = (y * SIZE + x) * 4;
                int val = 195;
                pixels[idx + 0] = val;
                pixels[idx + 1] = val - 35;
                pixels[idx + 2] = val - 55;
                pixels[idx + 3] = 255;
            }
        }
        adobeRuinTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Desert well
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Base
        for (int y = 20; y < 28; y++) {
            for (int x = 8; x < 24; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 180;
                pixels[idx + 1] = 140;
                pixels[idx + 2] = 100;
                pixels[idx + 3] = 255;
            }
        }
        // Rim
        for (int x = 6; x < 26; x++) {
            int idx = (20 * SIZE + x) * 4;
            pixels[idx + 0] = 160;
            pixels[idx + 1] = 120;
            pixels[idx + 2] = 80;
            pixels[idx + 3] = 255;
        }
        desertWellTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Cave entrance
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Rock frame
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int dx = x - 16;
                int dy = y - 16;
                if (dx * dx + dy * dy > 14 * 14) {
                    int idx = (y * SIZE + x) * 4;
                    int noise = ((x + y) % 7 - 3) * 10;
                    int val = 100 + noise;
                    pixels[idx + 0] = std::max(0, std::min(255, val));
                    pixels[idx + 1] = std::max(0, std::min(255, val - 15));
                    pixels[idx + 2] = std::max(0, std::min(255, val - 25));
                    pixels[idx + 3] = 255;
                }
            }
        }
        // Dark interior
        for (int y = 8; y < 24; y++) {
            for (int x = 10; x < 22; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 20;
                pixels[idx + 1] = 20;
                pixels[idx + 2] = 20;
                pixels[idx + 3] = 255;
            }
        }
        caveEntranceTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Campfire site
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Stones
        for (int i = 0; i < 6; i++) {
            float angle = i * 60.0f * 3.14159f / 180.0f;
            int x = 16 + (int)(cosf(angle) * 10);
            int y = 16 + (int)(sinf(angle) * 10);
            for (int dy = -2; dy <= 2; dy++) {
                for (int dx = -2; dx <= 2; dx++) {
                    int px = x + dx;
                    int py = y + dy;
                    if (px >= 0 && px < SIZE && py >= 0 && py < SIZE) {
                        int idx = (py * SIZE + px) * 4;
                        pixels[idx + 0] = 100;
                        pixels[idx + 1] = 100;
                        pixels[idx + 2] = 100;
                        pixels[idx + 3] = 255;
                    }
                }
            }
        }
        // Fire
        for (int y = 12; y < 20; y++) {
            for (int x = 12; x < 20; x++) {
                int idx = (y * SIZE + x) * 4;
                int dist = (x - 16) * (x - 16) + (y - 16) * (y - 16);
                if (dist < 25) {
                    pixels[idx + 0] = 255;
                    pixels[idx + 1] = 165 + (int)(rand() % 50);
                    pixels[idx + 2] = 0;
                    pixels[idx + 3] = 255;
                }
            }
        }
        campfireSiteTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

void Desert::createDesertDetailTextures() {
    const int SIZE = 32;
    
    // Bones
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Bone shape
        for (int y = 12; y < 20; y++) {
            for (int x = 8; x < 24; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 220;
                pixels[idx + 1] = 210;
                pixels[idx + 2] = 190;
                pixels[idx + 3] = 255;
            }
        }
        // Ends
        for (int y = 10; y < 22; y++) {
            for (int x = 4; x < 8; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 220;
                pixels[idx + 1] = 210;
                pixels[idx + 2] = 190;
                pixels[idx + 3] = 255;
            }
        }
        for (int y = 10; y < 22; y++) {
            for (int x = 24; x < 28; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 220;
                pixels[idx + 1] = 210;
                pixels[idx + 2] = 190;
                pixels[idx + 3] = 255;
            }
        }
        bonesTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Tumbleweed
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        for (int y = 10; y < 24; y++) {
            for (int x = 8; x < 24; x++) {
                if ((x * y) % 7 == 0 || (x + y) % 5 == 0) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = 139;
                    pixels[idx + 1] = 119;
                    pixels[idx + 2] = 95;
                    pixels[idx + 3] = 255;
                }
            }
        }
        tumbleweedTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Dead tree
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Trunk
        for (int y = 8; y < SIZE; y++) {
            for (int x = 14; x < 18; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 80;
                pixels[idx + 1] = 60;
                pixels[idx + 2] = 40;
                pixels[idx + 3] = 255;
            }
        }
        // Branches
        for (int y = 4; y < 12; y++) {
            for (int x = 8; x < 14; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 80;
                pixels[idx + 1] = 60;
                pixels[idx + 2] = 40;
                pixels[idx + 3] = 255;
            }
        }
        for (int y = 2; y < 10; y++) {
            for (int x = 18; x < 24; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 80;
                pixels[idx + 1] = 60;
                pixels[idx + 2] = 40;
                pixels[idx + 3] = 255;
            }
        }
        deadTreeTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Scorpion
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Body
        for (int y = 14; y < 20; y++) {
            for (int x = 12; x < 20; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 100;
                pixels[idx + 1] = 80;
                pixels[idx + 2] = 60;
                pixels[idx + 3] = 255;
            }
        }
        // Tail
        for (int y = 8; y < 14; y++) {
            for (int x = 14; x < 18; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 100;
                pixels[idx + 1] = 80;
                pixels[idx + 2] = 60;
                pixels[idx + 3] = 255;
            }
        }
        scorpionTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Snake
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        // Wavy body
        for (int x = 4; x < SIZE - 4; x++) {
            int y = 16 + (int)(sinf(x * 0.5f) * 6);
            for (int dy = -2; dy <= 2; dy++) {
                for (int dx = -1; dx <= 1; dx++) {
                    int px = x + dx;
                    int py = y + dy;
                    if (px >= 0 && px < SIZE && py >= 0 && py < SIZE) {
                        int idx = (py * SIZE + px) * 4;
                        pixels[idx + 0] = 80;
                        pixels[idx + 1] = 120;
                        pixels[idx + 2] = 60;
                        pixels[idx + 3] = 255;
                    }
                }
            }
        }
        snakeTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

// Getter implementations
SDL_Texture* Desert::getSandTexture() { return sandTexture; }
SDL_Texture* Desert::getSandDarkTexture() { return sandDarkTexture; }
SDL_Texture* Desert::getSandRockyTexture() { return sandRockyTexture; }
SDL_Texture* Desert::getCrackedEarthTexture() { return crackedEarthTexture; }
SDL_Texture* Desert::getRockyGroundTexture() { return rockyGroundTexture; }
SDL_Texture* Desert::getCactusSaguaro() { return cactusSaguaroTexture; }
SDL_Texture* Desert::getCactusSaguaro2Arms() { return cactusSaguaro2ArmsTexture; }
SDL_Texture* Desert::getCactusSaguaro3Arms() { return cactusSaguaro3ArmsTexture; }
SDL_Texture* Desert::getCactusPricklyPear() { return cactusPricklyPearTexture; }
SDL_Texture* Desert::getCactusCholla() { return cactusChollaTexture; }
SDL_Texture* Desert::getDesertBushTexture() { return desertBushTexture; }
SDL_Texture* Desert::getDesertGrassTexture() { return desertGrassTexture; }
SDL_Texture* Desert::getPaloVerdeTexture() { return paloVerdeTexture; }
SDL_Texture* Desert::getYuccaTexture() { return yuccaTexture; }
SDL_Texture* Desert::getAgaveTexture() { return agaveTexture; }
SDL_Texture* Desert::getRockSmallTexture() { return rockSmallTexture; }
SDL_Texture* Desert::getRockMediumTexture() { return rockMediumTexture; }
SDL_Texture* Desert::getRockLargeTexture() { return rockLargeTexture; }
SDL_Texture* Desert::getRockFormationTexture() { return rockFormationTexture; }
SDL_Texture* Desert::getAdobeWallTexture() { return adobeWallTexture; }
SDL_Texture* Desert::getAdobeRuinTexture() { return adobeRuinTexture; }
SDL_Texture* Desert::getDesertWellTexture() { return desertWellTexture; }
SDL_Texture* Desert::getCaveEntranceTexture() { return caveEntranceTexture; }
SDL_Texture* Desert::getCampfireSiteTexture() { return campfireSiteTexture; }
SDL_Texture* Desert::getBonesTexture() { return bonesTexture; }
SDL_Texture* Desert::getTumbleweedTexture() { return tumbleweedTexture; }
SDL_Texture* Desert::getDeadTreeTexture() { return deadTreeTexture; }
SDL_Texture* Desert::getScorpionTexture() { return scorpionTexture; }
SDL_Texture* Desert::getSnakeTexture() { return snakeTexture; }
