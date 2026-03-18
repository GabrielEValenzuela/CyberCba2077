//
// Created by tomas on 17/3/26.
//

#include "controller/exitCommand.hpp"
#include "model/gameModel.hpp"
#include <iomanip>
#include <iostream>


    void CyberpunkCba::ExitCommand::execute(CyberpunkCba::GameModel& model)
    {
        // Si execute() se llama dos veces, la segunda vez isRunning()
        // ya es false (lo pusimos en false en la primera invocación).
        // En ese caso mostramos un mensaje y salimos sin hacer nada más.
        if (!model.isRunning())
        {
            std::cout << "[EXODUS] Sistema ya detenido.\n";
            return;
            // "return" en un método void termina la función ahí mismo.
        }

        // ── CALCULAR DURACIÓN DE SESIÓN ────────────────────────────────
        // sessionDuration() devuelve un objeto chrono::seconds.
        // .count() nos da el número entero de segundos adentro.
        int totalSecs = static_cast<int>(model.sessionDuration().count());
        // static_cast<int>(...) convierte el resultado a int.
        // chrono::seconds usa internamente un tipo "long" en muchos
        // compiladores. El cast evita un warning de -Wconversion.

        // Ahora descomponemos los segundos totales en horas, minutos, segundos.
        // Ejemplo: 3723 segundos
        int hours   = totalSecs / 3600;
        // 3600 segundos = 1 hora
        // 3723 / 3600 = 1 (división entera, descarta el resto)

        int minutes = (totalSecs % 3600) / 60;
        // % es el operador módulo: devuelve el RESTO de la división.
        // 3723 % 3600 = 123 (lo que sobra después de sacar 1 hora completa)
        // 123 / 60 = 2 minutos

        int secs    = totalSecs % 60;
        // 3723 % 60 = 3 segundos restantes

        // ── MENSAJE DE DESPEDIDA SEGÚN HP ──────────────────────────────
        // isCriticalHp() devuelve true cuando hp <= maxHp * 0.20
        // (o sea, 20% o menos de vida). Lo vemos en GameModel:
        // static constexpr float CRITICAL_HP_RATIO {0.20f};
        if (model.isCriticalHp())
        {
            std::cout << "\n[ADVERTENCIA] Saliendo en estado crítico, runner.\n";
            std::cout << "Deberías haber buscado un ripperdoc antes de irte.\n";
        }
        else
        {
            std::cout << "\n[EXODUS] Sesión cerrada. Hasta la próxima, runner.\n";
        }

        // ── MOSTRAR MÉTRICAS ───────────────────────────────────────────
        std::cout << "Duración : "
                  << std::setfill('0')
                  // setfill('0') → cuando un número necesita relleno,
                  // usa '0' en lugar de espacios. Efecto global hasta
                  // que lo cambiés de nuevo.
                  << std::setw(2) << hours   << ":"
                  // setw(2) → el siguiente número ocupa mínimo 2 caracteres.
                  // Si hours = 1, imprime "01". Si hours = 10, imprime "10".
                  // setw se "consume" con el siguiente valor — hay que
                  // repetirlo para cada número.
                  << std::setw(2) << minutes << ":"
                  << std::setw(2) << secs
                  << "\n";

        std::cout << "Comandos : " << model.commandCount() << "\n";

        // ── APAGAR EL JUEGO ────────────────────────────────────────────
        // quit() setea m_running = false.
        // El loop principal en Console hace while(model.isRunning()),
        // así que en la próxima iteración va a ver false y va a terminar.
        // Esto es LIMPIO: los destructores se ejecutan, no hay leaks.
        // NUNCA usar ::exit() ni std::exit() — eso mataría el proceso
        // sin limpiar nada, y valgrind reportaría memory leaks.
        model.quit();
    }

    std::string CyberpunkCba::ExitCommand::name() const
    {
        return "exit";
        // Este string es lo que el jugador tipea en la consola.
        // CommandRegistry lo usa para saber qué comando ejecutar
        // cuando alguien escribe "exit".
    }

    std::string CyberpunkCba::ExitCommand::description() const
    {
        return "Termina la sesión y muestra métricas de actividad";
        // Este string aparece cuando el jugador ejecuta "help".
    }

std::string CyberpunkCba::ExitCommand::category() const
    {
        return "sistema";
        // la categoría agrupa comandos en el "help"
        // podés usar "sistema", "navegación", lo que corresponda
    }

