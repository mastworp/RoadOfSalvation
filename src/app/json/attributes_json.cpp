#include "RoadOfSalvation/json/attributes_json.hpp"

#include <stdexcept>
#include <string>

auto attributes_from_json(
    const nlohmann::json& stats) -> std::map<attributes, std::size_t> {
  std::map<attributes, std::size_t> result;
  for (const auto& [key, value] : stats.items()) {
    const nlohmann::json json_key = key;
    const auto attr = json_key.get<attributes>();
    if (nlohmann::json(attr) != json_key) {
      throw std::runtime_error("Unknown attribute in data file: " + key);
    }
    result[attr] = value.get<std::size_t>();
  }
  return result;
}