#include "cybercba/CreditsLoader.hpp"

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <vector>

namespace cybercba
{
    namespace
    {
        namespace fs = std::filesystem;

        bool isEntryFile(const fs::directory_entry& entry)
        {
            if (!entry.is_regular_file())
                return false;
            const std::string name = entry.path().filename().string();
            if (name == "README.md")
                return false;
            if (!name.empty() && name.front() == '_')
                return false;
            return true;
        }

        std::string trim(const std::string& text)
        {
            const auto first = text.find_first_not_of(" \t\r\n");
            if (first == std::string::npos)
                return "";
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
            return "";

        std::vector<fs::path> paths;
        for (const auto& entry : fs::directory_iterator(directory))
        {
            if (isEntryFile(entry))
                paths.push_back(entry.path());
        }
        std::sort(paths.begin(), paths.end());

        std::string result;
        for (const auto& path : paths)
        {
            const std::string content = readFile(path);
            if (content.empty())
                continue;
            if (!result.empty())
                result += "\n\n";
            result += content;
        }
        return result;
    }

} // namespace cybercba
