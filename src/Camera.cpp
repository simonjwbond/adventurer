#include "Camera.h"
#include <cmath>

/**
 * Camera Constructor - Start at room (0, 0)
 */
Camera::Camera()
    : currentRoom(0.0f, 0.0f)
    , targetRoom(0.0f, 0.0f)
    , transitionProgress(0.0f)
{
}

/**
 * Calculate which room the player should be viewing based on position
 */
Vector2D Camera::calculateTargetRoom(const Vector2D& playerPosition) const {
    // Convert player pixel position to room coordinates
    float worldWidth = SCREEN_WIDTH;
    float worldHeight = SCREEN_HEIGHT;
    
    // Calculate which room the player is in (can be negative for rooms left/up)
    int roomX = static_cast<int>(std::floor(playerPosition.x / worldWidth));
    int roomY = static_cast<int>(std::floor(playerPosition.y / worldHeight));
    
    return Vector2D(static_cast<float>(roomX), static_cast<float>(roomY));
}

/**
 * Update camera based on player position
 */
bool Camera::update(const Vector2D& playerPosition) {
    // Fixed mode: camera stays at origin, no panning
    if (isFixed) {
        currentRoom = Vector2D(0.0f, 0.0f);
        targetRoom = Vector2D(0.0f, 0.0f);
        transitionProgress = 1.0f;
        return false;
    }
    
    bool transitionOccurred = false;
    
    // Calculate which room the player should be viewing
    Vector2D desiredRoom = calculateTargetRoom(playerPosition);
    
    // If we want to move to a different room, start transitioning
    if (desiredRoom.x != targetRoom.x || desiredRoom.y != targetRoom.y) {
        targetRoom = desiredRoom;
        transitionProgress = 0.0f;
        transitionOccurred = true;
    }
    
    // Smoothly interpolate from current room to target room
    if (transitionProgress < 1.0f) {
        // Transition speed: complete in ~30 frames at 60 FPS (~0.5 seconds)
        float transitionSpeed = 0.033f;  // 1/30 per frame
        
        transitionProgress += transitionSpeed;
        if (transitionProgress > 1.0f) {
            transitionProgress = 1.0f;
            currentRoom = targetRoom;
        } else {
            // Interpolate current room position
            currentRoom.x = currentRoom.x + (targetRoom.x - currentRoom.x) * transitionSpeed;
            currentRoom.y = currentRoom.y + (targetRoom.y - currentRoom.y) * transitionSpeed;
        }
    }
    
    return transitionOccurred;
}

/**
 * Get current camera offset in pixels
 */
Vector2D Camera::getOffset() const {
    // Convert room coordinates to pixel offset
    float offsetX = currentRoom.x * SCREEN_WIDTH;
    float offsetY = currentRoom.y * SCREEN_HEIGHT;
    
    return Vector2D(offsetX, offsetY);
}

/**
 * Get current room coordinates
 */
Vector2D Camera::getCurrentRoom() const {
    return currentRoom;
}

/**
 * Check if camera is currently transitioning between rooms
 */
bool Camera::isTransitioning() const {
    return transitionProgress < 1.0f;
}
