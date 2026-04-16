#include "Settings.h"
#include "Constants.h"
#include <fstream>
#include <sstream>
#include <cstdio>

Settings::Settings()
    : playerSpeed(2.5f)
    , playerWalkSpeed(2.0f)
    , playerRunSpeed(3.5f)
    , cameraSmoothness(0.15f)
    , cameraZoomLevel(1.0f)
    , masterVolume(0.8f)
    , musicVolume(0.6f)
    , sfxVolume(0.7f)
    , renderDistance(16)
    , particleCount(100)
    , enableShadows(true)
{
}

Settings::~Settings() {
}

std::string Settings::getDefaultPath() {
    return "game_settings.json";
}

std::string Settings::extractValue(const std::string& json, const std::string& key) {
    std::string searchKey = "\"" + key + "\"";
    size_t keyPos = json.find(searchKey);
    if (keyPos == std::string::npos) {
        return "";
    }
    
    // Find the colon after the key
    size_t colonPos = json.find(':', keyPos);
    if (colonPos == std::string::npos) {
        return "";
    }
    
    // Skip whitespace after colon
    size_t valueStart = colonPos + 1;
    while (valueStart < json.length() && (json[valueStart] == ' ' || json[valueStart] == '\t')) {
        valueStart++;
    }
    
    // Extract the value
    std::string result;
    if (valueStart < json.length()) {
        // Check if it's a string value
        if (json[valueStart] == '"') {
            size_t stringStart = valueStart + 1;
            size_t stringEnd = json.find('"', stringStart);
            if (stringEnd != std::string::npos) {
                result = json.substr(stringStart, stringEnd - stringStart);
            }
        } else {
            // Numeric or boolean value
            size_t valueEnd = valueStart;
            while (valueEnd < json.length() && 
                   json[valueEnd] != ',' && 
                   json[valueEnd] != '}' && 
                   json[valueEnd] != '\n' &&
                   json[valueEnd] != ' ') {
                valueEnd++;
            }
            result = json.substr(valueStart, valueEnd - valueStart);
        }
    }
    
    return result;
}

bool Settings::loadFromFile(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        printf("Settings: Could not open file %s, using defaults\n", filepath.c_str());
        return false;
    }
    
    // Read entire file into string
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string json = buffer.str();
    file.close();
    
    // Parse player settings
    std::string val;
    
    val = extractValue(json, "player_speed");
    if (!val.empty()) {
        playerSpeed = std::stof(val);
    }
    
    val = extractValue(json, "player_walk_speed");
    if (!val.empty()) {
        playerWalkSpeed = std::stof(val);
    }
    
    val = extractValue(json, "player_run_speed");
    if (!val.empty()) {
        playerRunSpeed = std::stof(val);
    }
    
    // Parse camera settings
    val = extractValue(json, "smoothness");
    if (!val.empty()) {
        cameraSmoothness = std::stof(val);
    }
    
    val = extractValue(json, "zoom_level");
    if (!val.empty()) {
        cameraZoomLevel = std::stof(val);
    }
    
    // Parse audio settings
    val = extractValue(json, "master_volume");
    if (!val.empty()) {
        masterVolume = std::stof(val);
    }
    
    val = extractValue(json, "music_volume");
    if (!val.empty()) {
        musicVolume = std::stof(val);
    }
    
    val = extractValue(json, "sfx_volume");
    if (!val.empty()) {
        sfxVolume = std::stof(val);
    }
    
    // Parse graphics settings
    val = extractValue(json, "render_distance");
    if (!val.empty()) {
        renderDistance = std::stoi(val);
    }
    
    val = extractValue(json, "particle_count");
    if (!val.empty()) {
        particleCount = std::stoi(val);
    }
    
    val = extractValue(json, "enable_shadows");
    if (!val.empty()) {
        enableShadows = (val == "true" || val == "1");
    }
    
    printf("Settings: Loaded from %s\n", filepath.c_str());
    printf("  Player speed: %.1f\n", playerSpeed);
    printf("  Camera smoothness: %.2f\n", cameraSmoothness);
    
    return true;
}
