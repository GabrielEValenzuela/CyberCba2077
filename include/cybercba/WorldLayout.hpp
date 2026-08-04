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

// Interior partition walls that split the shelter into distinct zones (entrance/
// rest, workshop, comms, technical corridor). Each pair of entries represents one
// vertical wall split by a doorway gap. Shared between collision (blocksMovement)
// and the procedural architecture drawn in GameApp::drawShelterShell, so the
// visible wall and the collider can never drift apart.
const std::array<WorldRect, 6>& shelterInteriorWalls();
} // namespace cybercba
