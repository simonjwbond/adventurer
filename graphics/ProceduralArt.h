#ifndef PROCEDURAL_ART_H
#define PROCEDURAL_ART_H

#include <SDL2/SDL.h>
#include "Core/TextureCache.h"
#include "Biomes/BiomeBase.h"
#include "Biomes/Grassland.h"
#include "Biomes/Desert.h"
#include "Biomes/Forest.h"
#include "Biomes/Indoor.h"
#include "Characters/CharacterSprites.h"

/**
 * ProceduralArt - Main facade for all procedural texture generation
 * 
 * This class coordinates all biome and character texture generators,
 * providing a simple interface for the game to request textures.
 * 
 * Usage:
 *   ProceduralArt art;
 *   art.initialize(renderer);
 *   SDL_Texture* grass = art.getGrassTexture();
 *   SDL_Texture* cactus = art.getCactusSaguaro();
 *   SDL_Texture* player = art.getPlayerSpriteDown(0);
 */
class ProceduralArt {
public:
    ProceduralArt();
    ~ProceduralArt();
    
    /**
     * Initialize all texture generators
     * @param renderer SDL renderer to create textures with
     */
    void initialize(SDL_Renderer* renderer);
    
    /**
     * Cleanup all textures
     */
    void cleanup();
    
    // ==================== GRASSLAND BIOME ====================
    SDL_Texture* getGrassTexture();
    SDL_Texture* getGrassVariation(int index);
    SDL_Texture* getIceVariation(int index);
    SDL_Texture* getSnowTexture();
    
    // ==================== DESERT BIOME ====================
    SDL_Texture* getSandTexture();
    SDL_Texture* getSandDarkTexture();
    SDL_Texture* getSandRockyTexture();
    SDL_Texture* getCrackedEarthTexture();
    SDL_Texture* getRockyGroundTexture();
    SDL_Texture* getCactusSaguaro();
    SDL_Texture* getCactusSaguaro2Arms();
    SDL_Texture* getCactusSaguaro3Arms();
    SDL_Texture* getCactusPricklyPear();
    SDL_Texture* getCactusCholla();
    SDL_Texture* getDesertBushTexture();
    SDL_Texture* getDesertGrassTexture();
    SDL_Texture* getPaloVerdeTexture();
    SDL_Texture* getYuccaTexture();
    SDL_Texture* getAgaveTexture();
    SDL_Texture* getRockSmallTexture();
    SDL_Texture* getRockMediumTexture();
    SDL_Texture* getRockLargeTexture();
    SDL_Texture* getRockFormationTexture();
    SDL_Texture* getAdobeWallTexture();
    SDL_Texture* getAdobeRuinTexture();
    SDL_Texture* getDesertWellTexture();
    SDL_Texture* getCaveEntranceTexture();
    SDL_Texture* getCampfireSiteTexture();
    SDL_Texture* getBonesTexture();
    SDL_Texture* getTumbleweedTexture();
    SDL_Texture* getDeadTreeTexture();
    SDL_Texture* getScorpionTexture();
    SDL_Texture* getSnakeTexture();
    
    // ==================== FOREST BIOME ====================
    SDL_Texture* getTreeTexture();
    SDL_Texture* getTreeBorderVariation(int index);
    SDL_Texture* getTreeBorder(int index);
    SDL_Texture* getBushTexture();
    SDL_Texture* getFenceTexture();
    
    // ==================== INDOOR BIOME ====================
    SDL_Texture* getWoodFloorTexture();
    SDL_Texture* getStoneFloorTexture();
    SDL_Texture* getStoneWallTexture();
    SDL_Texture* getWoodWallTexture();
    SDL_Texture* getRoofTexture();
    
    // Wall corners and edges
    SDL_Texture* getWallCornerTopLeft();
    SDL_Texture* getWallCornerTopRight();
    SDL_Texture* getWallCornerBottomLeft();
    SDL_Texture* getWallCornerBottomRight();
    SDL_Texture* getWallEdgeHorizontal();
    SDL_Texture* getWallEdgeVertical();
    
    // ==================== CHARACTER SPRITES ====================
    SDL_Texture* getPlayerSprite(int direction, int frame);
    SDL_Texture* getPlayerSpriteDown(int frame);
    SDL_Texture* getPlayerSpriteLeft(int frame);
    SDL_Texture* getPlayerSpriteRight(int frame);
    SDL_Texture* getPlayerSpriteUp(int frame);
    SDL_Texture* getPlayerSpriteDownLeft(int frame);
    SDL_Texture* getPlayerSpriteDownRight(int frame);
    SDL_Texture* getPlayerSpriteUpLeft(int frame);
    SDL_Texture* getPlayerSpriteUpRight(int frame);
    
    // ==================== TEXTURE CACHE ====================
    SDL_Texture* getTexture(const std::string& key, std::function<SDL_Texture*()> creator);
    
private:
    SDL_Renderer* renderer = nullptr;
    TextureCache* textureCache = nullptr;
    
    // Biome instances
    Grassland* grassland = nullptr;
    Desert* desert = nullptr;
    Forest* forest = nullptr;
    Indoor* indoor = nullptr;
    
    // Character sprites
    CharacterSprites* characterSprites = nullptr;
};

#endif // PROCEDURAL_ART_H
