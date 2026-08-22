#include "cybercba/core/CreditsLoader.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>

namespace cybercba::core
{

namespace
{

namespace fs = std::filesystem;

bool isEntryFile(const fs::directory_entry& entry)
{
    if (!entry.is_regular_file())
    {
        return false;
    }
    const std::string name = entry.path().filename().string();
    if (name == "README.md")
    {
        return false;
    }
    if (!name.empty() && name.front() == '_')
    {
        return false;
    }
    return true;
}

std::string trim(const std::string& text)
{
    const auto first = text.find_first_not_of(" \t\r\n");
    if (first == std::string::npos)
    {
        return "";
    }
    const auto last = text.find_last_not_of(" \t\r\n");
    return text.substr(first, last - first + 1);
}

std::string readFile(const fs::path& path)
{
    std::ifstream file(path, std::ios::binary);
    std::ostringstream buffer;
    buffer << file.rdbuf();
    return trim(buffer.str());
}

} // namespace

std::string CreditsLoader::loadEntries(const std::string& directory)
{
    if (!fs::exists(directory) || !fs::is_directory(directory))
    {
        return "";
    }

    // Two-pass: count matching files, then own a right-sized raw array
    // (whoever creates with new must own and delete it — that
    // owner is this function, for its whole lifetime). Avoids both a
    // hard-coded cap and any STL container.
    std::size_t entryCount = 0;
    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (isEntryFile(entry))
        {
            ++entryCount;
        }
    }

    auto* paths           = new std::string[entryCount];
    std::size_t pathIndex = 0;
    for (const auto& entry : fs::directory_iterator(directory))
    {
        if (isEntryFile(entry))
        {
            paths[pathIndex++] = entry.path().string();
        }
    }
    std::sort(paths, paths + entryCount);

    std::string result;
    for (std::size_t i = 0; i < entryCount; ++i)
    {
        const std::string content = readFile(paths[i]);
        if (content.empty())
        {
            continue;
        }
        if (!result.empty())
        {
            result += "\n\n";
        }
        result += content;
    }

    delete[] paths;
    return result;
}

} // namespace cybercba::core
