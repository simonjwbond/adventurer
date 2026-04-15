#include "InputManager.h"
#include <cstdio>

/**
 * InputManager Constructor - Initialize state tracking
 */
InputManager::InputManager()
    : keyboardState(nullptr)
    , attackPressedThisFrame(false)
    , pausePressedThisFrame(false)
    , wasAttackDown(false)
    , wasPauseDown(false)
{
    // Get initial keyboard state
    keyboardState = SDL_GetKeyboardState(nullptr);
}

/**
 * InputManager Destructor - No resources to clean up
 */
InputManager::~InputManager() {
    keyboardState = nullptr;
}

/**
 * Process SDL events and update input state
 * This is the centralized event polling point per AGENTS.md protocol
 */
void InputManager::handleEvents(bool& shouldQuit) {
    SDL_Event event;
    
    // Poll all events from the queue (prevents "Not Responding" state)
    while (SDL_PollEvent(&event)) {
        switch (event.type) {
            case SDL_QUIT:
                shouldQuit = true;
                DEBUG_LOG("SDL_QUIT event received");
                break;
                
            case SDL_WINDOWEVENT:
                if (event.window.event == SDL_WINDOWEVENT_CLOSE) {
                    shouldQuit = true;
                    DEBUG_LOG("Window close requested");
                }
                // Handle window resize
                else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    DEBUG_LOG("Window resized to %dx%d", 
                              event.window.data1, event.window.data2);
                }
                break;
        }
    }
    
    // Update keyboard state cache
    keyboardState = SDL_GetKeyboardState(nullptr);
    
    // Detect single-frame button presses (rising edge detection)
    bool isAttackDown = (keyboardState[SDL_SCANCODE_Z] != 0);
    bool isPauseDown = (keyboardState[SDL_SCANCODE_P] != 0 || 
                        keyboardState[SDL_SCANCODE_ESCAPE] != 0);
    
    // Attack pressed this frame if currently down but wasn't last frame
    attackPressedThisFrame = isAttackDown && !wasAttackDown;
    pausePressedThisFrame = isPauseDown && !wasPauseDown;
    
    // Store current state for next frame comparison
    wasAttackDown = isAttackDown;
    wasPauseDown = isPauseDown;
}

/**
 * Get horizontal movement input (-1.0 = left, +1.0 = right)
 */
float InputManager::getHorizontalInput() const {
    float value = 0.0f;
    
    // Check A or Left Arrow for left movement
    if (keyboardState[SDL_SCANCODE_A] != 0 || keyboardState[SDL_SCANCODE_LEFT] != 0) {
        value -= 1.0f;
    }
    
    // Check D or Right Arrow for right movement
    if (keyboardState[SDL_SCANCODE_D] != 0 || keyboardState[SDL_SCANCODE_RIGHT] != 0) {
        value += 1.0f;
    }
    
    return value;
}

/**
 * Get vertical movement input (-1.0 = up, +1.0 = down)
 */
float InputManager::getVerticalInput() const {
    float value = 0.0f;
    
    // Check W or Up Arrow for up movement
    if (keyboardState[SDL_SCANCODE_W] != 0 || keyboardState[SDL_SCANCODE_UP] != 0) {
        value -= 1.0f;  // Negative because screen Y increases downward
    }
    
    // Check S or Down Arrow for down movement
    if (keyboardState[SDL_SCANCODE_S] != 0 || keyboardState[SDL_SCANCODE_DOWN] != 0) {
        value += 1.0f;
    }
    
    return value;
}

/**
 * Check if attack button was pressed this frame
 */
bool InputManager::isAttackPressed() const {
    return attackPressedThisFrame;
}

/**
 * Check if pause key was pressed this frame
 */
bool InputManager::isPausePressed() const {
    return pausePressedThisFrame;
}
