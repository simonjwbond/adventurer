#include "TileMap.h"
#include <cstring>

TileMap::TileMap(SDL_Renderer* renderer, ProceduralArt* art)
    : proceduralArt(art)
{
    // Default to indoor room size
    currentWidthTiles = DEFAULT_ROOM_WIDTH_TILES;
    currentHeightTiles = DEFAULT_ROOM_HEIGHT_TILES;
    memset(tiles, 0, sizeof(tiles));
}

TileMap::~TileMap() {
    if (staticBackgroundTexture != nullptr) {
        SDL_DestroyTexture(staticBackgroundTexture);
        staticBackgroundTexture = nullptr;
    }
}

void TileMap::initialize() {
    // Default to indoor house layout
    loadIndoorHouseLayout();
}

void TileMap::setMapLayout(const char* layoutName) {
    if (strcmp(layoutName, "indoor") == 0 || strcmp(layoutName, "house") == 0) {
        loadIndoorHouseLayout();
    } else if (strcmp(layoutName, "outdoor") == 0) {
        loadOutdoorWorldLayout();
    } else if (strcmp(layoutName, "desert") == 0) {
        loadDesertWorldLayout();
    }
}

void TileMap::loadIndoorHouseLayout() {
    // Set dimensions for indoor room
    currentWidthTiles = DEFAULT_ROOM_WIDTH_TILES;
    currentHeightTiles = DEFAULT_ROOM_HEIGHT_TILES;
    
    // Clear all tiles
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = 0; x < currentWidthTiles; x++) {
            tiles[y][x] = TileType::EMPTY;
        }
    }
    
    // Build walls around perimeter with door at bottom center
    for (int x = 0; x < currentWidthTiles; x++) {
        // Top wall
        tiles[0][x] = TileType::WALL_CORNER_TL;
        if (x > 0 && x < currentWidthTiles - 1) {
            tiles[0][x] = TileType::WALL_EDGE_H;
        }
        
        // Bottom wall with door opening
        int bottomRow = currentHeightTiles - 1;
        if (x == 0) {
            tiles[bottomRow][x] = TileType::WALL_CORNER_BL;
        } else if (x == currentWidthTiles - 1) {
            tiles[bottomRow][x] = TileType::WALL_CORNER_BR;
        } else if (x >= currentWidthTiles / 2 - 1 && x <= currentWidthTiles / 2 + 1) {
            // Door opening in the middle (3 tiles wide)
            tiles[bottomRow][x] = TileType::DOOR;
        } else {
            tiles[bottomRow][x] = TileType::WALL_EDGE_H;
        }
    }
    
    // Side walls
    for (int y = 0; y < currentHeightTiles; y++) {
        if (y == 0 || y == currentHeightTiles - 1) continue;
        
        int rightCol = currentWidthTiles - 1;
        tiles[y][0] = TileType::WALL_EDGE_V;
        tiles[y][rightCol] = TileType::WALL_EDGE_V;
    }
    
    // Fill interior with wooden floor
    for (int y = 1; y < currentHeightTiles - 1; y++) {
        for (int x = 1; x < currentWidthTiles - 1; x++) {
            tiles[y][x] = TileType::FLOOR_WOOD;
        }
    }
}

void TileMap::loadOutdoorWorldLayout() {
    // Set dimensions for large outdoor world (10x bigger than indoor)
    currentWidthTiles = OUTDOOR_WIDTH_TILES;   // 160 tiles
    currentHeightTiles = OUTDOOR_HEIGHT_TILES; // 80 tiles
    
    // Clear all tiles and fill with grass
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = 0; x < currentWidthTiles; x++) {
            tiles[y][x] = TileType::GRASS;
        }
    }
    
    // Place the house in the upper portion of the outdoor area
    // House dimensions: 9 tiles wide x 5 tiles tall (including walls)
    // Positioned at top center
    int houseLeftX = currentWidthTiles / 2 - 4;  // Center horizontally (around x=76)
    int houseTopY = 0;

/**
 * Load desert biome world layout
 */
void TileMap::loadDesertWorldLayout() {
    // Set dimensions for large outdoor desert world
    currentWidthTiles = OUTDOOR_WIDTH_TILES;   // 160 tiles
    currentHeightTiles = OUTDOOR_HEIGHT_TILES; // 80 tiles
    
    // Clear all tiles and fill with sand ground
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = 0; x < currentWidthTiles; x++) {
            // Create varied sand terrain
            int noise = ((x * 17 + y * 31) % 10);
            if (noise < 5) {
                tiles[y][x] = TileType::SAND;
            } else if (noise < 7) {
                tiles[y][x] = TileType::SAND_DARK;
            } else if (noise < 8) {
                tiles[y][x] = TileType::CRACKED_EARTH;
            } else if (noise < 9) {
                tiles[y][x] = TileType::SAND_ROCKY;
            } else {
                tiles[y][x] = TileType::ROCKY_GROUND;
            }
        }
    }
    
    // Add scattered cacti throughout the desert
    // Saguaro cacti (tall, iconic desert cactus)
    for (int i = 0; i < 15; i++) {
        int cx = (i * 37 + 13) % (currentWidthTiles - 4) + 2;
        int cy = (i * 43 + 7) % (currentHeightTiles - 4) + 2;
        
        // Randomly choose cactus type
        int cactusType = (i * 7) % 5;
        switch (cactusType) {
            case 0: tiles[cy][cx] = TileType::CACTUS_SAGUARO; break;
            case 1: tiles[cy][cx] = TileType::CACTUS_SAGUARO_2ARMS; break;
            case 2: tiles[cy][cx] = TileType::CACTUS_SAGUARO_3ARMS; break;
            case 3: tiles[cy][cx] = TileType::CACTUS_PRICKLY_PEAR; break;
            case 4: tiles[cy][cx] = TileType::CACTUS_CHOLLA; break;
        }
    }
    
    // Add desert bushes and vegetation
    for (int i = 0; i < 40; i++) {
        int vx = (i * 23 + 11) % (currentWidthTiles - 2) + 1;
        int vy = (i * 31 + 19) % (currentHeightTiles - 2) + 1;
        
        // Don't overwrite cacti
        if (tiles[vy][vx] >= TileType::CACTUS_SAGUARO && tiles[vy][vx] <= TileType::CACTUS_CHOLLA) {
            continue;
        }
        
        int vegType = (i * 11) % 5;
        switch (vegType) {
            case 0: tiles[vy][vx] = TileType::DESERT_BUSH; break;
            case 1: tiles[vy][vx] = TileType::DESERT_GRASS; break;
            case 2: tiles[vy][vx] = TileType::PALO_VERDE; break;
            case 3: tiles[vy][vx] = TileType::YUCCA; break;
            case 4: tiles[vy][vx] = TileType::AGAVE; break;
        }
    }
    
    // Add rocks and boulders
    for (int i = 0; i < 25; i++) {
        int rx = (i * 29 + 17) % (currentWidthTiles - 2) + 1;
        int ry = (i * 37 + 23) % (currentHeightTiles - 2) + 1;
        
        // Don't overwrite cacti or vegetation
        if (tiles[ry][rx] >= TileType::CACTUS_SAGUARO && tiles[ry][rx] <= TileType::AGAVE) {
            continue;
        }
        
        int rockType = (i * 13) % 4;
        switch (rockType) {
            case 0: tiles[ry][rx] = TileType::ROCK_SMALL; break;
            case 1: tiles[ry][rx] = TileType::ROCK_MEDIUM; break;
            case 2: tiles[ry][rx] = TileType::ROCK_LARGE; break;
            case 3: tiles[ry][rx] = TileType::ROCK_FORMATION; break;
        }
    }
    
    // Add desert structures
    // Adobe ruins in the center-left area
    int ruinX = 30;
    int ruinY = 35;
    tiles[ruinY][ruinX] = TileType::ADOBE_RUIN;
    tiles[ruinY][ruinX + 1] = TileType::ADOBE_WALL;
    tiles[ruinY][ruinX + 2] = TileType::ADOBE_WALL;
    tiles[ruinY + 1][ruinX] = TileType::ADOBE_WALL;
    tiles[ruinY + 1][ruinX + 1] = TileType::ADOBE_WALL;
    
    // Desert well near center
    int wellX = 78;
    int wellY = 40;
    tiles[wellY][wellX] = TileType::DESERT_WELL;
    
    // Cave entrance in the upper right
    int caveX = 120;
    int caveY = 15;
    tiles[caveY][caveX] = TileType::CAVE_ENTRANCE;
    
    // Campfire site in the lower area
    int fireX = 50;
    int fireY = 65;
    tiles[fireY][fireX] = TileType::CAMPFIRE_SITE;
    
    // Add decorative details (bones, tumbleweeds, dead trees)
    for (int i = 0; i < 20; i++) {
        int dx = (i * 31 + 23) % (currentWidthTiles - 2) + 1;
        int dy = (i * 41 + 31) % (currentHeightTiles - 2) + 1;
        
        // Don't overwrite important features
        if (tiles[dy][dx] >= TileType::CACTUS_SAGUARO && tiles[dy][dx] <= TileType::CAMPFIRE_SITE) {
            continue;
        }
        
        int detailType = (i * 17) % 5;
        switch (detailType) {
            case 0: tiles[dy][dx] = TileType::BONES; break;
            case 1: tiles[dy][dx] = TileType::TUMBLEWEED; break;
            case 2: tiles[dy][dx] = TileType::DEAD_TREE; break;
            case 3: tiles[dy][dx] = TileType::SCORPION; break;
            case 4: tiles[dy][dx] = TileType::SNAKE; break;
        }
    }
}
    int houseRightX = houseLeftX + 8;
    int houseBottomY = houseTopY + 4;
    
    // Top wall
    for (int x = houseLeftX; x <= houseRightX; x++) {
        if (x >= 0 && x < currentWidthTiles) {
            if (x == houseLeftX) tiles[houseTopY][x] = TileType::WALL_CORNER_TL;
            else if (x == houseRightX) tiles[houseTopY][x] = TileType::WALL_CORNER_TR;
            else tiles[houseTopY][x] = TileType::WALL_EDGE_H;
        }
    }
    
    // Bottom wall with door opening in the center
    for (int x = houseLeftX; x <= houseRightX; x++) {
        if (x >= 0 && x < currentWidthTiles) {
            if (x == houseLeftX) {
                tiles[houseBottomY][x] = TileType::WALL_CORNER_BL;
            } else if (x == houseRightX) {
                tiles[houseBottomY][x] = TileType::WALL_CORNER_BR;
            } else if (x == houseLeftX + 3 || x == houseLeftX + 4 || x == houseLeftX + 5) {
                // Door in the center (3 tiles wide)
                tiles[houseBottomY][x] = TileType::DOOR;
            } else {
                tiles[houseBottomY][x] = TileType::WALL_EDGE_H;
            }
        }
    }
    
    // Side walls
    for (int y = houseTopY + 1; y < houseBottomY; y++) {
        if (houseLeftX >= 0 && houseLeftX < currentWidthTiles) {
            tiles[y][houseLeftX] = TileType::WALL_EDGE_V;
        }
        if (houseRightX >= 0 && houseRightX < currentWidthTiles) {
            tiles[y][houseRightX] = TileType::WALL_EDGE_V;
        }
    }
    
    // House interior - ROOF tiles (visible when outside)
    for (int y = houseTopY + 1; y < houseBottomY; y++) {
        for (int x = houseLeftX + 1; x < houseRightX; x++) {
            if (x >= 0 && x < currentWidthTiles) {
                tiles[y][x] = TileType::ROOF;
            }
        }
    }
    
    // Add tree border around the map edges
    // Left border (full height)
    for (int y = 0; y < currentHeightTiles; y++) {
        if (0 < currentWidthTiles) {
            tiles[y][0] = TileType::TREE_LEFT;
        }
    }
    
    // Right border (full height)
    for (int y = 0; y < currentHeightTiles; y++) {
        if (currentWidthTiles - 1 >= 0) {
            tiles[y][currentWidthTiles - 1] = TileType::TREE_RIGHT;
        }
    }
    
    // Top border (excluding corners, already set by left/right borders)
    for (int x = 1; x < currentWidthTiles - 1; x++) {
        if (x >= 0 && x < currentWidthTiles) {
            tiles[0][x] = TileType::TREE_TOP;
        }
    }
    
    // Bottom border (excluding corners)
    for (int x = 1; x < currentWidthTiles - 1; x++) {
        if (x >= 0 && x < currentWidthTiles) {
            tiles[currentHeightTiles - 1][x] = TileType::TREE_BOTTOM;
        }
    }
    
    // Corner trees (override the border trees at corners)
    if (0 < currentWidthTiles && 0 < currentHeightTiles) {
        tiles[0][0] = TileType::TREE_CORNER_TL;  // Top-left
    }
    if (currentWidthTiles - 1 >= 0 && 0 < currentHeightTiles) {
        tiles[0][currentWidthTiles - 1] = TileType::TREE_CORNER_TR;  // Top-right
    }
    if (0 < currentWidthTiles && currentHeightTiles - 1 >= 0) {
        tiles[currentHeightTiles - 1][0] = TileType::TREE_CORNER_BL;  // Bottom-left
    }
    if (currentWidthTiles - 1 >= 0 && currentHeightTiles - 1 >= 0) {
        tiles[currentHeightTiles - 1][currentWidthTiles - 1] = TileType::TREE_CORNER_BR;  // Bottom-right
    }
    
    // Add ice biome on the right side of the map (last 20 tiles)
    int iceStartX = currentWidthTiles - 21;  // Start ice 20 tiles from the right edge
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = iceStartX; x < currentWidthTiles - 1; x++) {  // Exclude rightmost border tree
            // Randomly select one of 9 ice variations for natural variety
            int iceIndex = (x + y * 3) % 9;  // Pseudo-random based on position
            tiles[y][x] = (TileType)iceIndex;  // ICE_1 through ICE_9
        }
    }
    
    // Add snow tiles in a transition zone between grass and ice
    int snowStartX = iceStartX - 3;
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = snowStartX; x < iceStartX; x++) {
            if (x >= 0 && x < currentWidthTiles) {
                // Mix of grass and snow for transition
                if ((x + y) % 2 == 0) {
                    tiles[y][x] = TileType::SNOW;
                }
            }
        }
    }
}

void TileMap::render(SDL_Renderer* renderer, int offsetX, int offsetY) {
    // If static background texture exists, use it (optimized rendering)
    if (staticBackgroundTexture != nullptr) {
        SDL_Rect destRect = {offsetX, offsetY, currentWidthTiles * TILE_SIZE, currentHeightTiles * TILE_SIZE};
        SDL_RenderCopy(renderer, staticBackgroundTexture, nullptr, &destRect);
        return;
    }
    
    // Fallback: render individual tiles (used during initialization before background is generated)
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = 0; x < currentWidthTiles; x++) {
            drawTile(renderer, x, y, offsetX, offsetY);
        }
    }
}

void TileMap::drawTile(SDL_Renderer* renderer, int gridX, int gridY, int offsetX, int offsetY) {
    SDL_Texture* texture = nullptr;
    
    switch (tiles[gridY][gridX]) {
        case TileType::FLOOR_WOOD:
            texture = proceduralArt->getWoodFloorTexture();
            break;
        case TileType::GRASS:
            // Randomly select one of 9 grass variations for natural variety
            {
                int grassIndex = (gridX + gridY) % 9;  // Pseudo-random based on position
                texture = proceduralArt->getGrassVariation(grassIndex);
            }
            break;
        case TileType::SNOW:
            texture = proceduralArt->getSnowTexture();
            break;
        case TileType::ICE_1:
        case TileType::ICE_2:
        case TileType::ICE_3:
        case TileType::ICE_4:
        case TileType::ICE_5:
        case TileType::ICE_6:
        case TileType::ICE_7:
        case TileType::ICE_8:
        case TileType::ICE_9:
            // Map tile types to ice indices (ICE_1 = 0, ICE_2 = 1, etc.)
            {
                int iceIndex = static_cast<int>(tiles[gridY][gridX]) - static_cast<int>(TileType::ICE_1);
                if (iceIndex < 0 || iceIndex > 8) iceIndex = 0;
                texture = proceduralArt->getIceVariation(iceIndex);
            }
            break;
        case TileType::ROOF:
            texture = proceduralArt->getRoofTexture();
            break;
        case TileType::TREE:
            // Tree is 48x64, so we need special handling for centering
            texture = proceduralArt->getTreeTexture();
            {
                int screenX = offsetX + gridX * TILE_SIZE;
                int screenY = offsetY + gridY * TILE_SIZE;
                
                SDL_Rect destRect;
                destRect.x = screenX;  // Center horizontally in tile
                destRect.y = screenY - 16; // Offset up slightly for full tree height
                destRect.w = 48;
                destRect.h = 64;
                
                SDL_RenderCopy(renderer, texture, nullptr, &destRect);
            }
            return;  // Special case, return early
        case TileType::TREE_LEFT:
        case TileType::TREE_RIGHT:
        case TileType::TREE_TOP:
        case TileType::TREE_BOTTOM:
        case TileType::TREE_CORNER_TL:
        case TileType::TREE_CORNER_TR:
        case TileType::TREE_CORNER_BL:
        case TileType::TREE_CORNER_BR:
            // Border trees - 48x64, positioned based on type
            {
                int borderIndex = 0;
                switch (tiles[gridY][gridX]) {
                    case TileType::TREE_LEFT: borderIndex = 0; break;
                    case TileType::TREE_RIGHT: borderIndex = 1; break;
                    case TileType::TREE_TOP: borderIndex = 2; break;
                    case TileType::TREE_BOTTOM: borderIndex = 3; break;
                    case TileType::TREE_CORNER_TL: borderIndex = 4; break;
                    case TileType::TREE_CORNER_TR: borderIndex = 5; break;
                    case TileType::TREE_CORNER_BL: borderIndex = 6; break;
                    case TileType::TREE_CORNER_BR: borderIndex = 7; break;
                    default: borderIndex = 0; break;
                }
                
                texture = proceduralArt->getTreeBorder(borderIndex);
                int screenX = offsetX + gridX * TILE_SIZE;
                int screenY = offsetY + gridY * TILE_SIZE;
                
                SDL_Rect destRect;
                destRect.x = screenX;
                destRect.y = screenY - 16; // Offset up for full tree height
                destRect.w = 48;
                destRect.h = 64;
                
                SDL_RenderCopy(renderer, texture, nullptr, &destRect);
            }
            return;  // Special case, return early
        case TileType::BUSH:
            texture = proceduralArt->getBushTexture();
            break;
        case TileType::FENCE:
            texture = proceduralArt->getFenceTexture();
            break;
        case TileType::WALL_STONE:
            texture = proceduralArt->getStoneWallTexture();
            break;
        case TileType::WALL_CORNER_TL:
            texture = proceduralArt->getWallCornerTopLeft();
            break;
        case TileType::WALL_CORNER_TR:
            texture = proceduralArt->getWallCornerTopRight();
            break;
        case TileType::WALL_CORNER_BL:
            texture = proceduralArt->getWallCornerBottomLeft();
            break;
        case TileType::WALL_CORNER_BR:
            texture = proceduralArt->getWallCornerBottomRight();
            break;
        case TileType::WALL_EDGE_H:
            texture = proceduralArt->getWallEdgeHorizontal();
            break;
        case TileType::WALL_EDGE_V:
            texture = proceduralArt->getWallEdgeVertical();
            break;
        case TileType::DOOR:
            // Door uses wood floor texture but with a darker center
            texture = proceduralArt->getWoodFloorTexture();
            break;
        default:
            return;
    }
    
    if (texture == nullptr) return;
    
    // Grass variations are 96px (3x original 32px), other tiles use TILE_SIZE (48px)
    int tileSize = (tiles[gridY][gridX] == TileType::GRASS) ? 96 : TILE_SIZE;
    
    int screenX = offsetX + gridX * TILE_SIZE;
    int screenY = offsetY + gridY * TILE_SIZE;
    
    SDL_Rect destRect;
    destRect.x = screenX;
    destRect.y = screenY;
    destRect.w = tileSize;
    destRect.h = tileSize;
    
    SDL_RenderCopy(renderer, texture, nullptr, &destRect);
    
    // Draw door details (darker center for wood door)
    if (tiles[gridY][gridX] == TileType::DOOR) {
        SDL_SetRenderDrawColor(renderer, 80, 50, 30, 255);
        SDL_Rect doorRect;
        doorRect.x = screenX + 4;
        doorRect.y = screenY + 4;
        doorRect.w = 24;
        doorRect.h = 24;
        SDL_RenderFillRect(renderer, &doorRect);
    }
}

bool TileMap::checkCollision(float x, float y) const {
    int gridX = static_cast<int>(x / TILE_SIZE);
    int gridY = static_cast<int>(y / TILE_SIZE);
    
    if (gridX < 0 || gridX >= currentWidthTiles ||
        gridY < 0 || gridY >= currentHeightTiles) {
        return true;  // Out of bounds is solid
    }
    
    TileType tile = tiles[gridY][gridX];
    
    switch (tile) {
        case TileType::EMPTY:
            return false;
        case TileType::FLOOR_WOOD:
            return false;
        case TileType::GRASS:
            return false;
        case TileType::ROOF:
            return false;  // Roof is walkable when outside
        case TileType::DOOR:
            return false;  // Door is passable
        case TileType::TREE:
        case TileType::TREE_LEFT:
        case TileType::TREE_RIGHT:
        case TileType::TREE_TOP:
        case TileType::TREE_BOTTOM:
        case TileType::TREE_CORNER_TL:
        case TileType::TREE_CORNER_TR:
        case TileType::TREE_CORNER_BL:
        case TileType::TREE_CORNER_BR:
            return false;  // Trees are non-colliding decorations
        case TileType::BUSH:
            return false;  // Bushes are non-colliding decorations
        case TileType::FENCE:
            return false;  // Fences are non-colliding decorations
        default:
            return true;  // Walls are solid
    }
}

bool TileMap::checkCollisionBox(float x, float y, int width, int height) const {
    // Check all four corners of the bounding box
    const float INSET = 2.0f;
    
    if (checkCollision(x + INSET, y + INSET)) return true;
    if (checkCollision(x + width - INSET, y + INSET)) return true;
    if (checkCollision(x + INSET, y + height - INSET)) return true;
    if (checkCollision(x + width - INSET, y + height - INSET)) return true;
    
    return false;
}

TileType TileMap::getTile(int gridX, int gridY) const {
    if (gridX < 0 || gridX >= currentWidthTiles ||
        gridY < 0 || gridY >= currentHeightTiles) {
        return TileType::EMPTY;
    }
    return tiles[gridY][gridX];
}

void TileMap::setTile(int gridX, int gridY, TileType type) {
    if (gridX < 0 || gridX >= currentWidthTiles ||
        gridY < 0 || gridY >= currentHeightTiles) {
        return;
    }
    tiles[gridY][gridX] = type;
}

void TileMap::generateStaticBackground(SDL_Renderer* renderer) {
    // Create a render target texture for the entire background
    staticBackgroundTexture = SDL_CreateTexture(
        renderer,
        SDL_PIXELFORMAT_ABGR8888,
        SDL_TEXTUREACCESS_TARGET,
        currentWidthTiles * TILE_SIZE,
        currentHeightTiles * TILE_SIZE
    );
    
    if (staticBackgroundTexture == nullptr) {
        return;
    }
    
    // Set the renderer target to our background texture
    SDL_SetRenderTarget(renderer, staticBackgroundTexture);
    SDL_SetTextureBlendMode(staticBackgroundTexture, SDL_BLENDMODE_NONE);
    
    // Clear with transparent black first
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderClear(renderer);
    
    // Draw all tiles once onto the background texture
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = 0; x < currentWidthTiles; x++) {
            drawTile(renderer, x, y, 0, 0);
        }
    }
    
    // Reset render target to screen
    SDL_SetRenderTarget(renderer, nullptr);
}

bool TileMap::getDoorPosition(float& doorX, float& doorY) const {
    // Find the door tile
    for (int y = 0; y < currentHeightTiles; y++) {
        for (int x = 0; x < currentWidthTiles; x++) {
            if (tiles[y][x] == TileType::DOOR) {
                doorX = (x + 0.5f) * TILE_SIZE;
                doorY = (y + 0.5f) * TILE_SIZE;
                return true;
            }
        }
    }
    return false;
}

bool TileMap::isDoorTile(int gridX, int gridY) const {
    if (gridX < 0 || gridX >= currentWidthTiles ||
        gridY < 0 || gridY >= currentHeightTiles) {
        return false;
    }
    return tiles[gridY][gridX] == TileType::DOOR;
}
