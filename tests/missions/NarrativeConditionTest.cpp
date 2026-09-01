#include "cybercba/missions/DialogueBranchNode.hpp"
#include "cybercba/missions/NarrativeCondition.hpp"
#include "cybercba/progression/ProgressionSystem.hpp"
#include <gtest/gtest.h>

namespace cybercba::missions
{
namespace
{

// Test 1: Comprueba la resolución de un árbol de 3 niveles de profundidad
TEST(NarrativeConditionTest, ResuelveArbolDeTresNivelesCorrectamente)
{
    progression::SkillProgress progreso;
    progression::ProgressionSystem sistemaProgreso;

    // Subimos Hacking a LevelII (Nivel 2)
    sistemaProgreso.applyUpgrade(progreso, progression::SkillId::Hacking);

    // Condiciones individuales
    CondicionDeFlag condHacking(progression::SkillId::Hacking, progression::SkillLevel::LevelII);
    CondicionDeFlag condCombat(progression::SkillId::Combat, progression::SkillLevel::LevelII);

    // Construcción del árbol (3 Niveles)
    // Nivel 1 (Raíz): Sin condición
    DialogueBranchNode raiz("Inicio de mision");

    // Nivel 2: Rama A (Se cumple) y Rama B (No se cumple)
    DialogueBranchNode ramaHacking("Opcion Hackeo disponible", &condHacking);
    DialogueBranchNode ramaCombat("Opcion Combate disponible", &condCombat);

    // Nivel 3: Sub-rama de Hacking
    DialogueBranchNode subRamaHacking("Acceso a datos confidenciales obtenido");

    raiz.agregarHija(&ramaHacking);
    raiz.agregarHija(&ramaCombat);
    ramaHacking.agregarHija(&subRamaHacking);

    // Ejecución del resolutor
    const structures::DynamicArray<DialogueLine> resultado =
        NarrativeConsequenceResolver::resolver(&raiz, progreso);

    // Verificaciones
    ASSERT_EQ(resultado.size(), 3u);
    EXPECT_EQ(resultado[0], "Inicio de mision");
    EXPECT_EQ(resultado[1], "Opcion Hackeo disponible");
    EXPECT_EQ(resultado[2], "Acceso a datos confidenciales obtenido");
}

// Test 2: Verifica que una rama no activada no llega a evaluar a sus hijas
TEST(NarrativeConditionTest, RamaNoActivadaNoEvaluaSusHijas)
{
    progression::SkillProgress progreso;

    // Hacking no se mejora, por ende condHacking será false
    CondicionDeFlag condHacking(progression::SkillId::Hacking, progression::SkillLevel::LevelII);
    CondicionDeFlag condSubRama(progression::SkillId::Stealth, progression::SkillLevel::LevelII);

    DialogueBranchNode raiz("Raiz");
    DialogueBranchNode ramaBloqueada("Bloqueada por falta de Hacking", &condHacking);
    DialogueBranchNode subRamaHija("Sub-rama no alcanzable", &condSubRama);

    raiz.agregarHija(&ramaBloqueada);
    ramaBloqueada.agregarHija(&subRamaHija);

    // Ejecución
    const structures::DynamicArray<DialogueLine> resultado =
        NarrativeConsequenceResolver::resolver(&raiz, progreso);

    // La rama bloqueada se evaluó 1 vez (y dio false)
    EXPECT_EQ(condHacking.evalCount(), 1u);

    // REQUERIMIENTO DoD: La sub-rama jamás debió evaluarse (evalCount == 0)
    EXPECT_EQ(condSubRama.evalCount(), 0u);

    ASSERT_EQ(resultado.size(), 1u);
    EXPECT_EQ(resultado[0], "Raiz");
}

// Test 3: Evalúa el uso de condiciones compuestas (TodasLasCondiciones - AND)
TEST(NarrativeConditionTest, EvaluaCondicionCompuestaTodasLasCondiciones)
{
    progression::SkillProgress progreso;
    progression::ProgressionSystem sistemaProgreso;

    // Mejoramos Hacking y Stealth
    sistemaProgreso.applyUpgrade(progreso, progression::SkillId::Hacking);
    sistemaProgreso.applyUpgrade(progreso, progression::SkillId::Stealth);

    CondicionDeFlag condHacking(progression::SkillId::Hacking, progression::SkillLevel::LevelII);
    CondicionDeFlag condStealth(progression::SkillId::Stealth, progression::SkillLevel::LevelII);

    TodasLasCondiciones condCompuesta;
    condCompuesta.agregarCondicion(&condHacking);
    condCompuesta.agregarCondicion(&condStealth);

    DialogueBranchNode raiz("Raiz");
    DialogueBranchNode ramaInfiltracion("Infiltracion silenciosa ejecutada", &condCompuesta);

    raiz.agregarHija(&ramaInfiltracion);

    const structures::DynamicArray<DialogueLine> resultado =
        NarrativeConsequenceResolver::resolver(&raiz, progreso);

    ASSERT_EQ(resultado.size(), 2u);
    EXPECT_EQ(resultado[0], "Raiz");
    EXPECT_EQ(resultado[1], "Infiltracion silenciosa ejecutada");
}

} // namespace
} // namespace cybercba::missions