#include <gtest/gtest.h>

#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <utility>

#include "RoadOfSalvation/character/attributes.hpp"
#include "RoadOfSalvation/character/profession.hpp"
#include "RoadOfSalvation/character/professions.hpp"
#include "RoadOfSalvation/json/json_parser.hpp"
#include "test_support.hpp"

class ShippedProfessionTest
    : public testing::TestWithParam<std::pair<professions, std::string_view>> {
 protected:
  [[nodiscard]] static auto load(const professions id) -> profession {
    return profession{id, std::make_unique<json_parser>("professions.json")};
  }
};

TEST_P(ShippedProfessionTest, DefinesEveryAttribute) {
  auto shipped = load(GetParam().first);
  const auto stats = shipped.get_base_stats();
  for (const auto& [attribute, name] : attribute_names) {
    EXPECT_TRUE(stats.contains(attribute)) << "Missing attribute: " << name;
  }
}

TEST_P(ShippedProfessionTest, StartsAtLevelOne) {
  auto shipped = load(GetParam().first);
  EXPECT_EQ(shipped.get_base_stats().at(attributes::level), 1U);
}

TEST_P(ShippedProfessionTest, StartsWithHitPoints) {
  auto shipped = load(GetParam().first);
  EXPECT_GT(shipped.get_base_stats().at(attributes::hp), 0U);
}

INSTANTIATE_TEST_SUITE_P(AllProfessions, ShippedProfessionTest,
                         testing::ValuesIn(profession_names),
                         enum_param_name{});

TEST(ShippedDataTest, DefinesEachProfessionOnlyOnce) {
  json_parser parser{"professions.json"};
  parser.load();

  std::set<std::string> ids;
  for (const auto& entry : parser.get_data().at("professions")) {
    const auto id = entry.at("id").get<std::string>();
    EXPECT_TRUE(ids.insert(id).second) << "Duplicate profession: " << id;
  }
  EXPECT_EQ(ids.size(), profession_names.size());
}
