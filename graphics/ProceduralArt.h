#ifndef PROCEDURALART_H
#define PROCEDURALART_H

#include <SDL2/SDL.h>

/**
 * ProceduralArt - Generate Zelda-style procedural textures
 */
class ProceduralArt {
public:
    ProceduralArt(SDL_Renderer* renderer);
    ~ProceduralArt();
    
    SDL_Texture* getWoodFloorTexture();
    SDL_Texture* getStoneWallTexture();
    
    // Outdoor textures
    SDL_Texture* getGrassTexture();
    SDL_Texture* getTreeTexture();
    SDL_Texture* getBushTexture();
    SDL_Texture* getFenceTexture();
    
    SDL_Texture* getWallCornerTopLeft();
    SDL_Texture* getWallCornerTopRight();
    SDL_Texture* getWallCornerBottomLeft();
    SDL_Texture* getWallCornerBottomRight();
    
    SDL_Texture* getWallEdgeHorizontal();
    SDL_Texture* getWallEdgeVertical();
    
    // Player sprites - 8 directional movement (48x64 pixels each, HD with modular layers)
    // Each direction has 3 animation frames for walking cycle
    SDL_Texture* getPlayerSpriteDown(int frame = 0);
    SDL_Texture* getPlayerSpriteUp(int frame = 0);
    SDL_Texture* getPlayerSpriteLeft(int frame = 0);
    SDL_Texture* getPlayerSpriteRight(int frame = 0);
    SDL_Texture* getPlayerSpriteDownLeft(int frame = 0);
    SDL_Texture* getPlayerSpriteDownRight(int frame = 0);
    SDL_Texture* getPlayerSpriteUpLeft(int frame = 0);
    SDL_Texture* getPlayerSpriteUpRight(int frame = 0);
    
    // Modular Character System
    struct CharacterLoadout {
        int bodyType = 0;      // 0: Standard, 1: Elf, 2: Orc
        int clothingType = 0;   // 0: Tunic, 1: Shirt, 2: Robe
        unsigned int clothingColor = 0x00008B; // Default Dark Royal Blue
        int armorType = 0;      // 0: None, 1: Iron, 2: Gold
        int weaponType = 0;     // 0: None, 1: Sword, 2: Staff
        int shoeType = 0;       // 0: Brown Boots, 1: Black Boots
    };
    
    SDL_Texture* assembleCharacter(const CharacterLoadout& loadout, int frame = 0, int direction = 0);

private:
    SDL_Renderer* renderer;
    
    SDL_Texture* woodFloorTexture = nullptr;
    SDL_Texture* stoneWallTexture = nullptr;
    
    // Outdoor texture members
    SDL_Texture* grassTexture = nullptr;
    SDL_Texture* treeTexture = nullptr;
    SDL_Texture* bushTexture = nullptr;
    SDL_Texture* fenceTexture = nullptr;
    
    SDL_Texture* wallCornerTopLeft = nullptr;
    SDL_Texture* wallCornerTopRight = nullptr;
    SDL_Texture* wallCornerBottomLeft = nullptr;
    SDL_Texture* wallCornerBottomRight = nullptr;
    SDL_Texture* wallEdgeHorizontal = nullptr;
    SDL_Texture* wallEdgeVertical = nullptr;
    
    // Player sprite textures - 8 directions x 3 animation frames (48x64 pixels, HD modular)
    static const int MAX_ANIM_FRAMES = 3;
    SDL_Texture* playerSpriteDown[MAX_ANIM_FRAMES] = {nullptr};
    SDL_Texture* playerSpriteUp[MAX_ANIM_FRAMES] = {nullptr};
    SDL_Texture* playerSpriteLeft[MAX_ANIM_FRAMES] = {nullptr};
    SDL_Texture* playerSpriteRight[MAX_ANIM_FRAMES] = {nullptr};
    SDL_Texture* playerSpriteDownLeft[MAX_ANIM_FRAMES] = {nullptr};
    SDL_Texture* playerSpriteDownRight[MAX_ANIM_FRAMES] = {nullptr};
    SDL_Texture* playerSpriteUpLeft[MAX_ANIM_FRAMES] = {nullptr};
    SDL_Texture* playerSpriteUpRight[MAX_ANIM_FRAMES] = {nullptr};
    
    void createWoodFloorTexture();
    void createStoneWallTexture();
    void createGrassTexture();
    void createTreeTexture();
    void createBushTexture();
    void createFenceTexture();
    void createPlayerSprites();  // Creates all 8 directional sprites
    
    SDL_Texture* createTextureFromPixels(Uint8* pixels, int width = 32, int height = 32);
    
    void cleanup();
};

#endif // PROCEDURALART_H
