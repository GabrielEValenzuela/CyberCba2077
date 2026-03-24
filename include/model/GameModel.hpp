#ifndef CYBERPUNK_GAME_MODEL_HPP
#define CYBERPUNK_GAME_MODEL_HPP

#include <cstdint>

namespace cyberpunk
{

    class GameModel
    {
    public:
        GameModel() = default;

        // Example state: "credits" (placeholder)
        void addCredits(std::int64_t delta);
        [[nodiscard]] std::int64_t credits() const;

    private:
        std::int64_t mCredits {0};
    };

} // namespace cyberpunk

#endif // CYBERPUNK_GAME_MODEL_HPP