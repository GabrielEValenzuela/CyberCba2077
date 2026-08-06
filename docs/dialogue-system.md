# Diálogos

La v0.1.0 usa bloques de diálogo contextual en la apertura y mensajes de descubrimiento (dibujados directamente en `GameApp::drawIntro()`/overlay `Modal::Dialogue`, no vía un grafo de datos). La variación depende de `selectedCharacter` y `otherCharacter`; la entrada se bloquea en la introducción hasta que se confirma.

## DialogueGraph (Phase 2: contenido del prólogo conectado)

`DialogueGraph` (`include/cybercba/Dialogue.hpp`) implementa el formato declarativo que este documento pedía como "futuro": `DialogueLine{id, speaker, npcName, text, textForEmma, textForMagga, requiredFlag, nextLineId, choices}`. El hablante `OtherProtagonist` resuelve automáticamente al personaje **no** seleccionado por el jugador (la fuente de la transmisión). `cybercba::prologueDialogue()` (`src/domain/PrologueContent.cpp`) contiene las líneas reales del prólogo: apertura (`opening_network`, `opening_fireflies`, `opening_blackout`), evidencia (`evidence_photo`, `evidence_insignia`, `evidence_network_map`, `evidence_unsent_message`, `evidence_train41_record`), transmisión (`transmission_fragment`, `transmission_complete`), la elección narrativa (`choice_prompt`: responder/analizar/desconectar) y el cierre (`neometro_ending`).

Casi todas estas líneas usan `textForEmma`/`textForMagga` para diferenciar la voz según el protagonista elegido (p. ej. `opening_blackout` narra la versión de la transmisión falsa que cada uno recibió; `evidence_train41_record` cambia entre "destrucción confirmada" y "el tren no está perdido, está muerto"), siguiendo el documento canónico de historia (`CYBERCBA 2077.md`, §7 "v0.1.0 — La última transmisión").

**Estado real**: `GameApp::interact()` usa `prologueDialogue().resolveText(...)` para los textos de `inspect_photo`/`inspect_map` (Phase 2). El resto de las líneas (apertura, transmisión, elección, cierre) **todavía no están conectadas a `GameApp`** — la introducción y los mensajes de transmisión siguen siendo texto fijo en `drawIntro()`/`Modal::Dialogue`. Migrar esos puntos a `DialogueGraph` queda para el siguiente incremento.
