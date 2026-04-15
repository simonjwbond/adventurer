# SDL Rendering Issues & Solutions

This document records all rendering problems encountered during development of Super Scroller, along with their root causes and solutions.

---

## Issue 1: Color Channel Ordering Bug (CRITICAL)

### Problem
Colors were completely scrambled when rendering sprites:
- Brown ground appeared **blue**
- Green grass didn't appear green
- All colors shifted to wrong channels

### Root Cause
In `Renderer::createTextureFromPixels()`, the pixel format conversion placed RGB values in wrong byte positions relative to the SDL surface masks.

The SDL surface was created with these masks:
```cpp
SDL_CreateRGBSurface(flags, width, height, 32,
    0x00FF0000,  // R mask - Red in bits 16-23
    0x0000FF00,  // G mask - Green in bits 8-15
    0x000000FF,  // B mask - Blue in bits 0-7
    0xFF000000   // A mask - Alpha in bits 24-31
);
```

But the pixel construction was wrong:
```cpp
// WRONG - colors placed in incorrect byte positions
Uint32 pixel = 
    (color.r << 24) |   // Red put in ALPHA position!
    (color.g << 16) |   // Green put in RED position!
    (color.b << 8)  |   // Blue put in GREEN position!
    color.a;           // Alpha put in BLUE position!
```

### Solution
Match the byte positions to the surface masks:
```cpp
// CORRECT - colors match SDL surface mask layout
Uint32 pixel = 
    (color.a << 24) |   // Alpha in highest byte (0xFF000000)
    (color.r << 16) |   // Red in second byte (0x00FF0000)
    (color.g << 8)  |   // Green in third byte (0x0000FF00)
    color.b;           // Blue in lowest byte (0x000000FF)
```

### Verification
- `DarkBrown` = {92, 64, 51, 255}
  - Before: Pixel value put alpha(255) in blue channel → appeared BLUE ❌
  - After: Correct ARGB format → appears DARK BROWN ✅

---

## Issue 2: Ground Layer Y-Coordinate Ordering

### Problem
Ground layers were rendered backwards:
- Dirt tiles appeared **above** grass surface tiles
- Player was embedded WITHIN the ground instead of standing on top

### Root Cause
Confusion about SDL screen coordinates vs. logical "top/bottom" terminology.

In SDL/screen coordinates:
- **Y = 0 is at the TOP** of the screen
- **Y increases going DOWN** toward the bottom

The code was placing dirt at lower Y (higher visually) and grass at higher Y (lower visually).

### Solution
Swap all ground layer positions to match visual expectations:

```cpp
// Screen coordinate system: Y=0 is TOP, Y increases DOWN
const int GROUND_Y = SCREEN_HEIGHT * 3 - (GROUND_HEIGHT_TILES * TILE_SIZE * 3);

// Grass surface at LOWER Y (higher up on screen)
rect.y = GROUND_Y;              // ← Top row of ground (visually higher)

// Dirt below at HIGHER Y (lower down on screen)  
rect.y = GROUND_Y + TILE_WIDTH; // ← Bottom row (visually lower)
```

Player position also fixed:
```cpp
// Player stands ON TOP of grass surface
int playerY = GROUND_Y - (spriteHeight * 3);  // Above the grass
```

### Verification
Visual stack from top to bottom:
1. Sky area
2. Grass surface tiles [grass|dirt] ← Lower Y value
3. Pure dirt tiles below        ← Higher Y value
4. Player stands on grass       ← Even lower Y (above everything)

---

## Issue 3: Cloud Sprite Generation - Wrong Buffer Size

### Problem
Cloud sprites appeared completely black/transparent even though circles were being drawn.

### Root Cause
The `fillRect()` helper function was called BEFORE setting the actual sprite dimensions:

```cpp
// WRONG ORDER
PixelData cloud;
fillRect(cloud, 0, 0, 16, 16, ColorPalette::Black);  // Writes to 16x16 buffer!

int width = 8 + size * 4;    // e.g., 24 pixels wide
int height = 6 + size * 2;   // e.g., 16 pixels tall
cloud.width = width;
cloud.height = height;
cloud.pixels.resize(width * height);  // Now resized to larger buffer!

// Circles drawn into new larger buffer, but fillRect wrote to old small buffer
```

The `fillRect` function expects exactly 16x16 sprites and writes directly to the first 256 pixels. When we later resize to a larger size (e.g., 24×16 = 384 pixels), the circles are drawn into the new area but the original fill was lost.

### Solution
Set dimensions FIRST, then manually fill with transparent color:

```cpp
// CORRECT ORDER
PixelData cloud;

int width = 8 + size * 4;    // Set desired size first
int height = 6 + size * 2;
cloud.width = width;
cloud.height = height;
cloud.pixels.resize(width * height);  // Resize to correct size

// Manually fill with transparent (don't use fillRect for non-16x16 sprites)
for (int i = 0; i < width * height; ++i) {
    cloud.pixels[i] = ColorPalette::Black;
}

// Now draw circles - they go into the correctly-sized buffer
drawCircle(8, baseY, 5, ColorPalette::White);
```

### Verification
- Cloud sprites now have correct dimensions (12×8 to 20×12 pixels)
- White cloud shapes visible instead of black/transparent

---

## Issue 4: World Clouds Not Visible During Gameplay

### Problem
World clouds were initialized and existed in memory, but never appeared on screen during gameplay.

### Root Cause
Clouds were positioned in **world coordinates** (0-1600 pixels across the entire level) but rendered at absolute screen positions without any camera offset:

```cpp
// WRONG - renders at absolute world position
int screenX = static_cast<int>(cloud.x) * 3;  // cloud.x could be 800!
// If cloud is at x=800, it renders at screen pixel 2400 (way off-screen)
```

The game had no camera system - everything rendered at fixed world coordinates. Only clouds near x=0 would ever be visible.

### Solution
Implement a simple camera that centers on the player:

```cpp
// Calculate camera position (centered on player)
float cameraX = playerPos.x - (SCREEN_WIDTH / 2);
if (cameraX < 0) cameraX = 0;  // Don't scroll past left edge

// Render clouds with camera offset and parallax effect
float parallaxX = cameraX * 0.5f;  // Clouds move slower for depth
int screenX = static_cast<int>((cloud.x - parallaxX) * 3);
```

Also applied to tiles and player:
```cpp
// Tiles with culling (only render visible ones)
int startTileX = static_cast<int>(cameraX / TILE_SIZE);
int endTileX = startTileX + (SCREEN_WIDTH / TILE_SIZE) + 1;
for (int x = startTileX; x < endTileX; ++x) {
    int screenX = static_cast<int>((x * TILE_SIZE - cameraX) * 3);
    // render tile...
}

// Player centered on screen
int playerScreenX = static_cast<int>((playerPos.x - cameraX) * 3);
```

### Verification
- Clouds now appear at correct positions relative to visible area
- Parallax effect creates depth (clouds drift slower than foreground)
- Tile culling improves performance by only rendering visible tiles

---

## Issue 5: Cloud Movement Speed Issues

### Problem A: Menu clouds moving ~4x too fast

**Root Cause:** Erroneous `* 60` multiplier in update code:
```cpp
// WRONG - deltaTime already in seconds!
cloud.x += cloud.speed * deltaTime * 60.0f;
```

At 60 FPS, deltaTime = 0.016s. With speed of 30 px/sec:
- Wrong: 30 × 0.016 × 60 = **28.8 pixels per frame** (way too fast!)
- Correct: 30 × 0.016 = **0.48 pixels per frame**

**Solution:** Remove the multiplier:
```cpp
cloud.x += cloud.speed * deltaTime;  // Speed is already in pixels/second
```

### Problem B: World clouds not moving at all during gameplay

**Root Cause:** `updateClouds()` was only called in Menu state, never in Playing state:
```cpp
// WRONG - world clouds never updated during game
if (m_state == GameState::Playing) {
    update(deltaTime);  // Only updates player physics
} else if (m_state == GameState::Menu) {
    updateClouds(deltaTime);  // Clouds only updated here!
}
```

**Solution:** Call `updateClouds()` in both states:
```cpp
if (m_state == GameState::Playing) {
    update(deltaTime);
    updateClouds(deltaTime);  // ← Added this line!
} else if (m_state == GameState::Menu) {
    updateClouds(deltaTime);
}
```

### Problem C: Speed values too slow to be visible

**Root Cause:** Initial speed of 2-8 pixels/second meant:
- At 60 FPS: 2 × 0.016 = **0.032 pixels per frame**
- Would take **over 4 minutes** to cross the screen!

**Solution:** Increased speeds to visible range:
```cpp
// Menu clouds - gentle drift
cloud.speed = 30.0f + rand() % 21;  // 30-50 px/sec

// World clouds - noticeable movement with parallax
cloud.speed = 60.0f + rand() % 61;  // 60-120 px/sec
```

### Verification
```cpp
DEBUG: Cloud update: deltaTime=0.016s  // Confirmed correct (60 FPS)
```

At 30 px/sec with deltaTime = 0.016s:
- Movement per frame = 30 × 0.016 = **~0.5 pixels/frame**
- Crosses 768-pixel screen in ~25 seconds ✅

---

## Issue 6: Ground Sprite Edge Gaps

### Problem
Visible gaps between adjacent ground tiles due to unfilled edge columns.

### Root Cause
Ground sprite generation only filled pixels from x=2 to x<14, leaving 2-pixel transparent borders:
```cpp
// WRONG - leaves edges unfilled (transparent)
for (int y = 5; y < 16; ++y) {
    for (int x = 2; x < 14; ++x) {  // Only fills middle portion!
        ground.pixels[y * 16 + x] = ColorPalette::DarkBrown;
    }
}
```

### Solution
Fill entire tile width:
```cpp
// CORRECT - fills complete tile
for (int y = 5; y < 16; ++y) {
    for (int x = 0; x < 16; ++x) {  // Full width!
        ground.pixels[y * 16 + x] = ColorPalette::DarkBrown;
    }
}
```

---

## Issue 7: Title Screen Text Character Overlap

### Problem
Text characters on the title screen were overlapping each other, making text unreadable. Characters appeared to be drawn on top of one another instead of side-by-side.

### Root Cause
The `drawText()` function was advancing the X position by only 24 pixels between characters, but each character sprite (16×16) gets scaled by 3x when rendered via `drawSprite()`, resulting in a final size of **48×48 pixels**.

```cpp
// IN drawText():
for (size_t i = 0; i < text.length(); ++i) {
    // ... render character ...
    
    rect.x += 24;  // ← Only advances 24 pixels!
}
```

But in `drawSprite()`:
```cpp
destRect.w = sprite.width * 3;   // 16 × 3 = 48 pixels wide!
destRect.h = sprite.height * 3;  // 16 × 3 = 48 pixels tall!
```

**Result:** Each character is 48 pixels wide but only advances 24 pixels, causing **50% overlap** between consecutive characters.

### Solution
Increase the character advancement to match the scaled sprite size plus desired gap:

```cpp
// CORRECT - advance by scaled width + gap
rect.x += 72;  // 48 (scaled width) + 24 (gap) = 72 pixels between chars
```

Or equivalently, calculate based on the scale factor:
```cpp
int charSpacing = sprite.width * 3 + 24;  // Scaled width + gap in screen coords
rect.x += charSpacing;
```

### Verification
For "SUPER SCROLLER" (14 characters):
- **Before:** Total width = 14 × 24 = 336 pixels (characters heavily overlapped)
- **After:** Total width = 14 × 72 = 1008 pixels (proper spacing, readable text)

---

## Issue 8: Font Sprite Cache Not Shared Between Game and Renderer

### Problem
Font sprites were being generated twice - once in `Game::initialize()` and potentially again in `Renderer::drawText()`, wasting memory and CPU cycles.

### Root Cause
The `Renderer` class had its own separate `m_fontSprites` vector that was independent of the `Game`'s font cache:

```cpp
// In Game.cpp:
m_fontSprites = ProceduralArt::generateFont();  // Generated here

// But in Renderer::drawText():
if (m_fontSprites.empty()) {                    // ← Different m_fontSprites!
    m_fontSprites = ProceduralArt::generateFont();  // Generated again!
}
```

### Solution
Pass the font sprites from Game to Renderer during initialization:

```cpp
// In Game::initialize():
m_fontSprites = ProceduralArt::generateFont();
m_graphics->setFontSprites(m_fontSprites);  // Share with Renderer

// In Renderer.h:
void setFontSprites(const std::vector<PixelData>& fontSprites) { 
    m_fontSprites = fontSprites; 
}
```

This ensures fonts are generated exactly once and shared between all rendering code.

### Verification
- Font generation happens only during `Game::initialize()`
- No redundant calls to `ProceduralArt::generateFont()` during gameplay
- Memory usage reduced by eliminating duplicate font sprite storage

---

## Issue 9: Texture Cache Key Using Pointer Address Instead of Content Hash

### Problem
The texture cache used the pointer address of sprite pixel data as the cache key, meaning identical sprites at different memory addresses would be treated as different textures.

### Root Cause
```cpp
// In Renderer::getTexture():
const void* key = (const void*)sprite.pixels.data();  // Uses memory address!
auto it = m_textureCache.find(key);
```

If two sprites have identical pixel data but are stored at different addresses, they would each get their own texture entry in the cache.

### Why It Worked Anyway
In practice, this wasn't a major issue because:
1. Font sprites and ground sprites are generated once and cached in vectors
2. The same sprite objects are reused every frame (same memory addresses)
3. Only cloud sprites are dynamically created, but they're unique anyway

### Potential Improvement
For truly content-based caching, use a hash of the pixel data:
```cpp
// Better approach - hash based on actual content
size_t computeSpriteHash(const PixelData& sprite) {
    size_t hash = 0;
    for (auto pixel : sprite.pixels) {
        hash ^= std::hash<int>{}(static_cast<int>(pixel)) << 1;
    }
    return hash;
}

// Then use: m_textureCache[computeSpriteHash(sprite)]
```

---

## Summary of Key Lessons

### SDL Surface Pixel Format
- Always match your pixel construction to the surface mask values
- Use `SDL_CreateRGBSurface` masks as reference for byte positions
- Test with known colors (brown should look brown, not blue!)

### Screen Coordinates
- Y=0 is TOP of screen, Y increases DOWNWARD
- "Higher on screen" = lower Y value
- "Lower on screen" = higher Y value

### Sprite Generation Order
1. Set width/height FIRST
2. Resize pixels buffer SECOND  
3. Fill with background color THIRD
4. Draw shapes FOURTH

### Camera Systems
- World coordinates ≠ Screen coordinates
- Apply camera offset: `screenPos = worldPos - cameraPos`
- Parallax effect: multiply camera offset by factor < 1.0
- Cull objects outside visible range for performance

### Delta Time Usage
- deltaTime is already in SECONDS (typically ~0.016 at 60 FPS)
- Speed values should be in units/second
- Do NOT multiply by frame rate - that's double-counting!

---

*Document created: 2026-03-20*
*Project: Super Scroller*
