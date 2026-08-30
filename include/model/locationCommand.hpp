

#ifndef CYBERCBA2077_LOCATIONCOMMAND_HPP
#define CYBERCBA2077_LOCATIONCOMMAND_HPP
///#pragma once
#include "controller/command.hpp"
#include "common/types.hpp" ///para ver los tipos de datos del modelo
#include <string>
///@class locationCommand
///@Brief Clase que muestra la ubicacion del runner, derivando conectividad y nivel de peligro desde el estado del modelo
///@details locationCommand el gameModel para mostrar el estado de la zona actual
///@author Equipo MMMS

namespace CyberpunkCba
{
    ///@class locationCommand
    ///@brief muestra la ubi, peligro, red y salidas de la zona actual

    class locationCommand : public Command
    {
        public:
        ///@brief Constructor por defecto
        locationCommand()= default;
        ///@simbrief  Destructor por defecto
        ~locationCommand() override = default;

        /// @brief Muestra la ubicacion del runner por consola
        void execute(GameModel& model) override;

        ///@brief   identificador del comando
        ///@return "location"

        std::string name() const override ;
        /// @brief  Descripcion corta para el menu de ayuda
        /// @return String no vacio
        std::string description() const override ;
        ///@brief categoria del comando
        ///@return "mundo"
        std::string category() const override ;

        private:
        ///@brief Genera una barra visual del nivel de peligro
        ///@return String con la barra formateada, no vacio
        std::string buildDangerBar(AlertLevel level) const;
    };
}
#endif // CYBERCBA2077_LOCATIONCOMMAND_HPP