#ifndef INDOOR_H
#define INDOOR_H

#include "BiomeBase.h"

/**
 * Indoor Biome - Interior building textures
 * 
 * Provides:
 * - Wood floor textures
 * - Stone wall textures
 * - Roof textures
 * - Indoor furniture/decorations
 */
class Indoor : public BiomeBase {
public:
    Indoor();
    ~Indoor();
    
    void initialize(SDL_Renderer* renderer) override;
    std::string getName() const override { return "Indoor"; }
    
    // Floors
    SDL_Texture* getWoodFloorTexture();
    SDL_Texture* getStoneFloorTexture();
    
    // Walls
    SDL_Texture* getStoneWallTexture();
    SDL_Texture* getWoodWallTexture();
    
    // Wall corners and edges
    SDL_Texture* getWallCornerTopLeft();
    SDL_Texture* getWallCornerTopRight();
    SDL_Texture* getWallCornerBottomLeft();
    SDL_Texture* getWallCornerBottomRight();
    SDL_Texture* getWallEdgeHorizontal();
    SDL_Texture* getWallEdgeVertical();
    
    // Roof
    SDL_Texture* getRoofTexture();

private:
    void createWoodFloorTexture();
    void createStoneFloorTexture();
    void createStoneWallTexture();
    void createWoodWallTexture();
    void createRoofTexture();
    void createWallCornerTopLeft();
    void createWallCornerTopRight();
    void createWallCornerBottomLeft();
    void createWallCornerBottomRight();
    void createWallEdgeHorizontal();
    void createWallEdgeVertical();
    
    SDL_Texture* woodFloorTexture = nullptr;
    SDL_Texture* stoneFloorTexture = nullptr;
    SDL_Texture* stoneWallTexture = nullptr;
    SDL_Texture* woodWallTexture = nullptr;
    SDL_Texture* roofTexture = nullptr;
    
    // Wall corners and edges
    SDL_Texture* wallCornerTopLeft = nullptr;
    SDL_Texture* wallCornerTopRight = nullptr;
    SDL_Texture* wallCornerBottomLeft = nullptr;
    SDL_Texture* wallCornerBottomRight = nullptr;
    SDL_Texture* wallEdgeHorizontal = nullptr;
    SDL_Texture* wallEdgeVertical = nullptr;
};

#endif // INDOOR_H
