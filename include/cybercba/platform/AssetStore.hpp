#ifndef CYBERCBA_PLATFORM_ASSETSTORE_H
#define CYBERCBA_PLATFORM_ASSETSTORE_H

#include <cstddef>

#include <raylib.h>

#include "cybercba/platform/AssetIds.hpp"

namespace cybercba::platform
{

/// Single owner of every loaded texture/sound: loads once
// at construction, unloads once at destruction. Gameplay/rendering code
// receives observer handles via texture()/sound(), never calls
// LoadTexture()/UnloadTexture() itself.
//
// PLACEHOLDER (VS-001 §13 asset list not yet produced): until real art
// exists, every TextureId resolves to a generated solid-color placeholder
// instead of a file load. Swapping to LoadTexture(path) per ID is a local,
// reversible change confined to AssetStore's .cpp.
//
// Ownership: owns every Texture2D it loads. Copy is disabled — there must be
// exactly one AssetStore per set of GPU resources.
class AssetStore final
{
  public:
    AssetStore();
    ~AssetStore();

    AssetStore(const AssetStore&)            = delete;
    AssetStore& operator=(const AssetStore&) = delete;
    AssetStore(AssetStore&&)                 = delete;
    AssetStore& operator=(AssetStore&&)      = delete;

    const Texture2D& texture(TextureId id) const;

  private:
    Texture2D m_textures[static_cast<std::size_t>(TextureId::Count)];
    bool m_loaded[static_cast<std::size_t>(TextureId::Count)];
};

} // namespace cybercba::platform

#endif // CYBERCBA_PLATFORM_ASSETSTORE_H
