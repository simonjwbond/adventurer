#ifndef CHARACTER_SPRITES_H
#define CHARACTER_SPRITES_H

#include <SDL2/SDL.h>
#include <array>

/**
 * CharacterSprites - Player character sprite generation
 * 
 * Provides:
 * - Player sprites in 8 directions
 * - 3 animation frames per direction
 * - Modular character system (head, body, legs, accessories)
 */
class CharacterSprites {
public:
    CharacterSprites();
    ~CharacterSprites();
    
    void initialize(SDL_Renderer* renderer);
    void cleanup();
    
    // Get player sprites by direction and frame
    // Directions: 0=Down, 1=Down-Left, 2=Left, 3=Up-Left,
    //             4=Up, 5=Up-Right, 6=Right, 7=Down-Right
    // Frames: 0, 1, 2 (animation frames)
    SDL_Texture* getPlayerSprite(int direction, int frame);
    
    // Get specific body parts for modular assembly
    SDL_Texture* getHead();
    SDL_Texture* getBody();
    SDL_Texture* getLegs();
    SDL_Texture* getWeapon();

private:
    void createPlayerSprites();
    void assembleCharacter();
    
    SDL_Renderer* renderer = nullptr;
    std::array<std::array<SDL_Texture*, 3>, 8> playerSprites;
    
    // Modular parts
    SDL_Texture* headTexture = nullptr;
    SDL_Texture* bodyTexture = nullptr;
    SDL_Texture* legsTexture = nullptr;
    SDL_Texture* weaponTexture = nullptr;
};

#endif // CHARACTER_SPRITES_H
