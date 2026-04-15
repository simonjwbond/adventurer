#ifndef MATHUTILS_H
#define MATHUTILS_H

#include <cmath>
#include <algorithm>

// ============================================================================
// Vector2D Structure - 2D vector for positions and velocities
// ============================================================================
struct Vector2D {
    float x;
    float y;
    
    // Constructors
    constexpr Vector2D() : x(0.0f), y(0.0f) {}
    constexpr Vector2D(float x_, float y_) : x(x_), y(y_) {}
    
    // Conversion from integer coordinates
    static Vector2D FromInt(int x_, int y_) {
        return Vector2D(static_cast<float>(x_), static_cast<float>(y_));
    }
    
    // Arithmetic operators
    Vector2D operator+(const Vector2D& other) const {
        return Vector2D(x + other.x, y + other.y);
    }
    
    Vector2D operator-(const Vector2D& other) const {
        return Vector2D(x - other.x, y - other.y);
    }
    
    Vector2D operator*(float scalar) const {
        return Vector2D(x * scalar, y * scalar);
    }
    
    Vector2D operator/(float scalar) const {
        if (scalar != 0.0f)
            return Vector2D(x / scalar, y / scalar);
        return Vector2D(0.0f, 0.0f);
    }
    
    // Compound assignment operators
    Vector2D& operator+=(const Vector2D& other) {
        x += other.x;
        y += other.y;
        return *this;
    }
    
    Vector2D& operator-=(const Vector2D& other) {
        x -= other.x;
        y -= other.y;
        return *this;
    }
    
    Vector2D& operator*=(float scalar) {
        x *= scalar;
        y *= scalar;
        return *this;
    }
    
    // Unary negation
    Vector2D operator-() const {
        return Vector2D(-x, -y);
    }
    
    // Equality comparison (with epsilon for float comparison)
    bool operator==(const Vector2D& other) const {
        constexpr float EPSILON = 0.001f;
        return std::abs(x - other.x) < EPSILON && std::abs(y - other.y) < EPSILON;
    }
    
    // Magnitude (length) of vector
    float magnitude() const {
        return std::sqrt(x * x + y * y);
    }
    
    // Squared magnitude (faster when comparing lengths)
    float magnitudeSquared() const {
        return x * x + y * y;
    }
    
    // Normalize vector to unit length
    Vector2D normalized() const {
        float mag = magnitude();
        if (mag > 0.0f) {
            return *this / mag;
        }
        return Vector2D(0.0f, 0.0f);
    }
    
    // Dot product
    float dot(const Vector2D& other) const {
        return x * other.x + y * other.y;
    }
    
    // Set to zero vector
    void zero() {
        x = 0.0f;
        y = 0.0f;
    }
};

// Free function for scalar multiplication (float * Vector2D)
inline Vector2D operator*(float scalar, const Vector2D& v) {
    return v * scalar;
}

// ============================================================================
// AABB Structure - Axis-Aligned Bounding Box for collision detection
// ============================================================================
struct AABB {
    float x;        // Top-left corner
    float y;
    float width;
    float height;
    
    // Constructors
    constexpr AABB() : x(0.0f), y(0.0f), width(0.0f), height(0.0f) {}
    constexpr AABB(float x_, float y_, float w, float h)
        : x(x_), y(y_), width(w), height(h) {}
    
    // Get edges
    float left()   const { return x; }
    float right()  const { return x + width; }
    float top()    const { return y; }
    float bottom() const { return y + height; }
    
    // Get center point
    Vector2D center() const {
        return Vector2D(x + width * 0.5f, y + height * 0.5f);
    }
    
    // Check if two AABBs intersect (standard separating axis test)
    bool intersects(const AABB& other) const {
        return left() < other.right() &&
               right() > other.left() &&
               top() < other.bottom() &&
               bottom() > other.top();
    }
    
    // Check if this AABB completely contains another
    bool contains(const AABB& other) const {
        return left() <= other.left() &&
               right() >= other.right() &&
               top() <= other.top() &&
               bottom() >= other.bottom();
    }
    
    // Check if this AABB contains a point
    bool containsPoint(float px, float py) const {
        return px >= left() && px < right() &&
               py >= top() && py < bottom();
    }
    
    // Calculate minimum distance between two non-intersecting AABBs
    // Returns 0 if they intersect
    float distanceTo(const AABB& other) const {
        if (intersects(other)) {
            return 0.0f;
        }
        
        // Find closest points on each rectangle
        float dx = std::max(left() - other.right(),
                            std::max(other.left() - right(), 0.0f));
        float dy = std::max(top() - other.bottom(),
                            std::max(other.top() - bottom(), 0.0f));
        
        return std::sqrt(dx * dx + dy * dy);
    }
    
    // Expand AABB by margin on all sides
    AABB expanded(float margin) const {
        return AABB(x - margin, y - margin,
                    width + 2.0f * margin,
                    height + 2.0f * margin);
    }
};

// ============================================================================
// Utility Functions
// ============================================================================

// Clamp value between min and max
inline float clamp(float value, float minVal, float maxVal) {
    return std::max(minVal, std::min(maxVal, value));
}

// Clamp integer value
inline int clamp(int value, int minVal, int maxVal) {
    return std::max(minVal, std::min(value, maxVal));
}

// Linear interpolation
inline float lerp(float a, float b, float t) {
    return a + (b - a) * t;
}

// Calculate angle between two vectors in radians (-PI to PI)
inline float angleBetween(Vector2D a, Vector2D b) {
    float dot = a.dot(b);
    float det = a.x * b.y - a.y * b.x;  // 2D cross product (determinant)
    return std::atan2(det, dot);
}

// Normalize vector (free function version)
inline Vector2D normalize(Vector2D v) {
    return v.normalized();
}

// Convert degrees to radians
inline float degToRad(float degrees) {
    return degrees * 3.14159265359f / 180.0f;
}

// Convert radians to degrees
inline float radToDeg(float radians) {
    return radians * 180.0f / 3.14159265359f;
}

#endif // MATHUTILS_H
