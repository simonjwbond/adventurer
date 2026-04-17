# Agent Protocols: Adventurer

This document defines the standards and workflows for AI agents (like Gemini CLI, Claude, etc.) contributing to the Adventurer project.

## 1. Changelog Protocol
Every significant change or fix MUST be recorded in `CHANGELOG.md`. 
Entries must include:
- **Date**: YYYY-MM-DD
- **Time**: HH:MM:SS (UTC or Local)
- **Agent**: The name/model of the agent performing the work.
- **Description**: A concise summary of what was changed and *why*.

### Format:
```markdown
## [YYYY-MM-DD HH:MM:SS] - Agent Name
### Added/Fixed/Changed
- Description of change 1
- Description of change 2
```

## 2. Engineering Standards
- **Surgical Edits**: Prefer `edit` over `write` for existing files to maintain context and avoid overwriting unrelated code.
- **Performance First**: Avoid expensive operations (like texture creation or file I/O) inside the main game loop (`run`, `update`, `render`).
- **Memory Management**: Always clean up SDL resources (`SDL_Texture`, `SDL_Renderer`, `SDL_Window`) and heap-allocated objects in `cleanup()` or destructors.
- **Diagnostics**: Use the `DEBUG_LOG` macro for important state transitions. Ensure stderr output is useful for debugging.
- **Code Organization**: Keep C++ files lean and focused. Target ~1000 lines maximum per file. If a file approaches 2000 lines, split it into smaller, logically separated files. This improves:
  - AI agent readability and context management
  - Code maintainability and navigation
  - Compilation times
  - Single responsibility principle adherence

## 3. Communication
- Always explain the *intent* before calling tools.
- If a task is complex, use a todo list to track sub-tasks.

## 4. Performance & Event Protocol (Critical)

### Rendering Performance
- **Texture Cache**: NEVER create or destroy `SDL_Texture` objects inside the `render()` loop. This was a major project bottleneck that caused the window to freeze. 
- All pixel data must be converted to textures and stored in the `Renderer`'s texture cache during initialization.
- Use the `getCachedTexture()` method in the `Renderer` to retrieve cached textures.

### SDL Event Handling
- **Centralized Polling**: All SDL events MUST be processed through `InputManager::handleEvents()` using a `while(SDL_PollEvent(&event))` loop.
- **Queue Maintenance**: Do not use `SDL_PumpEvents()` or `SDL_GetKeyboardState()` in isolation if you need to handle window events. `SDL_PollEvent` is the only reliable way to clear the OS event queue and prevent the "Not Responding" state.
- **Responsiveness**: Ensure that `SDL_QUIT` and `SDL_WINDOWEVENT_CLOSE` always set the internal quit flag immediately. The game loop must terminate gracefully when requested.

### Frame Rate
- Maintain the 60 FPS target. If adding new features (like audio or complex physics), verify that the frame time logged in the heartbeat does not exceed 16.6ms.

## 5. Build Protocol (MANDATORY)

### ALWAYS Cross-Compile for Windows x64
**EVERY build MUST produce BOTH Linux and Windows executables.** This is non-negotiable.

#### Standard Build Command Sequence:
```bash
# Clean previous builds
rm -rf build build_windows

# Configure both targets
cmake -S . -B build                          # Native Linux build
cmake -S . -B build_windows \
    -DCMAKE_TOOLCHAIN_FILE=tools/mingw-toolchain.cmake  # Windows x64 cross-compile

# Build both targets
make -C build                    # Produces: build/adventurer_linux
make -C build_windows            # Produces: build_windows/adventurer_windows.exe
```

#### Verification Checklist After Every Code Change:
1. ✅ Linux build succeeds without errors
2. ✅ Windows cross-compile succeeds without errors
3. ✅ Both executables are produced in their respective build directories
4. ✅ Linux executable runs and shows correct behavior (if applicable)

#### Prerequisites for Cross-Compilation:
```bash
# Install MinGW-w64 toolchain and SDL2 for Windows
sudo apt update
sudo apt install mingw-w64 libSDL2-mingw-dev
```

#### Why This Matters:
- The project targets **both Linux and Windows** as first-class platforms
- Cross-compiling on every change catches platform-specific issues early
- Ensures the codebase remains truly cross-platform compatible
- Prevents "works on my machine" problems from accumulating

#### Windows Cross-Compilation Troubleshooting:

**Problem**: Cross-compilation failed with `undefined reference to 'WinMain'`.

**Causes**:
- **Entry Point Mismatch**: SDL2 by default redefines `main` to `SDL_main` on Windows, expecting to provide its own `WinMain` (the Windows GUI entry point) which handles initialization.
- **Linkage Issues**: The `libSDL2main.a` library was not being linked correctly, or was linked in the wrong order.
- **Target Conflicts**: Multiple redundant `add_executable` calls for the same output file caused configuration confusion.

**Resolution**:
- **Consolidated Targets**: Merged all Windows build definitions into a single, clean target in `CMakeLists.txt`.
- **Correct Linkage**: Explicitly linked `libSDL2main.a` *before* `libSDL2.a` and included necessary Windows system libraries (`-lsetupapi`, `-lwinmm`, `-limm32`, etc.).
- **Static Runtimes**: Added `-static-libgcc -static-libstdc++ -static` to the linker flags. This embeds the MinGW runtime libraries directly into the executable, eliminating the need to manually copy `libgcc_s_seh-1.dll` and `libstdc++-6.dll` to the target Windows machine.
- **Cross-Platform Sprites**: Refactored `ProceduralArt::createTextureFromPixels` to use `SDL_PIXELFORMAT_RGBA32`. This ensures that manual pixel filling (RGBA order) matches the surface format on both Linux (big/little-endian) and Windows (little-endian), preventing scrambled colors or invisible sprites.
- **Bypassing Redirection**: Added `#define SDL_MAIN_HANDLED` in `src/main.cpp` before including `SDL.h`. This prevents SDL from hijacking the `main` function, allowing the standard console `main` to work with the `-mconsole` linker flag.

#### Required Runtime DLLs (if not using static linking):
If the `-static` flags are removed, the following DLLs from the MinGW toolchain must be present in the same directory as `adventurer_windows.exe`:
- `libgcc_s_seh-1.dll`
- `libstdc++-6.dll`
- `libwinpthread-1.dll` (depending on threading usage)

> **MANDATORY**: Always include `#define SDL_MAIN_HANDLED` before `<SDL2/SDL.h>` in the entry point file to maintain cross-platform compatibility with the current build system.

## 6. Build Verification After Code Changes
After ANY code modification (including `write` or `edit`), you MUST:
1. Rebuild both Linux and Windows targets
2. Verify compilation succeeds for BOTH platforms
3. If compilation fails, debug and repeat until BOTH builds succeed
4. Never stop after a failed compilation - continue debugging until success
