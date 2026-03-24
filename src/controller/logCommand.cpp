#include "logCommand.hpp"
#include "../../include/model/gameModel.hpp"
#include "../../include/common/types.hpp"
#include <iomanip>
#include <sstream>
#include <iostream>
#include <vector>

namespace CyberpunkCba
{
    std::string logCommand::name() const { return "log"; }

    std::string logCommand::description() const { return "Muestra el historial y tiempo de sesion"; }

    std::string logCommand::category() const { return "system"; }

    void logCommand::execute(GameModel& model)
    {
        const std::vector<LogEntry>& logComplete = model.actionLog();
        size_t totalentradas1 = logComplete.size();

        // REQUISITO: Log vacío muestra mensaje apropiado
        if (totalentradas1 == 0)
        {
            std::cout << "El registro del sistema esta vacio. No hay eventos recientes." << std::endl;
            return; // Cortamos la ejecución acá, no hay nada más que hacer
        }

        size_t inicio = 0;
        if (totalentradas1 > 10)
        {
            inicio = totalentradas1 - 10;
            // REQUISITO: Texto de aviso exacto
            std::cout << "Mostrando ultimas 10 de " << totalentradas1 << " entradas:" << std::endl;
            std::cout << "---------" << std::endl;
        }

        for (size_t i = inicio; i < totalentradas1; i++)
        {
            const LogEntry& entrada = logComplete[i];

            // Llamamos al metodo privado para formatear la hora
            std::string time = formatTimestamp(entrada.hour, entrada.minute);
            std::cout << "[" << time << "] " << entrada.message << std::endl;
        }

        // REQUISITO: Mostrar duración de la sesión
        std::cout << "---------" << std::endl;
        std::cout << "Tiempo de sesion: " << formatDuration(model.sessionDuration()) << std::endl;
    }

    // --- IMPLEMENTACIÓN DE MÉTODOS PRIVADOS ---

    std::string logCommand::formatTimestamp(int hour, int minute) const
    {
        // REQUISITO: Fallback para -1
        if (hour == -1)
        {
            return "--:--";
        }

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hour << ":"
            << std::setfill('0') << std::setw(2) << minute;
        return oss.str();
    }

    std::string logCommand::formatDuration(std::chrono::seconds duration) const
    {
        // Matemática para convertir segundos totales a formato HH:MM:SS
        auto hrs = std::chrono::duration_cast<std::chrono::hours>(duration);
        duration -= hrs;
        auto mins = std::chrono::duration_cast<std::chrono::minutes>(duration);
        duration -= mins;

        std::ostringstream oss;
        oss << std::setfill('0') << std::setw(2) << hrs.count() << ":"
            << std::setfill('0') << std::setw(2) << mins.count() << ":"
            << std::setfill('0') << std::setw(2) << duration.count();
        return oss.str();
    }
}