//
// Created by diego on 22/3/26.
//

#include "logCommand.hpp" //header
#include "../include/model/gameModel.hpp"//
#include <iomanip> //formatea numeros con 0
#include <sstream> //constuir strings facil
#include <iostream>
#include <vector>
#include "../include/common/types.hpp"


namespace CyberpunkCba
{
    std::string logCommand::name() const
    {
        return "log";
    }

    std::string logCommand::description() const
    {
        return "historial";
    }

    std::string LogCommand::category() const
    {
        return "system";
    }

    void logCommand::execute(GameModel& model)
    {
     const std::vector<LogEntry>&logComplete =model.actionLog();
        int totalentradas1=logComplete.size();//cuenta entradas en log

        int inicio;
        if (totalentradas1>10)//si hay mas de 10 entradas, le resta 10 al total
        {
            inicio=totalentradas1-10;
        }
        else
        {
            inicio=0;
        }
        if (totalentradas1>10)//si hay mas de 10 entradass, solo muestra las ultimas 10
        {
            std::cout<<"Ultimas 10 de"<<totalentradas1;
            std::cout<<"entradas totales: "<<std::endl;
            std::cout<<"---------"<<std::endl;
        }
        for (int i = inicio; i<totalentradas1; i++)
        {
            const LogEntry& entrada =logComplete[i];
            std::string time=formatTimestamp(entrada.hour,entrada.minute);
            std::cout<<time<<std::endl;
        }






    }

}




