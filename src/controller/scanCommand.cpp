#include "scanCommand.hpp"

#include "common/types.hpp"
#include "model/gameModel.hpp"

#include <algorithm>
#include <cassert>
#include <iostream>
#include <string>
#include <vector>

namespace CyberpunkCba
{

    void ScanCommand::execute(GameModel& model)
    {
        const auto& nearby {model.nearbyEntities()};
        std::vector<const WorldEntity*> sortedEntities;
        sortedEntities.reserve(nearby.size());

        for (const auto* pEntity : nearby)
        {
            assert(pEntity != nullptr);
            if (pEntity == nullptr)
            {
                continue;
            }
            sortedEntities.emplace_back(pEntity);
        }

        std::sort(sortedEntities.begin(),
                  sortedEntities.end(),
                  [](const WorldEntity* lhs, const WorldEntity* rhs)
                  {
                      if (lhs->distanceMeters == rhs->distanceMeters)
                      {
                          return lhs->name < rhs->name;
                      }
                      return lhs->distanceMeters < rhs->distanceMeters;
                  });

        std::cout << "Escaneo de zona: " << model.currentZone() << "\n";

        if (sortedEntities.empty())
        {
            std::cout << "Sin entidades detectadas.\n";
            return;
        }

        bool hasHostile {false};
        for (const auto* pEntity : sortedEntities)
        {
            std::cout << "[" << entityDispositionToString(pEntity->disposition) << "] " << pEntity->name << " - "
                      << pEntity->distanceMeters << " m\n";
            hasHostile = hasHostile || pEntity->disposition == EntityDisposition::Hostile;
        }

        if (hasHostile)
        {
            model.incrementAlert("Hostiles detectados durante escaneo de zona.");
        }
    }

    std::string ScanCommand::name() const
    {
        return "scan";
    }

    std::string ScanCommand::description() const
    {
        return "Escanea entidades cercanas en la zona.";
    }

    std::string ScanCommand::category() const
    {
        return "mundo";
    }

} // namespace CyberpunkCba
