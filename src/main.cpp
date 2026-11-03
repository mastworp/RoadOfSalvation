#include <memory>
#include <ostream>
#include <ranges>

#include "RoadOfSalvation/character/profession.hpp"
#include "RoadOfSalvation/character/professions.hpp"
#include "RoadOfSalvation/json/json_parser.hpp"
#include "app/inc/RoadOfSalvation/character/player.hpp"

int main() {
  const auto hero = std::make_unique<player>(
      "hero", std::make_unique<statistics>(),
      std::make_unique<profession>(professions::none,
                                   std::make_unique<json_parser>(
                                       R"(professions.json)")));

  std::println("The heroes name is: {}", hero->name());
  for (const auto& stat : hero->get_statistics() | std::views::values) {
    std::println("{}", stat);
  }
  return 0;
}