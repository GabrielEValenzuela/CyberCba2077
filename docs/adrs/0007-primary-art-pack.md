# ADR 0007: Pack primario de arquitectura de interior

**Superseded**: el pack elegido acá (Pixel Cyberpunk Interior) y el resto de los packs de terceros evaluados fueron removidos del repositorio — nunca llegaron a integrarse de forma estable en el render real, y el proyecto pasó a generar su arte estático con IA (`assets/generated/`, ver ADR `0017-cinematic-ui-pivot.md` y `docs/third-party-assets.md`). Este documento queda como registro histórico de la decisión original.

## Contexto

Se auditaron cuatro archivos comprimidos presentes localmente en `assets/` (no versionados en git): un pack de GUI cyberpunk de CraftPix, un pack "Cyberpunk_City_Ext" sin licencia identificable, un tileset de interior "Pixel Cyberpunk Interior" de dylestorm, y un pack de sprites "Striker" sin autor identificable. Se necesita un único pack base para arquitectura/mobiliario de interior del refugio (zona de descanso, taller, comunicaciones, pasillo técnico, salida), evitando mezclar dos tilesets completos de interior (look "collage").

## Decisión

Se elige **Pixel Cyberpunk Interior** (DyLESTorm / livingtheindie, https://livingtheindie.itch.io/pixel-cyberpunk-interior) como pack primario de interior.

Razones:
- Autor identificado y licencia verificada directamente en la página oficial de itch.io (comercial y no comercial permitido, sin requisito de atribución, sin reventa del pack).
- Tile size 32x32 px, coincide exactamente con la convención de gameplay del proyecto (`docs/art-direction.md`).
- Perspectiva y paleta (fría, cian) compatibles con la dirección artística existente (Emma/cian, Magga/ámbar-rojo apagado se aplican como acentos sobre esta base, no como reemplazo de paleta).
- Es el único de los cuatro packs con licencia y autoría confirmables sin ambigüedad.

## Alternativas rechazadas

- **Cyberpunk_City_Ext_V1.0**: no se pudo confirmar autor ni licencia en esta sesión (ver `assets/imports/rejected/PENDING_VERIFICATION.md`). Se reserva únicamente como candidato para base **exterior**, no interior, y solo tras verificación manual futura.
- **CraftPix GUI**: es un pack de interfaz (frames, botones, iconos, cursores), no un tileset de arquitectura — no es candidato a pack de interior por diseño propio del pack.
- **Striker**: es un pack de sprites de personaje/enemigo, no de arquitectura — no aplica.

## Responsabilidades por pack (para evitar look "collage")

| Pack | Rol exclusivo |
|---|---|
| Pixel Cyberpunk Interior | Arquitectura y mobiliario de interior únicamente |
| CraftPix GUI | Iconos, frames, cursores y elementos de UI complementarios únicamente — nunca arquitectura ni personajes |
| Kenney (no descargado aún, gap documentado) | UI/prompts/iconos/minimapa/partículas/luces/cursores/sonido complementarios |
| "OcO" (no identificado) | Enemigos/NPCs — ningún pack local se confirmó con esta identidad |
| Cyberpunk_City_Ext (pendiente de verificación) | Base exterior únicamente, si se verifica licencia |
| Assets narrativos hechos a mano existentes | Props únicos de la narrativa (dron, terminal, transmisor, mobiliario del refugio ya modelado) — no se regeneran |

## Cómo se evita el look "collage"

- Un solo tileset resuelve toda la arquitectura/mobiliario de interior; los demás packs solo aportan iconografía/UI o entidades puntuales (enemigos), nunca geometría de nivel competidora.
- `docs/asset-integration-style.md` fija paleta base, escala de tile, perspectiva y reglas de outline derivadas de Pixel Cyberpunk Interior antes de mezclar cualquier prop de otro origen.
- `TEXTURE_FILTER_POINT` se mantiene como filtro único (`src/ui/AssetStore.cpp`), evitando que un pack se vea "suavizado" frente a otro con nearest-neighbor.

## Estado

Decisión tomada y documentada. **Integración parcial en código intentada y luego revertida**: una sesión posterior recortó 6 tiles a `assets/processed/environment/` y conectó 3 como `WorldProp` decorativos no bloqueantes en `src/application/WorldLayout.cpp` (sofá, silla, panel de consola en el refugio). Verificación visual del usuario mostró que, a tamaño nativo (25-57px, pixel-art plano), esos props se leían como iconos diminutos flotando junto al mobiliario propio existente (190-255px, ilustración semi-realista con sombreado) — un efecto "collage" claro. Se revirtió el wiring (`SHELTER` vuelve a 8 props); los PNG recortados y sus entradas de manifest se conservan como referencia para una futura migración completa del shell de la sala a tilemap (ver `docs/asset-integration-style.md` §"Estado real vs. pendiente"), pero ningún tile de este pack debe usarse como prop aislado mezclado con arte propio. El "shell" completo de la sala (paredes/piso, `GameApp::drawShelterShell`) sigue siendo arte procedural hecho a mano, no un tilemap. Se agregó además `assets/vendor/craftpix-184808-icons/` (quinto pack, CraftPix, verificado pero sin iconos individuales seleccionados todavía).
