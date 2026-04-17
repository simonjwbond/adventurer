#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

#include <SDL2/SDL.h>
#include <unordered_map>
#include <string>
#include <functional>

/**
 * TextureCache - Manages SDL texture lifecycle and caching
 * 
 * Responsibilities:
 * - Create and cache SDL_Texture objects by name
 * - Prevent duplicate texture creation
 * - Cleanup all textures on destruction
 * - Provide texture retrieval with automatic creation
 */
class TextureCache {
public:
    TextureCache(SDL_Renderer* renderer);
    ~TextureCache();
    
    /**
     * Get a cached texture, creating it if needed
     * @param key Unique identifier for the texture
     * @param creator Function that creates the texture if not cached
     * @return SDL_Texture pointer (cached or newly created)
     */
    SDL_Texture* getTexture(const std::string& key, std::function<SDL_Texture*()> creator);
    
    /**
     * Get a cached texture (returns nullptr if not found)
     */
    SDL_Texture* getTexture(const std::string& key);
    
    /**
     * Check if a texture is cached
     */
    bool hasTexture(const std::string& key) const;
    
    /**
     * Remove a cached texture
     */
    void removeTexture(const std::string& key);
    
    /**
     * Clear all cached textures
     */
    void clear();
    
    /**
     * Get renderer reference
     */
    SDL_Renderer* getRenderer() const { return renderer; }

private:
    SDL_Renderer* renderer;
    std::unordered_map<std::string, SDL_Texture*> textureCache;
};

#endif // TEXTURE_CACHE_H
