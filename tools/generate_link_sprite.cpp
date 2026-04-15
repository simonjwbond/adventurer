/**
 * Link-Style Sprite Generator (Facing Down - Standing Still)
 * 
 * This script generates a 24x28 pixel sprite in the style of A Link to the Past
 * Uses indexed color palette for easy swapping and 16-bit aesthetic
 * 
 * Palette:
 *   0 = Transparent (black)
 *   1 = Dark Green (hat outline, boots)
 *   2 = Medium Green (hat, tunic main)
 *   3 = Light Green (hat highlight)
 *   4 = Peach/Skin tone (face)
 *   5 = Brown (hair peeking out)
 *   6 = White (eyes)
 *   7 = Black (pupils)
 *   8 = Tan/Brown (belt, boots detail)
 */

#include <SDL2/SDL.h>
#include <cstdio>
#include <cstdlib>

// Sprite dimensions matching player constants
const int SPRITE_WIDTH = 24;
const int SPRITE_HEIGHT = 28;

// Indexed sprite data - 1D array representing 24x28 grid
// Each number is a palette index (0-8)
int playerSpriteDown[] = {
    // Row 0-3: Hat top (pointed green hat)
    0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,
    0,0,0,0,0,1,2,2,2,1,0,0,0,1,2,2,2,1,0,0,0,0,0,0,
    0,0,0,0,1,3,2,2,2,3,1,0,1,3,2,2,2,3,1,0,0,0,0,0,
    
    // Row 4-7: Hat brim and face top
    0,0,0,0,1,2,2,2,2,2,1,0,1,2,2,2,2,2,1,0,0,0,0,0,
    0,0,0,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,1,0,0,0,0,
    0,0,0,1,2,2,2,2,2,2,2,1,1,2,2,2,2,2,2,1,0,0,0,0,
    0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
    
    // Row 8-11: Face (skin tone with hair)
    0,0,1,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,1,0,0,
    0,1,5,5,4,4,6,7,7,6,4,4,4,4,6,7,7,6,4,4,5,5,1,0,
    0,1,5,5,4,4,6,7,7,6,4,4,4,4,6,7,7,6,4,4,5,5,1,0,
    0,0,1,5,5,4,4,4,4,4,4,4,4,4,4,4,4,4,4,5,5,1,0,0,
    
    // Row 12-15: Neck and shoulders (tunic starts)
    0,0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,0,
    0,0,1,2,2,2,2,2,2,2,2,4,4,2,2,2,2,2,2,2,2,1,0,0,
    0,1,2,2,2,8,8,2,2,2,4,4,4,4,2,2,2,8,8,2,2,2,1,0,
    0,1,2,2,2,8,8,2,2,4,4,4,4,4,4,2,2,8,8,2,2,2,1,0,
    
    // Row 16-19: Torso (tunic with belt)
    0,1,2,2,2,2,2,2,2,4,4,4,4,4,4,2,2,2,2,2,2,1,0,0,
    0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
    0,1,2,2,2,2,8,8,8,8,8,8,8,8,8,8,8,2,2,2,2,1,0,0,
    0,1,2,2,2,2,8,8,8,8,8,8,8,8,8,8,8,2,2,2,2,1,0,0,
    
    // Row 20-23: Legs and boots
    0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
    0,0,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,0,0,
    0,0,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,0,0,
    0,0,0,1,1,1,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,0,0,0,
    
    // Row 24-27: Boots (dark green/brown)
    0,0,0,0,1,1,1,8,8,8,8,8,8,8,8,8,8,1,1,1,0,0,0,0,
    0,0,0,0,1,1,1,8,8,8,8,8,8,8,8,8,8,1,1,1,0,0,0,0,
    0,0,0,0,0,1,1,1,1,1,1,2,2,1,1,1,1,1,1,0,0,0,0,0,
    0,0,0,0,0,0,1,1,1,1,1,2,2,1,1,1,1,1,0,0,0,0,0,0
};

// Palette colors (RGBA format)
Uint32 palette[] = {
    0x00000000,  // 0: Transparent
    0xFF1B4D1E,  // 1: Dark Green (outline, boots base)
    0xFF2E8B57,  // 2: Medium Green (hat, tunic)
    0xFF3CB371,  // 3: Light Green (highlight)
    0xFFFFDAB9,  // 4: Peach/Skin tone
    0xFF8B4513,  // 5: Brown (hair)
    0xFFFFFFFF,  // 6: White (eyes)
    0xFF000000,  // 7: Black (pupils)
    0xFFA0522D   // 8: Tan/Brown (belt, boot detail)
};

int main() {
    printf("Link-Style Sprite Generator\n");
    printf("===========================\n\n");
    
    // Verify sprite dimensions
    int expectedSize = SPRITE_WIDTH * SPRITE_HEIGHT;
    int actualSize = sizeof(playerSpriteDown) / sizeof(int);
    
    if (actualSize != expectedSize) {
        printf("ERROR: Sprite array size mismatch!\n");
        printf("Expected: %d pixels, Got: %d pixels\n", expectedSize, actualSize);
        return 1;
    }
    
    printf("Sprite dimensions: %dx%d (%d pixels)\n", SPRITE_WIDTH, SPRITE_HEIGHT, expectedSize);
    printf("Palette entries: %ld\n", (long)(sizeof(palette) / sizeof(Uint32)));
    printf("\nSample pixel data (first 4 rows):\n");
    
    for (int y = 0; y < 4; y++) {
        printf("Row %2d: ", y);
        for (int x = 0; x < SPRITE_WIDTH; x++) {
            int idx = y * SPRITE_WIDTH + x;
            printf("%d", playerSpriteDown[idx]);
        }
        printf("\n");
    }
    
    printf("\nPalette colors:\n");
    for (int i = 0; i < 9; i++) {
        Uint32 color = palette[i];
        unsigned char r = (color >> 16) & 0xFF;
        unsigned char g = (color >> 8) & 0xFF;
        unsigned char b = color & 0xFF;
        printf("  %d: #%02X%02X%02X\n", i, r, g, b);
    }
    
    // Create SDL surface to verify the sprite renders correctly
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL initialization failed: %s\n", SDL_GetError());
        return 1;
    }
    
    SDL_Window* window = SDL_CreateWindow(
        "Link Sprite Preview",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        SPRITE_WIDTH * 8, SPRITE_HEIGHT * 8,
        SDL_WINDOW_SHOWN
    );
    
    if (!window) {
        printf("Window creation failed: %s\n", SDL_GetError());
        SDL_Quit();
        return 1;
    }
    
    SDL_Renderer* renderer = SDL_CreateRenderer(
        window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
    );
    
    if (!renderer) {
        printf("Renderer creation failed: %s\n", SDL_GetError());
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Create surface for sprite
    SDL_Surface* surface = SDL_CreateRGBSurface(0, SPRITE_WIDTH, SPRITE_HEIGHT, 32,
                                                0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000);
    
    if (!surface) {
        printf("Surface creation failed: %s\n", SDL_GetError());
        SDL_DestroyRenderer(renderer);
        SDL_DestroyWindow(window);
        SDL_Quit();
        return 1;
    }
    
    // Lock surface and fill with indexed palette
    SDL_LockSurface(surface);
    Uint32* pixels = (Uint32*)surface->pixels;
    int pitch = surface->pitch / 4;
    
    for (int y = 0; y < SPRITE_HEIGHT; y++) {
        for (int x = 0; x < SPRITE_WIDTH; x++) {
            int idx = y * SPRITE_WIDTH + x;
            int paletteIdx = playerSpriteDown[idx];
            
            // Only draw non-transparent pixels
            if (paletteIdx > 0 && paletteIdx < 9) {
                pixels[y * pitch + x] = palette[paletteIdx];
            }
        }
    }
    
    SDL_UnlockSurface(surface);
    
    // Create texture and render
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_FreeSurface(surface);
    
    // Render to screen (scaled up for visibility)
    int scale = 8;
    SDL_Rect srcRect = {0, 0, SPRITE_WIDTH, SPRITE_HEIGHT};
    SDL_Rect dstRect = {(1536 - SPRITE_WIDTH * scale) / 2, 
                        (720 - SPRITE_HEIGHT * scale) / 2,
                        SPRITE_WIDTH * scale, SPRITE_HEIGHT * scale};
    
    bool running = true;
    while (running) {
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT || 
                (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE)) {
                running = false;
            }
        }
        
        // Clear and render
        SDL_SetRenderDrawColor(renderer, 0x1A, 0x2E, 0x3F, 255);
        SDL_RenderClear(renderer);
        
        SDL_RenderCopy(renderer, texture, &srcRect, &dstRect);
        
        SDL_RenderPresent(renderer);
        SDL_Delay(16); // ~60 FPS
    }
    
    // Cleanup
    SDL_DestroyTexture(texture);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();
    
    printf("\nSprite preview window closed successfully!\n");
    printf("The sprite data is ready to be integrated into ProceduralArt.cpp\n");
    
    return 0;
}
