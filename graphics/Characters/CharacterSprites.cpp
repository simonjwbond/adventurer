#include "CharacterSprites.h"
#include "../Core/PixelUtils.h"
#include <cstring>

CharacterSprites::CharacterSprites() {
    for (auto& dir : playerSprites) {
        for (auto& tex : dir) {
            tex = nullptr;
        }
    }
}

CharacterSprites::~CharacterSprites() {
    cleanup();
}

void CharacterSprites::initialize(SDL_Renderer* ren) {
    renderer = ren;
    createPlayerSprites();
    assembleCharacter();
}

void CharacterSprites::cleanup() {
    for (auto& dir : playerSprites) {
        for (auto& tex : dir) {
            if (tex) SDL_DestroyTexture(tex);
        }
    }
    if (headTexture) SDL_DestroyTexture(headTexture);
    if (bodyTexture) SDL_DestroyTexture(bodyTexture);
    if (legsTexture) SDL_DestroyTexture(legsTexture);
    if (weaponTexture) SDL_DestroyTexture(weaponTexture);
}

void CharacterSprites::createPlayerSprites() {
    const int SIZE = 32;
    
    // Create 8 directions × 3 frames = 24 sprites
    for (int dir = 0; dir < 8; dir++) {
        for (int frame = 0; frame < 3; frame++) {
            Uint8 pixels[SIZE * SIZE * 4];
            memset(pixels, 0, sizeof(pixels));
            
            // Base body color
            int skinR = 255, skinG = 210, skinB = 180;
            int shirtR = 34, shirtG = 139, shirtB = 34;
            int pantsR = 70, pantsG = 70, pantsB = 90;
            
            // Calculate animation offset
            int bounce = (frame == 0) ? 0 : ((frame == 1) ? 2 : 0);
            
            // Head
            int headY = 6 + bounce;
            for (int y = headY; y < headY + 10; y++) {
                for (int x = 11; x < 21; x++) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = skinR;
                    pixels[idx + 1] = skinG;
                    pixels[idx + 2] = skinB;
                    pixels[idx + 3] = 255;
                }
            }
            
            // Hair
            for (int y = headY; y < headY + 4; y++) {
                for (int x = 10; x < 22; x++) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = 60;
                    pixels[idx + 1] = 40;
                    pixels[idx + 2] = 20;
                    pixels[idx + 3] = 255;
                }
            }
            
            // Eyes
            int eyeY = headY + 4;
            int eyeOffset = (dir == 2 || dir == 3) ? -1 : ((dir == 6 || dir == 7) ? 1 : 0);
            pixels[(eyeY * SIZE + 14 + eyeOffset) * 4 + 0] = 0;
            pixels[(eyeY * SIZE + 14 + eyeOffset) * 4 + 1] = 0;
            pixels[(eyeY * SIZE + 14 + eyeOffset) * 4 + 2] = 0;
            pixels[(eyeY * SIZE + 14 + eyeOffset) * 4 + 3] = 255;
            pixels[(eyeY * SIZE + 18 + eyeOffset) * 4 + 0] = 0;
            pixels[(eyeY * SIZE + 18 + eyeOffset) * 4 + 1] = 0;
            pixels[(eyeY * SIZE + 18 + eyeOffset) * 4 + 2] = 0;
            pixels[(eyeY * SIZE + 18 + eyeOffset) * 4 + 3] = 255;
            
            // Body/Shirt
            int bodyY = headY + 10;
            for (int y = bodyY; y < bodyY + 8; y++) {
                for (int x = 10; x < 22; x++) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = shirtR;
                    pixels[idx + 1] = shirtG;
                    pixels[idx + 2] = shirtB;
                    pixels[idx + 3] = 255;
                }
            }
            
            // Arms (animated based on frame)
            int armOffset = (frame % 2 == 0) ? 0 : 2;
            // Left arm
            for (int y = bodyY + 1; y < bodyY + 7; y++) {
                for (int x = 6 - armOffset; x < 10; x++) {
                    if (x >= 0 && x < SIZE) {
                        int idx = (y * SIZE + x) * 4;
                        pixels[idx + 0] = skinR;
                        pixels[idx + 1] = skinG;
                        pixels[idx + 2] = skinB;
                        pixels[idx + 3] = 255;
                    }
                }
            }
            // Right arm
            for (int y = bodyY + 1; y < bodyY + 7; y++) {
                for (int x = 22; x < 26 + armOffset; x++) {
                    if (x >= 0 && x < SIZE) {
                        int idx = (y * SIZE + x) * 4;
                        pixels[idx + 0] = skinR;
                        pixels[idx + 1] = skinG;
                        pixels[idx + 2] = skinB;
                        pixels[idx + 3] = 255;
                    }
                }
            }
            
            // Pants/Legs
            int legY = bodyY + 8;
            for (int y = legY; y < legY + 10 + bounce; y++) {
                // Left leg
                for (int x = 11; x < 16; x++) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = pantsR;
                    pixels[idx + 1] = pantsG;
                    pixels[idx + 2] = pantsB;
                    pixels[idx + 3] = 255;
                }
                // Right leg
                for (int x = 16; x < 21; x++) {
                    int idx = (y * SIZE + x) * 4;
                    pixels[idx + 0] = pantsR;
                    pixels[idx + 1] = pantsG;
                    pixels[idx + 2] = pantsB;
                    pixels[idx + 3] = 255;
                }
            }
            
            // Boots
            int bootY = legY + 10 + bounce;
            for (int x = 10; x < 16; x++) {
                int idx = (bootY * SIZE + x) * 4;
                pixels[idx + 0] = 50;
                pixels[idx + 1] = 30;
                pixels[idx + 2] = 20;
                pixels[idx + 3] = 255;
            }
            for (int x = 16; x < 22; x++) {
                int idx = (bootY * SIZE + x) * 4;
                pixels[idx + 0] = 50;
                pixels[idx + 1] = 30;
                pixels[idx + 2] = 20;
                pixels[idx + 3] = 255;
            }
            
            playerSprites[dir][frame] = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
        }
    }
}

void CharacterSprites::assembleCharacter() {
    const int SIZE = 32;
    
    // Create modular head
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        
        // Head
        for (int y = 8; y < 18; y++) {
            for (int x = 11; x < 21; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 255;
                pixels[idx + 1] = 210;
                pixels[idx + 2] = 180;
                pixels[idx + 3] = 255;
            }
        }
        
        // Hair
        for (int y = 6; y < 10; y++) {
            for (int x = 10; x < 22; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 60;
                pixels[idx + 1] = 40;
                pixels[idx + 2] = 20;
                pixels[idx + 3] = 255;
            }
        }
        
        // Eyes
        for (int x = 14; x < 16; x++) {
            int idx = (12 * SIZE + x) * 4;
            pixels[idx + 0] = 0;
            pixels[idx + 1] = 0;
            pixels[idx + 2] = 0;
            pixels[idx + 3] = 255;
        }
        
        headTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Create modular body
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        
        // Shirt
        for (int y = 18; y < 26; y++) {
            for (int x = 10; x < 22; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 34;
                pixels[idx + 1] = 139;
                pixels[idx + 2] = 34;
                pixels[idx + 3] = 255;
            }
        }
        
        bodyTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Create modular legs
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        
        // Pants
        for (int y = 26; y < 32; y++) {
            for (int x = 11; x < 21; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 70;
                pixels[idx + 1] = 70;
                pixels[idx + 2] = 90;
                pixels[idx + 3] = 255;
            }
        }
        
        legsTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
    
    // Create weapon (sword)
    {
        Uint8 pixels[SIZE * SIZE * 4];
        memset(pixels, 0, sizeof(pixels));
        
        // Blade
        for (int y = 4; y < 24; y++) {
            for (int x = 15; x < 17; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 200;
                pixels[idx + 1] = 200;
                pixels[idx + 2] = 220;
                pixels[idx + 3] = 255;
            }
        }
        
        // Hilt
        for (int y = 24; y < 28; y++) {
            for (int x = 14; x < 18; x++) {
                int idx = (y * SIZE + x) * 4;
                pixels[idx + 0] = 180;
                pixels[idx + 1] = 140;
                pixels[idx + 2] = 60;
                pixels[idx + 3] = 255;
            }
        }
        
        weaponTexture = PixelUtils::createTextureFromPixels(pixels, SIZE, SIZE, renderer);
    }
}

SDL_Texture* CharacterSprites::getPlayerSprite(int direction, int frame) {
    if (direction >= 0 && direction < 8 && frame >= 0 && frame < 3) {
        return playerSprites[direction][frame];
    }
    return playerSprites[0][0];
}

SDL_Texture* CharacterSprites::getHead() {
    return headTexture;
}

SDL_Texture* CharacterSprites::getBody() {
    return bodyTexture;
}

SDL_Texture* CharacterSprites::getLegs() {
    return legsTexture;
}

SDL_Texture* CharacterSprites::getWeapon() {
    return weaponTexture;
}
