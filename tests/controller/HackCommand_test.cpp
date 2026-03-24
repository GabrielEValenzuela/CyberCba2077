#include "HackCommand.hpp"
#include "model/gameModel.hpp"

#include <gtest/gtest.h>
#include <sstream>
#include <string>

/**
 * @file hackCommand_test.cpp
 * @brief Pruebas unitarias para HackCommand.
 *
 * @details
 * Este archivo contiene pruebas unitarias para la clase HackCommand, que verifica la disponibilidad de hack
 * y muestra mensajes específicos según el estado de créditos e intentos del modelo de juego.
 * Las pruebas cubren los siguientes escenarios:
 * - Cuando el hackeo está disponible (créditos e intentos disponibles).
 * - Cuando no hay créditos disponibles pero sí intentos.
 * - Cuando no hay intentos disponibles pero sí créditos.
 * - Cuando no hay créditos ni intentos disponibles.
 * - Verificar que el comando no modifica el modelo de juego.
 *
 * @author Equipo 03 â€” Exodus Systems Inc.
 * @version 0.1.0
 */

using namespace CyberpunkCba;

class HackCommandTest : public ::testing::Test
{
    /**
     * @brief Fixture de pruebas para HackCommand.
     * Proporciona un modelo de juego inicializado y un comando HackCommand para ejecutar.
     * Incluye métodos auxiliares para capturar la salida de consola y modificar el estado
     * del modelo (agotar créditos e intentos) para probar diferentes escenarios.
     */
protected:
    /// @brief Ejecuta el comando y devuelve todo lo impreso en stdout.
    std::string captureOutput()
    {
        std::ostringstream oss;
        std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
        m_cmd.execute(m_model);
        std::cout.rdbuf(old);
        return oss.str();
    }

    /// @brief Gasta todos los créditos del modelo de a uno
    ///        para respetar la precondición de spendCredits().
    void agotarCreditos()
    {
        while (m_model.credits() > 0)
        {
            m_model.spendCredits(1);
        }
    }

    /// @brief Agota todos los intentos de hackeo del modelo.
    void agotarIntentos()
    {
        while (m_model.hackAttempts() > 0)
        {
            m_model.consumeHackAttempt();
        }
    }

    GameModel m_model {"Ghost_47"};
    HackCommand m_cmd;
};

/** @brief Test para verificar que el comando muestra "OK" cuando está disponible.
 *
 * Escenario: El modelo inicia con créditos e intentos disponibles.
 * Acción: Ejecutar el comando HackCommand.
 * Resultado esperado: La salida contiene "OK".
 */
TEST_F(HackCommandTest, Disponible_MuestraOK)
{
    // Estado inicial: 250 créditos, 3 intentos disponibles
    const std::string output {captureOutput()};

    // Debe mostrar "OK"
    EXPECT_NE(output.find("OK"), std::string::npos);
}

/** @brief Test para verificar que el comando muestra "SIN CREDITOS"
 * cuando no hay créditos disponibles.
 *
 * Escenario: El modelo inicia con créditos agotados pero intentos disponibles.
 * Acción: Ejecutar el comando HackCommand.
 * Resultado esperado: La salida contiene "SIN CREDITOS".
 */
TEST_F(HackCommandTest, SinCreditos_MensajeEspecifico)
{
    agotarCreditos(); // credits == 0, hackAttempts == 3

    const std::string output {captureOutput()};

    EXPECT_NE(output.find("SIN CREDITOS"), std::string::npos);

    // No debe mostrar OK ni el mensaje de sin intentos
    EXPECT_EQ(output.find("OK"), std::string::npos);
    EXPECT_EQ(output.find("SIN INTENTOS"), std::string::npos);
}

/** @brief Test para verificar que el comando muestra "SIN INTENTOS"
 * cuando no hay intentos disponibles.
 *
 * Escenario: El modelo inicia con intentos agotados pero créditos disponibles.
 * Acción: Ejecutar el comando HackCommand.
 * Resultado esperado: La salida contiene "SIN INTENTOS".
 */
TEST_F(HackCommandTest, SinIntentos_MensajeEspecifico)
{
    agotarIntentos(); // credits == 250, hackAttempts == 0

    const std::string output {captureOutput()};

    EXPECT_NE(output.find("SIN INTENTOS"), std::string::npos);

    // No debe mostrar OK ni el mensaje de sin créditos
    EXPECT_EQ(output.find("OK"), std::string::npos);
    EXPECT_EQ(output.find("SIN CREDITOS"), std::string::npos);
}

/** @brief Test para verificar que el comando muestra "SIN CREDITOS NI INTENTOS"
 * cuando no hay créditos ni intentos disponibles.
 *
 * Escenario: El modelo inicia con créditos e intentos agotados.
 * Acción: Ejecutar el comando HackCommand.
 * Resultado esperado: La salida contiene "SIN CREDITOS NI INTENTOS".
 */
TEST_F(HackCommandTest, SinCreditosNiIntentos_MensajeEspecifico)
{
    agotarCreditos(); // credits == 0, hackAttempts == 3
    agotarIntentos(); // credits == 0, hackAttempts == 0

    const std::string output {captureOutput()};

    EXPECT_NE(output.find("SIN CREDITOS NI INTENTOS"), std::string::npos);

    // No debe mostrar OK ni los mensajes individuales de sin créditos o sin intentos
    EXPECT_EQ(output.find("OK"), std::string::npos);
    EXPECT_EQ(output.find("SIN INTENTOS"), std::string::npos);
}

/** @brief Test para verificar que el comando no modifica el modelo.
 *
 * Escenario: El modelo inicia con créditos e intentos disponibles.
 * Acción: Ejecutar el comando HackCommand.
 * Resultado esperado: El modelo no debe ser modificado.
 */
TEST_F(HackCommandTest, noModificaModelo)
{
    const auto creditsBefore {m_model.credits()};
    const auto attemptsBefore {m_model.hackAttempts()};

    m_cmd.execute(m_model);

    // El comando no debe modificar créditos ni intentos
    EXPECT_EQ(m_model.credits(), creditsBefore);
    EXPECT_EQ(m_model.hackAttempts(), attemptsBefore);
}