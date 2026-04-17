#include "TextureCache.h"
#include <cstdio>

TextureCache::TextureCache(SDL_Renderer* ren) : renderer(ren) {
}

TextureCache::~TextureCache() {
    clear();
}

SDL_Texture* TextureCache::getTexture(const std::string& key, std::function<SDL_Texture*()> creator) {
    auto it = textureCache.find(key);
    if (it != textureCache.end() && it->second != nullptr) {
        return it->second;
    }
    
    // Create new texture
    SDL_Texture* newTexture = creator();
    if (newTexture != nullptr) {
        textureCache[key] = newTexture;
    }
    return newTexture;
}

SDL_Texture* TextureCache::getTexture(const std::string& key) {
    auto it = textureCache.find(key);
    if (it != textureCache.end()) {
        return it->second;
    }
    return nullptr;
}

bool TextureCache::hasTexture(const std::string& key) const {
    auto it = textureCache.find(key);
    return it != textureCache.end() && it->second != nullptr;
}

void TextureCache::removeTexture(const std::string& key) {
    auto it = textureCache.find(key);
    if (it != textureCache.end() && it->second != nullptr) {
        SDL_DestroyTexture(it->second);
        textureCache.erase(it);
    }
}

void TextureCache::clear() {
    for (auto& pair : textureCache) {
        if (pair.second != nullptr) {
            SDL_DestroyTexture(pair.second);
        }
    }
    textureCache.clear();
}
