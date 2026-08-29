#ifndef CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP
#ifndef CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP
#define CYBERCBA_MISSIONS_NARRATIVE_CONDITION_HPP

#include "cybercba/progression/ProgressionSystem.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>
/*
 *
 *  Paso de progreso por refencia
 *  Cuenta por niveles del progreso
 *  Division del codigo en las partes correspondientes
 *  Dividir CPP y HPP correspondientes
 *
 *
 *  Escribir brief y explicaciones por funcion
 *
 *
 */
namespace cybercba::missions
{

// Usamos alias o el tipo de línea propio del proyecto
using DialogueLine = std::string;

// --- Interfaz Base ---
class NarrativeCondition
{
public:
    virtual ~NarrativeCondition() = default;
    virtual bool estaSatisfecha(const progression::SkillProgress& progreso) = 0;
};

// --- Condición 1: Evaluación de Flag / Habilidad ---
class CondicionDeFlag : public NarrativeCondition
{
    progression::SkillId m_skillId;
    mutable std::size_t m_evalCount{0}; // Para verificar en tests si llegó a evaluarse

public:
    explicit CondicionDeFlag(progression::SkillId id) : m_skillId(id) {}

    bool estaSatisfecha(const progression::SkillProgress& progreso) override
    {
        ++m_evalCount;
        // Evalúa si el jugador subió la habilidad a Nivel II
        return progreso.levelOf(m_skillId) == progression::SkillLevel::LevelII;
    }

    std::size_t evalCount() const { return m_evalCount; }
};

// --- Condición 2: Compuesta (AND) ---
class TodasLasCondiciones : public NarrativeCondition
{
private:
    structures::DynamicArray<NarrativeCondition*> m_condiciones;

public:
    void agregarCondicion(NarrativeCondition* pCondicion)
    {
        m_condiciones.pushBack(pCondicion);
    }

    bool estaSatisfecha(const progression::SkillProgress& progreso) override
    {
        for (std::size_t i = 0; i < m_condiciones.size(); ++i)
        {
            if (!m_condiciones[i]->estaSatisfecha(progreso))
            {
                return false; // Cortocircuito
            }
        }
        return true;
    }
};

// --- Nodo del Árbol de Diálogos ---
class DialogueBranchNode
{
public:
    DialogueLine linea;
    NarrativeCondition* pCondicion;
    structures::DynamicArray<DialogueBranchNode*> hijas;

    explicit DialogueBranchNode(DialogueLine text, NarrativeCondition* cond = nullptr)
        : linea(std::move(text)), pCondicion(cond) {}

    void agregarHija(DialogueBranchNode* pHija)
    {
        hijas.pushBack(pHija);
    }
};

// --- Resolutor del Árbol ---
class NarrativeConsequenceResolver
{
public:
    static void resolverAuxiliar(DialogueBranchNode* pNodo,
                         const progression::SkillProgress& progreso,
                         structures::DynamicArray<DialogueLine>& resultado)
    {
        if (pNodo == nullptr)
        {
            return;
        }

        // Si el nodo tiene condición y NO se cumple, la rama se descarta completamente
        if (pNodo->pCondicion != nullptr && !pNodo->pCondicion->estaSatisfecha(progreso))
        {
            return; // Detiene el recorrido sin evaluar ramas hijas
        }

        // Si se cumple, agregamos la línea activa
        resultado.pushBack(pNodo->linea);

        // Recorrido recursivo sobre cada sub-rama
        for (std::size_t i = 0; i < pNodo->hijas.size(); ++i)
        {
            resolverAuxiliar(pNodo->hijas[i], progreso, resultado);
        }
    }

    static structures::DynamicArray<DialogueLine> resolver(DialogueBranchNode* pRaiz,
                                                   const progression::SkillProgress& progreso)
    {
        structures::DynamicArray<DialogueLine> resultado;
        resolverAuxiliar(pRaiz, progreso, resultado);
        return resultado;
    }
};

} // namespace cybercba::missions

#endif
#endif