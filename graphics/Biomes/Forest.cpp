#include "Forest.h"
#include "../Core/PixelUtils.h"
#include <cstring>
#include <cmath>

Forest::Forest() {
    for (int i = 0; i < 9; i++) {
        treeBorderVariations[i] = nullptr;
    }
}

Forest::~Forest() {
    if (treeTexture) SDL_DestroyTexture(treeTexture);
    for (int i = 0; i < 9; i++) {
        if (treeBorderVariations[i]) SDL_DestroyTexture(treeBorderVariations[i]);
    }
    if (bushTexture) SDL_DestroyTexture(bushTexture);
    if (fenceTexture) SDL_DestroyTexture(fenceTexture);
}

void Forest::initialize(SDL_Renderer* ren) {
    renderer = ren;
    createTreeTexture();
    createTreeBorderVariations();
    createBushTexture();
    createFenceTexture();
    initialized = true;
}

void Forest::createTreeTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Trunk
    for (int y = 12; y < SIZE; y++) {
        for (int x = 14; x < 18; x++) {
            int idx = (y * SIZE + x) * 4;
            pixels[idx + 0] = 101;
            pixels[idx + 1] = 67;
            pixels[idx + 2] = 34;
            pixels[idx + 3] = 255;
        }
    }
    
    // Foliage - rounded canopy
    for (int y = 4; y < 18; y++) {
        for (int x = 6; x < 26; x++) {
            int dx = x - 16;
            int dy = y - 10;
            int dist = dx * dx + dy * dy;
            
            if (dist < 64) {  // Main canopy
                int idx = (y * SIZE + x) * 4;
                int noise = ((x + y) % 11 - 5) * 8;
                int g = 34 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, 34 + noise / 2));
                pixels[idx + 1] = std::max(0, std::min(255, g));
                pixels[idx + 2] = std::max(0, std::min(255, 34 + noise / 3));
                pixels[idx + 3] = 255;
            }
        }
    }
    
    treeTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Forest::createTreeBorderVariations() {
    const int TILE_SIZE = 96;
    
    for (int v = 0; v < 9; v++) {
        Uint8 pixels[TILE_SIZE * TILE_SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        
        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                int idx = (y * TILE_SIZE + x) * 4;
                
                // Create forest floor with variation
                int pattern = (v * 13 + x * 5 + y * 7) % 100;
                int noise = ((x + y + v * 7) % 11 - 5) * 6;
                
                int r = 101 + noise;
                int g = 67 + pattern % 20 + noise * 2;
                int b = 34 + (pattern % 15) + noise;
                
                pixels[idx + 0] = std::max(0, std::min(255, r));
                pixels[idx + 1] = std::max(0, std::min(255, g));
                pixels[idx + 2] = std::max(0, std::min(255, b));
                pixels[idx + 3] = 255;
            }
        }
        
        treeBorderVariations[v] = PixelUtils::createTextureFromPixels(pixels, TILE_SIZE, TILE_SIZE, renderer);
    }
}

void Forest::createBushTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Create bush shape
    for (int y = 12; y < SIZE; y++) {
        for (int x = 8; x < 24; x++) {
            int dx = x - 16;
            int dy = y - 18;
            int dist = dx * dx + dy * dy;
            
            if (dist < 64) {
                int idx = (y * SIZE + x) * 4;
                int noise = ((x + y) % 9 - 4) * 10;
                int g = 50 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, 50 + noise / 2));
                pixels[idx + 1] = std::max(0, std::min(255, g));
                pixels[idx + 2] = std::max(0, std::min(255, 50 + noise / 3));
                pixels[idx + 3] = 255;
            }
        }
    }
    
    bushTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Forest::createFenceTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Fence post
    for (int y = 8; y < SIZE; y++) {
        for (int x = 14; x < 18; x++) {
            int idx = (y * SIZE + x) * 4;
            pixels[idx + 0] = 101;
            pixels[idx + 1] = 67;
            pixels[idx + 2] = 34;
            pixels[idx + 3] = 255;
        }
    }
    
    // Top rail
    for (int y = 8; y < 12; y++) {
        for (int x = 4; x < 28; x++) {
            int idx = (y * SIZE + x) * 4;
            pixels[idx + 0] = 101;
            pixels[idx + 1] = 67;
            pixels[idx + 2] = 34;
            pixels[idx + 3] = 255;
        }
    }
    
    // Bottom rail
    for (int y = 20; y < 24; y++) {
        for (int x = 4; x < 28; x++) {
            int idx = (y * SIZE + x) * 4;
            pixels[idx + 0] = 101;
            pixels[idx + 1] = 67;
            pixels[idx + 2] = 34;
            pixels[idx + 3] = 255;
        }
    }
    
    fenceTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

SDL_Texture* Forest::getTreeTexture() {
    return treeTexture;
}

SDL_Texture* Forest::getTreeBorderVariation(int index) {
    if (index >= 0 && index < 9) {
        return treeBorderVariations[index];
    }
    return treeBorderVariations[0];
}

SDL_Texture* Forest::getBushTexture() {
    return bushTexture;
}

SDL_Texture* Forest::getFenceTexture() {
    return fenceTexture;
}
