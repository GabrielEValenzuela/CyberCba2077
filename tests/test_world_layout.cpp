#include <gtest/gtest.h>

#include "cybercba/WorldLayout.hpp"

namespace
{
// Default player spawn used by GameApp::start()/resetCheckpoint().
constexpr float kSpawnX = 300.0F;
constexpr float kSpawnY = 470.0F;
} // namespace

TEST(WorldLayout, DefaultSpawnIsValid)
{
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, kSpawnX, kSpawnY));
}

TEST(WorldLayout, InteriorWallsBlockMovement)
{
    for (const cybercba::WorldRect& wall : cybercba::shelterInteriorWalls())
    {
        const float midX = wall.x + wall.width / 2.0F;
        const float midY = wall.y + wall.height / 2.0F;
        EXPECT_TRUE(cybercba::blocksMovement(cybercba::WorldZone::Shelter, midX, midY))
            << "wall at x=" << wall.x << " y=" << wall.y << " should block movement";
    }
}

TEST(WorldLayout, DoorwayGapsAreWalkable)
{
    // Each pair of wall segments (upper, lower) leaves a gap between them; the
    // midpoint of that gap must remain walkable so every zone stays reachable.
    const auto& walls = cybercba::shelterInteriorWalls();
    for (std::size_t i = 0; i + 1 < walls.size(); i += 2)
    {
        const cybercba::WorldRect& upper = walls[i];
        const cybercba::WorldRect& lower = walls[i + 1];
        const float gapX = upper.x + upper.width / 2.0F;
        const float gapY = (upper.y + upper.height + lower.y) / 2.0F;
        EXPECT_FALSE(cybercba::blocksMovement(cybercba::WorldZone::Shelter, gapX, gapY))
            << "doorway gap at wall x=" << upper.x << " should be walkable";
    }
}

TEST(WorldLayout, EachZoneHasAWalkableSpawnPoint)
{
    // One representative point per zone (Entrada/Descanso, Taller, Comunicaciones,
    // Pasillo) confirms the partition walls did not accidentally seal off a room.
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, 320.0F, 360.0F));  // entrada/descanso (clear of bed/shelf colliders)
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, 560.0F, 470.0F));  // taller
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, 850.0F, 400.0F));  // comunicaciones
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, 1100.0F, 430.0F)); // pasillo
}

TEST(WorldLayout, RelocatedDeskColliderBlocksMovement)
{
    const cybercba::WorldProp* desk = cybercba::worldProp("safehouse_desk");
    ASSERT_NE(desk, nullptr);
    const float insideX = desk->collider.x + desk->collider.width / 2.0F;
    const float insideY = desk->collider.y + desk->collider.height / 2.0F;
    EXPECT_TRUE(cybercba::blocksMovement(cybercba::WorldZone::Shelter, insideX, insideY));
    // The desk must sit inside the Taller zone (between the first two partition walls).
    EXPECT_GT(desk->pivotX, 420.0F);
    EXPECT_LT(desk->pivotX, 700.0F);
}

TEST(WorldLayout, ExitDoorIsStillReachableFromThePasillo)
{
    const cybercba::WorldProp* door = cybercba::worldProp("safehouse_exit");
    ASSERT_NE(door, nullptr);
    EXPECT_GT(door->pivotX, 980.0F);
    EXPECT_TRUE(cybercba::isValidSpawn(cybercba::WorldZone::Shelter, door->pivotX - 60.0F, door->pivotY));
}
