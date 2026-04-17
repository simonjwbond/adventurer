#include "Grassland.h"
#include "../Core/PixelUtils.h"
#include <cstring>
#include <cmath>

Grassland::Grassland() {
    grassVariation.fill(nullptr);
    iceVariation.fill(nullptr);
}

Grassland::~Grassland() {
    if (grassTexture) SDL_DestroyTexture(grassTexture);
    for (auto& tex : grassVariation) {
        if (tex) SDL_DestroyTexture(tex);
    }
    for (auto& tex : iceVariation) {
        if (tex) SDL_DestroyTexture(tex);
    }
    if (snowTexture) SDL_DestroyTexture(snowTexture);
}

void Grassland::initialize(SDL_Renderer* ren) {
    renderer = ren;
    createGrassTexture();
    createGrassVariations();
    createIceVariations();
    createSnowTexture();
    initialized = true;
}

void Grassland::createGrassTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Green grass base with noise
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            
            // Base green with variation
            int noise = (x + y) % 7 - 3;
            int g = 139 + noise * 8;
            int b = 42 + noise * 4;
            
            pixels[idx + 0] = 34;   // R
            pixels[idx + 1] = std::max(0, std::min(255, g));  // G
            pixels[idx + 2] = std::max(0, std::min(255, b));  // B
            pixels[idx + 3] = 255;  // A
        }
    }
    
    grassTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Grassland::createGrassVariations() {
    // Create 9 grass variations for seamless 3x3 tiling (96x96 each)
    const int TILE_SIZE = 96;
    
    for (int v = 0; v < 9; v++) {
        Uint8 pixels[TILE_SIZE * TILE_SIZE * 4];
        
        for (int y = 0; y < TILE_SIZE; y++) {
            for (int x = 0; x < TILE_SIZE; x++) {
                int idx = (y * TILE_SIZE + x) * 4;
                
                // Add variation based on pattern
                int pattern = (v * 17 + x * 3 + y * 5) % 100;
                int noise = (x + y + v * 7) % 11 - 5;
                
                int r = 34 + noise * 3;
                int g = 139 + pattern % 20 + noise * 5;
                int b = 42 + (pattern % 15) + noise * 3;
                
                pixels[idx + 0] = std::max(0, std::min(255, r));
                pixels[idx + 1] = std::max(0, std::min(255, g));
                pixels[idx + 2] = std::max(0, std::min(255, b));
                pixels[idx + 3] = 255;
            }
        }
        
        grassVariation[v] = PixelUtils::createTextureFromPixels(pixels, TILE_SIZE, TILE_SIZE, renderer);
    }
}

void Grassland::createIceVariations() {
    // Create 9 ice variations (32x32 each)
    const int SIZE = 32;
    
    for (int v = 0; v < 9; v++) {
        Uint8 pixels[SIZE * SIZE * 4];
        
        for (int y = 0; y < SIZE; y++) {
            for (int x = 0; x < SIZE; x++) {
                int idx = (y * SIZE + x) * 4;
                
                // Ice has blue/cyan tones with crystalline patterns
                int pattern = (v * 13 + x * 5 + y * 7) % 50;
                int noise = ((x * y) % 17 - 8) * 2;
                
                int r = 173 + noise + (pattern % 30);
                int g = 216 + (pattern % 20) - std::abs(noise);
                int b = 240 + (pattern % 15);
                
                pixels[idx + 0] = std::max(0, std::min(255, r));
                pixels[idx + 1] = std::max(0, std::min(255, g));
                pixels[idx + 2] = std::max(0, std::min(255, b));
                pixels[idx + 3] = 255;
            }
        }
        
        iceVariation[v] = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

void Grassland::createSnowTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            
            // Snow is white with subtle variation
            int noise = ((x + y) % 13 - 6) * 5;
            int val = 245 + noise;
            
            pixels[idx + 0] = std::max(0, std::min(255, val));
            pixels[idx + 1] = std::max(0, std::min(255, val));
            pixels[idx + 2] = std::max(0, std::min(255, val));
            pixels[idx + 3] = 255;
        }
    }
    
    snowTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

SDL_Texture* Grassland::getGrassTexture() {
    return grassTexture;
}

SDL_Texture* Grassland::getGrassVariation(int index) {
    if (index >= 0 && index < 9) {
        return grassVariation[index];
    }
    return grassTexture;
}

SDL_Texture* Grassland::getIceVariation(int index) {
    if (index >= 0 && index < 9) {
        return iceVariation[index];
    }
    return iceVariation[0];
}

SDL_Texture* Grassland::getSnowTexture() {
    return snowTexture;
}
