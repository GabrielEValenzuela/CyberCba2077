#ifndef _SCAN_COMMAND_HPP
#define _SCAN_COMMAND_HPP

#include "controller/command.hpp"

#include <string>

/**
 * @file ScanCommand.hpp
 * @brief Header privado de ScanCommand.
 *
 * @details
 * Este header NO es parte de la API pública del sistema.
 * Solo debe ser incluido por ScanCommand.cpp.
 * La API pública es Command.hpp.
 *
 * @author Equipo 02 — Exodus Systems Inc.
 * @version 0.1.0
 */

namespace CyberpunkCba
{

    /**
     * @class ScanCommand
     * @brief Escanea entidades cercanas en la zona actual.
     *
     * @details
     * Lee entidades observadas desde GameModel::nearbyEntities(),
     * las ordena por distanceMeters ascendente y muestra salida compacta.
     * Si detecta al menos una entidad Hostile, incrementa la alerta del modelo.
     */
    class ScanCommand final : public Command
    {
    public:
        /// @brief Construye ScanCommand sin estado propio.
        ScanCommand() = default;

        ~ScanCommand() override = default;

    private:
        // Wazuh convention: implementaciones de interfaz son private.

        /**
         * @brief Ejecuta el escaneo de la zona actual.
         * @details
         * - Toma punteros observadores desde nearbyEntities().
         * - Verifica precondición de no nulos (assert + guard defensivo).
         * - Ordena por distancia y muestra entidades en formato compacto.
         * - Si hay Hostile, incrementa alertLevel con causa descriptiva.
         * @param model Modelo de juego a consultar/modificar.
         */
        void execute(GameModel& model) override;

        /// @brief Nombre canónico del comando.
        std::string name() const override;

        /// @brief Descripción breve para help.
        std::string description() const override;

        /// @brief Categoría de help.
        std::string category() const override;
    };

} // namespace CyberpunkCba

#endif // _SCAN_COMMAND_HPP
