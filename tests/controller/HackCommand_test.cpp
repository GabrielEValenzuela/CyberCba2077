#include "HackCommand.hpp"
#include "model/gameModel.hpp"

#include <gtest/gtest.h>

#include <iostream>
#include <sstream>
#include <string>

using namespace CyberpunkCba;

// =============================================================================
// Fixture compartida
// =============================================================================

/**
 * @brief Fixture para todos los tests de HackCommand.
 *
 * @details
 * Estado inicial del modelo ("Ghost_47"):
 *   - credits()      == 250  → suficientes (hackCost == 15)
 *   - hackAttempts() == 3    → disponibles
 *   - alertLevel()   == Low  → dificultad EASY
 */
class HackCommandTest : public ::testing::Test
{
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

    GameModel   m_model {"Ghost_47"};
    HackCommand m_cmd;
};

// =============================================================================
// Test 1 — Disponible: ambas precondiciones OK
// =============================================================================

TEST_F(HackCommandTest, Disponible_MuestraOK)
{
    // Estado inicial: 250 créditos, 3 intentos → disponible
    const std::string output {captureOutput()};

    // Debe mostrar "OK"
    EXPECT_NE(output.find("OK"), std::string::npos);

    // Debe incluir la barra de dificultad
    EXPECT_NE(output.find("["), std::string::npos);

    // Con alertLevel Low → EASY
    EXPECT_NE(output.find("FACIL"), std::string::npos);

    // No debe mostrar mensajes de error
    EXPECT_EQ(output.find("SIN"), std::string::npos);
}

// =============================================================================
// Test 2 — Sin créditos: solo falla la precondición de créditos
// =============================================================================

TEST_F(HackCommandTest, SinCreditos_MensajeEspecifico)
{
    agotarCreditos(); // credits == 0, hackAttempts == 3

    const std::string output {captureOutput()};

    EXPECT_NE(output.find("SIN CREDITOS"), std::string::npos);

    // No debe mostrar OK ni el mensaje de sin intentos
    EXPECT_EQ(output.find("OK"),          std::string::npos);
    EXPECT_EQ(output.find("SIN INTENTOS"), std::string::npos);
}

// =============================================================================
// Test 3 — Sin intentos: solo falla la precondición de intentos
// =============================================================================

TEST_F(HackCommandTest, SinIntentos_MensajeEspecifico)
{
    agotarIntentos(); // hackAttempts == 0, credits == 250

    const std::string output {captureOutput()};

    EXPECT_NE(output.find("SIN INTENTOS"), std::string::npos);

    // No debe mostrar OK ni el mensaje de sin créditos
    EXPECT_EQ(output.find("OK"),           std::string::npos);
    EXPECT_EQ(output.find("SIN CREDITOS"), std::string::npos);
}

// =============================================================================
// Test 4 — Caso borde: créditos == 0 Y intentos == 0 simultáneamente
// =============================================================================

TEST_F(HackCommandTest, SinCreditosNiIntentos_NoCrashea)
{
    agotarCreditos();
    agotarIntentos(); // credits == 0, hackAttempts == 0

    EXPECT_NO_THROW({
        const std::string output {captureOutput()};

        // Debe mostrar el mensaje del caso borde
        EXPECT_NE(output.find("SIN CREDITOS NI INTENTOS"), std::string::npos);

        // No debe confundirse con ningún otro estado
        EXPECT_EQ(output.find("OK"),           std::string::npos);
        EXPECT_EQ(output.find("SIN INTENTOS"), std::string::npos);
    });
}

// =============================================================================
// Test 5 — GameModel no modificado en ningún estado
// =============================================================================

TEST_F(HackCommandTest, Execute_NoModificaGameModel)
{
    // --- Estado 1: disponible ---
    {
        const int  creditosAntes  {m_model.credits()};
        const int  intentosAntes  {m_model.hackAttempts()};
        const auto alertaAntes    {m_model.alertLevel()};
        const bool runningAntes   {m_model.isRunning()};

        captureOutput();

        EXPECT_EQ(m_model.credits(),      creditosAntes);
        EXPECT_EQ(m_model.hackAttempts(), intentosAntes);
        EXPECT_EQ(m_model.alertLevel(),   alertaAntes);
        EXPECT_EQ(m_model.isRunning(),    runningAntes);
    }

    // --- Estado 2: sin créditos ---
    {
        agotarCreditos();

        const int  creditosAntes  {m_model.credits()};
        const int  intentosAntes  {m_model.hackAttempts()};
        const auto alertaAntes    {m_model.alertLevel()};
        const bool runningAntes   {m_model.isRunning()};

        captureOutput();

        EXPECT_EQ(m_model.credits(),      creditosAntes);
        EXPECT_EQ(m_model.hackAttempts(), intentosAntes);
        EXPECT_EQ(m_model.alertLevel(),   alertaAntes);
        EXPECT_EQ(m_model.isRunning(),    runningAntes);
    }

    // --- Estado 3: sin intentos ---
    // Usamos modelo local porque m_model ya tiene créditos en 0
    {
        GameModel   modelLimpio {"Ghost_47"};
        HackCommand cmdLimpio;

        while (modelLimpio.hackAttempts() > 0)
        {
            modelLimpio.consumeHackAttempt();
        }

        const int  creditosAntes  {modelLimpio.credits()};
        const int  intentosAntes  {modelLimpio.hackAttempts()};
        const auto alertaAntes    {modelLimpio.alertLevel()};
        const bool runningAntes   {modelLimpio.isRunning()};

        std::ostringstream oss;
        std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
        cmdLimpio.execute(modelLimpio);
        std::cout.rdbuf(old);

        EXPECT_EQ(modelLimpio.credits(),      creditosAntes);
        EXPECT_EQ(modelLimpio.hackAttempts(), intentosAntes);
        EXPECT_EQ(modelLimpio.alertLevel(),   alertaAntes);
        EXPECT_EQ(modelLimpio.isRunning(),    runningAntes);
    }

    // --- Estado 4: sin ambos ---
    {
        GameModel   modelBorde {"Ghost_47"};
        HackCommand cmdBorde;

        while (modelBorde.credits() > 0)       { modelBorde.spendCredits(1); }
        while (modelBorde.hackAttempts() > 0)  { modelBorde.consumeHackAttempt(); }

        const int  creditosAntes  {modelBorde.credits()};
        const int  intentosAntes  {modelBorde.hackAttempts()};
        const auto alertaAntes    {modelBorde.alertLevel()};
        const bool runningAntes   {modelBorde.isRunning()};

        std::ostringstream oss;
        std::streambuf* old {std::cout.rdbuf(oss.rdbuf())};
        cmdBorde.execute(modelBorde);
        std::cout.rdbuf(old);

        EXPECT_EQ(modelBorde.credits(),      creditosAntes);
        EXPECT_EQ(modelBorde.hackAttempts(), intentosAntes);
        EXPECT_EQ(modelBorde.alertLevel(),   alertaAntes);
        EXPECT_EQ(modelBorde.isRunning(),    runningAntes);
    }
}