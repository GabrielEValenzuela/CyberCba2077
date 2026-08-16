#include "cybercba/platform/AssetStore.hpp"

namespace cybercba::platform
{

namespace
{

constexpr int PLACEHOLDER_SIZE = 4;

// Deterministic per-ID color so placeholders are at least visually
// distinguishable in-engine before real art exists (VS-001 §13).
Color placeholderColorFor(TextureId id)
{
    const auto index = static_cast<unsigned char>(id);
    return Color{static_cast<unsigned char>(64 + (index * 37) % 192),
                 static_cast<unsigned char>(64 + (index * 71) % 192),
                 static_cast<unsigned char>(64 + (index * 113) % 192), 255};
}

} // namespace

AssetStore::AssetStore()
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(TextureId::Count); ++i)
    {
        const auto id     = static_cast<TextureId>(i);
        Image placeholder = GenImageColor(PLACEHOLDER_SIZE, PLACEHOLDER_SIZE, placeholderColorFor(id));
        m_textures[i]     = LoadTextureFromImage(placeholder);
        UnloadImage(placeholder);
        m_loaded[i] = true;
    }
}

AssetStore::~AssetStore()
{
    for (std::size_t i = 0; i < static_cast<std::size_t>(TextureId::Count); ++i)
    {
        if (m_loaded[i])
        {
            UnloadTexture(m_textures[i]);
        }
    }
}

const Texture2D& AssetStore::texture(TextureId id) const
{
    return m_textures[static_cast<std::size_t>(id)];
}

} // namespace cybercba::platform
