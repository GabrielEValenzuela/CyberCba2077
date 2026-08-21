#ifndef CYBERCBA_PLATFORM_RENDERER_H
#define CYBERCBA_PLATFORM_RENDERER_H

#include "cybercba/game/Game.hpp"
#include "cybercba/platform/AssetStore.hpp"

namespace cybercba::platform
{

/// Draws already-computed Game state (the renderer observes state,
// does not decide gameplay"). Never mutates Game; never calls
// GameStateMachine::transitionTo.
//
// Ownership: holds an observer reference to the AssetStore it draws from;
// does not own it (AssetStore's owner is the app composition root).
class Renderer final
{
  public:
    explicit Renderer(const AssetStore& assets);

    void draw(const game::Game& gameState) const;

  private:
    const AssetStore* m_pAssets;
};

} // namespace cybercba::platform

#endif // CYBERCBA_PLATFORM_RENDERER_H
