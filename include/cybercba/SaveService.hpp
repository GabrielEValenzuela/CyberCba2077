#pragma once
#include <string>
#include "cybercba/GameSession.hpp"
namespace cybercba {
enum class SaveLoadStatus { Loaded, Missing, Corrupt, UnsupportedVersion, IoError };
class SaveService { public: explicit SaveService(std::string path); bool save(const GameSession& session, std::string* error = nullptr) const; SaveLoadStatus load(GameSession& session, std::string* error = nullptr) const; bool exists() const; const std::string& path() const; private: std::string m_path; };
} // namespace cybercba
