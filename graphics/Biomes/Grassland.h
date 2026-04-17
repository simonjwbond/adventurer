#ifndef GRASSLAND_H
#define GRASSLAND_H

#include "BiomeBase.h"
#include <array>

/**
 * Grassland Biome - Outdoor terrain textures
 * 
 * Provides:
 * - Grass variations (3x3 grid for seamless tiling)
 * - Ice variations (3x3 grid)
 * - Snow texture
 * - Outdoor ground tiles
 */
class Grassland : public BiomeBase {
public:
    Grassland();
    ~Grassland();
    
    void initialize(SDL_Renderer* renderer) override;
    std::string getName() const override { return "Grassland"; }
    
    // Grass textures
    SDL_Texture* getGrassTexture();
    SDL_Texture* getGrassVariation(int index);  // 0-8 for 3x3 grid
    
    // Ice textures
    SDL_Texture* getIceVariation(int index);    // 0-8 for ice grid
    SDL_Texture* getSnowTexture();

private:
    void createGrassTexture();
    void createGrassVariations();  // Creates 9 grass variations at 96x96
    void createIceVariations();    // Creates 9 ice variations at 32x32
    void createSnowTexture();
    
    SDL_Texture* grassTexture = nullptr;
    std::array<SDL_Texture*, 9> grassVariation;
    std::array<SDL_Texture*, 9> iceVariation;
    SDL_Texture* snowTexture = nullptr;
};

#endif // GRASSLAND_H
