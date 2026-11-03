#include "RoadOfSalvation/json/professions_json.hpp"

#include <algorithm>
#include <stdexcept>
#include <string>

auto find_profession(const nlohmann::json& doc,
                     professions id) -> const nlohmann::json& {
  const auto& list = doc.at("professions");
  const nlohmann::json wanted = id;

  const auto it = std::ranges::find_if(
      list, [&wanted](const nlohmann::json& entry) {
        return entry.at("id") == wanted;
      });
  if (it == list.end()) {
    throw std::runtime_error(
        "Profession '" + wanted.get<std::string>() + "' not found");
  }
  return *it;
}