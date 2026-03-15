#include "model/helpCommand.hpp"
#include <iostream>

namespace CyberpunkCba
{
    //Constructor de HelpCommand que se le pasa por referencia  "registry" y luego con esto
    //inicializa m_registry (lo inicializa con una "lista" de comandos )
    HelpCommand::HelpCommand(const CommandRegistry& registry): m_registry (registry)
    {

    }

    //Metodo name()
    std::string  HelpCommand::name() const
    {
        return "help";
    }

    //Metodo description() que devuelve una descripcion del comando
    std::string  HelpCommand:: description() const
    {
        return "Mostrar los comandos disponibles";
    }

    //Metodo category()
    std::string  HelpCommand::category() const
    {
        return "sistema";
    }

    //Metodo  execute()
    void HelpCommand:: execute(GameModel& model)
    {
        if ( m_registry.empty()) //valida que m_registry no este vacio usando metodo de su clase commandregistry
        {
            std::cout <<"\nNo existen comandos registrados en el sistema.\n"<< std::endl;
            return;
        }

        //crea el mapa donde categoria(string), lista de comandos
        std::map< std::string, std::vector<const Command*>> gruposDeComandos; 
        
        /*solicita el vector o lista de comandos de m_registry
        y lo guarda en una variable que se crea que va a alojar esos comandos**/
        const auto& commandos = m_registry.commands(); 
        // llama a los comandos del par y para cada uno los imprime
        //va a recorrer para cada comando de la lista creada                                                   
        for (const Command* comando : commandos) 
        {   
            //pregunta la categoria(campo de comando:command) y lo guarda en un string para agregarlo al mapa
            std::string categoria = comando -> category(); 
            
            if (categoria.empty()) {
            categoria = "otros";
            }

            //mete la categoria y el comando al mapa. Pus_back es un metodo de Map
            gruposDeComandos[categoria].push_back(comando); 
        }
        //va a recorrer para cada par categoria/comandos y va a imprimir
        for (const auto& pares : gruposDeComandos) 
        {   
            //llama a la categoria del par y la imprime
            std::cout << pares.first << std::endl; 

            // llama a los comandos del par y para cada uno los imprime
            for (const Command* comando : pares.second) 
            {
                std::cout << comando->name() << std::endl;
            }

       }

     }







}