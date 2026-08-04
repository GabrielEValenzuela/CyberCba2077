# Biblia de campaña — CYBERCBA 2077

Fuente de verdad narrativa. Los hechos aquí listados no deben contradecirse en código, diálogo ni documentación futura; si un documento difiere, este es el que manda para lore/nombres. El contenido jugable real que implementa estos hechos vive en `src/domain/PrologueContent.cpp` (`buildPrologueMissionGraph`, `prologueDialogue`, `buildPrologueEvidenceCatalog`).

## Título y subtítulo

- Título de campaña: **CYBERCBA 2077** — subtítulo **THE CITY THAT FORGOT US** / en español **LA CIUDAD QUE NOS OLVIDÓ**.
- `v0.1.0`: **THE LAST TRANSMISSION** / **LA ÚLTIMA TRANSMISIÓN** (implementado, ver `docs/roadmap.md`).
- `v0.2.0` (planeado): **NEOMETRO: THE LAST CONVOY** / **NEOMETRO: EL ÚLTIMO CONVOY**.

## La Red

Infraestructura que controla Neo-Córdoba: energía, transporte, identidad, seguridad, comunicaciones, registros médicos, permisos de acceso, distribución de recursos, archivos de la ciudad. Cuando falla, distritos enteros desaparecen. Se introduce en `prologueDialogue()` como `opening_network` (apertura del prólogo), no solo como lore de fondo.

## La Luciérnaga

Red clandestina de ayuda mutua que restauraba servicios abandonados por La Red: electricidad, rutas de evacuación, identidades borradas, información censurada, comunicaciones. Emma y Magga fueron miembros. Se introduce como `opening_fireflies`.

## La Noche del Apagón

La Luciérnaga fue expuesta; varios miembros desaparecieron. Emma y Magga llegaron a puntos de evacuación distintos y cada una recibió pruebas manipuladas de que la otra la había traicionado. Nunca volvieron a verse. Los registros de esa noche fueron alterados; la verdad completa es desconocida — no se resuelve en v0.1.0. Se introduce como `opening_blackout`.

## Emmanuel "Emma" Vázquez

Origen: Santa María. Especialidad: Intrusión de enlace (hacking). Analítica, observadora, emocionalmente reservada, hábil leyendo sistemas, reacia a exponer vulnerabilidad, todavía dolida por lo que pasó con Magga. Identidad de juego: análisis, debuffs, control, manipulación de reglas de encuentro.

## Magalí "Magga" Temerro

Origen: San Justo. Especialidad: Impacto táctico (combate). Cabello largo rubio oscuro/castaño claro y ojos verdes claramente visibles. Decidida, directa, protectora, desconfiada, emocionalmente intensa, todavía enojada por el aparente abandono de Emma. Identidad de juego: daño directo, guardia, contraataques, ruptura de defensas.

El jugador elige un protagonista; el no elegido es la voz de la transmisión, el foco emocional de la búsqueda y la fuente de recuerdos contradictorios (`DialogueSpeaker::OtherProtagonist` resuelve automáticamente al personaje no elegido — `include/cybercba/Dialogue.hpp`).

## Tren 41

Registro oficial: fue destruido la noche del apagón (`evidence_train41_record`). No debería existir señal alguna en esa línea. Su reaparición en la red (`transmission_complete`) es la prueba de que "empezaron de nuevo" — la pregunta central del prólogo.

## Pregunta central de la campaña

> ¿Qué hacés cuando la persona que creíste que te abandonó te pide que no confíes en tus propios recuerdos?

No se representa con un medidor de romance visible. Estado narrativo interno (`NarrativeState`, `include/cybercba/Campaign.hpp`): `trust`, `unresolvedHurt`, `recoveredTruth`, `caution`, `attachment` — enteros internos, nunca expuestos como barra numérica en la UI.

## Verdades conocidas (v0.1.0)

- La Red controla Neo-Córdoba y falla en los márgenes.
- La Luciérnaga existió y ayudó a esos márgenes.
- Emma y Magga fueron pareja antes del apagón.
- El Tren 41 fue destruido oficialmente y no debería transmitir nada.
- El símbolo de La Luciérnaga en la entrada del Neometro es reciente (`neometro_ending`): alguien volvió, o nunca se fue.

## Misterios sin resolver (v0.1.0)

- Quién manipuló los registros de la Noche del Apagón y por qué.
- Quién o qué reactivó el Tren 41.
- Si la voz de la transmisión es real, una copia, o algo más.
- Qué encontrará el protagonista dentro del Neometro (`v0.2.0`).
