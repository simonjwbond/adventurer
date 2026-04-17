#ifndef PIXEL_UTILS_H
#define PIXEL_UTILS_H

#include <SDL2/SDL.h>
#include <cstdint>

/**
 * PixelUtils - Shared utilities for pixel manipulation
 * 
 * Provides common functions for:
 * - Creating textures from pixel arrays
 * - Drawing primitives (rectangles, circles)
 * - Color manipulation
 * - Palette-based rendering
 */
class PixelUtils {
public:
    /**
     * Create an SDL_Texture from a pixel array
     * @param pixels Array of RGBA pixels (Uint8, 4 bytes per pixel)
     * @param width Texture width in pixels
     * @param height Texture height in pixels
     * @param renderer SDL renderer to create texture with
     * @return SDL_Texture pointer or nullptr on failure
     */
    static SDL_Texture* createTextureFromPixels(Uint8* pixels, int width, int height, SDL_Renderer* renderer);
    
    /**
     * Create a texture from indexed palette data
     * @param indexedData Array of palette indices (1 byte per pixel)
     * @param palette Array of RGBA colors
     * @param paletteSize Number of colors in palette
     * @param width Texture width
     * @param height Texture height
     * @param renderer SDL renderer
     * @return SDL_Texture pointer or nullptr on failure
     */
    static SDL_Texture* createPaletteTexture(Uint8* indexedData, Uint32* palette, int paletteSize, 
                                             int width, int height, SDL_Renderer* renderer);
    
    /**
     * Draw a filled rectangle on a pixel buffer
     * @param pixels Pixel buffer (RGBA, 4 bytes per pixel)
     * @param width Buffer width
     * @param height Buffer height
     * @param x Rectangle x position
     * @param y Rectangle y position
     * @param w Rectangle width
     * @param h Rectangle height
     * @param color RGBA color value
     */
    static void drawRect(Uint8* pixels, int width, int height, 
                         int x, int y, int w, int h, Uint32 color);
    
    /**
     * Draw a filled circle on a pixel buffer
     * @param pixels Pixel buffer (RGBA, 4 bytes per pixel)
     * @param width Buffer width
     * @param height Buffer height
     * @param cx Circle center x
     * @param cy Circle center y
     * @param radius Circle radius
     * @param color RGBA color value
     */
    static void drawCircle(Uint8* pixels, int width, int height,
                           int cx, int cy, int radius, Uint32 color);
    
    /**
     * Blend two colors with alpha
     * @param bg Background color (RGBA)
     * @param fg Foreground color (RGBA)
     * @param alpha Alpha value (0-255)
     * @return Blended color
     */
    static Uint32 blendColors(Uint32 bg, Uint32 fg, int alpha);
    
    /**
     * Convert RGBA color components to packed Uint32
     */
    static Uint32 packColor(Uint8 r, Uint8 g, Uint8 b, Uint8 a = 255);
    
    /**
     * Extract color components from packed Uint32
     */
    static void unpackColor(Uint32 color, Uint8& r, Uint8& g, Uint8& b, Uint8& a);
};

#endif // PIXEL_UTILS_H
