#pragma once

#include <array>
#include <string_view>

namespace cybercba
{
enum class WorldZone { Shelter, Exterior };
enum class WorldLayer { Low, Mid, High };

struct WorldRect
{
    float x;
    float y;
    float width;
    float height;
};

struct WorldProp
{
    const char* id;
    const char* assetId;
    const char* assetPath;
    WorldZone zone;
    WorldLayer layer;
    float pivotX;
    float pivotY;
    float width;
    float height;
    WorldRect collider;
    const char* interactionId;
    bool blocksMovement;
};

const std::array<WorldProp, 8>& shelterProps();
const std::array<WorldProp, 6>& exteriorProps();
const WorldProp* worldProp(std::string_view id);
bool blocksMovement(WorldZone zone, float x, float y);
bool isValidSpawn(WorldZone zone, float x, float y);
} // namespace cybercba
