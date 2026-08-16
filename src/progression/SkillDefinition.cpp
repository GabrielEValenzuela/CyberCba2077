#include "cybercba/progression/SkillDefinition.hpp"

#include <cstddef>

namespace cybercba::progression
{

namespace
{

constexpr int COMMON_SKILL_COUNT = 5;

constexpr SkillDefinition SKILL_TABLE[] = {
    {SkillId::Perception, "Percepción", "Detecta detalles obvios.", "Detecta trampas y rutas ocultas.",
     "Detecta contradicciones sutiles en el entorno."},
    {SkillId::Persuasion, "Persuasión", "Negocia en situaciones simples.", "Reduce hostilidad inicial.",
     "Consigue cooperación de NPCs reticentes."},
    {SkillId::Stealth, "Sigilo", "Reduce exposición básica.", "Evita confrontaciones menores.",
     "Utiliza rutas discretas avanzadas."},
    {SkillId::Resistance, "Resistencia", "Mejora tolerancia a daño leve.", "Mejora recuperación tras fallos.",
     "Sobrevive consecuencias físicas graves."},
    {SkillId::Improvisation, "Improvisación", "Aprovecha recursos inesperados básicos.",
     "Convierte fallos menores en alternativas.", "Resuelve con recursos muy limitados."},

    {SkillId::Hacking, "Hackeo", "Accede a sistemas simples.", "Manipula sistemas protegidos.",
     "Controla sistemas críticos de la RED."},
    {SkillId::Cryptography, "Criptografía", "Analiza mensajes simples.", "Descifra claves complejas.",
     "Descifra transmisiones de La RED en tiempo real."},
    {SkillId::Engineering, "Ingeniería", "Interactúa con dispositivos básicos.", "Repara mecanismos dañados.",
     "Reconfigura infraestructura compleja."},
    {SkillId::Drones, "Drones", "Controla un dron básico.", "Aprovecha drones en combate.",
     "Coordina múltiples drones."},
    {SkillId::Intrusion, "Intrusión", "Accede a redes simples.", "Accede a redes protegidas.",
     "Accede a redes críticas sin dejar rastro."},

    {SkillId::Marksmanship, "Puntería", "Resuelve enfrentamientos simples a distancia.",
     "Resuelve enfrentamientos a distancia eficientemente.", "Domina enfrentamientos a distancia complejos."},
    {SkillId::MeleeCombat, "Combate cercano", "Controla enfrentamientos cuerpo a cuerpo simples.",
     "Controla enfrentamientos cuerpo a cuerpo difíciles.", "Domina el combate cercano."},
    {SkillId::Tactics, "Táctica", "Lee el escenario básicamente.", "Utiliza el entorno estratégicamente.",
     "Domina la lectura completa del campo de batalla."},
    {SkillId::Intimidation, "Intimidación", "Genera presión básica.", "Obtiene cooperación mediante presión.",
     "Doblega resistencia significativa mediante presencia."},
    {SkillId::Survival, "Supervivencia", "Se adapta a situaciones hostiles simples.", "Se adapta a recursos limitados.",
     "Sobrevive condiciones extremas."},
};

constexpr std::size_t SKILL_TABLE_COUNT = sizeof(SKILL_TABLE) / sizeof(SKILL_TABLE[0]);

} // namespace

const SkillDefinition& skillDefinition(SkillId id)
{
    for (std::size_t i = 0; i < SKILL_TABLE_COUNT; ++i)
    {
        if (SKILL_TABLE[i].id == id)
        {
            return SKILL_TABLE[i];
        }
    }
    return SKILL_TABLE[0];
}

bool isSkillAvailableTo(SkillId id, cybercba::game::PlayerCharacter character)
{
    const auto value = static_cast<int>(id);
    if (value < COMMON_SKILL_COUNT)
    {
        return true;
    }
    const bool isEmmaSkill = value < COMMON_SKILL_COUNT * 2;
    return isEmmaSkill == (character == cybercba::game::PlayerCharacter::Emma);
}

} // namespace cybercba::progression
