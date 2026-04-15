#ifndef TIMER_H
#define TIMER_H

#include <SDL2/SDL.h>

/**
 * Timer - Simple delta time tracking utility
 * 
 * Provides frame-independent timing for smooth movement and animations.
 * Header-only implementation for simplicity.
 */
class Timer {
public:
    Timer();
    
    /**
     * Reset the timer to current time
     */
    void reset();
    
    /**
     * Get elapsed time since last tick in seconds
     * Call this each frame to get delta time
     */
    float tick();
    
    /**
     * Get total elapsed time since reset in seconds
     */
    float getTotalTime() const;
    
private:
    Uint32 startTime;
    Uint32 lastTickTime;
};

// Inline implementations for header-only usage
inline Timer::Timer() {
    reset();
}

inline void Timer::reset() {
    startTime = SDL_GetTicks();
    lastTickTime = startTime;
}

inline float Timer::tick() {
    Uint32 currentTime = SDL_GetTicks();
    float deltaTime = (currentTime - lastTickTime) / 1000.0f;
    lastTickTime = currentTime;
    return deltaTime;
}

inline float Timer::getTotalTime() const {
    return (SDL_GetTicks() - startTime) / 1000.0f;
}

#endif // TIMER_H
