#include "RoadOfSalvation/character/profession.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <map>
#include <memory>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string_view>

#include "RoadOfSalvation/character/attributes.hpp"
#include "RoadOfSalvation/character/professions.hpp"
#include "RoadOfSalvation/json/json_parser.hpp"
#include "test_support.hpp"

using attribute_map = std::map<attributes, std::size_t>;

constexpr std::string_view professions_json = R"({
  "professions": [
    {"id": "scout", "stats": {"level": 1, "hp": 200, "luck": 17}},
    {"id": "alchemist", "stats": {"level": 2, "mp": 500}}
  ]
})";

class ProfessionTest : public testing::Test {
 protected:
  [[nodiscard]] auto parser_for(const std::string_view content) const
      -> std::unique_ptr<json_parser> {
    return std::make_unique<json_parser>(
        directory_.write("professions.json", content));
  }

  [[nodiscard]] auto missing_file_parser() const
      -> std::unique_ptr<json_parser> {
    return std::make_unique<json_parser>(directory_.path() / "missing.json");
  }

 private:
  temporary_directory directory_;
};

TEST_F(ProfessionTest, ReportsItsName) {
  const profession scout{professions::scout, parser_for(professions_json)};
  EXPECT_EQ(scout.name(), professions::scout);
}

TEST_F(ProfessionTest, LoadsItsBaseStatsOnConstruction) {
  profession scout{professions::scout, parser_for(professions_json)};
  EXPECT_EQ(scout.get_base_stats(), (attribute_map{{attributes::level, 1},
                                                   {attributes::hp, 200},
                                                   {attributes::luck, 17}}));
}

TEST_F(ProfessionTest, LoadsOnlyItsOwnStats) {
  profession alchemist{professions::alchemist, parser_for(professions_json)};
  EXPECT_EQ(alchemist.get_base_stats(),
            (attribute_map{{attributes::level, 2}, {attributes::mp, 500}}));
}

TEST_F(ProfessionTest, ThrowsWhenTheProfessionIsNotDefined) {
  const auto load_crusader = [&] {
    profession{professions::crusader, parser_for(professions_json)};
  };
  EXPECT_THAT(load_crusader,
              testing::ThrowsMessage<std::runtime_error>(
                  testing::StrEq("Profession 'crusader' not found")));
}

TEST_F(ProfessionTest, ThrowsWhenStatsAreMissing) {
  EXPECT_THROW(
      (profession{professions::scout,
                  parser_for(R"({"professions": [{"id": "scout"}]})")}),
      nlohmann::json::out_of_range);
}

TEST_F(ProfessionTest, ThrowsWhenAStatIsUnknown) {
  const auto load_scout = [&] {
    profession{
        professions::scout,
        parser_for(
            R"({"professions": [{"id": "scout", "stats": {"strength": 3}}]})")};
  };
  EXPECT_THAT(load_scout,
              testing::ThrowsMessage<std::runtime_error>(
                  testing::StrEq("Unknown attribute in data file: strength")));
}

TEST_F(ProfessionTest, ThrowsWhenTheDataFileIsMissing) {
  EXPECT_THROW((profession{professions::scout, missing_file_parser()}),
               nlohmann::json::parse_error);
}
