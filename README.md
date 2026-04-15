# Adventurer - C++ Top-Down Action Adventure

A top-down action adventure game inspired by The Legend of Zelda on NES/SNES, written in C++ with procedural artwork generation.

## Features

- **Top-Down Exploration**: Room-based level design with smooth camera transitions
- **8-Way Movement**: Smooth movement in all directions with diagonal normalization
- **Sword Combat**: Attack mechanics with cooldowns and hit detection
- **Enemy AI**: Basic enemy behaviors (IDLE, CHASE states)
- **Procedural Artwork**: All tiles and sprites are generated programmatically
- **Tile-based Collision**: 32x32 pixel grid collision detection
- **Cross-platform**: Native Linux build + Windows x64 cross-compilation

## Building on Linux

### Prerequisites

```bash
sudo apt update
sudo apt install cmake g++ libsdl2-dev libsdl2-ttf-dev
```

### Build Steps

```bash
mkdir build && cd build
cmake ..
make
./adventurer_linux
```

## Cross-Compilation for Windows

### Prerequisites

You need SDL2 development libraries installed for MinGW:

```bash
sudo apt install mingw-w64-x86-64-dev libSDL2-mingw-dev
```

### Build Steps

```bash
mkdir build_windows && cd build_windows
cmake -DCMAKE_TOOLCHAIN_FILE=../tools/mingw-toolchain.cmake ..
make
# This produces: adventurer_windows.exe
```

## Project Structure

```
adventurer/
├── CMakeLists.txt                    # Build configuration
├── tools/
│   └── mingw-toolchain.cmake        # Windows cross-compilation toolchain
├── src/                              # Core engine files
│   ├── main.cpp                      # Entry point
│   ├── Game.h/cpp                    # Main game loop and state machine
│   ├── InputManager.h/cpp            # Keyboard input handling
│   └── Camera.h/cpp                  # Room-based camera system
├── utils/
│   ├── Constants.h                   # Game constants (physics, dimensions)
│   ├── MathUtils.h                   # Vector2D, AABB classes
│   ├── Random.h                      # Seeded random number generator
│   └── Timer.h                       # Delta time tracking
├── graphics/
│   ├── Renderer.h/cpp                # SDL2 rendering wrapper
│   └── ProceduralArt.h/cpp           # Tile and sprite generation
├── world/
│   ├── TileMap.h/cpp                 # Single room tile data
│   ├── World.h/cpp                   # Multi-room world management
│   └── TileTypes.h                   # Tile type enumerations
├── entities/
│   ├── Entity.h/cpp                  # Base entity class
│   ├── Player.h/cpp                  # Player character
│   ├── Enemy.h/cpp                   # Enemy AI and behavior
│   └── Item.h/cpp                    # Collectibles (rupees, hearts)
├── combat/
│   ├── Attack.h/cpp                  # Attack animation and hitbox
│   └── DamageSystem.h/cpp            # Hit detection and damage
└── audio/
    └── AudioEngine.h/cpp             # FM synthesis SFX generator
```

## Controls

- **Arrow Keys / WASD**: Move in 8 directions
- **Z Button**: Attack with sword
- **P / Escape**: Pause game
- **Q / F4**: Quit

## Technical Specifications

### Display & Resolution
- Target Resolution: 512×240 (doubled NES width for top-down visibility)
- Frame Rate: 60 FPS
- Tile Size: 32×32 pixels
- Room Size: 16×8 tiles (full screen per room)

### Physics Constants (Top-Down)
- Player Speed: 2.5 pixels/frame
- Acceleration: 0.4 pixels/frame²
- Friction: 0.85 (slide factor)
- Diagonal Normalization: 0.7071 (1/√2 for equal diagonal speed)

### Combat Constants
- Max Health: 3 heart containers (classic Zelda start)
- Attack Cooldown: 20 frames (~0.33 seconds)
- Attack Duration: 12 frames
- Pushback Force: 8.0 pixels on hit

## Development Status

- ✅ Core game loop with 60 FPS timing
- ⏳ Input handling (8-way movement)
- ⏳ Camera system with room transitions
- ⏳ Tile-based collision detection
- ⏳ Player controller with top-down physics
- ⏳ Procedural artwork generation for tiles and sprites
- ⏳ Sword combat mechanics
- ⏳ Enemy AI (IDLE/CHASE states)
- ⏳ Item collectibles (rupees, hearts)
- ⏳ Audio engine with FM synthesis

## Design Philosophy

This game aims to capture the essence of classic top-down action adventures:

1. **Exploration**: Room-based level design encourages discovery
2. **Combat**: Simple but satisfying sword mechanics
3. **Progression**: Collect items, defeat enemies, explore further
4. **Authenticity**: Retro aesthetics with modern polish (smooth transitions)

## License

MIT License - see LICENSE file for details.
