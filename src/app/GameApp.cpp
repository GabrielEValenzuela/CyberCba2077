#include "app/GameApp.hpp"

#include <algorithm>
#include <array>
#include <cmath>
#include <cstddef>

#include "config/GameConfig.hpp"
#include "cybercba/Campaign.hpp"
#include "cybercba/CreditsLoader.hpp"
#include "cybercba/EncounterContent.hpp"
#include "cybercba/PrologueContent.hpp"
#include "cybercba/WorldLayout.hpp"
#include "ui/AudioService.hpp"
#include "ui/AssetStore.hpp"
#include "ui/NlmColors.hpp"
#include "ui/widgets/DevOverlay.hpp"
#include "ui/widgets/ActionMenu.hpp"
#include "ui/widgets/ChoicePanel.hpp"
#include "ui/widgets/CurrentObjectivePanel.hpp"
#include "ui/widgets/DialoguePanel.hpp"
#include "ui/widgets/EnemyIntentPanel.hpp"
#include "ui/widgets/EvidencePanel.hpp"
#include "ui/widgets/InteractionPanel.hpp"
#include "ui/widgets/MissionResultsView.hpp"
#include "ui/widgets/ParticipantPanel.hpp"
#include "ui/widgets/PortraitFrame.hpp"
#include "ui/widgets/StoryPanel.hpp"
#include "ui/widgets/TextWrap.hpp"
#include "ui/widgets/TransmissionPanel.hpp"
#include "ui/widgets/TurnOrderPanel.hpp"
#include "ui/widgets/UiMetrics.hpp"

namespace
{
constexpr int W = 1280, H = 720;
// Logical canvas is a 320x180 handheld-style grid rendered at a fixed 4x integer
// scale (see docs/adrs/0016-logical-resolution-integer-scaling.md); the window
// scale below is the additional integer factor applied on top of that native 4x.
float integerScale(int screenWidth, int screenHeight)
{
    return static_cast<float>(std::max(1, std::min(screenWidth / W, screenHeight / H)));
}
constexpr Color DIM {152, 181, 197, 255};
constexpr Color CARD {5, 25, 40, 238};
constexpr const char* EMMA_PORTRAIT = "assets/generated/portraits/portrait_emma_neutral.png";
constexpr const char* MAGGA_PORTRAIT = "assets/generated/portraits/portrait_magga_neutral.png";
constexpr const char* EMMA_ANALYTICAL_PORTRAIT = "assets/generated/portraits/portrait_emma_analytical.png";
constexpr const char* EMMA_HURT_PORTRAIT = "assets/generated/portraits/portrait_emma_hurt.png";
constexpr const char* EMMA_DETERMINED_PORTRAIT = "assets/generated/portraits/portrait_emma_determined.png";
constexpr const char* EMMA_HACKING_PORTRAIT = "assets/generated/portraits/portrait_emma_hacking.png";
constexpr const char* EMMA_COMBAT_PORTRAIT = "assets/generated/portraits/portrait_emma_combat.png";
constexpr const char* MAGGA_SUSPICIOUS_PORTRAIT = "assets/generated/portraits/portrait_magga_suspicious.png";
constexpr const char* MAGGA_ANGRY_PORTRAIT = "assets/generated/portraits/portrait_magga_angry.png";
constexpr const char* MAGGA_HURT_PORTRAIT = "assets/generated/portraits/portrait_magga_hurt.png";
constexpr const char* MAGGA_DETERMINED_PORTRAIT = "assets/generated/portraits/portrait_magga_determined.png";
constexpr const char* MAGGA_COMBAT_PORTRAIT = "assets/generated/portraits/portrait_magga_combat.png";
constexpr const char* EMMA_FIGURE = "assets/generated/figures/figure_emma_idle.png";
constexpr const char* MAGGA_FIGURE = "assets/generated/figures/figure_magga_idle.png";
constexpr const char* EMMA_TRANSMISSION_PORTRAIT = "assets/generated/portraits/portrait_emma_transmission.png";
constexpr const char* MAGGA_TRANSMISSION_PORTRAIT = "assets/generated/portraits/portrait_magga_transmission.png";
constexpr const char* HACKING_ENCOUNTER_BACKGROUND = "assets/generated/encounters/encounter_hacking_background.png";
constexpr const char* SECURITY_DRONE = "assets/generated/encounters/enemy_security_drone.png";
constexpr const char* REFUGE_BACKGROUND = "assets/generated/backgrounds/bg_refuge_rest_area.png";
constexpr const char* NEOMETRO_BACKGROUND = "assets/generated/backgrounds/bg_neometro_entrance.png";
constexpr const char* menuItems[] {"CONTINUAR", "NUEVA PARTIDA", "CAPÍTULOS", "CONFIGURACIÓN", "CRÉDITOS", "SALIR"};
constexpr const char* pauseItems[] {"REANUDAR", "OBJETIVOS", "CONFIGURACIÓN", "REINICIAR CHECKPOINT", "VOLVER AL MENÚ", "SALIR DEL JUEGO"};
struct StageObjective { const char* title; const char* description; };
StageObjective stageObjectiveInfo(cybercba::PrologueStage stage)
{
    switch (stage)
    {
        case cybercba::PrologueStage::Shelter: return {"LA SEÑAL IMPOSIBLE", "Descubrí quién reactivó el transmisor de La Luciérnaga."};
        case cybercba::PrologueStage::Transmission: return {"RECUPERAR EL ENLACE", "Reconstruí la transmisión antes de perder la señal."};
        case cybercba::PrologueStage::Route: return {"TREN 41", "Recuperá las coordenadas antes de que la ruta se cierre."};
        case cybercba::PrologueStage::Convergence: return {"LÍNEA OESTE", "Escapá del refugio y seguí la línea oeste hasta el Neometro."};
        case cybercba::PrologueStage::Epilogue: return {"EL ÚLTIMO CONVOY", "Llegá a la entrada abandonada del Neometro."};
        case cybercba::PrologueStage::Complete: return {"PRÓLOGO COMPLETADO", "La señal se perdió, pero alguien respondió."};
    }
    return {"", ""};
}
// Back-compat short form used where only a single line fits (HUD objective card, menu status line).
const char* stageObjective(cybercba::PrologueStage stage) { return stageObjectiveInfo(stage).description; }
bool inRect(Vector2 p, float x, float y, float width, float height)
{
    return p.x >= x && p.x <= x + width && p.y >= y && p.y <= y + height;
}
// Keeps the original silhouette intact. The generated character assets are
// deliberately tall (2:3), so fitting them to an arbitrary rectangle makes
// them look noticeably wider and less pixel-art-like.
Rectangle aspectFit(const Texture2D& texture, Rectangle bounds)
{
    const float scale = std::min(bounds.width / static_cast<float>(texture.width),
                                 bounds.height / static_cast<float>(texture.height));
    const float width = static_cast<float>(texture.width) * scale;
    const float height = static_cast<float>(texture.height) * scale;
    return {bounds.x + (bounds.width - width) / 2.0F,
            bounds.y + (bounds.height - height) / 2.0F,
            width,
            height};
}

const char* portraitPath(cybercba::CharacterId character)
{
    return character == cybercba::CharacterId::Emma ? EMMA_PORTRAIT : MAGGA_PORTRAIT;
}

const char* portraitPath(cybercba::CharacterId character, const std::string& nodeId)
{
    if (character == cybercba::CharacterId::Emma)
    {
        if (nodeId == "power_recovery") return EMMA_HACKING_PORTRAIT;
        if (nodeId == "blackout") return EMMA_HURT_PORTRAIT;
        if (nodeId == "narrative_choice") return EMMA_DETERMINED_PORTRAIT;
        if (nodeId == "exterior_route" || nodeId == "security_control") return EMMA_COMBAT_PORTRAIT;
        if (nodeId == "refuge" || nodeId == "rest_area" || nodeId == "workshop" || nodeId == "comms_room" || nodeId == "archive")
            return EMMA_ANALYTICAL_PORTRAIT;
        return portraitPath(character);
    }

    if (nodeId == "blackout") return MAGGA_HURT_PORTRAIT;
    if (nodeId == "power_recovery" || nodeId == "narrative_choice") return MAGGA_DETERMINED_PORTRAIT;
    if (nodeId == "exterior_route" || nodeId == "security_control") return MAGGA_ANGRY_PORTRAIT;
    if (nodeId == "refuge" || nodeId == "rest_area" || nodeId == "workshop" || nodeId == "comms_room" || nodeId == "archive")
        return MAGGA_SUSPICIOUS_PORTRAIT;
    return portraitPath(character);
}

const char* figurePath(cybercba::CharacterId character)
{
    return character == cybercba::CharacterId::Emma ? EMMA_FIGURE : MAGGA_FIGURE;
}

const char* transmissionPortraitPath(cybercba::CharacterId character)
{
    return character == cybercba::CharacterId::Emma ? EMMA_TRANSMISSION_PORTRAIT : MAGGA_TRANSMISSION_PORTRAIT;
}

[[maybe_unused]] bool insideShelterFurniture(Vector2 point)
{
    constexpr Rectangle furniture[] {
        {150, 168, 230, 115}, // bed
        {444, 126, 220, 48},  // wall map and ledge
        {734, 182, 272, 118}, // workbench
        {150, 360, 105, 150}, // shelf
        {1037, 218, 88, 168}, // door recess
    };
    for (const Rectangle& item : furniture)
        if (CheckCollisionPointRec(point, item)) return true;
    return false;
}

Color withAlpha(Color color, unsigned char alpha)
{
    color.a = alpha;
    return color;
}

void drawSoftLight(Vector2 center, float radius, Color color)
{
    for (int ring = 5; ring >= 1; --ring)
    {
        const float t = static_cast<float>(ring) / 5.0F;
        DrawCircleV(center, radius * t, withAlpha(color, static_cast<unsigned char>(7 + (1.0F - t) * 12.0F)));
    }
}

[[maybe_unused]] void drawShelterRoom(cybercba::PrologueStage stage, bool reducedMotion, bool reduceFlashes)
{
    const bool transmitting = stage == cybercba::PrologueStage::Transmission;
    const float pulse = reducedMotion ? 0.0F : (std::sin(static_cast<float>(GetTime()) * 5.0F) + 1.0F) * 0.5F;
    const Color floorBase {25, 34, 40, 255};

    // Concrete floor, seams and worn plates make the space readable before any prop is drawn.
    DrawRectangle(88, 108, 1104, 470, floorBase);
    for (int y = 132; y < 570; y += 48)
        for (int x = 106; x < 1178; x += 72)
        {
            const bool alternate = ((x / 72) + (y / 48)) % 2 == 0;
            DrawRectangle(x, y, 68, 44, alternate ? Color {29, 39, 45, 255} : Color {22, 31, 37, 255});
            DrawLine(x, y + 44, x + 68, y + 44, Color {10, 18, 23, 180});
            DrawLine(x + 68, y, x + 68, y + 44, Color {44, 55, 58, 115});
        }

    // Thick walls, their inner shadow, a rain window, and a real doorway establish a room.
    DrawRectangle(70, 88, 1140, 38, Color {12, 19, 25, 255});
    DrawRectangle(70, 88, 1140, 8, Color {56, 69, 72, 255});
    DrawRectangle(70, 558, 1140, 40, Color {10, 17, 22, 255});
    DrawRectangle(70, 558, 1140, 7, Color {47, 58, 61, 255});
    DrawRectangle(70, 88, 28, 510, Color {10, 17, 22, 255});
    DrawRectangle(1182, 88, 28, 510, Color {10, 17, 22, 255});
    DrawRectangle(100, 126, 1080, 10, Color {7, 13, 17, 180});

    DrawRectangle(270, 94, 170, 42, Color {5, 20, 29, 255});
    DrawRectangleLinesEx({270, 94, 170, 42}, 3.0F, Color {42, 91, 105, 255});
    for (int x = 283; x < 430; x += 23) DrawLine(x, 99, x - 14, 131, Color {80, 145, 164, 130});

    // Bed: frame, mattress, pillow, rumpled blanket and under-bed contact shadow.
    DrawEllipse(266, 279, 126, 18, Color {4, 10, 13, 155});
    DrawRectangle(146, 177, 238, 104, Color {30, 22, 22, 255});
    DrawRectangle(153, 170, 225, 94, Color {83, 83, 73, 255});
    DrawRectangle(163, 180, 205, 76, Color {61, 76, 78, 255});
    DrawRectangle(170, 185, 68, 27, Color {173, 169, 147, 255});
    DrawRectangle(240, 182, 118, 69, Color {80, 61, 76, 255});
    DrawLine(245, 206, 352, 224, Color {117, 92, 104, 255});
    DrawLine(250, 234, 350, 243, Color {49, 46, 55, 255});
    DrawRectangle(142, 170, 12, 104, Color {107, 91, 70, 255});
    DrawRectangle(376, 170, 10, 104, Color {88, 72, 56, 255});

    // Shelf, personal photo and supplies.
    DrawRectangle(142, 362, 112, 151, Color {42, 31, 25, 255});
    DrawRectangleLinesEx({142, 362, 112, 151}, 3.0F, Color {102, 76, 53, 255});
    for (int y = 394; y < 500; y += 34) DrawLine(146, y, 250, y, Color {108, 80, 54, 255});
    DrawRectangle(158, 373, 20, 17, Color {126, 66, 46, 255}); DrawRectangle(184, 373, 31, 17, Color {49, 103, 104, 255});
    DrawRectangle(160, 433, 35, 21, Color {105, 96, 70, 255}); DrawRectangle(202, 429, 28, 27, Color {126, 67, 59, 255});
    DrawRectangle(297, 393, 45, 39, Color {49, 39, 34, 255});
    DrawRectangleLinesEx({297, 393, 45, 39}, 2.0F, Color {180, 156, 113, 255});
    DrawRectangle(303, 399, 33, 25, Color {91, 111, 111, 255});
    DrawCircle(313, 409, 5, Color {218, 190, 158, 255}); DrawCircle(327, 409, 5, Color {196, 165, 136, 255});
    // A loose, unsent message anchors a second optional narrative interaction.
    DrawRectangle(405, 438, 58, 40, Color {45, 37, 30, 255});
    DrawRectangle(410, 432, 48, 38, Color {181, 167, 126, 255});
    DrawLine(414, 439, 454, 463, Color {97, 78, 60, 255}); DrawLine(454, 439, 414, 463, Color {97, 78, 60, 255});

    // Mural map and the La Luciernaga mark: recognizable as a pinned wall map, not a UI panel.
    DrawRectangle(443, 112, 224, 69, Color {38, 31, 25, 255});
    DrawRectangle(450, 117, 210, 57, Color {154, 142, 104, 255});
    DrawRectangle(458, 123, 194, 45, Color {183, 169, 126, 255});
    DrawLine(470, 157, 506, 134, Color {45, 90, 102, 255}); DrawLine(506, 134, 548, 151, Color {45, 90, 102, 255});
    DrawLine(548, 151, 594, 130, Color {45, 90, 102, 255}); DrawLine(594, 130, 635, 157, Color {45, 90, 102, 255});
    DrawCircle(506, 134, 4, NlmColors::CYAN); DrawCircle(594, 130, 4, NlmColors::AMBER);
    DrawLine(619, 142, 639, 153, Color {139, 42, 62, 255}); DrawLine(639, 142, 619, 153, Color {139, 42, 62, 255});
    DrawCircleLines(693, 155, 13, NlmColors::AMBER); DrawLine(680, 155, 706, 155, NlmColors::AMBER); DrawLine(693, 142, 693, 168, NlmColors::AMBER);

    // Workbench / transmitter: silhouette, antenna, screen, knobs and cables.
    DrawEllipse(867, 302, 152, 20, Color {3, 8, 11, 170});
    DrawRectangle(730, 250, 285, 50, Color {41, 31, 26, 255});
    DrawRectangle(741, 235, 265, 58, Color {76, 61, 49, 255});
    DrawRectangle(752, 241, 244, 45, Color {31, 42, 44, 255});
    DrawRectangle(770, 251, 122, 26, Color {7, 19, 24, 255});
    DrawRectangleLinesEx({770, 251, 122, 26}, 2.0F, transmitting ? NlmColors::CYAN : Color {66, 105, 112, 255});
    DrawLine(910, 247, 928, 195, Color {113, 116, 101, 255}); DrawLine(928, 195, 940, 211, Color {113, 116, 101, 255});
    DrawRectangle(899, 245, 73, 41, Color {22, 27, 29, 255}); DrawRectangleLinesEx({899, 245, 73, 41}, 2.0F, Color {128, 101, 62, 255});
    DrawCircle(916, 266, 7, Color {121, 127, 114, 255}); DrawCircle(938, 266, 7, Color {121, 127, 114, 255}); DrawCircle(960, 266, 7, Color {121, 127, 114, 255});
    DrawLine(970, 284, 990, 319, Color {22, 137, 147, 255}); DrawLine(980, 284, 1018, 308, Color {192, 112, 37, 255});
    DrawRectangle(755, 296, 12, 45, Color {48, 34, 28, 255}); DrawRectangle(979, 296, 12, 45, Color {48, 34, 28, 255});

    const Color signal = transmitting ? NlmColors::AMBER : Color {83, 99, 87, 255};
    const float indicator = transmitting && !reduceFlashes ? 7.0F + pulse * 4.0F : 7.0F;
    drawSoftLight({938, 266}, transmitting ? 92.0F + pulse * 20.0F : 45.0F, transmitting ? NlmColors::AMBER : NlmColors::CYAN);
    DrawCircle(938, 266, indicator, signal); DrawCircleLines(938, 266, indicator + 5.0F, withAlpha(signal, 160));
    if (transmitting) { DrawLine(788, 264, 874, 264, NlmColors::CYAN); DrawLine(798, 270, 852, 270, withAlpha(NlmColors::CYAN, 120)); }

    // Exit with jamb, control panel and exterior spill; no debug outline.
    DrawRectangle(1034, 207, 96, 182, Color {25, 29, 29, 255});
    DrawRectangle(1042, 218, 80, 164, Color {12, 20, 23, 255});
    DrawRectangle(1052, 230, 60, 148, Color {36, 49, 49, 255});
    DrawRectangle(1061, 240, 42, 130, Color {14, 31, 36, 255});
    DrawRectangle(1020, 252, 14, 48, Color {44, 57, 56, 255}); DrawCircle(1027, 265, 5, NlmColors::AMBER);
    DrawLine(1058, 376, 1107, 376, Color {104, 99, 69, 255});

    drawSoftLight({204, 235}, 125.0F, Color {255, 180, 91, 255});
    drawSoftLight({790, 263}, 92.0F, NlmColors::CYAN);
}
} // namespace

GameApp::GameApp(const GameConfig& config) : m_config(config), m_save(config.savePath())
{
    m_virtual = LoadRenderTexture(W, H);
    SetTextureFilter(m_virtual.texture, TEXTURE_FILTER_POINT);
    std::string ignored;
    const auto loaded = m_save.load(m_session, &ignored);
    if (loaded == cybercba::SaveLoadStatus::Loaded)
        m_message = "Partida disponible";
    else if (loaded != cybercba::SaveLoadStatus::Missing)
        m_message = "No pudimos recuperar la ultima partida. Podes iniciar una nueva desde el menu.";
    m_audio = std::make_unique<AudioService>();
    m_assets = std::make_unique<AssetStore>();
    // Real UI font (ADR 0015-ui-font.md); falls back to raylib's default
    // bitmap font (drawText below) if the file is missing.
    m_uiFont = m_assets->font("assets/fonts/dejavu-sans-mono/DejaVuSansMono.ttf", 64);
    m_audio->setMuted(m_session.audio().muted);
    m_audio->setVolumes(m_session.audio().musicVolume, m_session.audio().effectsVolume);
    if (config.isDevelopmentMode()) m_overlay = std::make_unique<DevOverlay>();
    // Each contributor adds their own file under credits/ via a normal PR
    // (see credits/README.md) instead of everyone editing one shared list —
    // loaded once here, not re-read every frame on the credits screen.
    m_creditsEntries = cybercba::CreditsLoader::loadEntries("credits");
}
GameApp::~GameApp() { if (m_virtual.id) UnloadRenderTexture(m_virtual); }
bool GameApp::shouldExit() const { return m_exit; }
void GameApp::setScreen(Screen screen) { m_screen = screen; m_selected = 0; }
void GameApp::drawText(const std::string& text, int x, int y, int size, Color color) const
{
    if (m_uiFont)
        DrawTextEx(*m_uiFont, text.c_str(), {static_cast<float>(x), static_cast<float>(y)}, static_cast<float>(size), 1.0F, color);
    else
        DrawText(text.c_str(), x, y, size, color);
}
const char* GameApp::devicePrompt(const char* keyboard, const char* gamepad) const
{ return m_input.lastDevice() == InputDevice::Gamepad ? gamepad : keyboard; }
Vector2 GameApp::virtualMouse() const
{
    const float scale = integerScale(GetScreenWidth(), GetScreenHeight());
    return m_input.virtualMouse({(GetScreenWidth() - W * scale) / 2.0F, (GetScreenHeight() - H * scale) / 2.0F}, scale);
}
void GameApp::save()
{
    std::string error;
    if (!m_save.save(m_session, &error)) m_message = "No se pudo guardar. " + error;
    else { m_message = "GUARDANDO..."; m_toastSeconds = 1.4F; }
}
void GameApp::persistSettings()
{
    m_audio->setMuted(m_session.audio().muted);
    m_audio->setVolumes(m_session.audio().musicVolume, m_session.audio().effectsVolume);
    save();
}
void GameApp::start(cybercba::CharacterId character)
{
    m_session.startPrologue(character);
    m_session.reachCheckpoint("shelter");
    m_powerSequenceSeconds = 0.0F;
    save();
    setScreen(Screen::Intro);
}
void GameApp::resetCheckpoint()
{
    if (m_session.resetToCheckpoint())
    {
        // Coarse checkpoints map to the mission node reached at that point;
        // jump the graph there directly (setStart bypasses transition/flag
        // validation, which is exactly what a checkpoint restore needs) and
        // restore the flags that node's forward transitions depend on.
        const std::string& checkpoint = m_session.campaign().checkpoint;
        auto& graph = m_session.missionGraph();
        if (checkpoint == "transmission") { graph.setFlag("power_restored"); graph.setStart("fragmented_transmission"); }
        else if (checkpoint == "street") { graph.setFlag("power_restored"); graph.setStart("protagonist_encounter"); }
        else if (checkpoint == "service_gate") { graph.setFlag("power_restored"); graph.setFlag("encounter_resolved"); graph.setStart("narrative_choice"); }
        else if (checkpoint == "neometro_gate") { graph.setFlag("power_restored"); graph.setFlag("encounter_resolved"); graph.setFlag("choice_made"); graph.setFlag("obstacle_cleared"); graph.setStart("neometro_entrance"); }
        else graph.setStart("refuge");
        m_powerSequenceSeconds = 0.0F;
        m_message = "Checkpoint restaurado.";
        save();
        setScreen(Screen::World);
    }
    else m_message = "No hay un checkpoint valido para restaurar.";
}
cybercba::WorldZone GameApp::nodeZone(const std::string& id) const
{
    if (id == "refuge" || id == "rest_area" || id == "workshop" || id == "comms_room" || id == "archive" ||
        id == "blackout" || id == "power_recovery" || id == "fragmented_transmission")
        return cybercba::WorldZone::Shelter;
    return cybercba::WorldZone::Exterior;
}
std::string GameApp::nodeTitle(const std::string& id) const
{
    if (id == "refuge") return "REFUGIO";
    if (id == "rest_area") return "AREA DE DESCANSO";
    if (id == "workshop") return "TALLER";
    if (id == "comms_room") return "SALA DE COMUNICACIONES";
    if (id == "archive") return "ARCHIVO PERSONAL";
    if (id == "blackout") return "EL APAGON";
    if (id == "power_recovery") return "RECUPERAR ENERGIA";
    if (id == "fragmented_transmission") return "TRANSMISION FRAGMENTADA";
    if (id == "protagonist_encounter") return "OBSTACULO";
    if (id == "narrative_choice") return "LA ELECCION";
    if (id == "exterior_route") return "RUTA EXTERIOR";
    if (id == "security_control") return "CONTROL DE SEGURIDAD";
    if (id == "neometro_entrance") return "ENTRADA DEL NEOMETRO";
    if (id == "prologue_complete") return "ENTRAR AL NEOMETRO";
    return id;
}
std::string GameApp::nodeBody(const std::string& id) const
{
    const auto selected = m_session.campaign().selectedCharacter;
    const auto& dialogue = cybercba::prologueDialogue();
    if (id == "refuge")
        return dialogue.resolveText("opening_network", selected) + "\n\n" + dialogue.resolveText("opening_fireflies", selected);
    if (id == "rest_area") return dialogue.resolveText("evidence_photo", selected);
    if (id == "workshop") return dialogue.resolveText("evidence_insignia", selected);
    if (id == "comms_room") return dialogue.resolveText("evidence_network_map", selected);
    if (id == "archive") return dialogue.resolveText("evidence_unsent_message", selected);
    if (id == "blackout") return dialogue.resolveText("opening_blackout", selected);
    if (id == "power_recovery") return "El generador está inestable. Restaurar la energía va a reactivar el transmisor de La Luciérnaga.";
    if (id == "fragmented_transmission") return dialogue.resolveText("transmission_fragment", selected);
    if (id == "protagonist_encounter")
        return selected == cybercba::CharacterId::Emma
            ? "Una terminal de acceso bloquea el paso. Hay que intervenirla."
            : "Un dron de seguridad bloquea la pasarela.";
    if (id == "narrative_choice") return dialogue.resolveText("transmission_complete", selected);
    if (id == "exterior_route") return "Lluvia. Calles vacias. La linea oeste queda a unas cuadras, mas alla de un control de seguridad.";
    if (id == "security_control")
        return selected == cybercba::CharacterId::Emma
            ? "Camaras cubren el control de acceso. Hay que desactivar la vigilancia para pasar."
            : "Una patrulla corta el paso. Hay que confrontarla para abrir camino.";
    if (id == "neometro_entrance") return dialogue.resolveText("neometro_ending", selected);
    return "";
}
namespace
{
// Short forms for the mission bar (NodeMapView): the full nodeTitle() names
// ("SALA DE COMUNICACIONES") don't fit 14 nodes on one strip even with
// truncation — the PoC reference uses abbreviated stops ("SALA DE COM.",
// "ARCH. PER.") for the same reason.
std::string nodeShortLabel(const std::string& id)
{
    if (id == "refuge") return "REFUGIO";
    if (id == "rest_area") return "DESCANSO";
    if (id == "workshop") return "TALLER";
    if (id == "comms_room") return "COM.";
    if (id == "archive") return "ARCH.";
    if (id == "blackout") return "APAGON";
    if (id == "power_recovery") return "ENERGIA";
    if (id == "fragmented_transmission") return "TRANSM.";
    if (id == "protagonist_encounter") return "OBSTACULO";
    if (id == "narrative_choice") return "ELECCION";
    if (id == "exterior_route") return "EXTERIOR";
    if (id == "security_control") return "CONTROL";
    if (id == "neometro_entrance") return "NEOMETRO";
    if (id == "prologue_complete") return "FINAL";
    return id;
}
} // namespace
std::vector<NodeMapView::Entry> GameApp::nodeMapEntries() const
{
    const auto& graph = m_session.missionGraph();
    const auto reachable = graph.availableTransitions();
    std::vector<NodeMapView::Entry> entries;
    for (const auto& node : graph.nodes())
    {
        NodeMapView::Entry entry;
        entry.id = node.id;
        entry.label = nodeShortLabel(node.id);
        entry.current = node.id == graph.current();
        entry.completed = graph.isCompleted(node.id);
        entry.optional = node.optional;
        entry.locked = !entry.current && !entry.completed &&
                        std::find(reachable.begin(), reachable.end(), node.id) == reachable.end();
        for (const auto& transition : node.transitions)
            entry.connectsTo.push_back(transition.targetNodeId);
        entries.push_back(std::move(entry));
    }
    return entries;
}
std::vector<GameApp::NodeOption> GameApp::nodeOptions() const
{
    const auto& graph = m_session.missionGraph();
    const std::string& id = graph.current();
    const auto selected = m_session.campaign().selectedCharacter;
    std::vector<NodeOption> options;
    if (id == "power_recovery" && !graph.flag("power_restored"))
    {
        options.push_back({"Restaurar energía", "@restore_power"});
        return options;
    }
    if (id == "protagonist_encounter" && !graph.flag("encounter_resolved"))
    {
        options.push_back({selected == cybercba::CharacterId::Emma ? "Intervenir la terminal" : "Enfrentar al dron", "@start_encounter"});
        return options;
    }
    if (id == "narrative_choice")
    {
        if (const auto* line = cybercba::prologueDialogue().find("choice_prompt"))
            for (const auto& choice : line->choices)
                options.push_back({choice.text, "@choice:" + choice.id});
        return options;
    }
    if (id == "security_control" && !graph.flag("obstacle_cleared"))
    {
        options.push_back({selected == cybercba::CharacterId::Emma ? "Desactivar vigilancia" : "Confrontar la patrulla", "@clear_obstacle"});
        return options;
    }
    for (const auto& target : graph.availableTransitions())
        options.push_back({nodeTitle(target), target});
    return options;
}
void GameApp::enterNode(const std::string& id)
{
    if (id == "rest_area") m_session.evidenceJournal().discover("photo");
    else if (id == "workshop") m_session.evidenceJournal().discover("insignia");
    else if (id == "comms_room") m_session.evidenceJournal().discover("network_map");
    else if (id == "archive") m_session.evidenceJournal().discover("unsent_message");
    m_selected = 0;
}
void GameApp::selectNodeOption(const std::string& action)
{
    auto& campaign = m_session.campaign();
    auto& graph = m_session.missionGraph();
    if (action == "@restore_power")
    {
        graph.setFlag("power_restored");
        cybercba::MissionSystem::advance(campaign, cybercba::PrologueStage::Shelter, cybercba::PrologueStage::Transmission, "transmission");
        m_powerSequenceSeconds = 0.0F;
        if (graph.advanceTo("fragmented_transmission")) enterNode("fragmented_transmission");
        save();
        return;
    }
    if (action == "@start_encounter")
    {
        if (campaign.selectedCharacter == cybercba::CharacterId::Emma)
            startEncounter(cybercba::buildEmmaHackingEncounter(), cybercba::CharacterId::Emma);
        else
            startEncounter(cybercba::buildMaggaCombatEncounter(), cybercba::CharacterId::Magga);
        return;
    }
    if (action.rfind("@choice:", 0) == 0)
    {
        const std::string choiceId = action.substr(8);
        if (const auto* line = cybercba::prologueDialogue().find("choice_prompt"))
            for (const auto& choice : line->choices)
                if (choice.id == choiceId) graph.setFlag(choice.setFlag);
        graph.setFlag("choice_made");
        if (graph.advanceTo("exterior_route")) enterNode("exterior_route");
        save();
        return;
    }
    if (action == "@clear_obstacle")
    {
        graph.setFlag("obstacle_cleared");
        cybercba::MissionSystem::advance(campaign, cybercba::PrologueStage::Convergence, cybercba::PrologueStage::Epilogue, "neometro_gate");
        m_session.narrative().trust++;
        if (graph.advanceTo("neometro_entrance")) enterNode("neometro_entrance");
        save();
        return;
    }
    // Plain travel to a node id.
    if (action == "protagonist_encounter" && campaign.stage == cybercba::PrologueStage::Transmission)
        cybercba::MissionSystem::advance(campaign, cybercba::PrologueStage::Transmission, cybercba::PrologueStage::Route, "street");
    if (!graph.advanceTo(action)) return;
    enterNode(action);
    if (action == "prologue_complete")
    {
        cybercba::MissionSystem::complete(campaign);
        save();
        setScreen(Screen::End);
        return;
    }
    save();
}
void GameApp::updateWorldNode()
{
    if (m_session.campaign().stage == cybercba::PrologueStage::Transmission) m_powerSequenceSeconds += GetFrameTime();
    if (m_input.wasPressed(InputAction::Pause) || m_input.wasPressed(InputAction::Cancel)) { setScreen(Screen::Pause); return; }
    if (m_input.wasPressed(InputAction::OpenObjectives)) { setScreen(Screen::Objectives); return; }
    const auto options = nodeOptions();
    if (options.empty()) return;
    if (m_input.wasPressed(InputAction::NavigateDown)) m_selected = (m_selected + 1) % static_cast<int>(options.size());
    if (m_input.wasPressed(InputAction::NavigateUp)) m_selected = (m_selected + static_cast<int>(options.size()) - 1) % static_cast<int>(options.size());
    if (m_input.wasPressed(InputAction::Confirm) || m_input.wasPressed(InputAction::Interact))
        selectNodeOption(options[static_cast<std::size_t>(m_selected)].action);
}
void GameApp::drawBackground() const
{
    DrawRectangle(0, 0, W, H, NlmColors::BG);
    for (int y = -80; y < H; y += 60) DrawLine(0, y, W, y + 180, Color {18, 51, 69, 95});
    for (int x = 0; x < W; x += 96) DrawLine(x, 0, x - 240, H, Color {3, 24, 41, 130});
    if (!m_session.accessibility().reducedMotion)
        for (int i = 0; i < 75; ++i) { const int x = (i * 197) % W, y = (i * 83) % H; DrawLine(x, y, x - 7, y + 15, Color {120, 174, 203, 120}); }
}
void GameApp::drawButton(const char* label, int x, int y, int width, bool selected, bool disabled) const
{
    ActionMenu::drawItem({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 42.0F}, label, selected, disabled, m_uiFont);
}
void GameApp::drawMenu() const
{
    drawText("CYBERCBA 2077", 330, 72, 68, NlmColors::CYAN);
    drawText("LA CIUDAD QUE NOS OLVIDÓ", 405, 148, 25, NlmColors::AMBER);
    drawText("v0.1.0 // LA ÚLTIMA TRANSMISIÓN", 424, 185, 18, DIM);
    drawText("An Exodus Systems Production", 458, 212, 16, DIM);
    for (int i = 0; i < 6; ++i) drawButton(menuItems[i], 400, 245 + i * 49, 480, m_selected == i, i == 0 && !m_session.progress().hasSave);
    const auto& c = m_session.campaign();
    const std::string status = m_session.progress().hasSave ? std::string("CONTINUAR: ") + cybercba::characterName(c.selectedCharacter) + " // " + stageObjective(c.stage) : "CONTINUAR: sin partida valida";
    drawText(status, 325, 550, 16, DIM);
    drawText(devicePrompt("Flechas/WASD + Enter  |  Click para seleccionar", "Cruceta + A para seleccionar"), 370, 655, 17, DIM);
}
void GameApp::drawSelection() const
{
    drawText("¿QUIÉN RESPONDE LA TRANSMISIÓN?", 270, 48, 38, NlmColors::CYAN);
    drawText("Elegí desde qué memoria vas a reconstruir esta historia.", 350, 94, 19, DIM);
    DrawLineEx({160, 132}, {1120, 132}, 2, Color {48,94,110,255});
    DrawLineEx({610, 146}, {670, 146}, 4, NlmColors::CYAN);
    const cybercba::CharacterId ids[] {cybercba::CharacterId::Emma, cybercba::CharacterId::Magga};
    for (int i = 0; i < 2; ++i)
    {
        const auto& c = cybercba::characterDefinition(ids[i]); const int x = 125 + i * 535; const Color accent = i == 0 ? NlmColors::CYAN : NlmColors::AMBER; const bool focus = m_selected == i;
        DrawRectangle(x, 160, 495, 410, focus ? Color {8,37,50,248} : CARD); DrawRectangleLinesEx({static_cast<float>(x), 160, 495, 410}, focus ? 4.0F : 1.0F, focus ? accent : Color {48,94,110,255});
        DrawRectangle(x + 18, 178, 459, 6, accent); if (focus) { DrawRectangle(x + 22, 195, 142, 26, accent); drawText("SELECCIONADO", x + 32, 201, 14, NlmColors::BG); }
        // The runtime pack supplies full-height cinematic selection figures;
        // gameplay's legacy pixel sprites stay registered but are not used here.
        const char* spritePath = figurePath(ids[i]);
        const Rectangle portraitBounds {static_cast<float>(x + 24), 224.0F, 196.0F, 278.0F};
        DrawRectangleRec(portraitBounds, Color {3, 19, 31, 180});
        DrawRectangleLinesEx(portraitBounds, 1.0F, Color {48, 94, 110, 180});
        DrawLineEx({portraitBounds.x + 12.0F, portraitBounds.y + portraitBounds.height - 15.0F},
                   {portraitBounds.x + portraitBounds.width - 12.0F, portraitBounds.y + portraitBounds.height - 15.0F},
                   1.0F, Color {48, 94, 110, 180});
        if (const Texture2D* sprite = m_assets->texture(spritePath))
        {
            const Rectangle destination = aspectFit(*sprite, portraitBounds);
            DrawTexturePro(*sprite, {0, 0, static_cast<float>(sprite->width), static_cast<float>(sprite->height)}, destination, {0, 0}, 0, WHITE);
        }
        else
        { DrawRectangle(x + 76, 275, 90, 155, Color {12, 38, 50, 255}); DrawCircle(x + 121, 305, 28, accent); DrawRectangle(x + 96, 335, 50, 70, accent); }
        drawText(c.displayName, x + 238, 235, 17, NlmColors::TEXT); drawText(std::string("Origen: ") + c.origin, x + 238, 265, 17, DIM);
        drawText("ESPECIALIDAD", x + 238, 310, 14, accent); drawText(c.abilityName, x + 238, 333, 17, NlmColors::TEXT);
        drawText(i == 0 ? "Interviene terminales, sensores" : "Derriba obstaculos y abre", x + 238, 365, 15, DIM);
        drawText(i == 0 ? "y sistemas electricos." : "rutas fisicas.", x + 238, 385, 15, DIM);
        drawText(i == 0 ? "CONTROL · OBSERVACIÓN" : "FUERZA · MOVILIDAD", x + 238, 425, 14, accent);
        drawText(i == 0 ? "Todo sistema deja una puerta abierta." : "Si no existe una entrada, hacemos una.", x + 38, 530, 15, DIM);
    }
    drawText("LA LUCIÉRNAGA  //  Antes del apagón luchaban juntos. Después, cada uno creyó que el otro lo abandonó.", 205, 602, 16, DIM);
    drawText("Elegís quién responde. La otra persona seguirá esperando al otro lado de la señal.", 300, 627, 16, NlmColors::TEXT);
    drawText(devicePrompt("[← →] Elegir    [Enter] Confirmar    [Esc] Volver", "[D-Pad] Elegir    [A] Confirmar    [B] Volver"), 335, 675, 17, NlmColors::AMBER);
}
void GameApp::drawIntro() const
{
    // Story/prologue template (6.1): chapter label + portrait + story text,
    // reused across intro narration, memory scenes and evidence beats.
    const auto other = cybercba::otherCharacter(m_session.campaign().selectedCharacter);
    const Color otherColor = other == cybercba::CharacterId::Emma ? NlmColors::CYAN : NlmColors::AMBER;
    const Texture2D* portrait = m_assets ? m_assets->texture(transmissionPortraitPath(other)) : nullptr;
    drawText("LA ÚLTIMA TRANSMISIÓN", 365, 125, 44, NlmColors::CYAN);
    drawText("Lluvia. Interferencia. Un tren lejano sobre una línea muerta.", 255, 220, 23, DIM);
    StoryPanel::draw({170, 275, 940, 220}, cybercba::characterName(other), portrait,
                      "No sé cuánto tiempo va a conservarse esta señal.\nEl tren 41 volvió a aparecer en la red. Encontrame.",
                      m_uiFont, otherColor);
    drawText(devicePrompt("ENTER: responder", "A: responder"), 490, 550, 21, NlmColors::CYAN);
}
void GameApp::drawShelterShell() const
{
    // Four architectural zones, left to right: Entrada/Descanso, Taller,
    // Comunicaciones, Pasillo tecnico. Each gets its own floor material so the
    // room reads as a real floor plan instead of one uniform checkerboard.
    // Entrada/Descanso: hormigon tibio con juntas.
    DrawRectangle(88, 108, 332, 470, Color {27, 33, 35, 255});
    for (int y = 132; y < 570; y += 48)
        for (int x = 106; x < 420; x += 72)
        {
            const bool alternate = ((x / 72) + (y / 48)) % 2 == 0;
            DrawRectangle(x, y, 68, 44, alternate ? Color {34, 41, 42, 255} : Color {24, 30, 32, 255});
            DrawLine(x, y + 44, x + 68, y + 44, Color {8, 15, 20, 170});
        }
    DrawRectangle(495, 485, 84, 18, Color {28, 45, 50, 175});
    DrawRectangleLinesEx({495, 485, 84, 18}, 1.0F, Color {52, 70, 70, 120});
    DrawEllipse(604, 492, 61, 8, Color {28, 57, 66, 135});
    // Taller: chapa remachada con tinte cian.
    DrawRectangle(420, 108, 280, 470, Color {19, 29, 36, 255});
    for (int x = 434; x < 700; x += 92)
        for (int y = 140; y < 560; y += 96)
        {
            DrawRectangle(x, y, 84, 90, Color {25, 37, 45, 255});
            DrawRectangleLinesEx({static_cast<float>(x), static_cast<float>(y), 84.0F, 90.0F}, 1.0F, Color {11, 19, 25, 200});
            for (int cx : {x + 8, x + 76}) for (int cy : {y + 8, y + 82}) DrawCircle(cx, cy, 2, Color {70, 90, 95, 200});
        }
    // Comunicaciones: piso oscuro con canaletas de cable hacia el transmisor.
    DrawRectangle(700, 108, 280, 470, Color {16, 23, 27, 255});
    for (int y = 160; y < 560; y += 64) DrawLine(700, y, 980, y, Color {9, 15, 18, 160});
    for (int x = 730; x < 980; x += 90) { DrawLine(x, 300, x, 470, Color {30, 105, 112, 110}); DrawLine(x + 3, 300, x + 3, 470, Color {20, 55, 60, 150}); }
    // Pasillo tecnico: rejilla metalica con franjas de advertencia hacia la salida.
    DrawRectangle(980, 108, 212, 470, Color {13, 17, 19, 255});
    for (int x = 986; x < 1180; x += 22) DrawLine(x, 152, x, 540, Color {6, 9, 11, 200});
    for (int y = 152; y < 540; y += 22) DrawLine(986, y, 1180, y, Color {6, 9, 11, 180});
    DrawRectangle(986, 152, 194, 10, Color {150, 110, 20, 120});
    DrawRectangle(986, 528, 194, 10, Color {150, 110, 20, 120});

    // Interior partition walls with doorway gaps (shared with collision via
    // shelterInteriorWalls()) so the visible wall and the collider stay in sync.
    for (const cybercba::WorldRect& wall : cybercba::shelterInteriorWalls())
    {
        DrawRectangle(static_cast<int>(wall.x) - 6, static_cast<int>(wall.y), static_cast<int>(wall.width) + 12, static_cast<int>(wall.height), Color {6, 10, 13, 255});
        DrawRectangle(static_cast<int>(wall.x), static_cast<int>(wall.y), static_cast<int>(wall.width), static_cast<int>(wall.height), Color {58, 78, 86, 255});
        DrawRectangle(static_cast<int>(wall.x), static_cast<int>(wall.y), static_cast<int>(wall.width), 8, Color {124, 168, 178, 255});
        DrawRectangle(static_cast<int>(wall.x), static_cast<int>(wall.y), 2, static_cast<int>(wall.height), Color {96, 132, 140, 220});
        DrawRectangle(static_cast<int>(wall.x) + static_cast<int>(wall.width) - 2, static_cast<int>(wall.y), 2, static_cast<int>(wall.height), Color {32, 46, 51, 255});
    }
    for (std::size_t i = 0; i + 1 < cybercba::shelterInteriorWalls().size(); i += 2)
    {
        const cybercba::WorldRect& upper = cybercba::shelterInteriorWalls()[i];
        const cybercba::WorldRect& lower = cybercba::shelterInteriorWalls()[i + 1];
        const int gapTop = static_cast<int>(upper.y + upper.height), gapBottom = static_cast<int>(lower.y);
        DrawRectangle(static_cast<int>(upper.x) - 4, gapTop, static_cast<int>(upper.width) + 8, 5, Color {60, 86, 90, 220});
        DrawRectangle(static_cast<int>(upper.x) - 4, gapBottom - 5, static_cast<int>(upper.width) + 8, 5, Color {60, 86, 90, 220});
    }

    // Small connective props anchor each zone so it reads as lived-in, not a slot grid.
    DrawRectangle(128, 195, 4, 70, Color {70, 60, 50, 255}); DrawCircle(130, 195, 5, Color {90, 78, 64, 255}); // perchero
    DrawRectangle(112, 300, 20, 14, Color {40, 32, 26, 255}); DrawRectangle(138, 300, 20, 14, Color {40, 32, 26, 255}); // botas
    DrawRectangle(470, 470, 46, 30, Color {60, 52, 30, 255}); DrawRectangleLinesEx({470, 470, 46, 30}, 1.0F, Color {150, 120, 40, 200}); // caja de herramientas
    DrawLine(516, 485, 560, 460, Color {40, 90, 95, 200}); DrawLine(516, 490, 566, 470, Color {110, 80, 40, 200}); // cables sueltos
    DrawRectangle(760, 430, 38, 50, Color {30, 26, 22, 255}); DrawRectangleLinesEx({760, 430, 38, 50}, 1.0F, Color {130, 100, 60, 220}); // caja de fusibles
    DrawCircle(772, 445, 4, Color {80, 190, 90, 255}); DrawCircle(786, 445, 4, Color {190, 70, 60, 255});
    DrawLine(1000, 152, 1000, 540, Color {60, 68, 70, 220}); DrawCircle(1000, 300, 9, Color {90, 100, 102, 255}); // tuberia y valvula del pasillo

    DrawRectangle(70, 88, 1140, 38, Color {52, 70, 76, 255});
    DrawRectangle(70, 88, 1140, 8, Color {110, 148, 156, 255});
    DrawRectangle(70, 558, 1140, 40, Color {46, 62, 67, 255});
    DrawRectangle(70, 88, 28, 510, Color {40, 55, 60, 255});
    DrawRectangle(1182, 88, 28, 510, Color {40, 55, 60, 255});
    DrawRectangle(98, 88, 4, 510, Color {14, 22, 26, 220});
    DrawRectangle(1178, 88, 4, 510, Color {14, 22, 26, 220});
    DrawRectangle(100, 126, 1080, 9, Color {4, 10, 14, 170});
    drawSoftLight({235, 265}, 130.0F, Color {255, 181, 92, 255});
    drawSoftLight({560, 300}, 95.0F, NlmColors::CYAN);
    drawSoftLight({875, 300}, 120.0F, NlmColors::CYAN);
    drawSoftLight({1080, 350}, 70.0F, Color {255, 181, 92, 255});
}
void GameApp::drawExteriorShell() const
{
    DrawRectangle(0, 108, W, 612, Color {6, 19, 30, 255});
    DrawRectangle(0, 480, W, 240, Color {10, 22, 29, 255});
    for (int x = -40; x < W; x += 142)
    {
        const int roof = 165 + ((x + 80) % 4) * 28;
        DrawRectangle(x, roof, 112, 310, Color {13, 33, 45, 255});
        DrawRectangle(x + 18, roof + 54, 17, 62, Color {16, 78, 91, 255});
        DrawRectangle(x + 65, roof + 101, 12, 42, Color {105, 73, 41, 255});
    }
    for (int y = 492; y < 710; y += 42)
        DrawLine(0, y, W, y, Color {38, 53, 58, 160});
    for (int x = 0; x < W; x += 84)
        DrawLine(x, 492, x - 78, 720, Color {28, 43, 50, 140});
    for (int x = 110; x < W; x += 245)
    {
        DrawEllipse(x, 586 + (x % 3) * 19, 74, 10, Color {19, 58, 72, 150});
        DrawLine(x + 12, 595 + (x % 3) * 19, x + 110, 595 + (x % 3) * 19, Color {57, 117, 132, 80});
    }
    DrawLineEx({390, 520}, {720, 552}, 3.0F, Color {11, 15, 18, 220});
    DrawLineEx({402, 526}, {708, 558}, 1.0F, Color {44, 68, 72, 150});
    drawSoftLight({430, 425}, 120.0F, Color {90, 194, 211, 255});
    drawSoftLight({1070, 465}, 165.0F, NlmColors::AMBER);
    if (!m_session.accessibility().reducedMotion)
        for (int i = 0; i < 70; ++i)
        {
            const int x = (i * 167 + 31) % W;
            const int y = 125 + (i * 79) % 470;
            DrawLine(x, y, x - 5, y + 15, Color {115, 172, 194, 105});
        }
}
void GameApp::drawWorldProps(cybercba::WorldZone zone, cybercba::WorldLayer layer) const
{
    const auto drawProps = [&](const auto& props)
    {
        for (const auto& prop : props)
        {
            if (prop.layer != layer) continue;
            const Texture2D* texture = m_assets->texture(prop.assetPath);
            if (!texture) continue;
            if (prop.layer != cybercba::WorldLayer::High)
                DrawEllipse(static_cast<int>(prop.pivotX - prop.width * 0.22F), static_cast<int>(prop.pivotY - 8.0F), static_cast<int>(prop.width * 0.22F), 10, Color {0, 0, 0, 84});
            const Rectangle destination {prop.pivotX - prop.width / 2.0F, prop.pivotY - prop.height, prop.width, prop.height};
            DrawTexturePro(*texture, {0, 0, static_cast<float>(texture->width), static_cast<float>(texture->height)}, destination, {0, 0}, 0.0F, WHITE);
        }
    };
    if (zone == cybercba::WorldZone::Shelter) drawProps(cybercba::shelterProps());
    else drawProps(cybercba::exteriorProps());
}
void GameApp::drawWorldLighting(cybercba::WorldZone zone) const
{
    const bool reducedFlashes = m_session.accessibility().reduceFlashes;
    if (zone == cybercba::WorldZone::Shelter)
    {
        const float sequence = m_powerSequenceSeconds;
        const bool emergency = sequence >= 1.0F;
        const bool transmitterFocus = sequence >= 2.0F;
        const unsigned char darkness = emergency ? 154 : sequence > 0.4F ? 82 : 34;
        DrawRectangle(88, 108, 1104, 470, Color {2, 9, 15, darkness});
        BeginBlendMode(BLEND_ADDITIVE);
        drawSoftLight({235, 285}, emergency ? 70.0F : 155.0F, Color {255, 169, 82, 255});
        drawSoftLight({600, 300}, emergency ? 42.0F : 112.0F, NlmColors::CYAN);
        drawSoftLight({884, 300}, transmitterFocus ? 165.0F : 72.0F, transmitterFocus ? NlmColors::AMBER : NlmColors::CYAN);
        drawSoftLight({1080, 390}, emergency ? 74.0F : 38.0F, NlmColors::AMBER);
        EndBlendMode();
        if (sequence > 0.4F && sequence < 1.2F && !reducedFlashes)
        {
            const float flicker = std::sin(sequence * 27.0F);
            if (flicker > 0.55F) DrawRectangle(88, 108, 1104, 470, Color {1, 7, 11, 95});
        }
        return;
    }
    DrawRectangle(0, 108, W, 612, Color {1, 8, 14, 66});
    BeginBlendMode(BLEND_ADDITIVE);
    drawSoftLight({430, 438}, 145.0F, NlmColors::CYAN);
    drawSoftLight({785, 440}, 95.0F, NlmColors::CYAN);
    drawSoftLight({1070, 465}, 180.0F, NlmColors::AMBER);
    EndBlendMode();
}
void GameApp::drawWeather(cybercba::WorldZone zone) const
{
    if (zone != cybercba::WorldZone::Exterior || !m_config.rainEnabled()) return;
    const int density = m_session.accessibility().reducedMotion ? std::max(12, m_config.rainDensity() / 3) : m_config.rainDensity();
    const unsigned char alpha = static_cast<unsigned char>(50.0F + 100.0F * m_config.rainIntensity());
    const float time = static_cast<float>(GetTime());
    for (int i = 0; i < density; ++i)
    {
        const float x = static_cast<float>((i * 71 + static_cast<int>(time * 140.0F)) % W);
        const float y = 115.0F + static_cast<float>((i * 113 + static_cast<int>(time * 260.0F)) % 590);
        DrawLineEx({x, y}, {x - 5.0F, y + 14.0F}, 1.0F, Color {100, 170, 195, alpha});
    }
}
void GameApp::drawWorldSignage(cybercba::WorldZone zone) const
{
    if (zone != cybercba::WorldZone::Exterior) return;
    // Localized diegetic overlay replaces source art wording that belongs to no in-game line.
    DrawRectangle(966, 255, 195, 55, Color {17, 24, 25, 230});
    DrawRectangleLinesEx({966, 255, 195, 55}, 1.0F, Color {170, 112, 42, 210});
    drawText("NEOMETRO", 995, 260, 18, NlmColors::TEXT);
    drawText("LINEA OESTE", 993, 280, 12, NlmColors::CYAN);
    drawText("FUERA DE SERVICIO", 978, 295, 11, NlmColors::AMBER);
}
const cybercba::WorldProp* GameApp::interactionHighlight(const std::string& nodeId) const
{
    // Only nodes with a real, positioned WorldProp get a highlight — no
    // fabricated bracket for evidence that has no visual counterpart yet
    // (workshop/archive; see docs/mission-screen-templates.md asset gaps).
    if (nodeId == "rest_area") return cybercba::worldProp("safehouse_photo");
    if (nodeId == "comms_room") return cybercba::worldProp("safehouse_map");
    return nullptr;
}
void GameApp::drawWorld() const
{
    // Node-based exploration screen (see docs/mission-system.md). The
    // shelter/exterior "shell" is the cinematic scene (ADR 0017); no player
    // sprite, no movement, no collision.
    const std::string& nodeId = m_session.missionGraph().current();
    const cybercba::WorldZone zone = nodeZone(nodeId);
    const char* backgroundPath = zone == cybercba::WorldZone::Shelter ? REFUGE_BACKGROUND : NEOMETRO_BACKGROUND;
    const Texture2D* background = m_assets ? m_assets->texture(backgroundPath) : nullptr;
    if (background != nullptr)
    {
        DrawTexturePro(*background, {0, 0, static_cast<float>(background->width), static_cast<float>(background->height)},
                       {0, 0, static_cast<float>(W), static_cast<float>(H)}, {0, 0}, 0.0F, WHITE);
    }
    else
    {
        // The existing procedural scene remains an explicit safe fallback for
        // a missing/corrupt runtime-art installation.
        if (zone == cybercba::WorldZone::Shelter) drawShelterShell(); else drawExteriorShell();
        drawWorldProps(zone, cybercba::WorldLayer::Low);
        drawWorldProps(zone, cybercba::WorldLayer::Mid);
        drawWorldProps(zone, cybercba::WorldLayer::High);
        drawWorldSignage(zone);
        drawWorldLighting(zone);
        drawWeather(zone);
    }
    // Subtle object highlight (corner brackets), not a full-scene overlay —
    // the scene itself stays visible per ADR 0017, unlike the old full-canvas
    // dim rectangle this replaced.
    // The runtime refuge background declares the framed photo's normalized
    // hotspot in its manifest. Do not reuse WorldLayout screen coordinates on
    // that unrelated illustration; procedural fallback keeps its legacy prop
    // bounds instead.
    const Rectangle staticPhotoHighlight {0.68F * W, 0.48F * H, 0.12F * W, 0.18F * H};
    const cybercba::WorldProp* fallbackHighlight = background == nullptr ? interactionHighlight(nodeId) : nullptr;
    if ((background != nullptr && nodeId == "rest_area") || fallbackHighlight != nullptr)
    {
        const Rectangle rect = fallbackHighlight != nullptr
            ? Rectangle {fallbackHighlight->pivotX - fallbackHighlight->width / 2.0F, fallbackHighlight->pivotY - fallbackHighlight->height, fallbackHighlight->width, fallbackHighlight->height}
            : staticPhotoHighlight;
        const float bracket = 14.0F;
        const float pulse = 0.6F + 0.4F * std::sin(static_cast<float>(GetTime()) * 3.0F);
        const Color color {static_cast<unsigned char>(NlmColors::CYAN.r), static_cast<unsigned char>(NlmColors::CYAN.g), static_cast<unsigned char>(NlmColors::CYAN.b), static_cast<unsigned char>(160.0F + 80.0F * pulse)};
        for (const auto& corner : {std::pair {rect.x, rect.y}, {rect.x + rect.width, rect.y}, {rect.x, rect.y + rect.height}, {rect.x + rect.width, rect.y + rect.height}})
        {
            const float dx = corner.first < rect.x + rect.width / 2.0F ? bracket : -bracket;
            const float dy = corner.second < rect.y + rect.height / 2.0F ? bracket : -bracket;
            DrawLineEx({corner.first, corner.second}, {corner.first + dx, corner.second}, 2.0F, color);
            DrawLineEx({corner.first, corner.second}, {corner.first, corner.second + dy}, 2.0F, color);
        }
    }

    // Shared header row (ADR 0017): mission progress bar + current objective.
    const StageObjective objective = stageObjectiveInfo(m_session.campaign().stage);
    NodeMapView::draw({UiMetrics::contentX, UiMetrics::headerY + 18.0F, 780.0F, UiMetrics::headerHeight - 18.0F}, nodeMapEntries(), m_uiFont);
    CurrentObjectivePanel::draw({UiMetrics::contentX + 800.0F, UiMetrics::headerY, UiMetrics::contentWidth - 800.0F, UiMetrics::headerHeight}, objective.title, objective.description, m_uiFont);

    const auto options = nodeOptions();
    std::vector<std::string> labels;
    for (const auto& option : options) labels.push_back(option.label);
    const auto bottom = UiMetrics::bottomPanelBounds();
    const auto protagonist = m_session.campaign().selectedCharacter;
    const Color protagonistColor = protagonist == cybercba::CharacterId::Emma ? NlmColors::CYAN : NlmColors::AMBER;
    const Texture2D* protagonistPortrait = m_assets ? m_assets->texture(portraitPath(protagonist, nodeId)) : nullptr;

    if (nodeId == "narrative_choice")
    {
        // Dialogue-choice template (6.3): speaker + body + response row,
        // reused for every narrative branch point, not just this one.
        const auto speaker = cybercba::otherCharacter(protagonist);
        ChoicePanel::draw(UiMetrics::contentBounds(), cybercba::characterName(speaker), nodeBody(nodeId), labels, m_selected, m_uiFont);
    }
    else if (nodeId == "fragmented_transmission")
    {
        // Fragmented-transmission template: signal-quality bar + procedural
        // noise, per the narrative beat of a weak/interrupted signal (0.35 is
        // illustrative — there's no domain-tracked signal-strength value yet).
        TransmissionPanel::draw(UiMetrics::contentBounds(), nodeTitle(nodeId), nodeBody(nodeId), 0.35F, static_cast<float>(GetTime()), m_uiFont);
    }
    else
    {
        // Interaction template (8): protagonist portrait + reaction/narration
        // + action menu, the same composition for investigation nodes and
        // plain narrative beats alike (single-action nodes just show one
        // button, matching the "continue" feel of the dialogue PoC).
        InteractionPanel::draw(bottom, cybercba::characterName(protagonist), protagonistPortrait, protagonistColor, nodeBody(nodeId), labels, m_selected, m_uiFont);
    }

    drawText(devicePrompt("Flechas: elegir  ENTER: viajar/actuar  ESC: pausa  O: objetivos", "Cruceta: elegir  A: viajar/actuar  Menu: pausa  View: objetivos"), static_cast<int>(UiMetrics::contentX), 700, 14, DIM);
    if (!m_message.empty() && (m_toastSeconds > 0.0F || m_session.accessibility().persistentPrompts)) drawText(m_message, static_cast<int>(UiMetrics::contentX), 8, 16, NlmColors::AMBER);
}
void GameApp::drawChapters() const
{
    drawText("CAPITULOS", 515, 75, 45, NlmColors::CYAN); DrawRectangle(180, 160, 920, 155, CARD); DrawRectangleLinesEx({180, 160, 920, 155}, 2, NlmColors::CYAN);
    drawText("PRÓLOGO // LA ÚLTIMA TRANSMISIÓN", 220, 195, 28, NlmColors::TEXT); drawText("v0.1.0", 220, 235, 18, DIM); drawText(m_session.campaign().prologueCompleted ? "COMPLETADO" : m_session.campaign().prologueStarted ? "EN PROGRESO" : "DISPONIBLE", 850, 235, 18, m_session.campaign().prologueCompleted ? NlmColors::GREEN : NlmColors::AMBER);
    DrawRectangle(180, 350, 920, 125, Color {5, 16, 26, 210}); DrawRectangleLinesEx({180, 350, 920, 125}, 1, DIM); drawText("CAPÍTULO I // NEOMETRO: EL ÚLTIMO CONVOY", 220, 385, 27, DIM); drawText("v0.2.0  //  PRÓXIMAMENTE: contenido no disponible", 220, 425, 18, DIM);
    drawText(devicePrompt("ENTER o ESC: volver", "A o B: volver"), 480, 590, 19, DIM);
}
std::string GameApp::evidenceImagePath(const std::string& evidenceId) const
{
    // Mirrors assets/data/assets-manifest.json's evidenceImage.* entries; kept
    // here (not in cybercba::Evidence) because file paths are a presentation
    // concern, not domain state.
    if (evidenceId == "photo") return "assets/generated/evidence/evidence_emma_magga_photo.png";
    if (evidenceId == "network_map") return "assets/generated/evidence/evidence_network_access_map.png";
    if (evidenceId == "insignia") return "assets/generated/evidence/evidence_fireflies_badge.png";
    if (evidenceId == "unsent_message")
        return m_session.campaign().selectedCharacter == cybercba::CharacterId::Emma
            ? "assets/generated/evidence/evidence_unsent_message_emma.png"
            : "assets/generated/evidence/evidence_unsent_message_magga.png";
    if (evidenceId == "train41_record") return "assets/generated/evidence/evidence_train_41_record.png";
    return "";
}
void GameApp::drawObjectives() const
{
    const auto stage = m_session.campaign().stage;
    const auto rank = static_cast<int>(stage);
    const StageObjective objective = stageObjectiveInfo(stage);
    std::string body = "OBJETIVO ACTUAL\n" + std::string(objective.title) + "\n" + objective.description + "\n\nBITACORA\n";
    body += rank > static_cast<int>(cybercba::PrologueStage::Shelter) ? "[OK] " : "[ ] "; body += "La señal imposible: descubrir quién reactivó el transmisor.\n";
    body += rank > static_cast<int>(cybercba::PrologueStage::Transmission) ? "[OK] " : "[ ] "; body += "Reconstruir el enlace antes de perder la señal.\n";
    body += rank > static_cast<int>(cybercba::PrologueStage::Route) ? "[OK] " : "[ ] "; body += "Recuperar las coordenadas del tren 41.\n";
    body += rank > static_cast<int>(cybercba::PrologueStage::Convergence) ? "[OK] " : "[ ] "; body += "Seguir la línea oeste hasta el Neometro.";
    drawOverlay("MISIÓN ACTUAL", body, devicePrompt("O o ESC: volver", "View o B: volver"));
    // Evidence journal strip (EvidencePanel, src/ui/widgets/) — reusable
    // component per the "evidence journal" screen template, distinct from the
    // plain-text bitacora above.
    const auto discovered = m_session.evidenceJournal().discovered();
    const float chipY = 500.0F, chipW = 300.0F, chipH = 84.0F, gap = 12.0F;
    drawText("EVIDENCIAS", 220, static_cast<int>(chipY) - 28, 20, NlmColors::CYAN);
    if (discovered.empty())
    {
        drawText("Ninguna evidencia encontrada todavia.", 220, static_cast<int>(chipY), 16, DIM);
    }
    else
    {
        float x = 220.0F;
        for (std::size_t i = 0; i < discovered.size() && x + chipW <= 1105.0F; ++i)
        {
            const cybercba::EvidenceItem* item = discovered[i];
            const std::string path = evidenceImagePath(item->id);
            const Texture2D* tex = (!path.empty() && m_assets) ? m_assets->texture(path) : nullptr;
            EvidencePanel::draw({x, chipY, chipW, chipH}, tex, item->title, m_uiFont);
            x += chipW + gap;
        }
    }
}
void GameApp::drawSettings() const
{
    static const char* tabs[] {"VIDEO", "AUDIO", "CONTROLES", "ACCESIBILIDAD", "GAMEPLAY"};
    drawText("CONFIGURACIÓN", 440, 62, 42, NlmColors::CYAN);
    for (int i = 0; i < 5; ++i) drawButton(tabs[i], 95 + i * 220, 125, 190, i == m_settingsTab);
    const auto& a = m_session.accessibility(); const auto& audio = m_session.audio();
    const char* rows[5] {}; std::string values[5]; int count = 0;
    if (m_settingsTab == 0) { rows[0] = "ESCALA DE UI"; values[0] = std::to_string(static_cast<int>(a.uiScale * 100)) + "%"; rows[1] = "SCANLINES"; values[1] = a.scanlines ? "ACTIVADAS" : "DESACTIVADAS"; rows[2] = "FLASHES"; values[2] = a.reduceFlashes ? "REDUCIDOS" : "NORMALES"; count = 3; }
    else if (m_settingsTab == 1) { rows[0] = "SILENCIO"; values[0] = audio.muted ? "SI" : "NO"; rows[1] = "MUSICA"; values[1] = std::to_string(static_cast<int>(audio.musicVolume * 100)) + "%"; rows[2] = "EFECTOS"; values[2] = std::to_string(static_cast<int>(audio.effectsVolume * 100)) + "%"; rows[3] = "AMBIENTE"; values[3] = std::to_string(static_cast<int>(audio.ambienceVolume * 100)) + "%"; rows[4] = "DIALOGOS"; values[4] = std::to_string(static_cast<int>(audio.dialogueVolume * 100)) + "%"; count = 5; }
    else if (m_settingsTab == 2) { rows[0] = "TECLADO"; values[0] = "WASD / E / X / ESC"; rows[1] = "GAMEPAD"; values[1] = "STICK / X / Y / MENÚ"; rows[2] = "REMAPEO"; values[2] = "PRÓXIMAMENTE"; count = 3; }
    else if (m_settingsTab == 3) { rows[0] = "MOVIMIENTO REDUCIDO"; values[0] = a.reducedMotion ? "SI" : "NO"; rows[1] = "SUBTÍTULOS"; values[1] = a.subtitles ? "SI" : "NO"; rows[2] = "ALTO CONTRASTE"; values[2] = a.highContrast ? "SI" : "NO"; rows[3] = "PROMPTS PERSISTENTES"; values[3] = a.persistentPrompts ? "SI" : "NO"; rows[4] = "VELOCIDAD DIÁLOGO"; values[4] = std::to_string(static_cast<int>(a.dialogueSpeed * 100)) + "%"; count = 5; }
    else { rows[0] = "CONFIRMAR SOBRESCRITURA"; values[0] = "SI"; rows[1] = "MOSTRAR OBJETIVOS"; values[1] = a.persistentPrompts ? "SI" : "AL MANTENER O"; rows[2] = "DIFICULTAD"; values[2] = "PREPARADA PARA FUTURO"; count = 3; }
    for (int i = 0; i < count; ++i) { const int y = 215 + i * 58; DrawRectangle(240, y, 800, 46, CARD); DrawRectangleLinesEx({240.0F, static_cast<float>(y), 800, 46}, m_settingsRow == i ? 3.0F : 1.0F, m_settingsRow == i ? NlmColors::CYAN : Color {48,94,110,255}); drawText(rows[i], 270, y + 13, 19, NlmColors::TEXT); drawText(values[i], 730, y + 13, 18, DIM); }
    drawText(devicePrompt("Flechas: cambiar  ENTER: alternar  ESC: volver", "Cruceta: cambiar  A: alternar  B: volver"), 295, 620, 17, DIM);
}
void GameApp::drawCredits() const
{
    std::string body = "CYBERCBA 2077\nDirección y desarrollo: equipo CYBERCBA\nDiseño narrativo y visual: equipo CYBERCBA\n\nTecnología: raylib, CMake y C++17\nAssets propios: generados con IA y placeholders procedurales originales\nFuente de UI: DejaVu Sans Mono\nLicencias y procedencia completas: docs/third-party-assets.md y assets/data/assets-manifest.json\n\nNo se incorporaron recursos de Thiings.";
    body += "\n\nEQUIPO // ANUARIO\n";
    body += m_creditsEntries.empty() ? "Todavia no hay entradas. Ver credits/README.md para sumar la tuya." : m_creditsEntries;
    drawOverlay("CRÉDITOS", body, devicePrompt("ENTER o ESC: volver", "A o B: volver"));
}
void GameApp::drawPause() const
{
    drawWorld(); DrawRectangle(0, 0, W, H, Color {0, 0, 0, 155}); drawText("PAUSA", 530, 95, 45, NlmColors::CYAN);
    for (int i = 0; i < 6; ++i) drawButton(pauseItems[i], 370, 170 + i * 54, 540, m_selected == i);
}
void GameApp::drawEnd() const
{
    // Mission results template (7.4): title + decisions/outcome + recovered
    // evidence, composed via MissionResultsView instead of ad hoc layout.
    const auto& c = m_session.campaign();
    const std::string body = std::string("LA ÚLTIMA TRANSMISIÓN\n\nProtagonista: ") + cybercba::characterName(c.selectedCharacter) +
                              "\nRuta utilizada: " + cybercba::characterAbility(c.selectedCharacter) +
                              "\nCheckpoint final: " + c.checkpoint +
                              "\n\nPRÓXIMA RELEASE\nv0.2.0  NEOMETRO: EL ÚLTIMO CONVOY";
    std::vector<MissionResultsView::EvidenceEntry> evidence;
    for (const auto* item : m_session.evidenceJournal().discovered())
        evidence.push_back({m_assets ? m_assets->texture(evidenceImagePath(item->id)) : nullptr, item->title});
    MissionResultsView::draw(UiMetrics::contentBounds(), "PRÓLOGO COMPLETADO", body, "ENTER: ver capítulos   ESC: volver al menú", evidence, m_uiFont);
}
void GameApp::startEncounter(cybercba::Encounter encounter, cybercba::CharacterId protagonist)
{
    m_encounter = std::make_unique<cybercba::Encounter>(std::move(encounter));
    m_encounter->start();
    m_encounterProtagonist = protagonist;
    m_encounterAction = 0;
    setScreen(Screen::Encounter);
}
void GameApp::resolveEncounterOutcome()
{
    if (!m_encounter) return;
    const auto outcome = m_encounter->outcome();
    if (outcome == cybercba::EncounterOutcome::Victory)
    {
        cybercba::MissionSystem::advance(m_session.campaign(), cybercba::PrologueStage::Route, cybercba::PrologueStage::Convergence, "service_gate");
        m_session.narrative().recoveredTruth++;
        m_session.evidenceJournal().discover("train41_record");
        auto& graph = m_session.missionGraph();
        graph.setFlag("encounter_resolved");
        if (graph.advanceTo("narrative_choice")) enterNode("narrative_choice");
        m_message = m_encounterProtagonist == cybercba::CharacterId::Emma
            ? "Enlace intervenido. Registro oculto recuperado: TREN 41."
            : "Dron derribado. Tras el humo: una identificacion del tren 41.";
        m_toastSeconds = 3.0F;
        m_encounter.reset();
        save();
        setScreen(Screen::World);
    }
    else if (outcome == cybercba::EncounterOutcome::Defeat)
    {
        m_encounter.reset();
        setScreen(Screen::Defeat);
    }
}
std::string GameApp::nextEnemyActionId() const
{
    // Deterministic and known ahead of time (no general EncounterAI yet — see
    // docs/architecture.md Phase 3 notes), so EnemyIntentPanel can show a real
    // prediction instead of a fabricated one.
    return m_encounterProtagonist == cybercba::CharacterId::Emma
        ? "relay_pulse"
        : (m_encounter->round() % 2 == 0 ? "rush" : "charge_shot");
}
void GameApp::updateEncounter()
{
    if (!m_encounter) { setScreen(Screen::World); return; }
    const std::string playerId = m_encounterProtagonist == cybercba::CharacterId::Emma ? "emma" : "magga";
    const std::string enemyId = m_encounterProtagonist == cybercba::CharacterId::Emma ? "relay" : "drone";
    if (m_encounter->activeParticipantId() != playerId)
    {
        m_encounter->performAction(enemyId, nextEnemyActionId(), playerId);
        resolveEncounterOutcome();
        return;
    }
    const auto& actions = m_encounter->actions();
    if (m_input.wasPressed(InputAction::NavigateDown)) m_encounterAction = (m_encounterAction + 1) % static_cast<int>(actions.size());
    if (m_input.wasPressed(InputAction::NavigateUp)) m_encounterAction = (m_encounterAction + static_cast<int>(actions.size()) - 1) % static_cast<int>(actions.size());
    if (m_input.wasPressed(InputAction::Cancel)) { m_encounter.reset(); m_message = "Encuentro abortado."; m_toastSeconds = 2.0F; setScreen(Screen::World); return; }
    if (!m_input.wasPressed(InputAction::Confirm)) return;
    const auto& action = actions[static_cast<std::size_t>(m_encounterAction)];
    const auto result = m_encounter->performAction(playerId, action.id, enemyId);
    if (!result.valid) { m_message = "Accion no disponible: " + result.reason; m_toastSeconds = 2.0F; return; }
    resolveEncounterOutcome();
}
void GameApp::drawEncounter() const
{
    // Turn-based encounter template (7.2), composed from reusable widgets
    // (TurnOrderPanel/ParticipantPanel/StatusPanel/EnemyIntentPanel) instead
    // of ad hoc text lines.
    if (!m_encounter) return;
    const std::string playerId = m_encounterProtagonist == cybercba::CharacterId::Emma ? "emma" : "magga";
    const std::string enemyId = m_encounterProtagonist == cybercba::CharacterId::Emma ? "relay" : "drone";
    const auto* player = m_encounter->participant(playerId);
    const auto* enemy = m_encounter->participant(enemyId);
    const char* title = m_encounterProtagonist == cybercba::CharacterId::Emma ? "INTRUSION DE ENLACE" : "ENFRENTAMIENTO CON EL DRON";
    if (m_encounterProtagonist == cybercba::CharacterId::Emma)
    {
        if (const Texture2D* background = m_assets ? m_assets->texture(HACKING_ENCOUNTER_BACKGROUND) : nullptr)
            DrawTexturePro(*background, {0, 0, static_cast<float>(background->width), static_cast<float>(background->height)},
                           {0, 0, static_cast<float>(W), static_cast<float>(H)}, {0, 0}, 0.0F, WHITE);
    }
    DrawRectangle(0, 0, W, H, Color {0, 0, 0, 165});
    DrawRectangle(120, 80, 1040, 560, Color {3, 16, 29, 248}); DrawRectangleLinesEx({120, 80, 1040, 560}, 3, NlmColors::CYAN);
    drawText(title, 155, 105, 32, NlmColors::CYAN);
    drawText("Turno " + std::to_string(m_encounter->round()) + (m_encounter->tension() > 0 ? "   Tension: " + std::to_string(m_encounter->tension()) : ""), 155, 148, 18, DIM);

    std::vector<TurnOrderPanel::Entry> order;
    for (const auto& id : m_encounter->turnOrder())
    {
        const auto* participant = m_encounter->participant(id);
        order.push_back({id, participant ? participant->displayName : id});
    }
    TurnOrderPanel::draw({155, 172, 970, 22}, order, m_encounter->activeParticipantId(), m_uiFont);

    const bool playerTurn = m_encounter->activeParticipantId() == playerId;
    if (player) ParticipantPanel::draw({155, 202, 460, 110}, *player, playerTurn, NlmColors::CYAN, m_uiFont);
    if (enemy)
    {
        ParticipantPanel::draw({665, 202, 460, 70}, *enemy, !playerTurn, NlmColors::AMBER, m_uiFont);
        const auto& enemyActions = m_encounter->actions();
        const auto found = std::find_if(enemyActions.begin(), enemyActions.end(), [this](const auto& a) { return a.id == nextEnemyActionId(); });
        EnemyIntentPanel::draw({665, 278, 460, 28}, found != enemyActions.end() ? found->displayName : nextEnemyActionId(), m_uiFont);
    }

    const char* combatPortrait = m_encounterProtagonist == cybercba::CharacterId::Emma ? EMMA_COMBAT_PORTRAIT : MAGGA_COMBAT_PORTRAIT;
    const char* encounterArt = m_encounterProtagonist == cybercba::CharacterId::Magga ? SECURITY_DRONE : figurePath(m_encounterProtagonist);
    const Rectangle combatPortraitBounds {885.0F, 322.0F, 105.0F, 258.0F};
    const Rectangle encounterArtBounds {1000.0F, 322.0F, 115.0F, 258.0F};
    if (const Texture2D* portrait = m_assets ? m_assets->texture(combatPortrait) : nullptr)
    {
        DrawRectangleRec(combatPortraitBounds, Color {5, 19, 30, 175});
        DrawRectangleLinesEx(combatPortraitBounds, 1.0F, m_encounterProtagonist == cybercba::CharacterId::Emma ? NlmColors::CYAN : NlmColors::AMBER);
        const Rectangle destination = aspectFit(*portrait, combatPortraitBounds);
        DrawTexturePro(*portrait, {0, 0, static_cast<float>(portrait->width), static_cast<float>(portrait->height)}, destination, {0, 0}, 0.0F, WHITE);
    }
    if (const Texture2D* figure = m_assets ? m_assets->texture(encounterArt) : nullptr)
    {
        DrawRectangleRec(encounterArtBounds, Color {5, 19, 30, 175});
        DrawRectangleLinesEx(encounterArtBounds, 1.0F, m_encounterProtagonist == cybercba::CharacterId::Emma ? NlmColors::CYAN : NlmColors::AMBER);
        const Rectangle destination = aspectFit(*figure, encounterArtBounds);
        DrawTexturePro(*figure, {0, 0, static_cast<float>(figure->width), static_cast<float>(figure->height)}, destination, {0, 0}, 0.0F, WHITE);
    }

    const auto& actions = m_encounter->actions();
    std::vector<std::string> labels;
    for (const auto& action : actions)
    {
        std::string label = action.displayName;
        if (!action.cost.resource.empty()) label += " (" + action.cost.resource + " " + std::to_string(action.cost.amount) + ")";
        labels.push_back(label);
    }
    ActionMenu::draw({155, 320, 700, 0}, labels, m_encounterAction, m_uiFont, 34.0F, 6.0F);

    drawText(devicePrompt("Flechas: elegir  ENTER: usar  ESC: retirarse", "Cruceta: elegir  A: usar  B: retirarse"), 155, 605, 17, DIM);
    if (!m_message.empty() && m_toastSeconds > 0.0F) drawText(m_message, 155, 630, 17, NlmColors::TEXT);
}
void GameApp::drawOverlay(const std::string& title, const std::string& body, const std::string& hint) const
{
    DrawRectangle(155, 105, 970, 525, Color {3, 16, 29, 248}); DrawRectangleLinesEx({155, 105, 970, 525}, 3, NlmColors::CYAN); drawText(title, 220, 150, 38, NlmColors::CYAN);
    int y = 225, start = 0; while (start <= static_cast<int>(body.size())) { const int end = static_cast<int>(body.find('\n', start)); drawText(body.substr(start, end < 0 ? std::string::npos : end - start), 220, y, 20, NlmColors::TEXT); y += 30; if (end < 0) break; start = end + 1; }
    drawText(hint, 220, 585, 18, NlmColors::AMBER);
}
void GameApp::drawModal() const
{
    if (m_modal == Modal::None) return;
    DrawRectangle(0, 0, W, H, Color {0, 0, 0, 165});
    if (m_modal == Modal::ConfirmNewGame) drawOverlay("SOBRESCRIBIR PARTIDA", "La partida actual será reemplazada. Esta acción no se puede deshacer.", "ENTER: sobrescribir   ESC: cancelar");
    else if (m_modal == Modal::ConfirmExit) drawOverlay("SALIR DEL JUEGO", "Se cerrará la aplicación de forma segura.", "ENTER: salir   ESC: cancelar");
    else if (m_modal == Modal::ConfirmRestart) drawOverlay("REINICIAR CHECKPOINT", "Se perderá el progreso posterior al último checkpoint.", "ENTER: reiniciar   ESC: cancelar");
    else if (m_modal == Modal::ConfirmCharacter)
    {
        // Rewritten to read as a narrative beat, not a UX confirmation
        // ("elegís a quién revivís esta noche", not "vas a vivir la historia
        // desde la perspectiva de").
        const auto chosen = m_selected == 0 ? cybercba::CharacterId::Emma : cybercba::CharacterId::Magga;
        const auto other = cybercba::otherCharacter(chosen);
        drawOverlay(std::string("¿RESPONDÉS COMO ") + cybercba::characterName(chosen) + "?",
                    std::string("Elegís revivir esta noche desde los ojos de ") + cybercba::characterName(chosen) + ".\n\n" +
                        cybercba::characterName(other) + " sigue ahí, del otro lado de la señal, esperando una respuesta que quizás nunca llegue.",
                    "ENTER: confirmar elección   ESC: volver");
    }
    else if (m_modal == Modal::ConfirmMenu) drawOverlay("VOLVER AL MENÚ", "La partida se guardará antes de abandonar la misión.", "ENTER: guardar y volver   ESC: cancelar");
    else if (m_modal == Modal::Dialogue)
    {
        const auto other = cybercba::otherCharacter(m_session.campaign().selectedCharacter);
        const Color otherColor = other == cybercba::CharacterId::Emma ? NlmColors::CYAN : NlmColors::AMBER;
        const Texture2D* otherPortrait = m_assets ? m_assets->texture(transmissionPortraitPath(other)) : nullptr;
        DialoguePanel::draw(UiMetrics::bottomPanelBounds(), cybercba::characterName(other), otherPortrait, otherColor,
                             "La transmisión se corta entre la lluvia. El tren 41 sigue llamando desde una línea muerta.",
                             devicePrompt("ENTER: continuar", "A: continuar"), m_uiFont);
    }
}
void GameApp::updateMenu()
{
    if (m_input.wasPressed(InputAction::NavigateDown)) m_selected = (m_selected + 1) % 6;
    if (m_input.wasPressed(InputAction::NavigateUp)) m_selected = (m_selected + 5) % 6;
    const Vector2 mouse = virtualMouse(); if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) for (int i = 0; i < 6; ++i) if (inRect(mouse, 400, 245 + i * 49, 480, 42)) m_selected = i;
    if (m_input.wasPressed(InputAction::Cancel)) { m_modal = Modal::ConfirmExit; return; }
    if (!m_input.wasPressed(InputAction::Confirm)) return;
    if (m_selected == 0) { if (m_session.progress().hasSave) setScreen(Screen::World); else { m_message = "No hay una partida valida para continuar."; m_toastSeconds = 2.0F; } }
    else if (m_selected == 1) { m_modal = m_session.progress().hasSave ? Modal::ConfirmNewGame : Modal::None; if (m_modal == Modal::None) setScreen(Screen::CharacterSelection); }
    else if (m_selected == 2) setScreen(Screen::Chapters); else if (m_selected == 3) setScreen(Screen::Settings); else if (m_selected == 4) setScreen(Screen::Credits); else m_modal = Modal::ConfirmExit;
}
void GameApp::updateSettings()
{
    const Vector2 mouse = virtualMouse();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
    {
        for (int i = 0; i < 5; ++i)
            if (inRect(mouse, 95 + i * 220, 125, 190, 42)) { m_settingsTab = i; m_settingsRow = 0; return; }
        for (int i = 0; i < 5; ++i)
            if (inRect(mouse, 240, 215 + i * 58, 800, 46)) { m_settingsRow = i; break; }
    }
    if (m_input.wasPressed(InputAction::NavigateLeft)) { m_settingsTab = (m_settingsTab + 4) % 5; m_settingsRow = 0; }
    if (m_input.wasPressed(InputAction::NavigateRight)) { m_settingsTab = (m_settingsTab + 1) % 5; m_settingsRow = 0; }
    const int count = m_settingsTab == 1 || m_settingsTab == 3 ? 5 : 3;
    if (m_input.wasPressed(InputAction::NavigateDown)) m_settingsRow = (m_settingsRow + 1) % count;
    if (m_input.wasPressed(InputAction::NavigateUp)) m_settingsRow = (m_settingsRow + count - 1) % count;
    auto& a = m_session.accessibility(); auto& audio = m_session.audio(); const bool change = m_input.wasPressed(InputAction::Confirm) || m_input.wasPressed(InputAction::NavigateLeft) || m_input.wasPressed(InputAction::NavigateRight);
    if (m_input.wasPressed(InputAction::Confirm))
    {
        if (m_settingsTab == 0) { if (m_settingsRow == 0) a.uiScale = a.uiScale >= 1.25F ? 0.85F : a.uiScale + 0.05F; else if (m_settingsRow == 1) a.scanlines = !a.scanlines; else a.reduceFlashes = !a.reduceFlashes; }
        else if (m_settingsTab == 1) { if (m_settingsRow == 0) audio.muted = !audio.muted; else { float* levels[] {&audio.musicVolume, &audio.effectsVolume, &audio.ambienceVolume, &audio.dialogueVolume}; float& level = *levels[m_settingsRow - 1]; level = level >= 1.0F ? 0.0F : level + 0.1F; } }
        else if (m_settingsTab == 3) { bool* toggles[] {&a.reducedMotion, &a.subtitles, &a.highContrast, &a.persistentPrompts}; if (m_settingsRow < 4) *toggles[m_settingsRow] = !*toggles[m_settingsRow]; else a.dialogueSpeed = a.dialogueSpeed >= 1.5F ? 0.5F : a.dialogueSpeed + 0.25F; }
        persistSettings();
    }
    (void)change;
    if (m_input.wasPressed(InputAction::Cancel)) setScreen(m_session.campaign().prologueStarted ? Screen::Pause : Screen::Menu);
}
void GameApp::updatePause()
{
    const Vector2 mouse = virtualMouse();
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        for (int i = 0; i < 6; ++i)
            if (inRect(mouse, 370, 170 + i * 54, 540, 42)) { m_selected = i; break; }
    if (m_input.wasPressed(InputAction::NavigateDown))
        m_selected = (m_selected + 1) % 6;
    if (m_input.wasPressed(InputAction::NavigateUp))
        m_selected = (m_selected + 5) % 6;
    if (m_input.wasPressed(InputAction::Pause) || m_input.wasPressed(InputAction::Cancel)) { setScreen(Screen::World); return; }
    if (!m_input.wasPressed(InputAction::Confirm)) return;
    if (m_selected == 0) setScreen(Screen::World); else if (m_selected == 1) setScreen(Screen::Objectives); else if (m_selected == 2) setScreen(Screen::Settings); else if (m_selected == 3) m_modal = Modal::ConfirmRestart; else if (m_selected == 4) m_modal = Modal::ConfirmMenu; else m_modal = Modal::ConfirmExit;
}
void GameApp::updateModal()
{
    if (m_input.wasPressed(InputAction::Cancel)) { m_modal = Modal::None; return; }
    if (!m_input.wasPressed(InputAction::Confirm)) return;
    const Modal modal = m_modal; m_modal = Modal::None;
    if (modal == Modal::ConfirmNewGame) setScreen(Screen::CharacterSelection); else if (modal == Modal::ConfirmCharacter) start(m_selected == 0 ? cybercba::CharacterId::Emma : cybercba::CharacterId::Magga); else if (modal == Modal::ConfirmExit) m_exit = true; else if (modal == Modal::ConfirmRestart) resetCheckpoint(); else if (modal == Modal::ConfirmMenu) { save(); setScreen(Screen::Menu); }
}
void GameApp::update(float dt)
{
    m_input.update(); if (m_toastSeconds > 0.0F) m_toastSeconds -= dt; if (m_overlay) m_overlay->update(dt); if (m_input.wasPressed(InputAction::ToggleDevOverlay) && m_overlay) m_overlayVisible = !m_overlayVisible;
    if (m_modal != Modal::None) { updateModal(); return; }
    if (m_screen == Screen::Splash) { m_splashElapsed += dt; if (m_splashElapsed >= m_config.splashSeconds() || m_input.wasPressed(InputAction::Confirm)) setScreen(Screen::Menu); return; }
    if (m_screen == Screen::Menu) { updateMenu(); return; }
    if (m_screen == Screen::CharacterSelection) { if (m_input.wasPressed(InputAction::NavigateLeft) || m_input.wasPressed(InputAction::NavigateRight)) m_selected = 1 - m_selected; if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) { const Vector2 mouse = virtualMouse(); if (inRect(mouse, 125, 160, 495, 410)) m_selected = 0; else if (inRect(mouse, 660, 160, 495, 410)) m_selected = 1; } if (m_input.wasPressed(InputAction::Confirm)) m_modal = Modal::ConfirmCharacter; if (m_input.wasPressed(InputAction::Cancel)) setScreen(Screen::Menu); return; }
    if (m_screen == Screen::Intro) { if (m_input.wasPressed(InputAction::Confirm)) setScreen(Screen::World); if (m_input.wasPressed(InputAction::Cancel)) setScreen(Screen::Menu); return; }
    if (m_screen == Screen::World) { updateWorldNode(); return; }
    if (m_screen == Screen::Encounter) { updateEncounter(); return; }
    if (m_screen == Screen::Pause) { updatePause(); return; }
    if (m_screen == Screen::Settings) { updateSettings(); return; }
    if (m_screen == Screen::Defeat) { if (m_input.wasPressed(InputAction::Confirm)) resetCheckpoint(); else if (m_input.wasPressed(InputAction::NavigateDown)) setScreen(Screen::Settings); else if (m_input.wasPressed(InputAction::Cancel)) setScreen(Screen::Menu); return; }
    if (m_input.wasPressed(InputAction::Confirm) || m_input.wasPressed(InputAction::Cancel)) { if (m_screen == Screen::End && m_input.wasPressed(InputAction::Confirm)) setScreen(Screen::Chapters); else setScreen(Screen::Menu); }
}
void GameApp::draw()
{
    BeginTextureMode(m_virtual); drawBackground();
    if (m_screen == Screen::Splash) { drawText("EXODUS SYSTEMS", 415, 260, 48, NlmColors::CYAN); drawText("AN EXODUS SYSTEMS PRODUCTION", 430, 330, 19, DIM); drawText("LA ÚLTIMA TRANSMISIÓN", 410, 370, 23, NlmColors::AMBER); }
    else if (m_screen == Screen::Menu) drawMenu(); else if (m_screen == Screen::CharacterSelection) drawSelection(); else if (m_screen == Screen::Intro) drawIntro(); else if (m_screen == Screen::World) drawWorld(); else if (m_screen == Screen::Chapters) drawChapters(); else if (m_screen == Screen::Settings) drawSettings(); else if (m_screen == Screen::Credits) drawCredits(); else if (m_screen == Screen::Pause) drawPause(); else if (m_screen == Screen::Objectives) drawObjectives(); else if (m_screen == Screen::End) drawEnd(); else if (m_screen == Screen::Encounter) drawEncounter(); else if (m_screen == Screen::Defeat) drawOverlay("SEÑAL PERDIDA", "La transmisión se pierde en la lluvia.", "ENTER: reintentar checkpoint   Flecha abajo: configuración   ESC: menú");
    drawModal(); if (m_overlay && m_overlayVisible) m_overlay->draw(); EndTextureMode();
    const float scale = integerScale(GetScreenWidth(), GetScreenHeight()); const float ox = (GetScreenWidth() - W * scale) / 2, oy = (GetScreenHeight() - H * scale) / 2;
    BeginDrawing(); ClearBackground(BLACK); DrawTexturePro(m_virtual.texture, {0, 0, static_cast<float>(m_virtual.texture.width), -static_cast<float>(m_virtual.texture.height)}, {ox, oy, W * scale, H * scale}, {0, 0}, 0, WHITE); EndDrawing();
}
