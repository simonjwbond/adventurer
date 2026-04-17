#ifndef DESERT_H
#define DESERT_H

#include "BiomeBase.h"
#include <array>

/**
 * Desert Biome - Desert terrain and structures
 * 
 * Provides:
 * - Sand and ground variations
 * - Cacti (Saguaro, Prickly Pear, Cholla)
 * - Rocks and boulders
 * - Desert structures (Adobe walls, ruins, wells)
 * - Desert vegetation (bushes, yucca, agave)
 * - Desert details (bones, tumbleweed, dead trees)
 */
class Desert : public BiomeBase {
public:
    Desert();
    ~Desert();
    
    void initialize(SDL_Renderer* renderer) override;
    std::string getName() const override { return "Desert"; }
    
    // Ground textures
    SDL_Texture* getSandTexture();
    SDL_Texture* getSandDarkTexture();
    SDL_Texture* getSandRockyTexture();
    SDL_Texture* getCrackedEarthTexture();
    SDL_Texture* getRockyGroundTexture();
    
    // Cacti
    SDL_Texture* getCactusSaguaro();
    SDL_Texture* getCactusSaguaro2Arms();
    SDL_Texture* getCactusSaguaro3Arms();
    SDL_Texture* getCactusPricklyPear();
    SDL_Texture* getCactusCholla();
    
    // Desert vegetation
    SDL_Texture* getDesertBushTexture();
    SDL_Texture* getDesertGrassTexture();
    SDL_Texture* getPaloVerdeTexture();
    SDL_Texture* getYuccaTexture();
    SDL_Texture* getAgaveTexture();
    
    // Rocks
    SDL_Texture* getRockSmallTexture();
    SDL_Texture* getRockMediumTexture();
    SDL_Texture* getRockLargeTexture();
    SDL_Texture* getRockFormationTexture();
    
    // Structures
    SDL_Texture* getAdobeWallTexture();
    SDL_Texture* getAdobeRuinTexture();
    SDL_Texture* getDesertWellTexture();
    SDL_Texture* getCaveEntranceTexture();
    SDL_Texture* getCampfireSiteTexture();
    
    // Details
    SDL_Texture* getBonesTexture();
    SDL_Texture* getTumbleweedTexture();
    SDL_Texture* getDeadTreeTexture();
    SDL_Texture* getScorpionTexture();
    SDL_Texture* getSnakeTexture();

private:
    void createSandTextures();
    void createCactusTextures();
    void createDesertVegetationTextures();
    void createRockTextures();
    void createDesertStructureTextures();
    void createDesertDetailTextures();
    
    // Ground
    SDL_Texture* sandTexture = nullptr;
    SDL_Texture* sandDarkTexture = nullptr;
    SDL_Texture* sandRockyTexture = nullptr;
    SDL_Texture* crackedEarthTexture = nullptr;
    SDL_Texture* rockyGroundTexture = nullptr;
    
    // Cacti
    SDL_Texture* cactusSaguaroTexture = nullptr;
    SDL_Texture* cactusSaguaro2ArmsTexture = nullptr;
    SDL_Texture* cactusSaguaro3ArmsTexture = nullptr;
    SDL_Texture* cactusPricklyPearTexture = nullptr;
    SDL_Texture* cactusChollaTexture = nullptr;
    
    // Vegetation
    SDL_Texture* desertBushTexture = nullptr;
    SDL_Texture* desertGrassTexture = nullptr;
    SDL_Texture* paloVerdeTexture = nullptr;
    SDL_Texture* yuccaTexture = nullptr;
    SDL_Texture* agaveTexture = nullptr;
    
    // Rocks
    SDL_Texture* rockSmallTexture = nullptr;
    SDL_Texture* rockMediumTexture = nullptr;
    SDL_Texture* rockLargeTexture = nullptr;
    SDL_Texture* rockFormationTexture = nullptr;
    
    // Structures
    SDL_Texture* adobeWallTexture = nullptr;
    SDL_Texture* adobeRuinTexture = nullptr;
    SDL_Texture* desertWellTexture = nullptr;
    SDL_Texture* caveEntranceTexture = nullptr;
    SDL_Texture* campfireSiteTexture = nullptr;
    
    // Details
    SDL_Texture* bonesTexture = nullptr;
    SDL_Texture* tumbleweedTexture = nullptr;
    SDL_Texture* deadTreeTexture = nullptr;
    SDL_Texture* scorpionTexture = nullptr;
    SDL_Texture* snakeTexture = nullptr;
};

#endif // DESERT_H
