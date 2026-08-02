#include "ui/AssetStore.hpp"

AssetStore::~AssetStore()
{
    for (auto& item : m_textures)
        UnloadTexture(item.second);
    for (auto& item : m_fonts)
        UnloadFont(item.second);
}

const Texture2D* AssetStore::texture(const std::string& path)
{
    const auto existing = m_textures.find(path);
    if (existing != m_textures.end())
        return &existing->second;

    Texture2D value = LoadTexture(path.c_str());
    if (!value.id)
        return nullptr;

    // The game world uses pixel art. Keep every loaded gameplay texture crisp;
    // high-resolution portraits are also safe because their UI scale is explicit.
    SetTextureFilter(value, TEXTURE_FILTER_POINT);
    return &m_textures.emplace(path, value).first->second;
}

const Font* AssetStore::font(const std::string& path, int size)
{
    const std::string key = path + ":" + std::to_string(size);
    const auto existing = m_fonts.find(key);
    if (existing != m_fonts.end())
        return &existing->second;

    Font value = LoadFontEx(path.c_str(), size, nullptr, 0);
    if (!value.texture.id)
        return nullptr;
    return &m_fonts.emplace(key, value).first->second;
}
