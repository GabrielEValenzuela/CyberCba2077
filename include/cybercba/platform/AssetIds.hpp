#ifndef CYBERCBA_PLATFORM_ASSETIDS_H
#define CYBERCBA_PLATFORM_ASSETIDS_H

#include <cstdint>

namespace cybercba::platform
{

// Explicit texture IDs for VS-001's minimal asset list (§13). Enum-driven on
// purpose (TAD-001 §20) so AssetStore never needs a map-like STL container.
enum class TextureId : std::uint8_t
{
    // Backgrounds.
    RefugeBackground,
    RefugeBackgroundDim,
    SurveillanceInstallationExterior,
    ServerRoom,
    SafehouseStreet,

    // Portraits.
    EmmaPortrait,
    MaggaPortrait,
    FantasmaPortrait,
    RatonPortrait,

    // Character sprites.
    EmmaSprite,
    MaggaSprite,
    FantasmaSprite,
    RatonSprite,
    GuardSprite,

    // Props.
    ElectricalPanelProp,
    ServerTerminalProp,
    SurveillanceCameraProp,

    Count,
};

enum class SoundId : std::uint8_t
{
    Count,
};

} // namespace cybercba::platform

#endif // CYBERCBA_PLATFORM_ASSETIDS_H
