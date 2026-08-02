#include "cybercba/WorldLayout.hpp"

namespace cybercba
{
namespace
{
constexpr std::array<WorldProp, 8> SHELTER {{
    {"safehouse_bed", "prop.shelter.bed", "assets/processed/props/shelter_bed.png", WorldZone::Shelter, WorldLayer::Low, 280, 310, 240, 240, {155, 235, 220, 62}, "", true},
    {"safehouse_shelf", "prop.shelter.shelf", "assets/processed/props/shelter_shelf.png", WorldZone::Shelter, WorldLayer::Mid, 210, 525, 150, 150, {145, 395, 118, 112}, "", true},
    {"safehouse_photo", "prop.shelter.emma_magga_photo", "assets/processed/props/emma_magga_photo.png", WorldZone::Shelter, WorldLayer::High, 340, 440, 76, 76, {0, 0, 0, 0}, "inspect_photo", false},
    {"safehouse_map", "prop.shelter.neo_cordoba_map_panel", "assets/processed/props/neo_cordoba_map_panel.png", WorldZone::Shelter, WorldLayer::High, 550, 195, 220, 150, {445, 125, 220, 38}, "inspect_map", true},
    {"safehouse_window", "prop.shelter.rain_window", "assets/processed/props/rain_window.png", WorldZone::Shelter, WorldLayer::High, 355, 150, 190, 110, {0, 0, 0, 0}, "", false},
    {"safehouse_desk", "prop.shelter.desk", "assets/processed/props/shelter_desk.png", WorldZone::Shelter, WorldLayer::Mid, 800, 335, 220, 220, {710, 250, 190, 67}, "", true},
    {"safehouse_transmitter", "prop.shelter.luciernaga_transmitter", "assets/processed/props/luciernaga_transmitter.png", WorldZone::Shelter, WorldLayer::High, 885, 300, 255, 191, {0, 0, 0, 0}, "inspect_transmitter", false},
    {"safehouse_exit", "prop.shelter.industrial_door", "assets/processed/props/industrial_door.png", WorldZone::Shelter, WorldLayer::High, 1080, 405, 130, 195, {1040, 230, 78, 148}, "exit_shelter", true},
}};

constexpr std::array<WorldProp, 6> EXTERIOR {{
    {"exterior_shelter", "building.shelter.exterior", "assets/processed/buildings/shelter_exterior.png", WorldZone::Exterior, WorldLayer::High, 250, 555, 340, 340, {90, 360, 290, 150}, "", true},
    {"exterior_lamp", "environment.exterior.surveillance_lamp", "assets/processed/environment/surveillance_lamp.png", WorldZone::Exterior, WorldLayer::High, 430, 500, 120, 180, {400, 350, 56, 150}, "", true},
    {"exterior_barricade", "prop.exterior.street_barricade", "assets/processed/props/street_barricade.png", WorldZone::Exterior, WorldLayer::Mid, 610, 535, 250, 166, {490, 445, 240, 55}, "", true},
    {"exterior_terminal", "prop.exterior.access_terminal", "assets/processed/props/access_terminal.png", WorldZone::Exterior, WorldLayer::Mid, 785, 505, 135, 202, {745, 335, 75, 150}, "use_terminal", true},
    {"exterior_drone", "prop.exterior.security_drone", "assets/processed/props/security_drone.png", WorldZone::Exterior, WorldLayer::High, 875, 365, 142, 142, {0, 0, 0, 0}, "face_drone", false},
    {"neometro_entrance", "building.neometro.entrance", "assets/processed/buildings/neometro_entrance.png", WorldZone::Exterior, WorldLayer::High, 1070, 520, 300, 300, {925, 300, 250, 165}, "enter_neometro", true},
}};

bool contains(const WorldRect& rect, float x, float y)
{
    return rect.width > 0.0F && rect.height > 0.0F && x >= rect.x && x <= rect.x + rect.width && y >= rect.y && y <= rect.y + rect.height;
}
} // namespace

const std::array<WorldProp, 8>& shelterProps() { return SHELTER; }
const std::array<WorldProp, 6>& exteriorProps() { return EXTERIOR; }

const WorldProp* worldProp(std::string_view id)
{
    for (const WorldProp& prop : SHELTER) if (id == prop.id) return &prop;
    for (const WorldProp& prop : EXTERIOR) if (id == prop.id) return &prop;
    return nullptr;
}

bool blocksMovement(WorldZone zone, float x, float y)
{
    if (zone == WorldZone::Shelter)
    {
        for (const WorldProp& prop : SHELTER)
            if (prop.blocksMovement && contains(prop.collider, x, y)) return true;
    }
    else
    {
        for (const WorldProp& prop : EXTERIOR)
            if (prop.blocksMovement && contains(prop.collider, x, y)) return true;
    }
    return false;
}

bool isValidSpawn(WorldZone zone, float x, float y)
{
    const WorldRect bounds = zone == WorldZone::Shelter ? WorldRect {118, 152, 1042, 388} : WorldRect {35, 135, 1210, 500};
    return contains(bounds, x, y) && !blocksMovement(zone, x, y);
}
} // namespace cybercba
