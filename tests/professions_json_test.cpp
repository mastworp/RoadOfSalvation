#include "RoadOfSalvation/json/professions_json.hpp"

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <nlohmann/json.hpp>
#include <stdexcept>
#include <string_view>
#include <utility>

#include "RoadOfSalvation/character/professions.hpp"
#include "test_support.hpp"

static auto professions_document() -> nlohmann::json {
  return nlohmann::json::parse(R"({
    "professions": [
      {"id": "alchemist", "stats": {"hp": 100}},
      {"id": "scout", "stats": {"hp": 200}}
    ]
  })");
}

TEST(ProfessionsJsonTest, FindsTheRequestedProfession) {
  const auto document = professions_document();
  EXPECT_EQ(find_profession(document, professions::scout).at("stats").at("hp"),
            200);
}

TEST(ProfessionsJsonTest, ReturnsTheEntryInsideTheDocument) {
  const auto document = professions_document();
  EXPECT_EQ(&find_profession(document, professions::alchemist),
            &document.at("professions").at(0));
}

TEST(ProfessionsJsonTest, ThrowsWhenTheProfessionIsMissing) {
  const auto document = professions_document();
  EXPECT_THAT([&] { find_profession(document, professions::crusader); },
              testing::ThrowsMessage<std::runtime_error>(
                  testing::StrEq("Profession 'crusader' not found")));
}

TEST(ProfessionsJsonTest, ThrowsWhenTheProfessionListIsMissing) {
  const auto document = nlohmann::json::parse(R"({"classes": []})");
  EXPECT_THROW(find_profession(document, professions::scout),
               nlohmann::json::out_of_range);
}

TEST(ProfessionsJsonTest, ThrowsWhenAnEntryHasNoId) {
  const auto document =
      nlohmann::json::parse(R"({"professions": [{"stats": {"hp": 1}}]})");
  EXPECT_THROW(find_profession(document, professions::scout),
               nlohmann::json::out_of_range);
}

class ProfessionNameTest
    : public testing::TestWithParam<std::pair<professions, std::string_view>> {
};

TEST_P(ProfessionNameTest, SerializesToItsName) {
  const auto& [id, name] = GetParam();
  EXPECT_EQ(nlohmann::json(id), name);
}

TEST_P(ProfessionNameTest, DeserializesFromItsName) {
  const auto& [id, name] = GetParam();
  EXPECT_EQ(nlohmann::json(name).get<professions>(), id);
}

TEST_P(ProfessionNameTest, IsFoundByItsId) {
  const auto& [id, name] = GetParam();
  nlohmann::json entry;
  entry["id"] = name;
  nlohmann::json document;
  document["professions"].push_back(entry);
  EXPECT_EQ(find_profession(document, id).at("id"), name);
}

INSTANTIATE_TEST_SUITE_P(AllProfessions, ProfessionNameTest,
                         testing::ValuesIn(profession_names),
                         enum_param_name{});
