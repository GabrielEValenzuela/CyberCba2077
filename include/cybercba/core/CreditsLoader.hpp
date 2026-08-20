#ifndef CYBERCBA_CORE_CREDITSLOADER_H
#define CYBERCBA_CORE_CREDITSLOADER_H

#include <string>

namespace cybercba::core
{

// Reads every contributor "anuario" file from `directory` (std::filesystem,
// no raylib dependency — testable in isolation) and joins them into a single
// block of text for the in-game credits screen. Each contributor adds their
// own file via a normal git/GitHub PR (see credits/README.md) instead of
// editing one shared file everyone would conflict on.
//
// Ownership: stateless. Returns an owned std::string; callers own the
// result.
class CreditsLoader final
{
  public:
    // Skips README.md and any file whose name starts with '_' (templates,
    // e.g. credits/_ejemplo.txt). Entries are sorted by filename for a
    // deterministic, reproducible order. Returns an empty string if the
    // directory doesn't exist or has no valid entries.
    static std::string loadEntries(const std::string& directory);
};

} // namespace cybercba::core

#endif // CYBERCBA_CORE_CREDITSLOADER_H
