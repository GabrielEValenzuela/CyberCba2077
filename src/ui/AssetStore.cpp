#include "ui/AssetStore.hpp"

#include <vector>

namespace
{
// raylib's LoadFontEx with codepoints=nullptr only loads ASCII 32-126, so
// accented Spanish characters (á é í ó ú ñ ¿ ¡ and uppercase/ü variants)
// render as tofu ('?') — every screen shows this. Load an explicit charset
// that covers AGENTS.md's required glyph list instead.
std::vector<int> spanishCodepoints()
{
    std::vector<int> codepoints;
    for (int c = 32; c <= 126; ++c)
        codepoints.push_back(c);
    for (int c : {0x00A1, 0x00BF, 0x00C1, 0x00C9, 0x00CD, 0x00D3, 0x00DA, 0x00D1, 0x00DC,
                  0x00E1, 0x00E9, 0x00ED, 0x00F3, 0x00FA, 0x00F1, 0x00FC})
        codepoints.push_back(c);
    return codepoints;
}
} // namespace

AssetStore::~AssetStore()
{
    for (auto& item : m_textures)
        UnloadTexture(item.second);
    for (auto& item : m_fonts)
        UnloadFont(item.second);
}

const Texture2D* AssetStore::texture(const std::string& path, bool pointFilter)
{
    const auto existing = m_textures.find(path);
    if (existing != m_textures.end())
        return &existing->second;

    Texture2D value = LoadTexture(path.c_str());
    if (!value.id)
        return nullptr;

    // Bilinear by default (cinematic illustrations/portraits, ADR 0017);
    // point filter only for assets explicitly requested as pixel art.
    SetTextureFilter(value, pointFilter ? TEXTURE_FILTER_POINT : TEXTURE_FILTER_BILINEAR);
    return &m_textures.emplace(path, value).first->second;
}

const Font* AssetStore::font(const std::string& path, int size)
{
    const std::string key = path + ":" + std::to_string(size);
    const auto existing = m_fonts.find(key);
    if (existing != m_fonts.end())
        return &existing->second;

    static const std::vector<int> codepoints = spanishCodepoints();
    Font value = LoadFontEx(path.c_str(), size, const_cast<int*>(codepoints.data()), static_cast<int>(codepoints.size()));
    if (!value.texture.id)
        return nullptr;
    return &m_fonts.emplace(key, value).first->second;
}
