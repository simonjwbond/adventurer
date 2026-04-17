#include "ProceduralArt.h"

ProceduralArt::ProceduralArt() {
}

ProceduralArt::~ProceduralArt() {
    cleanup();
}

void ProceduralArt::initialize(SDL_Renderer* ren) {
    renderer = ren;
    textureCache = new TextureCache(ren);
    
    // Initialize all biomes
    grassland = new Grassland();
    grassland->initialize(ren);
    
    desert = new Desert();
    desert->initialize(ren);
    
    forest = new Forest();
    forest->initialize(ren);
    
    indoor = new Indoor();
    indoor->initialize(ren);
    
    // Initialize character sprites
    characterSprites = new CharacterSprites();
    characterSprites->initialize(ren);
}

void ProceduralArt::cleanup() {
    if (grassland) {
        delete grassland;
        grassland = nullptr;
    }
    if (desert) {
        delete desert;
        desert = nullptr;
    }
    if (forest) {
        delete forest;
        forest = nullptr;
    }
    if (indoor) {
        delete indoor;
        indoor = nullptr;
    }
    if (characterSprites) {
        delete characterSprites;
        characterSprites = nullptr;
    }
    if (textureCache) {
        delete textureCache;
        textureCache = nullptr;
    }
}

// ==================== GRASSLAND BIOME ====================
SDL_Texture* ProceduralArt::getGrassTexture() {
    return grassland ? grassland->getGrassTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getGrassVariation(int index) {
    return grassland ? grassland->getGrassVariation(index) : nullptr;
}

SDL_Texture* ProceduralArt::getIceVariation(int index) {
    return grassland ? grassland->getIceVariation(index) : nullptr;
}

SDL_Texture* ProceduralArt::getSnowTexture() {
    return grassland ? grassland->getSnowTexture() : nullptr;
}

// ==================== DESERT BIOME ====================
SDL_Texture* ProceduralArt::getSandTexture() {
    return desert ? desert->getSandTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getSandDarkTexture() {
    return desert ? desert->getSandDarkTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getSandRockyTexture() {
    return desert ? desert->getSandRockyTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getCrackedEarthTexture() {
    return desert ? desert->getCrackedEarthTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getRockyGroundTexture() {
    return desert ? desert->getRockyGroundTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getCactusSaguaro() {
    return desert ? desert->getCactusSaguaro() : nullptr;
}

SDL_Texture* ProceduralArt::getCactusSaguaro2Arms() {
    return desert ? desert->getCactusSaguaro2Arms() : nullptr;
}

SDL_Texture* ProceduralArt::getCactusSaguaro3Arms() {
    return desert ? desert->getCactusSaguaro3Arms() : nullptr;
}

SDL_Texture* ProceduralArt::getCactusPricklyPear() {
    return desert ? desert->getCactusPricklyPear() : nullptr;
}

SDL_Texture* ProceduralArt::getCactusCholla() {
    return desert ? desert->getCactusCholla() : nullptr;
}

SDL_Texture* ProceduralArt::getDesertBushTexture() {
    return desert ? desert->getDesertBushTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getDesertGrassTexture() {
    return desert ? desert->getDesertGrassTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getPaloVerdeTexture() {
    return desert ? desert->getPaloVerdeTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getYuccaTexture() {
    return desert ? desert->getYuccaTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getAgaveTexture() {
    return desert ? desert->getAgaveTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getRockSmallTexture() {
    return desert ? desert->getRockSmallTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getRockMediumTexture() {
    return desert ? desert->getRockMediumTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getRockLargeTexture() {
    return desert ? desert->getRockLargeTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getRockFormationTexture() {
    return desert ? desert->getRockFormationTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getAdobeWallTexture() {
    return desert ? desert->getAdobeWallTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getAdobeRuinTexture() {
    return desert ? desert->getAdobeRuinTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getDesertWellTexture() {
    return desert ? desert->getDesertWellTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getCaveEntranceTexture() {
    return desert ? desert->getCaveEntranceTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getCampfireSiteTexture() {
    return desert ? desert->getCampfireSiteTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getBonesTexture() {
    return desert ? desert->getBonesTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getTumbleweedTexture() {
    return desert ? desert->getTumbleweedTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getDeadTreeTexture() {
    return desert ? desert->getDeadTreeTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getScorpionTexture() {
    return desert ? desert->getScorpionTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getSnakeTexture() {
    return desert ? desert->getSnakeTexture() : nullptr;
}

// ==================== FOREST BIOME ====================
SDL_Texture* ProceduralArt::getTreeTexture() {
    return forest ? forest->getTreeTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getTreeBorderVariation(int index) {
    return forest ? forest->getTreeBorderVariation(index) : nullptr;
}

SDL_Texture* ProceduralArt::getTreeBorder(int index) {
    return forest ? forest->getTreeBorderVariation(index) : nullptr;
}

SDL_Texture* ProceduralArt::getBushTexture() {
    return forest ? forest->getBushTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getFenceTexture() {
    return forest ? forest->getFenceTexture() : nullptr;
}

// ==================== INDOOR BIOME ====================
SDL_Texture* ProceduralArt::getWoodFloorTexture() {
    return indoor ? indoor->getWoodFloorTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getStoneFloorTexture() {
    return indoor ? indoor->getStoneFloorTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getStoneWallTexture() {
    return indoor ? indoor->getStoneWallTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getWoodWallTexture() {
    return indoor ? indoor->getWoodWallTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getRoofTexture() {
    return indoor ? indoor->getRoofTexture() : nullptr;
}

SDL_Texture* ProceduralArt::getWallCornerTopLeft() {
    return indoor ? indoor->getWallCornerTopLeft() : nullptr;
}

SDL_Texture* ProceduralArt::getWallCornerTopRight() {
    return indoor ? indoor->getWallCornerTopRight() : nullptr;
}

SDL_Texture* ProceduralArt::getWallCornerBottomLeft() {
    return indoor ? indoor->getWallCornerBottomLeft() : nullptr;
}

SDL_Texture* ProceduralArt::getWallCornerBottomRight() {
    return indoor ? indoor->getWallCornerBottomRight() : nullptr;
}

SDL_Texture* ProceduralArt::getWallEdgeHorizontal() {
    return indoor ? indoor->getWallEdgeHorizontal() : nullptr;
}

SDL_Texture* ProceduralArt::getWallEdgeVertical() {
    return indoor ? indoor->getWallEdgeVertical() : nullptr;
}

// ==================== CHARACTER SPRITES ====================
SDL_Texture* ProceduralArt::getPlayerSprite(int direction, int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(direction, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteDown(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(0, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteLeft(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(2, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteRight(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(6, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteUp(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(4, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteDownLeft(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(1, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteDownRight(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(7, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteUpLeft(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(3, frame) : nullptr;
}

SDL_Texture* ProceduralArt::getPlayerSpriteUpRight(int frame) {
    return characterSprites ? characterSprites->getPlayerSprite(5, frame) : nullptr;
}

// ==================== TEXTURE CACHE ====================
SDL_Texture* ProceduralArt::getTexture(const std::string& key, std::function<SDL_Texture*()> creator) {
    return textureCache ? textureCache->getTexture(key, creator) : nullptr;
}
