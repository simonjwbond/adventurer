#ifndef CAMERA_H
#define CAMERA_H

#include "utils/MathUtils.h"
#include "utils/Constants.h"

/**
 * Camera - Room-based camera with smooth transitions
 * 
 * Responsibilities:
 * - Track current room being viewed (room coordinates)
 * - Smoothly transition between rooms when player crosses boundaries
 * - Provide pixel offset for rendering entities relative to screen
 */
class Camera {
public:
    Camera();
    
    /**
     * Set fixed mode: camera stays at origin, no panning
     * @param fixed true to lock camera at (0,0), false for room-based tracking
     */
    void setFixed(bool fixed) { isFixed = fixed; }
    
    /**
     * Update camera based on player position
     * @param playerPosition Player's world position in pixels
     * @return true if a room transition occurred this frame
     */
    bool update(const Vector2D& playerPosition);
    
    /**
     * Get current camera offset in pixels
     * Use this to convert world coordinates to screen coordinates:
     *   screenX = worldX - cameraOffset.x
     *   screenY = worldY - cameraOffset.y
     */
    Vector2D getOffset() const;
    
    /**
     * Get current room coordinates (0,0 = starting room)
     */
    Vector2D getCurrentRoom() const;
    
    /**
     * Check if camera is currently transitioning between rooms
     */
    bool isTransitioning() const;
    
private:
    // Fixed mode: camera stays at origin (for testing)
    bool isFixed = false;
    
    // Current room being viewed (in room coordinates, not pixels)
    Vector2D currentRoom;
    
    // Target room we're transitioning toward
    Vector2D targetRoom;
    
    // Transition progress: 0.0 = at current room, 1.0 = reached target room
    float transitionProgress;
    
    /**
     * Determine which room the player should be viewing based on position
     */
    Vector2D calculateTargetRoom(const Vector2D& playerPosition) const;
};

#endif // CAMERA_H
