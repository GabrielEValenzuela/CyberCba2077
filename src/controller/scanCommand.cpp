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
        int countHostile(const std::vector<EntityEntry>& entities, std::size_t index):
                {
                    // Caso base: si llegamos al final del vector
                    if (index == entities.size()) {
                        return 0;
                    }

                    // Caso recursivo
                    if (entities[index].disposition == EntityDisposition::Hostile) {
                        return 1 + countHostile(entities, index + 1);
                    } else {
                        return countHostile(entities, index + 1);
                    }
                }

        int hostileCount {0};
        for (const auto* pEntity : sortedEntities)
        {
            std::cout << "[" << entityDispositionToString(pEntity->disposition) << "] " << pEntity->name << " - "
                      << pEntity->distanceMeters << " m\n";
            if (pEntity->disposition == EntityDisposition::Hostile)
            {
                ++hostileCount;
            }
        }

        const int capped {std::min(hostileCount, static_cast<int>(AlertLevel::Maximum))};
        const auto targetLevel {static_cast<AlertLevel>(capped)};
        while (model.alertLevel() < targetLevel)
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
