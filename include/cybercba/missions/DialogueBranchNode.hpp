#ifndef CYBERCBA_MISSIONS_DIALOGUE_BRANCH_NODE_HPP
#define CYBERCBA_MISSIONS_DIALOGUE_BRANCH_NODE_HPP

#include "cybercba/missions/NarrativeCondition.hpp"
#include "cybercba/structures/DynamicArray.hpp"
#include <string>

namespace cybercba::missions
{

/// Alias para la representación de las líneas de diálogo
using DialogueLine = std::string;

/**
 * @brief Representa un nodo en el árbol de ramificación de diálogos.
 */
class DialogueBranchNode
{
  public:
    DialogueLine linea;
    NarrativeCondition* pCondicion;
    structures::DynamicArray<DialogueBranchNode*> hijas;

    /**
     * @brief Construye un nodo con su línea de diálogo y condición asociada.
     * @param text Línea de texto candidata.
     * @param cond Puntero a la condición necesaria para activar esta rama (nullptr por defecto).
     */
    explicit DialogueBranchNode(DialogueLine text, NarrativeCondition* cond = nullptr);

    /**
     * @brief Agrega un nodo hijo como sub-rama dependiente.
     * @param pHija Puntero al nodo hijo.
     */
    void agregarHija(DialogueBranchNode* pHija);
};

/**
 * @brief Sistema encargado de resolver y determinar qué líneas de diálogo aplican según el árbol.
 */
class NarrativeConsequenceResolver
{
  private:
    /**
     * @brief Método auxiliar recursivo para explorar y filtrar ramas válidas.
     */
    static void resolverAuxiliar(DialogueBranchNode* pNodo, const progression::SkillProgress& progreso,
                                 structures::DynamicArray<DialogueLine>& resultado);

  public:
    /**
     * @brief Recorre el árbol y retorna las líneas cuyas condiciones fueron satisfechas.
     * @param pRaiz Raíz del árbol de diálogo.
     * @param progreso Progreso actual del jugador.
     * @return DynamicArray con las líneas que corresponden mostrar.
     */
    static structures::DynamicArray<DialogueLine> resolver(DialogueBranchNode* pRaiz,
                                                           const progression::SkillProgress& progreso);
};

} // namespace cybercba::missions

#endif // CYBERCBA_MISSIONS_DIALOGUE_BRANCH_NODE_HPP