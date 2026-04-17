#ifndef FOREST_H
#define FOREST_H

#include "BiomeBase.h"

/**
 * Forest Biome - Forest vegetation and terrain
 * 
 * Provides:
 * - Tree textures (various types)
 * - Tree border variations (for seamless tiling)
 * - Bush textures
 * - Fence textures
 */
class Forest : public BiomeBase {
public:
    Forest();
    ~Forest();
    
    void initialize(SDL_Renderer* renderer) override;
    std::string getName() const override { return "Forest"; }
    
    // Trees
    SDL_Texture* getTreeTexture();
    SDL_Texture* getTreeBorderVariation(int index);  // 0-8 for 3x3 grid
    
    // Bushes
    SDL_Texture* getBushTexture();
    
    // Fences
    SDL_Texture* getFenceTexture();

private:
    void createTreeTexture();
    void createTreeBorderVariations();  // Creates 9 tree border variations at 96x96
    void createBushTexture();
    void createFenceTexture();
    
    SDL_Texture* treeTexture = nullptr;
    SDL_Texture* treeBorderVariations[9];
    SDL_Texture* bushTexture = nullptr;
    SDL_Texture* fenceTexture = nullptr;
};

#endif // FOREST_H
