#include "RoadOfSalvation/json/attributes_json.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <cstddef>
#include <map>
#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "RoadOfSalvation/character/attributes.hpp"
#include "test_support.hpp"

using attribute_map = std::map<attributes, std::size_t>;

TEST(AttributesJsonTest, ConvertsEveryKnownAttribute) {
  const auto stats = nlohmann::json::parse(R"({
    "level": 1, "hp": 2, "mp": 3, "vigor": 4, "mana": 5, "attack": 6,
    "defense": 7, "special_attack": 8, "special_defense": 9,
    "accuracy": 10, "luck": 11
  })");

  const attribute_map expected{{attributes::level, 1},
                               {attributes::hp, 2},
                               {attributes::mp, 3},
                               {attributes::vigor, 4},
                               {attributes::mana, 5},
                               {attributes::attack, 6},
                               {attributes::defense, 7},
                               {attributes::special_attack, 8},
                               {attributes::special_defense, 9},
                               {attributes::accuracy, 10},
                               {attributes::luck, 11}};
  EXPECT_EQ(attributes_from_json(stats), expected);
}

TEST(AttributesJsonTest, ConvertsAPartialSetOfAttributes) {
  const auto stats = nlohmann::json::parse(R"({"hp": 40, "luck": 7})");
  EXPECT_EQ(attributes_from_json(stats),
            (attribute_map{{attributes::hp, 40}, {attributes::luck, 7}}));
}

TEST(AttributesJsonTest, ConvertsAnEmptyObjectToNoAttributes) {
  EXPECT_TRUE(attributes_from_json(nlohmann::json::object()).empty());
}

TEST(AttributesJsonTest, RejectsUnknownAttributes) {
  const auto stats = nlohmann::json::parse(R"({"hp": 10, "strength": 5})");
  EXPECT_THAT([&] { attributes_from_json(stats); },
              testing::ThrowsMessage<std::runtime_error>(
                  testing::StrEq("Unknown attribute in data file: strength")));
}

TEST(AttributesJsonTest, RejectsNonNumericValues) {
  const auto stats = nlohmann::json::parse(R"({"hp": "plenty"})");
  EXPECT_THROW(attributes_from_json(stats), nlohmann::json::type_error);
}

class AttributeNameTest
    : public testing::TestWithParam<std::pair<attributes, std::string_view>> {};

TEST_P(AttributeNameTest, SerializesToItsName) {
  const auto& [attribute, name] = GetParam();
  EXPECT_EQ(nlohmann::json(attribute), name);
}

TEST_P(AttributeNameTest, DeserializesFromItsName) {
  const auto& [attribute, name] = GetParam();
  EXPECT_EQ(nlohmann::json(name).get<attributes>(), attribute);
}

TEST_P(AttributeNameTest, IsAcceptedAsADataFileKey) {
  const auto& [attribute, name] = GetParam();
  nlohmann::json stats;
  stats[name] = 7;
  EXPECT_EQ(attributes_from_json(stats), (attribute_map{{attribute, 7}}));
}

INSTANTIATE_TEST_SUITE_P(AllAttributes, AttributeNameTest,
                         testing::ValuesIn(attribute_names), enum_param_name{});
