#ifndef CYBERCBA_PLATFORM_ASSETIDS_H
#define CYBERCBA_PLATFORM_ASSETIDS_H

#include <cstdint>

namespace cybercba::platform
{

/// Explicit texture IDs for the minimal asset list.
///
/// The enum keeps AssetStore free from map-like containers.
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
