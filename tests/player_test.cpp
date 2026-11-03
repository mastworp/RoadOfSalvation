#include "RoadOfSalvation/character/player.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <filesystem>
#include <map>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

#include "RoadOfSalvation/character/attributes.hpp"
#include "RoadOfSalvation/character/character.hpp"
#include "RoadOfSalvation/character/profession.hpp"
#include "RoadOfSalvation/character/professions.hpp"
#include "RoadOfSalvation/character/statistics.hpp"
#include "RoadOfSalvation/json/json_parser.hpp"
#include "test_support.hpp"

using attribute_map = std::map<attributes, std::size_t>;

class PlayerTest : public testing::Test {
 protected:
  [[nodiscard]] auto make_profession(const professions id) const
      -> std::unique_ptr<profession> {
    return std::make_unique<profession>(
        id, std::make_unique<json_parser>(data_file_));
  }

  [[nodiscard]] auto make_player(const std::string_view name,
                                 const professions id) const
      -> std::unique_ptr<player> {
    return std::make_unique<player>(name, std::make_unique<statistics>(),
                                    make_profession(id));
  }

 private:
  temporary_directory directory_;
  std::filesystem::path data_file_{directory_.write("professions.json", R"({
    "professions": [
      {"id": "crusader", "stats": {"level": 1, "hp": 300, "defense": 12}},
      {"id": "scout", "stats": {"level": 1, "hp": 200, "luck": 17}}
    ]
  })")};
};

TEST_F(PlayerTest, ReportsItsName) {
  EXPECT_EQ(make_player("hero", professions::scout)->name(), "hero");
}

TEST_F(PlayerTest, KeepsItsOwnCopyOfTheName) {
  std::string name{"wanderer"};
  const auto wanderer = make_player(name, professions::scout);
  name = "someone else";
  EXPECT_EQ(wanderer->name(), "wanderer");
}

TEST_F(PlayerTest, ReportsItsProfession) {
  EXPECT_EQ(make_player("hero", professions::crusader)->get_profession(),
            professions::crusader);
}

TEST_F(PlayerTest, StartsWithTheBaseStatsOfItsProfession) {
  EXPECT_EQ(make_player("hero", professions::crusader)->get_statistics(),
            (attribute_map{{attributes::level, 1},
                           {attributes::hp, 300},
                           {attributes::defense, 12}}));
}

TEST_F(PlayerTest, ReplacesExistingStatisticsWithTheBaseStats) {
  auto stats = std::make_unique<statistics>();
  stats->set({{attributes::level, 9}, {attributes::mana, 99}});

  const player hero{"hero", std::move(stats),
                    make_profession(professions::scout)};
  EXPECT_EQ(hero.get_statistics(), (attribute_map{{attributes::level, 1},
                                                  {attributes::hp, 200},
                                                  {attributes::luck, 17}}));
}

TEST_F(PlayerTest, CanBeUsedThroughTheCharacterInterface) {
  const std::unique_ptr<character> hero =
      make_player("hero", professions::scout);
  EXPECT_EQ(hero->name(), "hero");
}
