# Adventurer: Project Overview

## Description
A top-down action adventure game inspired by The Legend of Zelda on NES/SNES. The game features room-based exploration, sword combat, enemy AI, and procedural artwork generation.

## Architecture
- **Core Engine**: `src/Game.cpp` / `src/Game.h` (Main Loop, State Management)
- **Input**: `src/InputManager.cpp` / `src/InputManager.h` (8-directional movement tracking)
- **Camera**: `src/Camera.cpp` / `src/Camera.h` (Room-based with smooth transitions)
- **Graphics**: `graphics/Renderer.cpp` (Texture caching, Render ordering)
- **Procedural Art**: `graphics/ProceduralArt.cpp` (Zelda-style tile and sprite generation)
- **World**: `world/TileMap.cpp`, `world/World.cpp` (Multi-room level system)
- **Entities**: `entities/Player.cpp`, `entities/Enemy.cpp` (Entity hierarchy)
- **Combat**: `combat/Attack.cpp`, `combat/DamageSystem.cpp` (Sword combat mechanics)

## Technical Features
- **Resolution**: 512×240 internal resolution with integer scaling (doubled NES width for top-down visibility)
- **Tile System**: 32×32 pixel tiles, room-based level design
- **Movement**: 8-directional with diagonal normalization for fair gameplay
- **Combat**: Sword attacks with cooldowns, enemy AI states (IDLE/CHASE/ATTACK)
- **Performance**: High-efficiency texture caching in the `Renderer` to maintain a stable 60 FPS.
- **Cross-Platform**: SDL2-based with a CMake build system supporting Linux and Windows x64.

## Current Build Targets
- `adventurer_linux`: Native Linux binary.
- `adventurer_windows.exe`: Windows x64 executable (requires MinGW-w64).

## Physics Constants
```cpp
const float PLAYER_SPEED = 2.5f;           // Pixels per frame
const float PLAYER_ACCELERATION = 0.4f;    // Smooth movement start/stop
const float PLAYER_FRICTION = 0.85f;       // Slide to stop
const float DIAGONAL_NORMALIZE = 0.7071f;  // Equal diagonal speed

const int TILE_SIZE = 32;                  // Larger tiles for top-down clarity
const int ROOM_WIDTH_TILES = 16;           // 512/32 = 16 tiles wide
const int ROOM_HEIGHT_TILES = 8;           // 240/32 ≈ 7.5, use 8 with margins
```

## Combat Constants
```cpp
const int MAX_HEALTH = 3;              // Classic Zelda: 3 heart containers start
const float ATTACK_COOLDOWN_FRAMES = 20.0f;
const float ATTACK_ACTIVE_FRAMES = 12.0f;
const float PUSHBACK_FORCE = 8.0f;
```
