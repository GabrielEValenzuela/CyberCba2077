#include "cybercba/game/Resources.hpp"

#include <gtest/gtest.h>

namespace cybercba::game
{
namespace
{

namespace fs = std::filesystem;

class ResourceTest : public ::testing::Test
{
  protected:
    fs::path m_dir;

    void SetUp() override {}

    void TearDown() override {}

    void write(const std::string& name, const std::string& content) {}
};

TEST_F(ResourceTest, ShoudAllowToTakeLessAmountOfAvaiableEMP)
{
    // dado un enginerules con disponibilidad para 5 emp
    // intento tomar 3
    // resultado esperado: true
}

TEST_F(ResourceTest, ReadsEntriesSortedByFilename) {}

TEST_F(ResourceTest, SkipsReadmeAndUnderscorePrefixedTemplates) {}

TEST_F(ResourceTest, TrimsSurroundingWhitespacePerFile) {}

TEST_F(ResourceTest, SkipsEmptyFilesWithoutLeavingBlankEntries) {}

TEST_F(ResourceTest, HandlesManyEntriesWithoutTruncation) {}

} // namespace
} // namespace cybercba::game
