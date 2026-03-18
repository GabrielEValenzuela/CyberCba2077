// Siempre incluimos primero el header propio del archivo.
#include "alertCommand.hpp"

// iostream nos da acceso a std::cout para imprimir en pantalla.
#include <iostream>

// string ya viene incluido via alertCommand.hpp, pero no está de más saberlo.
#include <string>

// ============================================================================
// COLORES para la terminal (códigos ANSI — igual que en HelpCommand)
// Estos son string_view: vistas de strings constantes, muy eficientes.
// ============================================================================
namespace
{
    // Rojo para alertas altas
    constexpr std::string_view COLOR_RED     {"\033[31m"};
    // Amarillo para alertas medias
    constexpr std::string_view COLOR_YELLOW  {"\033[33m"};
    // Verde para alerta ninguna/baja
    constexpr std::string_view COLOR_GREEN   {"\033[32m"};
    // Cyan para títulos y separadores
    constexpr std::string_view COLOR_CYAN    {"\033[36m"};
    // Blanco brillante para labels
    constexpr std::string_view COLOR_WHITE   {"\033[97m"};
    // Reset: vuelve al color por defecto de la terminal
    constexpr std::string_view COLOR_RESET   {"\033[0m"};
}

namespace CyberpunkCba
{

    // =========================================================================
    // execute()
    //
    // Este es el método central del comando. Se llama cuando el jugador
    // tipea "alert" en la terminal del juego.
    //
    // Recibe GameModel& model — una REFERENCIA al estado del juego.
    // Una referencia es como un alias: model ES el GameModel real,
    // no una copia. Todo lo que leemos con model.xxx() es el estado actual.
    //
    // Este comando es READ ONLY: solo llamamos métodos que no empiezan
    // con verbos como increment/spend/consume. Solo leemos.
    // =========================================================================
    void AlertCommand::execute(GameModel& model)
    {
        // Leemos el nivel de alerta actual del modelo.
        // AlertLevel es un enum (tipo enumerado) definido en types.hpp.
        // Guardarlo en una variable local hace el código más legible.
        const AlertLevel level {model.alertLevel()};

        // Calculamos las patrullas activas según el issue:
        // "static_cast<int>(alertLevel) * 2"
        //
        // static_cast<int>() convierte el enum a su valor numérico:
        //   None=0, Low=1, Medium=2, High=3, Maximum=4
        // Multiplicado por 2: 0, 2, 4, 6, 8 patrullas respectivamente.
        const int activePatrols {static_cast<int>(level) * 2};

        // Elegimos el color del encabezado según la gravedad del nivel.
        // El operador ternario ( condición ? valor_si_true : valor_si_false )
        // es una forma compacta de un if/else de una línea.
        const std::string_view headerColor {
            (level >= AlertLevel::High)   ? COLOR_RED    :
            (level >= AlertLevel::Medium) ? COLOR_YELLOW :
                                            COLOR_GREEN
        };

        // =====================================================================
        // ENCABEZADO del output
        // =====================================================================
        std::cout << "\n"
                  << COLOR_CYAN
                  << "╔══════════════════════════════════════╗\n"
                  << "║     CYBERPUNK CBA 2077 — ALERTA      ║\n"
                  << "╚══════════════════════════════════════╝\n"
                  << COLOR_RESET;

        // =====================================================================
        // NIVEL DE ALERTA — campo siempre presente
        //
        // alertLevelToString() es una función libre definida en types.cpp
        // que convierte el enum a texto legible: "NINGUNA", "BAJA", etc.
        // =====================================================================
        std::cout << COLOR_WHITE << "  Nivel de alerta : "
                  << COLOR_RESET
                  << headerColor << alertLevelToString(level) << "\n"
                  << COLOR_RESET;

        // =====================================================================
        // PATRULLAS ACTIVAS — campo siempre presente
        // =====================================================================
        std::cout << COLOR_WHITE << "  Patrullas activas: "
                  << COLOR_RESET
                  << activePatrols << "\n";

        // =====================================================================
        // ORIGEN DE ALERTA — campo siempre presente
        //
        // lastAlertCause() puede retornar un string vacío si nunca hubo
        // una causa registrada. Lo manejamos mostrando "N/A" en ese caso.
        // =====================================================================
        const std::string& cause {model.lastAlertCause()};
        std::cout << COLOR_WHITE << "  Origen           : "
                  << COLOR_RESET
                  << (cause.empty() ? "N/A" : cause) << "\n";

        // =====================================================================
        // TIEMPO PARA DESPEJE — campo CONDICIONAL
        //
        // Requisito del issue: "solo presente si alertLevel >= Medium"
        //
        // El operador >= sobre enums funciona porque los valores están
        // ordenados: None(0) < Low(1) < Medium(2) < High(3) < Maximum(4)
        //
        // Si el nivel es None o Low, directamente no imprimimos esta línea.
        // =====================================================================
        if (level >= AlertLevel::Medium)
        {
            // El tiempo de despeje es estimado: a mayor nivel, más turnos.
            // static_cast<int>(level) nos da el número del nivel (2, 3 o 4).
            const int clearTime {static_cast<int>(level) * 3};
            std::cout << COLOR_WHITE << "  Tiempo p/despeje : "
                      << COLOR_RESET
                      << clearTime << " turnos\n";
        }

        // =====================================================================
        // CONSEJO TÁCTICO — campo siempre presente
        //
        // Delegamos la lógica al método privado adviceForLevel().
        // Esto cumple el requisito: "sin switch en execute()".
        // execute() no sabe NI le importa cómo se calcula el consejo.
        // =====================================================================
        std::cout << COLOR_WHITE << "  Consejo          : "
                  << COLOR_RESET
                  << adviceForLevel(level) << "\n";

        // Línea de cierre decorativa
        std::cout << COLOR_CYAN
                  << "  ──────────────────────────────────────\n"
                  << COLOR_RESET << "\n";
    }

    // =========================================================================
    // name(), description(), category()
    //
    // Estos tres son simples: solo retornan un string constante.
    // "const" al final del método significa que no modifica el objeto.
    // =========================================================================

    std::string AlertCommand::name() const
    {
        return "alert";
    }

    std::string AlertCommand::description() const
    {
        return "Muestra el nivel de alerta actual de la zona.";
    }

    std::string AlertCommand::category() const
    {
        // "mundo" agrupa comandos relacionados con el estado del entorno.
        return "mundo";
    }

    // =========================================================================
    // adviceForLevel()
    //
    // Método PRIVADO que centraliza los mensajes de consejo.
    // Recibe un AlertLevel y retorna el string correspondiente.
    //
    // ¿Por qué un método separado y no un switch en execute()?
    // Porque separa responsabilidades: execute() orquesta el output,
    // adviceForLevel() sabe qué decir según el nivel. Si mañana
    // queremos cambiar los mensajes, solo tocamos este método.
    //
    // "const" al final: promesa de que no modifica ningún atributo del objeto.
    // =========================================================================
    std::string AlertCommand::adviceForLevel(const AlertLevel level) const
    {
        // Un switch sobre un enum es la forma idiomática en C++
        // de manejar todos los casos posibles de forma explícita.
        // El compilador con -Wall avisa si olvidamos algún caso.
        switch (level)
        {
            case AlertLevel::None:
                // Nivel None tiene un mensaje propio, distinto a Low (requisito del issue).
                return "La zona está tranquila. Buen momento para moverse.";

            case AlertLevel::Low:
                // Distinto al de None — cumple el criterio de aceptación.
                return "Mantené perfil bajo. Evitá zonas iluminadas.";

            case AlertLevel::Medium:
                return "Patrullas activas. Usá rutas alternativas.";

            case AlertLevel::High:
                return "Zona caliente. Considerá abortar la misión.";

            case AlertLevel::Maximum:
                return "PELIGRO MÁXIMO. Evacuá la zona inmediatamente.";

            default:
                // El default cubre casos inesperados (buena práctica defensiva).
                return "Estado de alerta desconocido.";
        }
    }

} // namespace CyberpunkCba