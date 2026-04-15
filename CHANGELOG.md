# Adventurer Changelog

All significant changes to this project are documented in this file.

## [2026-04-03 09:35:00] - Qwen3-Coder-Next Implementation Agent
### Fixed
- **Wall Collision Detection**: Fixed player to properly stop at walls instead of walking through them

### Rationale
The collision detection was broken because the `Game::update()` function was applying movement to `playerPosition` BEFORE checking if that position was valid. This meant:
1. Player position was updated (`playerPosition.x += horizontalInput * moveSpeed`)
2. Then a new future position was calculated from the already-updated position
3. Collision checks were performed on incorrect positions

The fix restructures `Game::update()` to:
1. Calculate the desired new position FIRST
2. Check collision at that position using `checkCollisionBox()`
3. Only update `playerPosition` if no collision is detected
4. This ensures walls are properly respected and player cannot walk through them

---

## [2026-04-03 09:35:00] - Qwen3-Coder-Next Implementation Agent
### Fixed
- **Wall Collision Detection**: Fixed player to properly stop at walls instead of walking through them

### Rationale
The collision detection was broken because the `Game::update()` function was applying movement to `playerPosition` BEFORE checking if that position was valid. This meant:
1. Player position was updated (`playerPosition.x += horizontalInput * moveSpeed`)
2. Then a new future position was calculated from the already-updated position
3. Collision checks were performed on incorrect positions

The fix restructures `Game::update()` to:
1. Calculate the desired new position FIRST
2. Check collision at that position using `checkCollisionBox()`
3. Only update `playerPosition` if no collision is detected
4. This ensures walls are properly respected and player cannot walk through them

---

## [2026-04-02 14:44:00] - Qwen3-Coder-Next Implementation Agent
### Added/Fixed/Changed
- **Performance Optimization - Render-to-Texture (RTT)**: Implemented `TileMap::generateStaticBackground()` to reduce draw calls from 112 per frame to 1 per frame
- Changed pixel format in `ProceduralArt::createTextureFromPixels()` from `SDL_PIXELFORMAT_RGBA32` to `SDL_PIXELFORMAT_ABGR8888` for proper color rendering on Windows (little-endian)
- Removed all `fprintf(stderr, ...)` debug calls from `Game.cpp`, `TileMap.cpp`, and `ProceduralArt.cpp` to eliminate I/O bottlenecks
- Changed CMake default build type to `Release` with `-O3` optimization for both GNU/Clang and MSVC

### Rationale
The game was running extremely slowly ("FPS is dog slow") due to:
1. **Draw call overhead**: 112 individual `SDL_RenderCopy` calls per frame (one per tile) on Windows where driver call overhead is high
2. **Pixel format mismatch**: Using `SDL_PIXELFORMAT_RGBA32` caused color issues on little-endian systems like Windows; `ABGR8888` matches the manual RGBA byte order in memory
3. **I/O bottleneck**: Console I/O (`fprintf`) is synchronous and can block for several milliseconds per call on Windows
4. **No compiler optimizations**: The build was running in Debug mode with no optimization

The fix implements render-to-texture (RTT) where all 112 tiles are drawn once to an off-screen texture during initialization, then the entire room is rendered with a single draw call each frame.

---

## [2026-04-02 13:28:00] - Qwen3.5 Implementation Agent
### Fixed
- **Windows Color Rendering Issue**: Changed pixel format from `SDL_PIXELFORMAT_RGBA8888` to `SDL_PIXELFORMAT_RGBA32` in `ProceduralArt::createTextureFromPixels()`
- The RGBA byte order was being misinterpreted on Windows (little-endian), causing all textures to appear in shades of red
- `SDL_PIXELFORMAT_RGBA32` automatically handles endianness correctly, ensuring consistent color rendering across Linux and Windows

### Rationale
The textures were rendering incorrectly on Windows because `SDL_PIXELFORMAT_RGBA8888` expects pixel data in ABGR order on little-endian systems (Windows), but our code was providing RGBA order. By using `SDL_PIXELFORMAT_RGBA32`, SDL correctly handles the byte order conversion based on the platform's endianness.

---

## [2026-04-02 13:38:00] - Qwen3.5 Implementation Agent
### Fixed
- **Wall Collision Detection**: Implemented proper bounding box collision detection in `TileMap::checkCollisionBox()` and integrated it into `Game::update()`
- Player now properly stops at walls instead of walking through them

### Rationale
The previous implementation only clamped player position to screen boundaries with a hardcoded margin, which didn't account for the actual tile-based wall layout. The fix:
1. Added `checkCollisionBox()` that checks all four corners of the player's bounding box against the tile map
2. Modified `Game::update()` to test X and Y movement separately before applying it
3. This allows smooth movement along walls while properly blocking entry into wall tiles

---

## [2026-04-02 00:56:00] - Qwen3.5 Implementation Agent
### Added
- **Procedural Art System**: Created `graphics/ProceduralArt.cpp/h` for generating wooden floor and stone wall textures programmatically
- **Tile Map System**: Created `world/TileMap.cpp/h` with tile-based rendering and collision detection
- **Enclosed Room Layout**: Implemented a 16x8 room with walls around the perimeter and wooden floor interior
- **Tile Types**: Added `world/TileTypes.h` for tile type enumerations (FLOOR_WOOD, WALL_STONE, corners, edges)

### Changed
- **Game Class**: Updated to use TileMap and ProceduralArt subsystems
- **CMakeLists.txt**: Added new source files and include directories

### Rationale
This adds the visual foundation for the game - a proper room with walls and floor that the player can move around in. The procedural art system generates textures at runtime without needing external image files.

---

## [2026-04-01 00:00:00] - Qwen3.5 Implementation Agent
### Changed
- **Project Rebranding**: Transformed "Super Scroller" (side-scrolling platformer) into "Adventurer" (top-down action adventure)
- **Resolution Update**: Changed from 256×240 to 512×240 for better top-down visibility
- **Tile Size Update**: Increased from 16×16 to 32×32 pixels for clearer top-down perspective
- **Physics Model**: Replaced platformer physics (gravity, jumping) with top-down movement (8-way, no gravity)
- **Architecture Changes**:
  - Added `Camera` system for room-based exploration with smooth transitions
  - Modified `InputManager` to support 8-directional movement tracking
  - Added `Combat` subsystem for sword attacks and damage mechanics
  - Restructured entity hierarchy for Player, Enemy, and Item types
- **Documentation**: Updated PROJECT.md and README.md to reflect new game identity and mechanics

### Rationale
The project was repurposed from a side-scrolling NES platformer to a top-down Zelda-style action adventure. This required fundamental changes to:
1. Resolution (wider screen for situational awareness)
2. Physics model (no gravity, 8-way movement)
3. Camera system (room-based vs scrolling)
4. Core gameplay loop (exploration/combat vs platforming)

---

*Format follows AGENTS.md protocol: Date, Time, Agent Name, Description with rationale.*
