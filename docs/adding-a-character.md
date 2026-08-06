# Agregar un personaje (NPC/enemigo) — v0.1.0

`CharacterModule` (`include/cybercba/CharacterModule.hpp`) es el contrato que cada estudiante implementa para uno de los nueve roles de NPC/enemigo de "La última transmisión" que hoy no tienen identidad propia (solo texto plano en `GameApp::nodeBody()` o un `EncounterParticipant` genérico). Es andamiaje: define la interfaz y registra los nueve módulos en `CharacterRoster`, pero **no los conecta todavía** a `MissionGraph`/`GameApp` — esa integración queda como paso siguiente, explícito y separado (ver "Pendiente" más abajo).

## Los nueve roles (v0.1.0, `CYBERCBA 2077.md` §7)

| id | Personaje | Rol técnico | Beat narrativo |
| --- | --- | --- | --- |
| `luma` | Nora "Luma" Pereyra | `NarrativeVoice` | Voz en el recuerdo del apagón ("Emma, tenemos que movernos.") |
| `sistema` | Sistema | `NarrativeVoice` | Voz automatizada del refugio/generador |
| `voz_falsa_magga` | Voz falsa de Magga | `NarrativeVoice` | Prueba fabricada que recibe Emma durante el apagón |
| `orden_falsa` | Orden falsa (firma de Emma) | `NarrativeVoice` | Prueba fabricada que recibe Magga durante el apagón |
| `rele_interferencia` | Relé de interferencia | `EncounterOpponent` | Antagonista del hacking de Emma (`protagonist_encounter`) |
| `dron_seguridad` | Dron de seguridad | `EncounterOpponent` | Antagonista del combate de Magga (`protagonist_encounter`) |
| `sistema_vigilancia` | Sistema de vigilancia | `EncounterOpponent` | Obstáculo de Emma en `security_control` (hoy sin encuentro propio) |
| `patrulla_seguridad` | Patrulla de seguridad | `EncounterOpponent` | Obstáculo de Magga en `security_control` (hoy sin encuentro propio) |
| `tomo` | Tomás "Tomo" Ferreyra | `NarrativeVoice` | Cameo/anticipo en `neometro_entrance`; su rol completo empieza en v0.2.0 |

## El contrato

```cpp
class ICharacterModule
{
public:
    virtual std::string id() const = 0;
    virtual std::string displayName() const = 0;
    virtual CharacterRole role() const = 0; // NarrativeVoice | EncounterOpponent

    virtual void contributeDialogue(DialogueGraph& graph) const = 0;
    virtual EncounterParticipant buildParticipant() const = 0;
    virtual std::vector<ActionDefinition> buildActions() const = 0;
};
```

Cada módulo contribuye a **un solo sistema**, según `role()`:

- `NarrativeVoice`: implementa `contributeDialogue()` con las líneas reales del personaje (`DialogueSpeaker::Npc`, `npcName` propio). `buildParticipant()`/`buildActions()` devuelven valores vacíos — no se llaman.
- `EncounterOpponent`: implementa `buildParticipant()`/`buildActions()` con los stats/acciones reales del oponente. `contributeDialogue()` es no-op — no se llama.

Los nueve archivos ya existentes (`src/domain/characters/*Character.{hpp,cpp}`) son stubs compilables marcados `TODO(student)`: devuelven una línea de diálogo placeholder o un participante/acción genéricos. Reemplazar el contenido de un stub no requiere tocar la interfaz ni el roster.

## Roster

`buildPrologueCharacterRoster()` (`src/domain/characters/PrologueCharacters.cpp`) instancia los nueve módulos. `CharacterRoster::find(id)` permite ubicar uno por id; `modules()` expone la lista completa. Ver `tests/test_character_module.cpp` para el contrato verificado (9 módulos, ids únicos, roles cubiertos, contribuciones no vacías).

## Pendiente (fuera de esta pasada)

El roster hoy vive desconectado del flujo jugable. Conectarlo implica, por rol:

- **`NarrativeVoice`**: mezclar `contributeDialogue()` de cada módulo dentro de `prologueDialogue()` (o llamarlos por separado desde `GameApp::nodeBody()` en el punto narrativo correspondiente — recuerdo del apagón, generador, entrada del Neometro).
- **`EncounterOpponent`**: reemplazar el `EncounterParticipant` inline de `buildEmmaHackingEncounter()`/`buildMaggaCombatEncounter()` en `src/domain/EncounterContent.cpp` por `rele_interferencia`/`dron_seguridad`; diseñar los encuentros nuevos de `security_control` para `sistema_vigilancia`/`patrulla_seguridad` (hoy ese nodo es una única acción de confirmación, no un `Encounter` — ver "Pendientes no bloqueantes" en `docs/roadmap.md`).

Ninguno de estos pasos debe requerir cambios en `CharacterModule.hpp` — si un rol necesita un campo nuevo del contrato, es señal de que falta un caso en el contrato genérico, no una excepción por personaje.
