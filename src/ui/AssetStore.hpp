#pragma once
#include <map>
#include <string>
#include "raylib.h"
class AssetStore { public: ~AssetStore(); const Texture2D* texture(const std::string& path); const Font* font(const std::string& path,int size); private: std::map<std::string,Texture2D> m_textures; std::map<std::string,Font> m_fonts; };
