#ifndef CYBERCBA_MISSIONS_DIALOGUE_BRANCH_NODE_HPP
#define CYBERCBA_MISSIONS_DIALOGUE_BRANCH_NODE_HPP

#include "cybercba/missions/NarrativeCondition.hpp"
#include "cybercba/progression/ProgressionSystem.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

namespace cybercba::missions
{

/// Alias para representar una línea de texto dentro del sistema de diálogo.
using DialogueLine = std::string;

/**
 * @brief Estructura que representa un nodo dentro del árbol de decisiones narrativas.
 */
struct DialogueBranchNode
{
    DialogueLine linea;
    NarrativeCondition* pCondicion{nullptr};
    structures::DynamicArray<DialogueBranchNode*> hijas;

    /**
     * @brief Constructor del nodo de diálogo.
     * @param text Texto o línea de diálogo a mostrar.
     * @param cond Puntero a la condición narrativa asociada (puede ser nullptr).
     */
    DialogueBranchNode(DialogueLine text, NarrativeCondition* cond = nullptr);

    /**
     * @brief Agrega un nodo hijo a la lista de ramificaciones.
     * @param pHija Puntero al nodo hijo.
     */
    void agregarHija(DialogueBranchNode* pHija);
};

/**
 * @brief Evaluador encargado de recorrer el árbol y recolectar las líneas válidas.
 */
class NarrativeConsequenceResolver
{
  private:
    /**
     * @brief Función auxiliar recursiva para explorar los nodos del árbol.
     */
    static void resolverAuxiliar(DialogueBranchNode* pNodo, const progression::SkillProgress& progreso,
                                 structures::DynamicArray<DialogueLine>& resultado);

  public:
    /**
     * @brief Evalúa el árbol desde la raíz y devuelve las líneas de diálogo habilitadas.
     * @param pRaiz Puntero al nodo raíz del árbol.
     * @param progreso Estado actual de habilidades del jugador.
     * @return Arreglo dinámico con las líneas de diálogo que se deben mostrar.
     */
    static structures::DynamicArray<DialogueLine> resolver(DialogueBranchNode* pRaiz,
                                                           const progression::SkillProgress& progreso);
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_DIALOGUE_BRANCH_NODE_HPP