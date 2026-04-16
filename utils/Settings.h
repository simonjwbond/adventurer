#ifndef SETTINGS_H
#define SETTINGS_H

#include <string>

/**
 * Game Settings - Load and manage configuration from JSON file
 */
class Settings {
public:
    // Player settings
    float playerSpeed;
    float playerWalkSpeed;
    float playerRunSpeed;
    
    // Camera settings
    float cameraSmoothness;
    float cameraZoomLevel;
    
    // Audio settings
    float masterVolume;
    float musicVolume;
    float sfxVolume;
    
    // Graphics settings
    int renderDistance;
    int particleCount;
    bool enableShadows;
    
    Settings();
    ~Settings();
    
    /**
     * Load settings from JSON file
     * @param filepath Path to the JSON settings file
     * @return true if loaded successfully, false otherwise
     */
    bool loadFromFile(const std::string& filepath);
    
    /**
     * Get the default settings path
     * @return Path to default settings file
     */
    static std::string getDefaultPath();

private:
    /**
     * Simple JSON value extraction (for basic key-value parsing)
     */
    std::string extractValue(const std::string& json, const std::string& key);
};

#endif // SETTINGS_H
