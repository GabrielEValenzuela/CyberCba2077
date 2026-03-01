#include "model/GameModel.hpp"

namespace cyberpunk
{

    void GameModel::addCredits(std::int64_t delta)
    {
        mCredits += delta;
    }

    std::int64_t GameModel::credits() const
    {
        return mCredits;
    }

} // namespace cyberpunk
