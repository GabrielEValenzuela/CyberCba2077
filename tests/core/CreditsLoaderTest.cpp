#include "cybercba/core/CreditsLoader.hpp"

#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

namespace cybercba::core
{
namespace
{

namespace fs = std::filesystem;

class CreditsLoaderTest : public ::testing::Test
{
  protected:
    fs::path m_dir;

    void SetUp() override
    {
        m_dir = fs::temp_directory_path() / fs::path("cybercba_credits_test");
        fs::remove_all(m_dir);
        fs::create_directories(m_dir);
    }

    void TearDown() override
    {
        fs::remove_all(m_dir);
    }

    void write(const std::string& name, const std::string& content)
    {
        std::ofstream file(m_dir / name, std::ios::binary);
        file << content;
    }
};

TEST_F(CreditsLoaderTest, MissingDirectoryReturnsEmptyString)
{
    EXPECT_EQ(CreditsLoader::loadEntries((m_dir / "does_not_exist").string()), "");
}

TEST_F(CreditsLoaderTest, ReadsEntriesSortedByFilename)
{
    write("b_second.txt", "Segunda entrada");
    write("a_first.txt", "Primera entrada");
    const std::string result = CreditsLoader::loadEntries(m_dir.string());
    EXPECT_EQ(result, "Primera entrada\n\nSegunda entrada");
}

TEST_F(CreditsLoaderTest, SkipsReadmeAndUnderscorePrefixedTemplates)
{
    write("README.md", "no debería aparecer");
    write("_ejemplo.txt", "tampoco debería aparecer");
    write("real.txt", "Entrada real");
    EXPECT_EQ(CreditsLoader::loadEntries(m_dir.string()), "Entrada real");
}

TEST_F(CreditsLoaderTest, TrimsSurroundingWhitespacePerFile)
{
    write("padded.txt", "\n\n  Con espacios  \n\n");
    EXPECT_EQ(CreditsLoader::loadEntries(m_dir.string()), "Con espacios");
}

TEST_F(CreditsLoaderTest, SkipsEmptyFilesWithoutLeavingBlankEntries)
{
    write("a_empty.txt", "   \n  ");
    write("b_real.txt", "Contenido real");
    EXPECT_EQ(CreditsLoader::loadEntries(m_dir.string()), "Contenido real");
}

TEST_F(CreditsLoaderTest, HandlesManyEntriesWithoutTruncation)
{
    for (int i = 0; i < 40; ++i)
    {
        write("entry_" + std::to_string(i) + ".txt", "Entrada " + std::to_string(i));
    }
    const std::string result = CreditsLoader::loadEntries(m_dir.string());
    std::size_t blockCount   = 1;
    for (std::size_t pos = result.find("\n\n"); pos != std::string::npos; pos = result.find("\n\n", pos + 1))
    {
        ++blockCount;
    }
    EXPECT_EQ(blockCount, 40u);
}

} // namespace
} // namespace cybercba::core
