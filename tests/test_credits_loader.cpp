#include "cybercba/CreditsLoader.hpp"

#include <filesystem>
#include <fstream>

#include <gtest/gtest.h>

namespace
{
namespace fs = std::filesystem;

class CreditsLoaderTest : public ::testing::Test
{
protected:
    fs::path dir;

    void SetUp() override
    {
        dir = fs::temp_directory_path() / fs::path("cybercba_credits_test");
        fs::remove_all(dir);
        fs::create_directories(dir);
    }

    void TearDown() override { fs::remove_all(dir); }

    void write(const std::string& name, const std::string& content)
    {
        std::ofstream file(dir / name, std::ios::binary);
        file << content;
    }
};

TEST_F(CreditsLoaderTest, MissingDirectoryReturnsEmptyString)
{
    EXPECT_EQ(cybercba::CreditsLoader::loadEntries((dir / "does_not_exist").string()), "");
}

TEST_F(CreditsLoaderTest, ReadsEntriesSortedByFilename)
{
    write("b_second.txt", "Segunda entrada");
    write("a_first.txt", "Primera entrada");
    const std::string result = cybercba::CreditsLoader::loadEntries(dir.string());
    EXPECT_EQ(result, "Primera entrada\n\nSegunda entrada");
}

TEST_F(CreditsLoaderTest, SkipsReadmeAndUnderscorePrefixedTemplates)
{
    write("README.md", "no debería aparecer");
    write("_ejemplo.txt", "tampoco debería aparecer");
    write("real.txt", "Entrada real");
    EXPECT_EQ(cybercba::CreditsLoader::loadEntries(dir.string()), "Entrada real");
}

TEST_F(CreditsLoaderTest, TrimsSurroundingWhitespacePerFile)
{
    write("padded.txt", "\n\n  Con espacios  \n\n");
    EXPECT_EQ(cybercba::CreditsLoader::loadEntries(dir.string()), "Con espacios");
}

TEST_F(CreditsLoaderTest, SkipsEmptyFilesWithoutLeavingBlankEntries)
{
    write("a_empty.txt", "   \n  ");
    write("b_real.txt", "Contenido real");
    EXPECT_EQ(cybercba::CreditsLoader::loadEntries(dir.string()), "Contenido real");
}
} // namespace
