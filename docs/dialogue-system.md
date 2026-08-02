# Diálogos

La v0.1.0 usa bloques de diálogo contextual en la apertura y mensajes de descubrimiento. La variación depende de `selectedCharacter` y `otherCharacter`; la entrada se bloquea en la introducción hasta que se confirma. Los datos narrativos futuros deben declarar hablante, texto, condiciones, opciones y evento de fin; el evento sólo debe avanzar `MissionSystem` o `NarrativeState`.
