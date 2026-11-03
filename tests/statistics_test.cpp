#include "RoadOfSalvation/character/statistics.hpp"

#include <gtest/gtest.h>

#include <cstddef>
#include <map>
#include <ranges>

#include "RoadOfSalvation/character/attributes.hpp"
#include "test_support.hpp"

using attribute_map = std::map<attributes, std::size_t>;

static auto sample_attributes() -> attribute_map {
  return {{attributes::level, 3},
          {attributes::hp, 120},
          {attributes::mp, 80},
          {attributes::vigor, 11},
          {attributes::mana, 12},
          {attributes::attack, 13},
          {attributes::defense, 14},
          {attributes::special_attack, 15},
          {attributes::special_defense, 16},
          {attributes::accuracy, 17},
          {attributes::luck, 18}};
}

TEST(StatisticsTest, StartsWithoutAttributes) {
  statistics stats;
  EXPECT_TRUE(stats.get_attributes().empty());
}

TEST(StatisticsTest, SetStoresTheGivenAttributes) {
  statistics stats;
  stats.set(sample_attributes());
  EXPECT_EQ(stats.get_attributes(), sample_attributes());
}

TEST(StatisticsTest, SetReplacesPreviousAttributes) {
  statistics stats;
  stats.set({{attributes::hp, 1}, {attributes::luck, 2}});
  stats.set({{attributes::mp, 3}});
  EXPECT_EQ(stats.get_attributes(), (attribute_map{{attributes::mp, 3}}));
}

TEST(StatisticsTest, GettersReturnTheMatchingAttribute) {
  statistics stats;
  stats.set(sample_attributes());
  EXPECT_EQ(stats.get_level(), 3U);
  EXPECT_EQ(stats.get_hp(), 120U);
  EXPECT_EQ(stats.get_mp(), 80U);
  EXPECT_EQ(stats.get_vigor(), 11U);
  EXPECT_EQ(stats.get_mana(), 12U);
  EXPECT_EQ(stats.get_attack(), 13U);
  EXPECT_EQ(stats.get_defense(), 14U);
  EXPECT_EQ(stats.get_special_attack(), 15U);
  EXPECT_EQ(stats.get_special_defense(), 16U);
  EXPECT_EQ(stats.get_accuracy(), 17U);
  EXPECT_EQ(stats.get_luck(), 18U);
}

TEST(StatisticsTest, GettersReturnZeroForMissingAttributes) {
  statistics stats;
  EXPECT_EQ(stats.get_level(), 0U);
  EXPECT_EQ(stats.get_hp(), 0U);
  EXPECT_EQ(stats.get_mp(), 0U);
  EXPECT_EQ(stats.get_vigor(), 0U);
  EXPECT_EQ(stats.get_mana(), 0U);
  EXPECT_EQ(stats.get_attack(), 0U);
  EXPECT_EQ(stats.get_defense(), 0U);
  EXPECT_EQ(stats.get_special_attack(), 0U);
  EXPECT_EQ(stats.get_special_defense(), 0U);
  EXPECT_EQ(stats.get_accuracy(), 0U);
  EXPECT_EQ(stats.get_luck(), 0U);
}

TEST(StatisticsTest, LevelUpIncrementsOnlyTheLevel) {
  statistics stats;
  stats.set(sample_attributes());
  stats.level_up();

  auto expected = sample_attributes();
  expected[attributes::level] = 4;
  EXPECT_EQ(stats.get_attributes(), expected);
}

TEST(StatisticsTest, LevelUpStartsFromZeroWithoutALevel) {
  statistics stats;
  stats.level_up();
  EXPECT_EQ(stats.get_attributes(), (attribute_map{{attributes::level, 1}}));
}

TEST(StatisticsTest, IncreaseAttributesIncrementsEveryAttribute) {
  statistics stats;
  stats.set(sample_attributes());
  stats.increase_attributes();

  auto expected = sample_attributes();
  for (auto& value : expected | std::views::values) {
    ++value;
  }
  EXPECT_EQ(stats.get_attributes(), expected);
}

TEST(StatisticsTest, IncreaseAttributesDoesNotAddMissingAttributes) {
  statistics stats;
  stats.set({{attributes::hp, 9}});
  stats.increase_attributes();
  EXPECT_EQ(stats.get_attributes(), (attribute_map{{attributes::hp, 10}}));
}

TEST(StatisticsTest, IncreaseAttributesOnEmptyStatisticsIsANoOp) {
  statistics stats;
  stats.increase_attributes();
  EXPECT_TRUE(stats.get_attributes().empty());
}
