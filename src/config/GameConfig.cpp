#include "config/GameConfig.hpp"

#include <algorithm>
#include <exception>
#include <fstream>
#include <string>

namespace
{
    bool parsePositiveInt(const std::string& value, int& destination)
    {
        try
        {
            const int parsed = std::stoi(value);
            if (parsed > 0)
            {
                destination = parsed;
                return true;
            }
        }
        catch (const std::exception&)
        {
        }
        return false;
    }

    bool parseNonNegativeFloat(const std::string& value, float& destination)
    {
        try
        {
            const float parsed = std::stof(value);
            if (parsed >= 0.0F)
            {
                destination = parsed;
                return true;
            }
        }
        catch (const std::exception&)
        {
        }
        return false;
    }
} // namespace

GameConfig::GameConfig()
    : m_windowTitle("Cyber CBA")
    , m_screenWidth(1280)
    , m_screenHeight(720)
    , m_targetFps(60)
    , m_splashSeconds(2.0F)
    , m_isDevelopmentMode(false)
    , m_savePath("cybercba.save")
    , m_rainEnabled(true)
    , m_rainDensity(54)
    , m_rainIntensity(0.65F)
{
}

bool GameConfig::loadFromFile(const char* path)
{
    std::ifstream file(path);
    if (!file)
    {
        return false;
    }
    std::string line;
    while (std::getline(file, line))
    {
        if (line.empty() || line.front() == '#')
        {
            continue;
        }
        const std::string::size_type separator = line.find('=');
        if (separator == std::string::npos)
        {
            continue;
        }
        const std::string key = line.substr(0, separator);
        const std::string value = line.substr(separator + 1);
        if (key == "window_title" && !value.empty())
        {
            m_windowTitle = value;
        }
        else if (key == "screen_width")
        {
            parsePositiveInt(value, m_screenWidth);
        }
        else if (key == "screen_height")
        {
            parsePositiveInt(value, m_screenHeight);
        }
        else if (key == "target_fps")
        {
            parsePositiveInt(value, m_targetFps);
        }
        else if (key == "splash_seconds")
        {
            parseNonNegativeFloat(value, m_splashSeconds);
        }
        else if (key == "ui_mode")
        {
            m_isDevelopmentMode = value == "development";
        }
        else if (key == "save_path" && !value.empty())
        {
            m_savePath = value;
        }
        else if (key == "rain_enabled")
        {
            m_rainEnabled = value != "false" && value != "0";
        }
        else if (key == "rain_density")
        {
            int density = m_rainDensity;
            if (parsePositiveInt(value, density)) m_rainDensity = std::min(density, 120);
        }
        else if (key == "rain_intensity")
        {
            float intensity = m_rainIntensity;
            if (parseNonNegativeFloat(value, intensity)) m_rainIntensity = std::min(intensity, 1.0F);
        }
    }
    return true;
}

const std::string& GameConfig::windowTitle() const
{
    return m_windowTitle;
}

int GameConfig::screenWidth() const
{
    return m_screenWidth;
}

int GameConfig::screenHeight() const
{
    return m_screenHeight;
}

int GameConfig::targetFps() const
{
    return m_targetFps;
}

float GameConfig::splashSeconds() const
{
    return m_splashSeconds;
}

bool GameConfig::isDevelopmentMode() const
{
    return m_isDevelopmentMode;
}

const std::string& GameConfig::savePath() const { return m_savePath; }
bool GameConfig::rainEnabled() const { return m_rainEnabled; }
int GameConfig::rainDensity() const { return m_rainDensity; }
float GameConfig::rainIntensity() const { return m_rainIntensity; }
