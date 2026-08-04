#pragma once
#include <map>
#include <string>
#include "raylib.h"
// pointFilter=false (default) applies bilinear filtering, correct for the
// cinematic illustrations/portraits used by the narrative pivot (ADR 0017).
// Pass pointFilter=true only for assets intentionally authored as pixel art.
class AssetStore { public: ~AssetStore(); const Texture2D* texture(const std::string& path, bool pointFilter = false); const Font* font(const std::string& path,int size); private: std::map<std::string,Texture2D> m_textures; std::map<std::string,Font> m_fonts; };
