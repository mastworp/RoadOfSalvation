#include "RoadOfSalvation/json/json_parser.hpp"

#include <gtest/gtest.h>

#include <filesystem>
#include <fstream>
#include <ios>
#include <nlohmann/json.hpp>

#include "test_support.hpp"

TEST(JsonParserTest, HoldsNoDataBeforeLoading) {
  json_parser parser{"professions.json"};
  EXPECT_TRUE(parser.get_data().is_null());
}

TEST(JsonParserTest, LoadsAFileFromAnAbsolutePath) {
  const temporary_directory directory;
  json_parser parser{directory.write("answer.json", R"({"answer": 42})")};
  parser.load();
  EXPECT_EQ(parser.get_data(), nlohmann::json::parse(R"({"answer": 42})"));
}

TEST(JsonParserTest, ResolvesRelativePathsAgainstTheGameDataDirectory) {
  json_parser parser{"professions.json"};
  parser.load();
  EXPECT_TRUE(parser.get_data().contains("professions"));
}

TEST(JsonParserTest, ReloadingPicksUpChangesToTheFile) {
  const temporary_directory directory;
  json_parser parser{directory.write("state.json", R"({"version": 1})")};
  parser.load();
  directory.write("state.json", R"({"version": 2})");
  parser.load();
  EXPECT_EQ(parser.get_data().at("version"), 2);
}

TEST(JsonParserTest, LoadsReadOnlyFiles) {
  const temporary_directory directory;
  const auto file = directory.write("read_only.json", R"({"answer": 42})");
  std::filesystem::permissions(file, std::filesystem::perms::owner_read |
                                         std::filesystem::perms::group_read |
                                         std::filesystem::perms::others_read);
  if (std::ofstream{file, std::ios::app}) {
    GTEST_SKIP() << "File permissions are not enforced for this user.";
  }

  json_parser parser{file};
  ASSERT_NO_THROW(parser.load());
  EXPECT_EQ(parser.get_data().at("answer"), 42);
}

TEST(JsonParserTest, LoadingAMissingFileThrows) {
  const temporary_directory directory;
  json_parser parser{directory.path() / "missing.json"};
  EXPECT_THROW(parser.load(), nlohmann::json::parse_error);
}

TEST(JsonParserTest, LoadingMalformedJsonThrows) {
  const temporary_directory directory;
  json_parser parser{directory.write("malformed.json", R"({"professions": [)")};
  EXPECT_THROW(parser.load(), nlohmann::json::parse_error);
}
