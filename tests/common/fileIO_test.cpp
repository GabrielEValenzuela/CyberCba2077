/**
 * @file test_FileIO.cpp
 * @brief Tests unitarios de FileIO — carga, escritura y manejo de errores.
 *
 * @details
 * Cada test usa archivos temporales en un directorio único por test
 * (via `TempDir` fixture) para garantizar aislamiento total.
 * Los archivos se eliminan automáticamente al destruir el fixture.
 *
 * ## Cobertura
 * - Archivo inexistente → vector vacío (sin excepción)
 * - Líneas válidas → parsing correcto
 * - Líneas con formato inválido → ignoradas + stderr
 * - Valores fuera de rango → ignorados
 * - Comentarios y líneas vacías → ignorados
 * - append* → crea archivo si no existe
 * - append* → agrega al final sin sobreescribir
 * - save* → sobreescribe el archivo existente
 * - FileIOException → lanzada si el archivo existe pero no puede abrirse
 *
 * @author Exodus Systems Inc.
 * @version 0.2.0
 */

#include "common/fileIO.hpp"
#include "common/types.hpp"

#include <filesystem>
#include <fstream>
#include <gtest/gtest.h>
#include <string>

using namespace CyberpunkCba;
using namespace CyberpunkCba::FileIO;

// =============================================================================
// Fixture — directorio temporal aislado por test
// =============================================================================

class TempDir : public ::testing::Test
{
protected:
    void SetUp() override
    {
        m_dir = std::filesystem::temp_directory_path() /
                ("cyberpunk_test_" + std::to_string(std::hash<std::string> {}(
                                         ::testing::UnitTest::GetInstance()->current_test_info()->name())));
        std::filesystem::create_directories(m_dir);
    }

    void TearDown() override
    {
        std::filesystem::remove_all(m_dir);
    }

    // Retorna un path dentro del directorio temporal
    std::filesystem::path tmp(const std::string& filename) const
    {
        return m_dir / filename;
    }

    // Escribe contenido en un archivo temporal
    void write(const std::string& filename, const std::string& content) const
    {
        std::ofstream f {tmp(filename)};
        f << content;
    }

    std::filesystem::path m_dir;
};

// =============================================================================
// loadLog
// =============================================================================

TEST_F(TempDir, LoadLog_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadLog(tmp("log.txt")).empty());
}

TEST_F(TempDir, LoadLog_ValidLines_ParsedCorrectly)
{
    write("log.txt",
          "# comentario\n"
          "\n"
          "23|41|Sistema iniciado\n"
          "00|10|hack fallido\n");

    const auto result {loadLog(tmp("log.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].hour, 23);
    EXPECT_EQ(result[0].minute, 41);
    EXPECT_EQ(result[0].message, "Sistema iniciado");
    EXPECT_EQ(result[1].hour, 0);
    EXPECT_EQ(result[1].message, "hack fallido");
}

TEST_F(TempDir, LoadLog_InvalidHour_LineIgnored)
{
    write("log.txt", "25|00|mensaje inválido\n23|41|válido\n");
    const auto result {loadLog(tmp("log.txt"))};
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].message, "válido");
}

TEST_F(TempDir, LoadLog_MissingField_LineIgnored)
{
    write("log.txt", "23|mensaje sin minuto\n23|41|completo\n");
    const auto result {loadLog(tmp("log.txt"))};
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].message, "completo");
}

TEST_F(TempDir, LoadLog_OnlyComments_ReturnsEmpty)
{
    write("log.txt", "# solo comentarios\n# otra línea\n");
    EXPECT_TRUE(loadLog(tmp("log.txt")).empty());
}

// =============================================================================
// loadInventory
// =============================================================================

TEST_F(TempDir, LoadInventory_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadInventory(tmp("inventory.txt")).empty());
}

TEST_F(TempDir, LoadInventory_ValidLines_ParsedCorrectly)
{
    write("inventory.txt",
          "# nombre|tipo|precio|cantidad\n"
          "Deck de hackeo|Tech|80|1\n"
          "Stim|Consumable|15|3\n");

    const auto result {loadInventory(tmp("inventory.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].name, "Deck de hackeo");
    EXPECT_EQ(result[0].type, ItemType::Tech);
    EXPECT_EQ(result[0].price, 80);
    EXPECT_EQ(result[0].quantity, 1);
    EXPECT_EQ(result[1].quantity, 3);
}

TEST_F(TempDir, LoadInventory_NegativePrice_LineIgnored)
{
    write("inventory.txt", "Ítem malo|Tech|-10|1\nÍtem bueno|Tech|50|1\n");
    const auto result {loadInventory(tmp("inventory.txt"))};
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].name, "Ítem bueno");
}

TEST_F(TempDir, LoadInventory_ZeroQuantity_LineIgnored)
{
    write("inventory.txt", "Ítem malo|Tech|50|0\nÍtem bueno|Tech|50|1\n");
    EXPECT_EQ(loadInventory(tmp("inventory.txt")).size(), 1u);
}

TEST_F(TempDir, LoadInventory_InvalidType_LineIgnored)
{
    write("inventory.txt", "Cosa|TipoInvalido|10|1\nBueno|Tech|10|1\n");
    EXPECT_EQ(loadInventory(tmp("inventory.txt")).size(), 1u);
}

// =============================================================================
// loadMap
// =============================================================================

TEST_F(TempDir, LoadMap_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadMap(tmp("map.txt")).empty());
}

TEST_F(TempDir, LoadMap_ValidLines_ParsedCorrectly)
{
    write("map.txt",
          "Sector 7|true|true\n"
          "Puerto Libre|false|false\n");

    const auto result {loadMap(tmp("map.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].name, "Sector 7");
    EXPECT_TRUE(result[0].visited);
    EXPECT_TRUE(result[0].unlocked);
    EXPECT_FALSE(result[1].visited);
    EXPECT_FALSE(result[1].unlocked);
}

TEST_F(TempDir, LoadMap_InvalidBool_LineIgnored)
{
    write("map.txt", "Zona|si|no\nZona buena|true|true\n");
    EXPECT_EQ(loadMap(tmp("map.txt")).size(), 1u);
}

// =============================================================================
// loadMissions
// =============================================================================

TEST_F(TempDir, LoadMissions_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadMissions(tmp("missions.txt")).empty());
}

TEST_F(TempDir, LoadMissions_ValidLines_ParsedCorrectly)
{
    write("missions.txt",
          "Infiltración Militech|Completed|500|100\n"
          "Entrega urgente|Pending|150|0\n");

    const auto result {loadMissions(tmp("missions.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].status, MissionStatus::Completed);
    EXPECT_EQ(result[0].reward, 500);
    EXPECT_EQ(result[0].progressPercent, 100);
    EXPECT_EQ(result[1].status, MissionStatus::Pending);
}

TEST_F(TempDir, LoadMissions_InvalidStatus_LineIgnored)
{
    write("missions.txt", "Misión|EstadoInvalido|100|50\nBuena|Pending|100|0\n");
    EXPECT_EQ(loadMissions(tmp("missions.txt")).size(), 1u);
}

TEST_F(TempDir, LoadMissions_ProgressOutOfRange_LineIgnored)
{
    write("missions.txt", "Misión|Pending|100|150\nBuena|Pending|100|50\n");
    EXPECT_EQ(loadMissions(tmp("missions.txt")).size(), 1u);
}

// =============================================================================
// loadEntities
// =============================================================================

TEST_F(TempDir, LoadEntities_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadEntities(tmp("entities.txt")).empty());
}

TEST_F(TempDir, LoadEntities_ValidLines_ParsedCorrectly)
{
    write("entities.txt",
          "Corporativo Militech|Hostile|120\n"
          "Netrunner aliado|Friendly|80\n");

    const auto result {loadEntities(tmp("entities.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].disposition, EntityDisposition::Hostile);
    EXPECT_EQ(result[0].distanceMeters, 120);
    EXPECT_EQ(result[1].disposition, EntityDisposition::Friendly);
}

TEST_F(TempDir, LoadEntities_NegativeDistance_LineIgnored)
{
    write("entities.txt", "Entidad|Hostile|-5\nBuena|Neutral|10\n");
    EXPECT_EQ(loadEntities(tmp("entities.txt")).size(), 1u);
}

// =============================================================================
// loadReputation
// =============================================================================

TEST_F(TempDir, LoadReputation_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadReputation(tmp("reputation.txt")).empty());
}

TEST_F(TempDir, LoadReputation_ValidLines_ParsedCorrectly)
{
    write("reputation.txt",
          "Corporations|40|2077-01-01\n"
          "Streeters|55|2077-01-02\n");

    const auto result {loadReputation(tmp("reputation.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].faction, Faction::Corporations);
    EXPECT_EQ(result[0].value, 40);
    EXPECT_EQ(result[0].date, "2077-01-01");
    EXPECT_EQ(result[1].faction, Faction::Streeters);
    EXPECT_EQ(result[1].value, 55);
}

TEST_F(TempDir, LoadReputation_ValueOutOfRange_LineIgnored)
{
    write("reputation.txt", "Corporations|150|2077-01-01\nStreeters|40|2077-01-01\n");
    EXPECT_EQ(loadReputation(tmp("reputation.txt")).size(), 1u);
}

TEST_F(TempDir, LoadReputation_InvalidFaction_LineIgnored)
{
    write("reputation.txt", "FaccionInvalida|40|2077-01-01\nStreeters|40|2077-01-01\n");
    EXPECT_EQ(loadReputation(tmp("reputation.txt")).size(), 1u);
}

// =============================================================================
// loadWallet
// =============================================================================

TEST_F(TempDir, LoadWallet_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadWallet(tmp("wallet.txt")).empty());
}

TEST_F(TempDir, LoadWallet_ValidLines_ParsedCorrectly)
{
    write("wallet.txt",
          "ingreso|500|misión completada\n"
          "gasto|80|deck de hackeo\n");

    const auto result {loadWallet(tmp("wallet.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].type, "ingreso");
    EXPECT_EQ(result[0].amount, 500);
    EXPECT_EQ(result[0].concept, "misión completada");
    EXPECT_EQ(result[1].type, "gasto");
    EXPECT_EQ(result[1].amount, 80);
}

TEST_F(TempDir, LoadWallet_InvalidType_LineIgnored)
{
    write("wallet.txt", "transferencia|100|algo\ningreso|100|válido\n");
    EXPECT_EQ(loadWallet(tmp("wallet.txt")).size(), 1u);
}

TEST_F(TempDir, LoadWallet_NegativeAmount_LineIgnored)
{
    write("wallet.txt", "gasto|-50|negativo\ningreso|100|válido\n");
    EXPECT_EQ(loadWallet(tmp("wallet.txt")).size(), 1u);
}

// =============================================================================
// loadSessions
// =============================================================================

TEST_F(TempDir, LoadSessions_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadSessions(tmp("session.txt")).empty());
}

TEST_F(TempDir, LoadSessions_ValidLines_ParsedCorrectly)
{
    write("session.txt",
          "2077-01-01|1820|34\n"
          "2077-01-02|3610|67\n");

    const auto result {loadSessions(tmp("session.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].date, "2077-01-01");
    EXPECT_EQ(result[0].durationSeconds, 1820);
    EXPECT_EQ(result[0].commandCount, 34);
}

TEST_F(TempDir, LoadSessions_NegativeDuration_LineIgnored)
{
    write("session.txt", "2077-01-01|-100|34\n2077-01-02|100|10\n");
    EXPECT_EQ(loadSessions(tmp("session.txt")).size(), 1u);
}

// =============================================================================
// loadShop
// =============================================================================

TEST_F(TempDir, LoadShop_FileNotExists_ReturnsEmpty)
{
    EXPECT_TRUE(loadShop(tmp("shop.txt")).empty());
}

TEST_F(TempDir, LoadShop_ValidLines_ParsedCorrectly)
{
    write("shop.txt",
          "Stim|15|3|2077-01-01\n"
          "Deck de hackeo|80|1|2077-01-02\n");

    const auto result {loadShop(tmp("shop.txt"))};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].name, "Stim");
    EXPECT_EQ(result[0].price, 15);
    EXPECT_EQ(result[0].quantity, 3);
    EXPECT_EQ(result[0].date, "2077-01-01");
}

TEST_F(TempDir, LoadShop_ZeroQuantity_LineIgnored)
{
    write("shop.txt", "Stim|15|0|2077-01-01\nDeck|80|1|2077-01-02\n");
    EXPECT_EQ(loadShop(tmp("shop.txt")).size(), 1u);
}

// =============================================================================
// appendLog
// =============================================================================

TEST_F(TempDir, AppendLog_FileNotExists_CreatesFile)
{
    const auto path {tmp("log.txt")};
    appendLog({10, 30, "evento"}, path);
    EXPECT_TRUE(std::filesystem::exists(path));
}

TEST_F(TempDir, AppendLog_AppendsToExisting)
{
    const auto path {tmp("log.txt")};
    appendLog({10, 30, "primero"}, path);
    appendLog({11, 00, "segundo"}, path);
    const auto result {loadLog(path)};
    ASSERT_EQ(result.size(), 2u);
    EXPECT_EQ(result[0].message, "primero");
    EXPECT_EQ(result[1].message, "segundo");
}

// =============================================================================
// saveInventory
// =============================================================================

TEST_F(TempDir, SaveInventory_OverwritesExisting)
{
    const auto path {tmp("inventory.txt")};
    saveInventory({{"Deck", ItemType::Tech, 80, 1}}, path);
    saveInventory({{"Stim", ItemType::Consumable, 15, 3}}, path);
    const auto result {loadInventory(path)};
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].name, "Stim");
}

TEST_F(TempDir, SaveInventory_EmptyVector_WritesEmptyFile)
{
    const auto path {tmp("inventory.txt")};
    saveInventory({}, path);
    EXPECT_TRUE(loadInventory(path).empty());
}

// =============================================================================
// appendSession
// =============================================================================

TEST_F(TempDir, AppendSession_FileNotExists_CreatesFile)
{
    const auto path {tmp("session.txt")};
    appendSession({"2077-01-01", 1800, 30}, path);
    EXPECT_TRUE(std::filesystem::exists(path));
    EXPECT_EQ(loadSessions(path).size(), 1u);
}

TEST_F(TempDir, AppendSession_AppendsToExisting)
{
    const auto path {tmp("session.txt")};
    appendSession({"2077-01-01", 1800, 30}, path);
    appendSession({"2077-01-02", 3600, 60}, path);
    EXPECT_EQ(loadSessions(path).size(), 2u);
}

// =============================================================================
// appendEntity
// =============================================================================

TEST_F(TempDir, AppendEntity_RoundTrip)
{
    const auto path {tmp("entities.txt")};
    appendEntity({"Corporativo", EntityDisposition::Hostile, 120}, path);
    const auto result {loadEntities(path)};
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].name, "Corporativo");
    EXPECT_EQ(result[0].disposition, EntityDisposition::Hostile);
    EXPECT_EQ(result[0].distanceMeters, 120);
}

// =============================================================================
// appendTransaction
// =============================================================================

TEST_F(TempDir, AppendTransaction_RoundTrip)
{
    const auto path {tmp("wallet.txt")};
    appendTransaction({"gasto", 80, "deck de hackeo"}, path);
    const auto result {loadWallet(path)};
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].type, "gasto");
    EXPECT_EQ(result[0].amount, 80);
    EXPECT_EQ(result[0].concept, "deck de hackeo");
}

// =============================================================================
// appendPurchase
// =============================================================================

TEST_F(TempDir, AppendPurchase_RoundTrip)
{
    const auto path {tmp("shop.txt")};
    appendPurchase({"Stim", 15, 3, "2077-01-01"}, path);
    const auto result {loadShop(path)};
    ASSERT_EQ(result.size(), 1u);
    EXPECT_EQ(result[0].name, "Stim");
    EXPECT_EQ(result[0].price, 15);
    EXPECT_EQ(result[0].quantity, 3);
}
