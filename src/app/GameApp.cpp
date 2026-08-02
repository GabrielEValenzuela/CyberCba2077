#include "app/GameApp.hpp"

#include <algorithm>
#include <cmath>

#include "config/GameConfig.hpp"
#include "cybercba/Campaign.hpp"
#include "cybercba/WorldLayout.hpp"
#include "ui/AudioService.hpp"
#include "ui/AssetStore.hpp"
#include "ui/NlmColors.hpp"
#include "ui/widgets/DevOverlay.hpp"

namespace
{
constexpr int W = 1280, H = 720;
constexpr Color DIM {152, 181, 197, 255};
constexpr Color CARD {5, 25, 40, 238};
constexpr const char* menuItems[] {"CONTINUAR", "NUEVA PARTIDA", "CAPITULOS", "CONFIGURACION", "CREDITOS", "SALIR"};
constexpr const char* pauseItems[] {"REANUDAR", "OBJETIVOS", "CONFIGURACION", "REINICIAR CHECKPOINT", "VOLVER AL MENU", "SALIR DEL JUEGO"};
const char* stageObjective(cybercba::PrologueStage stage)
{
    switch (stage)
    {
        case cybercba::PrologueStage::Shelter: return "Inspecciona el transmisor de La Luciernaga";
        case cybercba::PrologueStage::Transmission: return "Reconstruye la senal antes de que desaparezca";
        case cybercba::PrologueStage::Route: return "Recupera las coordenadas del tren 41";
        case cybercba::PrologueStage::Convergence: return "Escapa del refugio y sigue la linea oeste";
        case cybercba::PrologueStage::Epilogue: return "Llega a la entrada abandonada del Neometro";
        case cybercba::PrologueStage::Complete: return "Prologo completado";
    }
    return "";
}
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

bool isShelterStage(cybercba::PrologueStage stage)
{
    return stage == cybercba::PrologueStage::Shelter || stage == cybercba::PrologueStage::Transmission;
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
    m_audio->setMuted(m_session.audio().muted);
    m_audio->setVolumes(m_session.audio().musicVolume, m_session.audio().effectsVolume);
    if (config.isDevelopmentMode()) m_overlay = std::make_unique<DevOverlay>();
}
GameApp::~GameApp() { if (m_virtual.id) UnloadRenderTexture(m_virtual); }
bool GameApp::shouldExit() const { return m_exit; }
void GameApp::setScreen(Screen screen) { m_screen = screen; m_selected = 0; }
void GameApp::drawText(const std::string& text, int x, int y, int size, Color color) const { DrawText(text.c_str(), x, y, size, color); }
const char* GameApp::devicePrompt(const char* keyboard, const char* gamepad) const
{ return m_input.lastDevice() == InputDevice::Gamepad ? gamepad : keyboard; }
Vector2 GameApp::virtualMouse() const
{
    const float scale = std::min(static_cast<float>(GetScreenWidth()) / W, static_cast<float>(GetScreenHeight()) / H);
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
    m_playerPosition = {300.0F, 470.0F};
    m_exitUnlocked = false;
    m_powerSequenceSeconds = 0.0F;
    save();
    setScreen(Screen::Intro);
}
void GameApp::resetCheckpoint()
{
    if (m_session.resetToCheckpoint())
    {
        m_playerPosition = {300.0F, 470.0F};
        m_exitUnlocked = false;
        m_powerSequenceSeconds = 0.0F;
        m_message = "Checkpoint restaurado.";
        save();
        setScreen(Screen::World);
    }
    else m_message = "No hay un checkpoint valido para restaurar.";
}
void GameApp::interact()
{
    auto& campaign = m_session.campaign();
    const auto selected = campaign.selectedCharacter;
    const cybercba::WorldProp* prop = nearbyWorldProp();
    if (!prop) return;
    const std::string_view interaction = prop->interactionId;
    if (interaction == "inspect_photo")
    {
        m_message = selected == cybercba::CharacterId::Emma
            ? "FOTOGRAFIA DETERIORADA\nMagga todavía miraba a cámara. Yo ya sabía que la fecha importaba."
            : "FOTOGRAFIA DETERIORADA\nEmma fingía que no estaba cansado. La fecha fue eliminada del archivo.";
        m_toastSeconds = 6.0F; return;
    }
    if (interaction == "inspect_map")
    {
        m_message = "MAPA DE NEO-CORDOBA\nLinea Oeste: Santa Maria, San Justo y un acceso clausurado al Neometro.";
        m_toastSeconds = 6.0F; return;
    }
    if (interaction == "inspect_transmitter")
    {
        if (campaign.stage == cybercba::PrologueStage::Shelter)
        {
            cybercba::MissionSystem::advance(campaign, cybercba::PrologueStage::Shelter, cybercba::PrologueStage::Transmission, "transmission");
            m_powerSequenceSeconds = 0.0F;
            m_session.narrative().unresolvedHurt = 2;
            m_message = "LA SENAL IMPOSIBLE\nEl codigo de La Luciernaga vuelve a transmitir. Segun Exodus, su propietaria esta muerta.";
            m_modal = Modal::Dialogue;
        }
        else if (campaign.stage == cybercba::PrologueStage::Transmission)
        {
            m_exitUnlocked = true;
            m_message = "La señal libera el cierre. La puerta industrial ya puede abrirse.";
            m_toastSeconds = 3.0F;
        }
        return;
    }
    if (interaction == "exit_shelter")
    {
        if (!m_exitUnlocked) { m_message = "La puerta sigue bloqueada. Reconstrui la señal primero."; m_toastSeconds = 2.0F; return; }
        cybercba::MissionSystem::advance(campaign, cybercba::PrologueStage::Transmission, cybercba::PrologueStage::Route, "street");
        m_playerPosition = {350.0F, 540.0F}; m_message = "Objetivo actualizado: ruta hacia el Neometro."; save(); return;
    }
    if (interaction == "use_terminal" && campaign.stage == cybercba::PrologueStage::Route && selected == cybercba::CharacterId::Emma)
    {
        m_hackProgress = 0.0F; m_modal = Modal::Hacking; return;
    }
    if (interaction == "face_drone" && campaign.stage == cybercba::PrologueStage::Route && selected == cybercba::CharacterId::Magga)
    {
        m_message = "El dron bloquea la pasarela. Usa Impacto tactico para abrir la ruta."; m_toastSeconds = 2.5F; return;
    }
    if (interaction == "enter_neometro" && campaign.stage == cybercba::PrologueStage::Convergence)
    {
        cybercba::MissionSystem::advance(campaign, cybercba::PrologueStage::Convergence, cybercba::PrologueStage::Epilogue, "neometro_gate");
        m_session.narrative().trust++; m_message = "Una luz amarilla parpadea bajo el símbolo de La Luciernaga."; save(); return;
    }
    if (interaction == "enter_neometro" && campaign.stage == cybercba::PrologueStage::Epilogue)
    {
        cybercba::MissionSystem::complete(campaign); save(); setScreen(Screen::End);
    }
    save();
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
    const Color border = disabled ? Color {48, 70, 80, 255} : selected ? NlmColors::CYAN : Color {48, 94, 110, 255};
    DrawRectangle(x, y, width, 42, CARD);
    DrawRectangleLinesEx({static_cast<float>(x), static_cast<float>(y), static_cast<float>(width), 42}, selected ? 3.0F : 1.0F, border);
    if (selected) DrawRectangle(x + 10, y + 9, 5, 24, NlmColors::AMBER);
    drawText(label, x + 32, y + 10, 21, disabled ? DIM : NlmColors::TEXT);
}
void GameApp::drawMenu() const
{
    drawText("CYBERCBA 2077", 330, 72, 68, NlmColors::CYAN);
    drawText("LA CIUDAD QUE NOS OLVIDO", 405, 148, 25, NlmColors::AMBER);
    drawText("v0.1.0 // LA ULTIMA TRANSMISION", 424, 185, 18, DIM);
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
        // Emma's selection portrait uses a dedicated asset with a baked-in optical
        // scale correction (her crouched pose reads visually smaller than Magga's at
        // an identical alpha bounding box); the World sprite below is unaffected.
        const char* spritePath = i == 0 ? "assets/processed/characters/emma_idle_selection.png" : "assets/processed/characters/magga_idle.png";
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
        drawText(c.displayName, x + 238, 235, 20, NlmColors::TEXT); drawText(std::string("Origen: ") + c.origin, x + 238, 265, 17, DIM);
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
    const auto other = cybercba::otherCharacter(m_session.campaign().selectedCharacter);
    drawText("LA ULTIMA TRANSMISION", 365, 125, 44, NlmColors::CYAN);
    drawText("Lluvia. Interferencia. Un tren lejano sobre una linea muerta.", 255, 220, 23, DIM);
    DrawRectangle(170, 275, 940, 160, CARD); drawText(cybercba::characterName(other), 210, 302, 20, NlmColors::AMBER);
    drawText("No se cuanto tiempo va a conservarse esta senal.", 210, 342, 22, NlmColors::TEXT);
    drawText("El tren 41 volvio a aparecer en la red. Encontrame.", 210, 382, 22, NlmColors::TEXT);
    drawText(devicePrompt("ENTER: responder", "A: responder"), 490, 550, 21, NlmColors::CYAN);
}
std::string GameApp::prompt() const
{
    const auto stage = m_session.campaign().stage;
    const cybercba::WorldProp* prop = nearbyWorldProp();
    if (!prop) return "";
    const std::string key = devicePrompt("[E]", "[X]");
    if (std::string_view(prop->interactionId) == "inspect_photo") return key + " Revisar la fotografia";
    if (std::string_view(prop->interactionId) == "inspect_map") return key + " Examinar el mapa de Neo-Cordoba";
    if (std::string_view(prop->interactionId) == "inspect_transmitter") return key + (stage == cybercba::PrologueStage::Shelter ? " Examinar el transmisor" : " Reconstruir la transmision");
    if (std::string_view(prop->interactionId) == "exit_shelter") return key + (m_exitUnlocked ? " Salir del refugio" : " Puerta bloqueada");
    if (std::string_view(prop->interactionId) == "use_terminal" && stage == cybercba::PrologueStage::Route) return key + " Intervenir la terminal de acceso";
    if (std::string_view(prop->interactionId) == "face_drone" && stage == cybercba::PrologueStage::Route && m_session.campaign().selectedCharacter == cybercba::CharacterId::Magga) return std::string(devicePrompt("[X]", "[Y]")) + " Usar Impacto tactico";
    if (std::string_view(prop->interactionId) == "enter_neometro" && (stage == cybercba::PrologueStage::Convergence || stage == cybercba::PrologueStage::Epilogue)) return key + " Entrar al Neometro";
    return "";
}
const cybercba::WorldProp* GameApp::nearbyWorldProp() const
{
    const auto& campaign = m_session.campaign();
    const auto nearFrom = [&](const auto& props) -> const cybercba::WorldProp*
    {
        const cybercba::WorldProp* nearest = nullptr;
        float nearestDistance = 78.0F * 78.0F;
        for (const cybercba::WorldProp& prop : props)
        {
            if (prop.interactionId[0] == '\0') continue;
            const float dx = m_playerPosition.x - prop.pivotX;
            const float dy = m_playerPosition.y - prop.pivotY;
            const float distance = dx * dx + dy * dy;
            if (distance <= nearestDistance) { nearest = &prop; nearestDistance = distance; }
        }
        return nearest;
    };
    return isShelterStage(campaign.stage) ? nearFrom(cybercba::shelterProps()) : nearFrom(cybercba::exteriorProps());
}
void GameApp::drawHud() const
{
    const auto& player = m_session.player(); const auto selected = m_session.campaign().selectedCharacter;
    const bool showCombatHud = !isShelterStage(m_session.campaign().stage) || player.health < 99.9F || player.stamina < 99.9F;
    if (!showCombatHud) return;
    DrawRectangle(28, 22, 220, 52, Color {3, 16, 29, 210}); drawText("SALUD", 42, 31, 13, DIM); DrawRectangle(42, 48, 190, 9, Color {40, 55, 65, 255}); DrawRectangle(42, 48, static_cast<int>(190 * player.health / 100.0F), 9, NlmColors::AMBER);
    drawText(selected == cybercba::CharacterId::Emma ? "ENLACE" : "STAMINA", 42, 61, 13, DIM); DrawRectangle(112, 64, 120, 7, Color {40, 55, 65, 255}); DrawRectangle(112, 64, static_cast<int>(120 * player.stamina / 100.0F), 7, selected == cybercba::CharacterId::Emma ? NlmColors::CYAN : NlmColors::AMBER);
    DrawRectangle(895, 20, 355, 42, Color {3, 16, 29, 205}); drawText(stageObjective(m_session.campaign().stage), 912, 33, 15, NlmColors::TEXT);
}
void GameApp::drawShelterShell() const
{
    DrawRectangle(88, 108, 1104, 470, Color {24, 34, 40, 255});
    for (int y = 132; y < 570; y += 48)
        for (int x = 106; x < 1178; x += 72)
        {
            const bool alternate = ((x / 72) + (y / 48)) % 2 == 0;
            DrawRectangle(x, y, 68, 44, alternate ? Color {31, 42, 47, 255} : Color {21, 30, 36, 255});
            DrawLine(x, y + 44, x + 68, y + 44, Color {8, 15, 20, 170});
        }
    for (int x = 405; x < 700; x += 68) DrawLine(x, 525, x + 48, 530, Color {11, 15, 17, 230});
    DrawRectangle(495, 485, 84, 18, Color {28, 45, 50, 175});
    DrawRectangleLinesEx({495, 485, 84, 18}, 1.0F, Color {52, 70, 70, 120});
    DrawEllipse(604, 492, 61, 8, Color {28, 57, 66, 135});
    DrawRectangle(70, 88, 1140, 38, Color {10, 17, 23, 255});
    DrawRectangle(70, 558, 1140, 40, Color {9, 15, 20, 255});
    DrawRectangle(70, 88, 28, 510, Color {10, 17, 23, 255});
    DrawRectangle(1182, 88, 28, 510, Color {10, 17, 23, 255});
    DrawRectangle(100, 126, 1080, 9, Color {4, 10, 14, 170});
    drawSoftLight({235, 265}, 145.0F, Color {255, 181, 92, 255});
    drawSoftLight({875, 300}, 135.0F, NlmColors::CYAN);
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
void GameApp::drawWorldDebug(cybercba::WorldZone zone) const
{
    if (!m_overlayVisible) return;
    const auto drawDebug = [&](const auto& props)
    {
        for (const auto& prop : props)
        {
            if (prop.collider.width > 0.0F) DrawRectangleLinesEx({prop.collider.x, prop.collider.y, prop.collider.width, prop.collider.height}, 1.0F, withAlpha(NlmColors::VIOLET, 210));
            if (prop.interactionId[0] != '\0') DrawCircleLines(static_cast<int>(prop.pivotX), static_cast<int>(prop.pivotY), 78.0F, withAlpha(NlmColors::AMBER, 180));
            DrawCircleV({prop.pivotX, prop.pivotY}, 3.0F, NlmColors::CYAN);
            drawText(prop.id, static_cast<int>(prop.pivotX) - 42, static_cast<int>(prop.pivotY) - 12, 11, NlmColors::AMBER);
        }
    };
    if (zone == cybercba::WorldZone::Shelter) drawDebug(cybercba::shelterProps());
    else drawDebug(cybercba::exteriorProps());
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
        drawSoftLight({808, 296}, emergency ? 42.0F : 112.0F, NlmColors::CYAN);
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
void GameApp::drawWorld() const
{
    const auto& campaign = m_session.campaign(); const auto selected = campaign.selectedCharacter;
    const cybercba::WorldZone zone = isShelterStage(campaign.stage) ? cybercba::WorldZone::Shelter : cybercba::WorldZone::Exterior;
    if (zone == cybercba::WorldZone::Shelter) drawShelterShell(); else drawExteriorShell();
    drawWorldProps(zone, cybercba::WorldLayer::Low);
    drawWorldProps(zone, cybercba::WorldLayer::Mid);
    DrawEllipse(static_cast<int>(m_playerPosition.x - 19.0F), static_cast<int>(m_playerPosition.y - 7.0F), 19, 7, Color {0, 0, 0, 100});
    const char* playerSpritePath = selected == cybercba::CharacterId::Emma ? "assets/processed/characters/emma_idle.png" : "assets/processed/characters/magga_idle.png";
    if (const Texture2D* sprite = m_assets->texture(playerSpritePath))
    {
        const Rectangle destination = aspectFit(*sprite, {m_playerPosition.x - 48.0F, m_playerPosition.y - 128.0F, 96.0F, 128.0F});
        DrawTexturePro(*sprite, {0, 0, static_cast<float>(sprite->width), static_cast<float>(sprite->height)}, destination, {0, 0}, 0, WHITE);
    }
    else
    { DrawCircleV(m_playerPosition, 22, selected == cybercba::CharacterId::Emma ? NlmColors::CYAN : NlmColors::AMBER); DrawCircleLinesV(m_playerPosition, 26, NlmColors::TEXT); }
    drawWorldProps(zone, cybercba::WorldLayer::High);
    drawWorldSignage(zone);
    drawWorldLighting(zone);
    drawWeather(zone);
    drawWorldDebug(zone);
    drawHud();
    const std::string contextualPrompt = prompt();
    if (!contextualPrompt.empty())
    {
        const int promptWidth = static_cast<int>(contextualPrompt.size()) * 10 + 28;
        const int promptX = std::clamp(static_cast<int>(m_playerPosition.x) - promptWidth / 2, 20, W - promptWidth - 20);
        const int promptY = std::clamp(static_cast<int>(m_playerPosition.y) - 122, 120, 580);
        DrawRectangle(promptX, promptY, promptWidth, 30, Color {3, 16, 25, 225}); DrawRectangleLinesEx({static_cast<float>(promptX), static_cast<float>(promptY), static_cast<float>(promptWidth), 30}, 1.0F, NlmColors::CYAN);
        drawText(contextualPrompt, promptX + 14, promptY + 8, 15, NlmColors::TEXT);
    }
    drawText(devicePrompt("WASD mover  ESC pausa  O objetivos", "Stick mover  Menu pausa  View objetivos"), 755, 680, 16, DIM);
    if (!m_message.empty() && (m_toastSeconds > 0.0F || m_session.accessibility().persistentPrompts)) drawText(m_message, 38, 690, 17, NlmColors::TEXT);
}
void GameApp::drawChapters() const
{
    drawText("CAPITULOS", 515, 75, 45, NlmColors::CYAN); DrawRectangle(180, 160, 920, 155, CARD); DrawRectangleLinesEx({180, 160, 920, 155}, 2, NlmColors::CYAN);
    drawText("PROLOGO // LA ULTIMA TRANSMISION", 220, 195, 28, NlmColors::TEXT); drawText("v0.1.0", 220, 235, 18, DIM); drawText(m_session.campaign().prologueCompleted ? "COMPLETADO" : m_session.campaign().prologueStarted ? "EN PROGRESO" : "DISPONIBLE", 850, 235, 18, m_session.campaign().prologueCompleted ? NlmColors::GREEN : NlmColors::AMBER);
    DrawRectangle(180, 350, 920, 125, Color {5, 16, 26, 210}); DrawRectangleLinesEx({180, 350, 920, 125}, 1, DIM); drawText("CAPITULO I // NEOMETRO: EL ULTIMO CONVOY", 220, 385, 27, DIM); drawText("v0.2.0  //  PROXIMAMENTE: contenido no disponible", 220, 425, 18, DIM);
    drawText(devicePrompt("ENTER o ESC: volver", "A o B: volver"), 480, 590, 19, DIM);
}
void GameApp::drawObjectives() const
{
    drawOverlay("MISION ACTUAL", "LA ULTIMA TRANSMISION\n\nPRINCIPAL\n[OK] Revisa el transmisor.\n[OK] Sal del refugio.\n[ ] " + std::string(stageObjective(m_session.campaign().stage)) + "\n\nEVIDENCIAS\n[•] Simbolo de La Luciernaga\n[•] Registro del tren 41", devicePrompt("O o ESC: volver", "View o B: volver"));
}
void GameApp::drawSettings() const
{
    static const char* tabs[] {"VIDEO", "AUDIO", "CONTROLES", "ACCESIBILIDAD", "GAMEPLAY"};
    drawText("CONFIGURACION", 440, 62, 42, NlmColors::CYAN);
    for (int i = 0; i < 5; ++i) drawButton(tabs[i], 95 + i * 220, 125, 190, i == m_settingsTab);
    const auto& a = m_session.accessibility(); const auto& audio = m_session.audio();
    const char* rows[5] {}; std::string values[5]; int count = 0;
    if (m_settingsTab == 0) { rows[0] = "ESCALA DE UI"; values[0] = std::to_string(static_cast<int>(a.uiScale * 100)) + "%"; rows[1] = "SCANLINES"; values[1] = a.scanlines ? "ACTIVADAS" : "DESACTIVADAS"; rows[2] = "FLASHES"; values[2] = a.reduceFlashes ? "REDUCIDOS" : "NORMALES"; count = 3; }
    else if (m_settingsTab == 1) { rows[0] = "SILENCIO"; values[0] = audio.muted ? "SI" : "NO"; rows[1] = "MUSICA"; values[1] = std::to_string(static_cast<int>(audio.musicVolume * 100)) + "%"; rows[2] = "EFECTOS"; values[2] = std::to_string(static_cast<int>(audio.effectsVolume * 100)) + "%"; rows[3] = "AMBIENTE"; values[3] = std::to_string(static_cast<int>(audio.ambienceVolume * 100)) + "%"; rows[4] = "DIALOGOS"; values[4] = std::to_string(static_cast<int>(audio.dialogueVolume * 100)) + "%"; count = 5; }
    else if (m_settingsTab == 2) { rows[0] = "TECLADO"; values[0] = "WASD / E / X / ESC"; rows[1] = "GAMEPAD"; values[1] = "STICK / X / Y / MENU"; rows[2] = "REMAPEO"; values[2] = "PROXIMAMENTE"; count = 3; }
    else if (m_settingsTab == 3) { rows[0] = "MOVIMIENTO REDUCIDO"; values[0] = a.reducedMotion ? "SI" : "NO"; rows[1] = "SUBTITULOS"; values[1] = a.subtitles ? "SI" : "NO"; rows[2] = "ALTO CONTRASTE"; values[2] = a.highContrast ? "SI" : "NO"; rows[3] = "PROMPTS PERSISTENTES"; values[3] = a.persistentPrompts ? "SI" : "NO"; rows[4] = "VELOCIDAD DIALOGO"; values[4] = std::to_string(static_cast<int>(a.dialogueSpeed * 100)) + "%"; count = 5; }
    else { rows[0] = "CONFIRMAR SOBRESCRITURA"; values[0] = "SI"; rows[1] = "MOSTRAR OBJETIVOS"; values[1] = a.persistentPrompts ? "SI" : "AL MANTENER O"; rows[2] = "DIFICULTAD"; values[2] = "PREPARADA PARA FUTURO"; count = 3; }
    for (int i = 0; i < count; ++i) { const int y = 215 + i * 58; DrawRectangle(240, y, 800, 46, CARD); DrawRectangleLinesEx({240.0F, static_cast<float>(y), 800, 46}, m_settingsRow == i ? 3.0F : 1.0F, m_settingsRow == i ? NlmColors::CYAN : Color {48,94,110,255}); drawText(rows[i], 270, y + 13, 19, NlmColors::TEXT); drawText(values[i], 730, y + 13, 18, DIM); }
    drawText(devicePrompt("Flechas: cambiar  ENTER: alternar  ESC: volver", "Cruceta: cambiar  A: alternar  B: volver"), 295, 620, 17, DIM);
}
void GameApp::drawCredits() const
{
    drawOverlay("CREDITOS", "CYBERCBA 2077\nDireccion y desarrollo: equipo CYBERCBA\nDiseno narrativo y visual: equipo CYBERCBA\n\nTecnologia: raylib, CMake y C++17\nAssets: placeholders procedurales originales\nLicencias y procedencia: assets/data/assets-manifest.json\n\nNo se incorporaron recursos de Thiings.", devicePrompt("ENTER o ESC: volver", "A o B: volver"));
}
void GameApp::drawPause() const
{
    drawWorld(); DrawRectangle(0, 0, W, H, Color {0, 0, 0, 155}); drawText("PAUSA", 530, 95, 45, NlmColors::CYAN);
    for (int i = 0; i < 6; ++i) drawButton(pauseItems[i], 370, 170 + i * 54, 540, m_selected == i);
}
void GameApp::drawEnd() const
{
    const auto& c = m_session.campaign(); drawOverlay("PROLOGO COMPLETADO", std::string("LA ULTIMA TRANSMISION\n\nProtagonista: ") + cybercba::characterName(c.selectedCharacter) + "\nEvidencias encontradas: " + std::to_string(m_session.narrative().recoveredTruth) + "\nRuta utilizada: " + cybercba::characterAbility(c.selectedCharacter) + "\nCheckpoint final: " + c.checkpoint + "\n\nPROXIMA RELEASE\nv0.2.0  NEOMETRO: EL ULTIMO CONVOY", "ENTER: ver capitulos   ESC: volver al menu");
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
    if (m_modal == Modal::ConfirmNewGame) drawOverlay("SOBRESCRIBIR PARTIDA", "La partida actual sera reemplazada. Esta accion no se puede deshacer.", "ENTER: sobrescribir   ESC: cancelar");
    else if (m_modal == Modal::ConfirmExit) drawOverlay("SALIR DEL JUEGO", "Se cerrara la aplicacion de forma segura.", "ENTER: salir   ESC: cancelar");
    else if (m_modal == Modal::ConfirmRestart) drawOverlay("REINICIAR CHECKPOINT", "Se perdera el progreso posterior al ultimo checkpoint.", "ENTER: reiniciar   ESC: cancelar");
    else if (m_modal == Modal::ConfirmCharacter) { const auto chosen = m_selected == 0 ? cybercba::CharacterId::Emma : cybercba::CharacterId::Magga; drawOverlay(std::string("RESPONDER COMO ") + cybercba::characterName(chosen), std::string("Vas a vivir la historia desde la perspectiva de ") + cybercba::characterName(chosen) + ".\n\n" + cybercba::characterName(cybercba::otherCharacter(chosen)) + " será la voz al otro lado de la transmisión.", "ENTER: confirmar elección   ESC: volver"); }
    else if (m_modal == Modal::ConfirmMenu) drawOverlay("VOLVER AL MENU", "La partida se guardara antes de abandonar la mision.", "ENTER: guardar y volver   ESC: cancelar");
    else if (m_modal == Modal::Dialogue) drawOverlay(cybercba::characterName(cybercba::otherCharacter(m_session.campaign().selectedCharacter)), "La transmision se corta entre la lluvia. El tren 41 sigue llamando desde una linea muerta.", devicePrompt("ENTER: continuar", "A: continuar"));
    else if (m_modal == Modal::Hacking) { drawOverlay("INTRUSION DE ENLACE", "Objetivo: Control de acceso B-17\nSeguridad: Nivel 1\nEstado: Estableciendo conexion...", devicePrompt("Manten ENTER para completar; ESC cancela", "Manten A para completar; B cancela")); DrawRectangle(330, 490, 620, 18, Color {40,55,65,255}); DrawRectangle(330, 490, static_cast<int>(620 * m_hackProgress), 18, NlmColors::CYAN); }
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
    if (m_modal == Modal::Hacking)
    {
        if (m_input.wasPressed(InputAction::Cancel)) { m_modal = Modal::None; m_message = "Intrusion cancelada."; return; }
        if (m_input.wasDown(InputAction::Confirm)) m_hackProgress += GetFrameTime() / 1.3F;
        if (m_hackProgress >= 1.0F) { cybercba::AbilitySystem::hack(m_session.player(), true); cybercba::MissionSystem::advance(m_session.campaign(), cybercba::PrologueStage::Route, cybercba::PrologueStage::Convergence, "service_gate"); m_session.narrative().recoveredTruth++; m_message = "Enlace intervenido. Registro oculto recuperado: TREN 41."; m_modal = Modal::None; save(); }
        return;
    }
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
    if (m_screen == Screen::World)
    {
        if (m_session.campaign().stage == cybercba::PrologueStage::Transmission) m_powerSequenceSeconds += dt;
        cybercba::AbilitySystem::update(m_session.player(), dt); if (m_input.wasPressed(InputAction::Pause) || m_input.wasPressed(InputAction::Cancel)) { setScreen(Screen::Pause); return; } if (m_input.wasPressed(InputAction::OpenObjectives)) { setScreen(Screen::Objectives); return; }
        Vector2 move {}; if (m_input.wasDown(InputAction::MoveUp)) move.y -= 1; if (m_input.wasDown(InputAction::MoveDown)) move.y += 1; if (m_input.wasDown(InputAction::MoveLeft)) move.x -= 1; if (m_input.wasDown(InputAction::MoveRight)) move.x += 1;
        if (move.x || move.y)
        {
            const float length = std::sqrt(move.x * move.x + move.y * move.y);
            Vector2 candidate {m_playerPosition.x + move.x / length * 190.0F * dt, m_playerPosition.y + move.y / length * 190.0F * dt};
            const cybercba::WorldZone zone = isShelterStage(m_session.campaign().stage) ? cybercba::WorldZone::Shelter : cybercba::WorldZone::Exterior;
            candidate.x = std::clamp(candidate.x, zone == cybercba::WorldZone::Shelter ? 118.0F : 35.0F, zone == cybercba::WorldZone::Shelter ? 1160.0F : 1245.0F);
            candidate.y = std::clamp(candidate.y, zone == cybercba::WorldZone::Shelter ? 152.0F : 135.0F, zone == cybercba::WorldZone::Shelter ? 540.0F : 635.0F);
            if (!cybercba::blocksMovement(zone, candidate.x, candidate.y)) m_playerPosition = candidate;
        }
        if (m_input.wasPressed(InputAction::Interact)) interact();
        const auto* nearby = nearbyWorldProp();
        const bool nearDrone = nearby && std::string_view(nearby->interactionId) == "face_drone";
        if (m_session.campaign().stage == cybercba::PrologueStage::Route && m_session.campaign().selectedCharacter == cybercba::CharacterId::Magga && nearDrone && m_input.wasPressed(InputAction::PrimaryAttack) && cybercba::AbilitySystem::strike(m_session.player(), true, true) == cybercba::AbilityResult::Success) { cybercba::MissionSystem::advance(m_session.campaign(), cybercba::PrologueStage::Route, cybercba::PrologueStage::Convergence, "service_gate"); m_session.narrative().recoveredTruth++; m_message = "Dron derribado. Tras el humo: una identificacion del tren 41."; save(); }
        const auto* drone = cybercba::worldProp("exterior_drone");
        const float droneDx = m_playerPosition.x - drone->pivotX;
        const float droneDy = m_playerPosition.y - drone->pivotY;
        if (m_session.campaign().stage == cybercba::PrologueStage::Route &&
            m_session.campaign().selectedCharacter == cybercba::CharacterId::Magga &&
            std::sqrt(droneDx * droneDx + droneDy * droneDy) < 92.0F)
        {
            if (cybercba::AbilitySystem::applyDamage(m_session.player(), 20.0F))
            {
                m_message = "Impacto del dron. Busca cobertura o responde con Impacto tactico.";
                m_toastSeconds = 1.5F;
            }
        }
        if (m_session.player().health <= 0.0F)
            setScreen(Screen::Defeat);
        return;
    }
    if (m_screen == Screen::Pause) { updatePause(); return; }
    if (m_screen == Screen::Settings) { updateSettings(); return; }
    if (m_screen == Screen::Defeat) { if (m_input.wasPressed(InputAction::Confirm)) resetCheckpoint(); else if (m_input.wasPressed(InputAction::NavigateDown)) setScreen(Screen::Settings); else if (m_input.wasPressed(InputAction::Cancel)) setScreen(Screen::Menu); return; }
    if (m_input.wasPressed(InputAction::Confirm) || m_input.wasPressed(InputAction::Cancel)) { if (m_screen == Screen::End && m_input.wasPressed(InputAction::Confirm)) setScreen(Screen::Chapters); else setScreen(Screen::Menu); }
}
void GameApp::draw()
{
    BeginTextureMode(m_virtual); drawBackground();
    if (m_screen == Screen::Splash) { drawText("EXODUS SYSTEMS", 415, 260, 48, NlmColors::CYAN); drawText("AN EXODUS SYSTEMS PRODUCTION", 430, 330, 19, DIM); drawText("LA ULTIMA TRANSMISION", 410, 370, 23, NlmColors::AMBER); }
    else if (m_screen == Screen::Menu) drawMenu(); else if (m_screen == Screen::CharacterSelection) drawSelection(); else if (m_screen == Screen::Intro) drawIntro(); else if (m_screen == Screen::World) drawWorld(); else if (m_screen == Screen::Chapters) drawChapters(); else if (m_screen == Screen::Settings) drawSettings(); else if (m_screen == Screen::Credits) drawCredits(); else if (m_screen == Screen::Pause) drawPause(); else if (m_screen == Screen::Objectives) drawObjectives(); else if (m_screen == Screen::End) drawEnd(); else if (m_screen == Screen::Defeat) drawOverlay("SENAL PERDIDA", "La transmision se pierde en la lluvia.", "ENTER: reintentar checkpoint   Flecha abajo: configuracion   ESC: menu");
    drawModal(); if (m_overlay && m_overlayVisible) m_overlay->draw(); EndTextureMode();
    const float scale = std::min(static_cast<float>(GetScreenWidth()) / W, static_cast<float>(GetScreenHeight()) / H); const float ox = (GetScreenWidth() - W * scale) / 2, oy = (GetScreenHeight() - H * scale) / 2;
    BeginDrawing(); ClearBackground(BLACK); DrawTexturePro(m_virtual.texture, {0, 0, static_cast<float>(m_virtual.texture.width), -static_cast<float>(m_virtual.texture.height)}, {ox, oy, W * scale, H * scale}, {0, 0}, 0, WHITE); EndDrawing();
}
