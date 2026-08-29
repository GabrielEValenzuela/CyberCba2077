#include "cybercba/game/ResourceRules/ResourceRulesEngine.hpp"

#include <cstddef>
#include <stdexcept>

namespace cybercba::game
{

ResourceRulesEngine::ResourceRulesEngine(const CampaignState* pState,
                                         const structures::DynamicArray<const IResourceRule*>& reglas)
    : m_pState(pState), m_reglas(reglas)
{
    validarGrafo();
}

bool ResourceRulesEngine::consultar(ResourceType tipo, int cantidad) const
{
    if (cantidad == 0 || m_pState == nullptr)
    {
        return false;
    }

    const bool bConsumir = cantidad > 0;
    bool bEncontroRegla  = false;
    for (std::size_t i = 0; i < m_reglas.size(); ++i)
    {
        const IResourceRule* pRegla = m_reglas[i];
        if (pRegla == nullptr || pRegla->tipo() != tipo)
        {
            continue;
        }

        bEncontroRegla = true;

        const bool bValida =
            bConsumir ? pRegla->puedeConsumir(*m_pState, cantidad) : pRegla->puedeOtorgar(*m_pState, cantidad);
        if (!bValida)
        {
            return false;
        }
    }

    return bEncontroRegla;
}

void ResourceRulesEngine::validarGrafo() const
{
    const int PROFUNDIDAD_MAXIMA = 8;

    for (std::size_t i = 0; i < m_reglas.size(); ++i)
    {
        const IResourceRule* pActual = m_reglas[i];
        const IResourceRule* pRuta[PROFUNDIDAD_MAXIMA]{};
        int profundidad = 0;
        while (pActual != nullptr)
        {
            if (profundidad >= PROFUNDIDAD_MAXIMA)
            {
                throw std::invalid_argument("ResourceRulesEngine: invalid rule dependency graph");
            }
            for (int j = 0; j < profundidad; ++j)
            {
                if (pRuta[j] == pActual)
                {
                    throw std::invalid_argument("ResourceRulesEngine: invalid rule dependency graph");
                }
            }
            pRuta[profundidad] = pActual;
            ++profundidad;
            pActual = pActual->reglaDeLaQueDepende();
        }
    }
}

} // namespace cybercba::game
