#include "cybercba/missions/DialogueBranchNode.hpp"
#include <utility>

namespace cybercba::missions
{

DialogueBranchNode::DialogueBranchNode(DialogueLine text, NarrativeCondition* cond)
    : linea(std::move(text)), pCondicion(cond)
{
}

void DialogueBranchNode::agregarHija(DialogueBranchNode* pHija)
{
    hijas.pushBack(pHija);
}

void NarrativeConsequenceResolver::resolverAuxiliar(DialogueBranchNode* pNodo,
                                                    const progression::SkillProgress& progreso,
                                                    structures::DynamicArray<DialogueLine>& resultado)
{
    if (pNodo == nullptr)
    {
        return;
    }

    // Si el nodo tiene condición y no se cumple, detenemos la evaluación de la rama
    if (pNodo->pCondicion != nullptr && !pNodo->pCondicion->estaSatisfecha(progreso))
    {
        return;
    }

    resultado.pushBack(pNodo->linea);

    // Recorrido recursivo de los nodos hijos
    for (std::size_t i = 0; i < pNodo->hijas.size(); ++i)
    {
        resolverAuxiliar(pNodo->hijas[i], progreso, resultado);
    }
}

structures::DynamicArray<DialogueLine>
NarrativeConsequenceResolver::resolver(DialogueBranchNode* pRaiz, const progression::SkillProgress& progreso)
{
    structures::DynamicArray<DialogueLine> resultado;
    resolverAuxiliar(pRaiz, progreso, resultado);
    return resultado;
}

} // namespace cybercba::missions