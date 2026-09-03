#include "cybercba/combat/GuardSquad.hpp"

#include <gtest/gtest.h>

using cybercba::combat::GuardSquad;

// ---------------------------------------------------------------------
// Propagación en fila: g0 - g1 - g2 - g3
// El nivel debe ir bajando un escalón por cada salto desde el origen.
// ---------------------------------------------------------------------
TEST(GuardSquadTest, PropagacionEnFilaBajaUnNivelPorSalto)
{
    GuardSquad squad;

    const std::size_t g0 = squad.addMiembro();
    const std::size_t g1 = squad.addMiembro();
    const std::size_t g2 = squad.addMiembro();
    const std::size_t g3 = squad.addMiembro();

    // Se conecta en fila: cada guardia es vecino del siguiente.
    squad.conectar(g0, g1);
    squad.conectar(g1, g2);
    squad.conectar(g2, g3);

    // Se dispara la alerta desde el origen (g0) con nivel 3.
    squad.propagarAlerta(g0, 3);

    // El origen se queda con el nivel completo, y cada salto siguiente
    // recibe un escalón menos, tal como indica el diagrama de flujo.
    EXPECT_EQ(squad.miembro(g0).nivelAlerta(), 3);
    EXPECT_EQ(squad.miembro(g1).nivelAlerta(), 2);
    EXPECT_EQ(squad.miembro(g2).nivelAlerta(), 1);
    EXPECT_EQ(squad.miembro(g3).nivelAlerta(), 0);
}

// ---------------------------------------------------------------------
// Propagación en círculo: g0 - g1 - g2 - g0
// No debe colgarse (test con timeout implícito de gtest) ni romperse,
// gracias a la poda por nivel decreciente.
// ---------------------------------------------------------------------
TEST(GuardSquadTest, PropagacionEnCirculoNoSeCuelga)
{
    GuardSquad squad;

    const std::size_t g0 = squad.addMiembro();
    const std::size_t g1 = squad.addMiembro();
    const std::size_t g2 = squad.addMiembro();

    // Círculo: g0 -> g1 -> g2 -> g0
    squad.conectar(g0, g1, /*bidireccional=*/false);
    squad.conectar(g1, g2, /*bidireccional=*/false);
    squad.conectar(g2, g0, /*bidireccional=*/false);

    // Si esta línea se cuelga, el test directamente no termina y el
    // framework lo va a reportar como timeout/falla — esa es la prueba
    // de que no hay loop infinito.
    squad.propagarAlerta(g0, 3);

    // Verificamos que efectivamente se haya propagado dando la vuelta:
    // g0 (nivel 3) -> g1 (nivel 2) -> g2 (nivel 1) -> intenta volver a
    // g0 con nivel 0, pero 0 no supera el 3 que g0 ya tenía, así que
    // ahí corta.
    EXPECT_EQ(squad.miembro(g0).nivelAlerta(), 3);
    EXPECT_EQ(squad.miembro(g1).nivelAlerta(), 2);
    EXPECT_EQ(squad.miembro(g2).nivelAlerta(), 1);
}

// ---------------------------------------------------------------------
// Un guardia no debe "bajar" de nivel por recibir un aviso más débil
// desde otro lado.
// ---------------------------------------------------------------------
TEST(GuardSquadTest, GuardiaNoBajaDeNivelPorAvisoMasDebil)
{
    GuardSquad squad;

    const std::size_t g0 = squad.addMiembro();
    const std::size_t g1 = squad.addMiembro();

    squad.conectar(g0, g1);

    // Primero g1 recibe una alerta fuerte, directamente.
    squad.propagarAlerta(g1, 5);
    EXPECT_EQ(squad.miembro(g1).nivelAlerta(), 5);

    // Ahora g0 se alerta con un nivel bajo y le avisa a g1 con nivel 0
    // (un escalón menos). Como 0 no supera el 5 que g1 ya tenía, el
    // nivel de g1 no debe cambiar.
    squad.propagarAlerta(g0, 1);

    EXPECT_EQ(squad.miembro(g0).nivelAlerta(), 1);
    EXPECT_EQ(squad.miembro(g1).nivelAlerta(), 5); // sigue en 5, no bajó a 0
}
