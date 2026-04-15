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
    
    // House interior (wood floor)
    for (int y = houseTopY + 1; y < houseBottomY; y++) {
        for (int x = houseLeftX + 1; x < houseRightX; x++) {
            if (x >= 0 && x < currentWidthTiles) {
                tiles[y][x] = TileType::FLOOR_WOOD;
            }
        }
    }
    
    // NOTE: NO TREES, BUSHES, or FENCES - pure open grass field
    // The entire area outside the house is walkable grass
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
            texture = proceduralArt->getGrassTexture();
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
    
    int screenX = offsetX + gridX * TILE_SIZE;
    int screenY = offsetY + gridY * TILE_SIZE;
    
    SDL_Rect destRect;
    destRect.x = screenX;
    destRect.y = screenY;
    destRect.w = TILE_SIZE;
    destRect.h = TILE_SIZE;
    
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
        case TileType::DOOR:
            return false;  // Door is passable
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
