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
    SDL_Texture* getRoofTexture();
    SDL_Texture* getGrassVariation(int index);  // 0-8 for 3x3 grid
    SDL_Texture* getIceVariation(int index);    // 0-8 for ice grid
    SDL_Texture* getSnowTexture();
    
    // Outdoor textures
    SDL_Texture* getGrassTexture();
    SDL_Texture* getTreeTexture();
    SDL_Texture* getTreeBorder(int index);  // 0-7 for border positions
    SDL_Texture* getBushTexture();
    SDL_Texture* getFenceTexture();
    
    // Desert biome textures
    // Ground tiles
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
    
    // Rocks and boulders
    SDL_Texture* getRockSmallTexture();
    SDL_Texture* getRockMediumTexture();
    SDL_Texture* getRockLargeTexture();
    SDL_Texture* getRockFormationTexture();
    
    // Desert structures
    SDL_Texture* getAdobeWallTexture();
    SDL_Texture* getAdobeRuinTexture();
    SDL_Texture* getDesertWellTexture();
    SDL_Texture* getCaveEntranceTexture();
    SDL_Texture* getCampfireSiteTexture();
    
    // Decorative details
    SDL_Texture* getBonesTexture();
    SDL_Texture* getTumbleweedTexture();
    SDL_Texture* getDeadTreeTexture();
    SDL_Texture* getScorpionTexture();
    SDL_Texture* getSnakeTexture();
    
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
    SDL_Texture* roofTexture = nullptr;
    
    // Outdoor texture members
    SDL_Texture* grassTexture = nullptr;           // Original grass (will be removed)
    SDL_Texture* grassVariation[9] = {nullptr};   // 3x3 grid of grass variations (96x96)
    SDL_Texture* iceVariation[9] = {nullptr};     // 3x3 grid of ice variations (32x32)
    SDL_Texture* snowTexture = nullptr;           // Snow tile
    SDL_Texture* treeTexture = nullptr;
    SDL_Texture* treeBorder[8] = {nullptr};  // 8 border tree variations
    SDL_Texture* bushTexture = nullptr;
    SDL_Texture* fenceTexture = nullptr;
    
    // Desert texture members
    SDL_Texture* sandTexture = nullptr;
    SDL_Texture* sandDarkTexture = nullptr;
    SDL_Texture* sandRockyTexture = nullptr;
    SDL_Texture* crackedEarthTexture = nullptr;
    SDL_Texture* rockyGroundTexture = nullptr;
    SDL_Texture* cactusSaguaroTexture = nullptr;
    SDL_Texture* cactusSaguaro2ArmsTexture = nullptr;
    SDL_Texture* cactusSaguaro3ArmsTexture = nullptr;
    SDL_Texture* cactusPricklyPearTexture = nullptr;
    SDL_Texture* cactusChollaTexture = nullptr;
    SDL_Texture* desertBushTexture = nullptr;
    SDL_Texture* desertGrassTexture = nullptr;
    SDL_Texture* paloVerdeTexture = nullptr;
    SDL_Texture* yuccaTexture = nullptr;
    SDL_Texture* agaveTexture = nullptr;
    SDL_Texture* rockSmallTexture = nullptr;
    SDL_Texture* rockMediumTexture = nullptr;
    SDL_Texture* rockLargeTexture = nullptr;
    SDL_Texture* rockFormationTexture = nullptr;
    SDL_Texture* adobeWallTexture = nullptr;
    SDL_Texture* adobeRuinTexture = nullptr;
    SDL_Texture* desertWellTexture = nullptr;
    SDL_Texture* caveEntranceTexture = nullptr;
    SDL_Texture* campfireSiteTexture = nullptr;
    SDL_Texture* bonesTexture = nullptr;
    SDL_Texture* tumbleweedTexture = nullptr;
    SDL_Texture* deadTreeTexture = nullptr;
    SDL_Texture* scorpionTexture = nullptr;
    SDL_Texture* snakeTexture = nullptr;
    
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
    void createRoofTexture();
    void createGrassVariations();  // Creates 9 grass variations at 96x96
    void createGrassTexture();
    void createIceVariations();    // Creates 9 ice variations at 32x32
    void createSnowTexture();
    void createTreeTexture();
    void createTreeBorderVariations();  // Creates 8 border tree variations
    void createBushTexture();
    void createFenceTexture();
    void createPlayerSprites();  // Creates all 8 directional sprites
    
    // Desert texture creation methods
    void createSandTextures();
    void createCactusTextures();
    void createDesertVegetationTextures();
    void createRockTextures();
    void createDesertStructureTextures();
    void createDesertDetailTextures();
    
    SDL_Texture* createTextureFromPixels(Uint8* pixels, int width = 32, int height = 32);
    
    void cleanup();
};

#endif // PROCEDURALART_H
