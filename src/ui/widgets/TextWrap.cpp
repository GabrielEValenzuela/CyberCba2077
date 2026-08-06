#include "ui/widgets/TextWrap.hpp"

// Greedy word-wrap respecting explicit newlines as paragraph breaks.
std::vector<std::string> wrapText(const std::string& text, int maxWidth, int fontSize, const Font* font)
{
    const auto measure = [&](const std::string& s)
    {
        return font != nullptr ? static_cast<int>(MeasureTextEx(*font, s.c_str(), static_cast<float>(fontSize), 1.0F).x)
                                : MeasureText(s.c_str(), fontSize);
    };
    std::vector<std::string> lines;
    std::size_t paragraphStart = 0;
    while (paragraphStart <= text.size())
    {
        const std::size_t paragraphEnd = text.find('\n', paragraphStart);
        const std::string paragraph = text.substr(paragraphStart, paragraphEnd == std::string::npos ? std::string::npos : paragraphEnd - paragraphStart);
        std::string line;
        std::size_t wordStart = 0;
        while (wordStart <= paragraph.size())
        {
            std::size_t wordEnd = paragraph.find(' ', wordStart);
            if (wordEnd == std::string::npos) wordEnd = paragraph.size();
            const std::string word = paragraph.substr(wordStart, wordEnd - wordStart);
            const std::string candidate = line.empty() ? word : line + " " + word;
            if (!candidate.empty() && measure(candidate) > maxWidth && !line.empty())
            {
                lines.push_back(line);
                line = word;
            }
            else
            {
                line = candidate;
            }
            wordStart = wordEnd + 1;
        }
        lines.push_back(line);
        if (paragraphEnd == std::string::npos) break;
        paragraphStart = paragraphEnd + 1;
    }
    return lines;
}
