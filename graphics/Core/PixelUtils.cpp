#include "PixelUtils.h"
#include <cmath>
#include <algorithm>

SDL_Texture* PixelUtils::createTextureFromPixels(Uint8* pixels, int width, int height, SDL_Renderer* renderer) {
    if (pixels == nullptr || width <= 0 || height <= 0 || renderer == nullptr) {
        return nullptr;
    }
    
    SDL_Texture* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA32,
                                              SDL_TEXTUREACCESS_STATIC, width, height);
    if (texture == nullptr) {
        return nullptr;
    }
    
    SDL_UpdateTexture(texture, nullptr, pixels, width * 4);
    return texture;
}

SDL_Texture* PixelUtils::createPaletteTexture(Uint8* indexedData, Uint32* palette, int paletteSize,
                                               int width, int height, SDL_Renderer* renderer) {
    if (indexedData == nullptr || palette == nullptr || width <= 0 || height <= 0 || renderer == nullptr) {
        return nullptr;
    }
    
    // Create RGBA buffer
    int bufferSize = width * height * 4;
    Uint8* rgbaBuffer = new Uint8[bufferSize];
    
    // Convert indexed to RGBA
    for (int i = 0; i < width * height; i++) {
        int paletteIndex = indexedData[i];
        if (paletteIndex >= 0 && paletteIndex < paletteSize) {
            Uint32 color = palette[paletteIndex];
            int pixelIdx = i * 4;
            rgbaBuffer[pixelIdx + 0] = (color >> 16) & 0xFF;  // R
            rgbaBuffer[pixelIdx + 1] = (color >> 8) & 0xFF;   // G
            rgbaBuffer[pixelIdx + 2] = color & 0xFF;          // B
            rgbaBuffer[pixelIdx + 3] = (color >> 24) & 0xFF;  // A
        } else {
            // Transparent
            int pixelIdx = i * 4;
            rgbaBuffer[pixelIdx + 0] = 0;
            rgbaBuffer[pixelIdx + 1] = 0;
            rgbaBuffer[pixelIdx + 2] = 0;
            rgbaBuffer[pixelIdx + 3] = 0;
        }
    }
    
    SDL_Texture* texture = createTextureFromPixels(rgbaBuffer, width, height, renderer);
    delete[] rgbaBuffer;
    
    return texture;
}

void PixelUtils::drawRect(Uint8* pixels, int width, int height,
                          int x, int y, int w, int h, Uint32 color) {
    if (pixels == nullptr || width <= 0 || height <= 0) return;
    
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    int a = (color >> 24) & 0xFF;
    
    // Clamp coordinates
    x = std::max(0, std::min(x, width - 1));
    y = std::max(0, std::min(y, height - 1));
    w = std::min(w, width - x);
    h = std::min(h, height - y);
    
    for (int dy = 0; dy < h; dy++) {
        for (int dx = 0; dx < w; dx++) {
            int idx = ((y + dy) * width + (x + dx)) * 4;
            pixels[idx + 0] = r;
            pixels[idx + 1] = g;
            pixels[idx + 2] = b;
            pixels[idx + 3] = a;
        }
    }
}

void PixelUtils::drawCircle(Uint8* pixels, int width, int height,
                            int cx, int cy, int radius, Uint32 color) {
    if (pixels == nullptr || width <= 0 || height <= 0) return;
    
    int r = (color >> 16) & 0xFF;
    int g = (color >> 8) & 0xFF;
    int b = color & 0xFF;
    int a = (color >> 24) & 0xFF;
    
    int rSquared = radius * radius;
    
    for (int y = cy - radius; y <= cy + radius; y++) {
        for (int x = cx - radius; x <= cx + radius; x++) {
            int dx = x - cx;
            int dy = y - cy;
            
            if (dx * dx + dy * dy <= rSquared) {
                if (x >= 0 && x < width && y >= 0 && y < height) {
                    int idx = (y * width + x) * 4;
                    pixels[idx + 0] = r;
                    pixels[idx + 1] = g;
                    pixels[idx + 2] = b;
                    pixels[idx + 3] = a;
                }
            }
        }
    }
}

Uint32 PixelUtils::blendColors(Uint32 bg, Uint32 fg, int alpha) {
    int bgR = (bg >> 16) & 0xFF;
    int bgG = (bg >> 8) & 0xFF;
    int bgB = bg & 0xFF;
    
    int fgR = (fg >> 16) & 0xFF;
    int fgG = (fg >> 8) & 0xFF;
    int fgB = fg & 0xFF;
    
    int blendedR = (fgR * alpha + bgR * (255 - alpha)) / 255;
    int blendedG = (fgG * alpha + bgG * (255 - alpha)) / 255;
    int blendedB = (fgB * alpha + bgB * (255 - alpha)) / 255;
    
    return packColor(blendedR, blendedG, blendedB);
}

Uint32 PixelUtils::packColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a) {
    return (static_cast<Uint32>(a) << 24) |
           (static_cast<Uint32>(r) << 16) |
           (static_cast<Uint32>(g) << 8) |
           static_cast<Uint32>(b);
}

void PixelUtils::unpackColor(Uint32 color, Uint8& r, Uint8& g, Uint8& b, Uint8& a) {
    r = (color >> 16) & 0xFF;
    g = (color >> 8) & 0xFF;
    b = color & 0xFF;
    a = (color >> 24) & 0xFF;
}
