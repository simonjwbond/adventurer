#ifndef INPUTMANAGER_H
#define INPUTMANAGER_H

#include <SDL2/SDL.h>
#include "utils/Constants.h"

/**
 * InputManager - Centralized keyboard input handling
 * 
 * Responsibilities:
 * - Poll SDL events for window/input events
 * - Track keyboard state for 8-way movement
 * - Detect single-frame button presses (attack)
 * - Provide normalized movement values (-1.0 to +1.0)
 */
class InputManager {
public:
    InputManager();
    ~InputManager();
    
    /**
     * Process SDL events and update input state
     * Must be called once per frame before querying input
     */
    void handleEvents(bool& shouldQuit);
    
    /**
     * Get horizontal movement input (-1.0 = left, +1.0 = right)
     */
    float getHorizontalInput() const;
    
    /**
     * Get vertical movement input (-1.0 = up, +1.0 = down)
     */
    float getVerticalInput() const;
    
    /**
     * Check if attack button was pressed this frame (single-frame detection)
     */
    bool isAttackPressed() const;
    
    /**
     * Check if pause key was pressed this frame
     */
    bool isPausePressed() const;
    
private:
    // Keyboard state cache (const because SDL_GetKeyboardState returns const pointer)
    const Uint8* keyboardState;
    
    // Single-frame press detection flags
    bool attackPressedThisFrame;
    bool pausePressedThisFrame;
    
    // Track previous frame's key states for edge detection
    bool wasAttackDown;
    bool wasPauseDown;
};

#endif // INPUTMANAGER_H
