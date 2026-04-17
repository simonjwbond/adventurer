#ifndef BIOME_BASE_H
#define BIOME_BASE_H

#include <SDL2/SDL.h>
#include <string>

/**
 * BiomeBase - Abstract base class for all biome texture generators
 * 
 * Each biome (Grassland, Desert, Forest, etc.) inherits from this class
 * and implements its own texture generation methods.
 */
class BiomeBase {
public:
    BiomeBase() : initialized(false) {}
    virtual ~BiomeBase() {}
    
    /**
     * Initialize the biome - create all textures
     * @param renderer SDL renderer to create textures with
     */
    virtual void initialize(SDL_Renderer* renderer) = 0;
    
    /**
     * Get biome name
     */
    virtual std::string getName() const = 0;
    
    /**
     * Check if biome is initialized
     */
    bool isInitialized() const { return initialized; }
    
protected:
    bool initialized;
    SDL_Renderer* renderer;
};

#endif // BIOME_BASE_H
