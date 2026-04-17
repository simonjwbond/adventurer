#include "Indoor.h"
#include "../Core/PixelUtils.h"
#include <cstring>
#include <cmath>

Indoor::Indoor() {}

Indoor::~Indoor() {
    if (woodFloorTexture) SDL_DestroyTexture(woodFloorTexture);
    if (stoneFloorTexture) SDL_DestroyTexture(stoneFloorTexture);
    if (stoneWallTexture) SDL_DestroyTexture(stoneWallTexture);
    if (woodWallTexture) SDL_DestroyTexture(woodWallTexture);
    if (roofTexture) SDL_DestroyTexture(roofTexture);
    if (wallCornerTopLeft) SDL_DestroyTexture(wallCornerTopLeft);
    if (wallCornerTopRight) SDL_DestroyTexture(wallCornerTopRight);
    if (wallCornerBottomLeft) SDL_DestroyTexture(wallCornerBottomLeft);
    if (wallCornerBottomRight) SDL_DestroyTexture(wallCornerBottomRight);
    if (wallEdgeHorizontal) SDL_DestroyTexture(wallEdgeHorizontal);
    if (wallEdgeVertical) SDL_DestroyTexture(wallEdgeVertical);
}

void Indoor::initialize(SDL_Renderer* ren) {
    renderer = ren;
    createWoodFloorTexture();
    createStoneFloorTexture();
    createStoneWallTexture();
    createWoodWallTexture();
    createRoofTexture();
    createWallCornerTopLeft();
    createWallCornerTopRight();
    createWallCornerBottomLeft();
    createWallCornerBottomRight();
    createWallEdgeHorizontal();
    createWallEdgeVertical();
    initialized = true;
}

void Indoor::createWoodFloorTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            
            // Wood grain pattern
            int grain = (x / 4) % 8;
            int noise = ((x + y) % 7 - 3) * 8;
            
            int r = 139 + grain * 5 + noise;
            int g = 69 + grain * 3 + noise / 2;
            int b = 42 + grain * 2 + noise / 3;
            
            pixels[idx + 0] = std::max(0, std::min(255, r));
            pixels[idx + 1] = std::max(0, std::min(255, g));
            pixels[idx + 2] = std::max(0, std::min(255, b));
            pixels[idx + 3] = 255;
        }
    }
    
    woodFloorTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createStoneFloorTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            
            // Stone tile pattern with grout lines
            int tileX = x / 8;
            int tileY = y / 8;
            
            // Grout lines
            if (x % 8 == 0 || y % 8 == 0) {
                pixels[idx + 0] = 80;
                pixels[idx + 1] = 80;
                pixels[idx + 2] = 80;
                pixels[idx + 3] = 255;
            } else {
                int noise = ((x + y) % 11 - 5) * 6;
                int val = 150 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 5));
                pixels[idx + 2] = std::max(0, std::min(255, val - 10));
                pixels[idx + 3] = 255;
            }
        }
    }
    
    stoneFloorTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createStoneWallTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            
            // Stone block pattern
            int blockX = x / 16;
            int blockY = y / 8;
            int offset = (blockY % 2) * 8;
            
            // Mortar lines
            if (y % 8 == 0 || (x + offset) % 16 == 0) {
                pixels[idx + 0] = 100;
                pixels[idx + 1] = 100;
                pixels[idx + 2] = 100;
                pixels[idx + 3] = 255;
            } else {
                int noise = ((x * 3 + y * 5) % 13 - 6) * 10;
                int val = 120 + noise;
                pixels[idx + 0] = std::max(0, std::min(255, val));
                pixels[idx + 1] = std::max(0, std::min(255, val - 8));
                pixels[idx + 2] = std::max(0, std::min(255, val - 15));
                pixels[idx + 3] = 255;
            }
        }
    }
    
    stoneWallTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createWoodWallTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            
            // Vertical wood planks
            int plank = x / 8;
            int noise = ((y) % 9 - 4) * 6;
            
            // Plank edges
            if (x % 8 == 0) {
                pixels[idx + 0] = 80;
                pixels[idx + 1] = 50;
                pixels[idx + 2] = 30;
                pixels[idx + 3] = 255;
            } else {
                int r = 139 + noise;
                int g = 89 + noise / 2;
                int b = 54 + noise / 3;
                
                pixels[idx + 0] = std::max(0, std::min(255, r));
                pixels[idx + 1] = std::max(0, std::min(255, g));
                pixels[idx + 2] = std::max(0, std::min(255, b));
                pixels[idx + 3] = 255;
            }
        }
    }
    
    woodWallTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createRoofTexture() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            
            // Roof shingles/tiles pattern
            int row = y / 4;
            int col = (x + (row % 2) * 4) / 8;
            
            int noise = ((x + y) % 7 - 3) * 8;
            int r = 139 + noise;
            int g = 69 + noise / 2;
            int b = 42 + noise / 3;
            
            // Darker grout lines
            if (y % 4 == 0) {
                r = 100;
                g = 50;
                b = 30;
            }
            
            pixels[idx + 0] = std::max(0, std::min(255, r));
            pixels[idx + 1] = std::max(0, std::min(255, g));
            pixels[idx + 2] = std::max(0, std::min(255, b));
            pixels[idx + 3] = 255;
        }
    }
    
    roofTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

SDL_Texture* Indoor::getWoodFloorTexture() {
    return woodFloorTexture;
}

SDL_Texture* Indoor::getStoneFloorTexture() {
    return stoneFloorTexture;
}

SDL_Texture* Indoor::getStoneWallTexture() {
    return stoneWallTexture;
}

SDL_Texture* Indoor::getWoodWallTexture() {
    return woodWallTexture;
}

SDL_Texture* Indoor::getRoofTexture() {
    return roofTexture;
}

SDL_Texture* Indoor::getWallCornerTopLeft() {
    return wallCornerTopLeft;
}

SDL_Texture* Indoor::getWallCornerTopRight() {
    return wallCornerTopRight;
}

SDL_Texture* Indoor::getWallCornerBottomLeft() {
    return wallCornerBottomLeft;
}

SDL_Texture* Indoor::getWallCornerBottomRight() {
    return wallCornerBottomRight;
}

SDL_Texture* Indoor::getWallEdgeHorizontal() {
    return wallEdgeHorizontal;
}

SDL_Texture* Indoor::getWallEdgeVertical() {
    return wallEdgeVertical;
}

void Indoor::createWallCornerTopLeft() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Top-left corner - L-shape
    for (int y = 0; y < 16; y++) {
        for (int x = 0; x < 8; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < 16; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    
    wallCornerTopLeft = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createWallCornerTopRight() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Top-right corner - reverse L-shape
    for (int y = 0; y < 16; y++) {
        for (int x = 24; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    for (int y = 0; y < 8; y++) {
        for (int x = 16; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    
    wallCornerTopRight = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createWallCornerBottomLeft() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Bottom-left corner - reverse L-shape
    for (int y = 16; y < SIZE; y++) {
        for (int x = 0; x < 8; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    for (int y = 24; y < SIZE; y++) {
        for (int x = 0; x < 16; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    
    wallCornerBottomLeft = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createWallCornerBottomRight() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Bottom-right corner - L-shape
    for (int y = 16; y < SIZE; y++) {
        for (int x = 24; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    for (int y = 24; y < SIZE; y++) {
        for (int x = 16; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    
    wallCornerBottomRight = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createWallEdgeHorizontal() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Horizontal edge - top or bottom
    for (int y = 0; y < 8; y++) {
        for (int x = 0; x < SIZE; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    
    wallEdgeHorizontal = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}

void Indoor::createWallEdgeVertical() {
    const int SIZE = 32;
    Uint8 pixels[SIZE * SIZE * 4];
    memset(pixels, 0, sizeof(pixels));
    
    // Vertical edge - left or right
    for (int y = 0; y < SIZE; y++) {
        for (int x = 0; x < 8; x++) {
            int idx = (y * SIZE + x) * 4;
            int noise = ((x + y) % 5 - 2) * 10;
            pixels[idx + 0] = std::max(0, std::min(255, 100 + noise));
            pixels[idx + 1] = std::max(0, std::min(255, 80 + noise / 2));
            pixels[idx + 2] = std::max(0, std::min(255, 60 + noise / 3));
            pixels[idx + 3] = 255;
        }
    }
    
    wallEdgeVertical = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
}
