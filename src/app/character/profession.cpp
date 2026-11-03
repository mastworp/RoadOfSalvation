#include "RoadOfSalvation/character/profession.hpp"
#include "RoadOfSalvation/character/professions.hpp"
#include "RoadOfSalvation/json/attributes_json.hpp"
#include "RoadOfSalvation/json/professions_json.hpp"

#include <fstream>

profession::profession(const professions name,
                       std::unique_ptr<json_parser> parser)
  : name_(name), parser_(std::move(parser)) {
  load();
}

auto profession::name() const -> professions {
  return name_;
}

auto profession::get_base_stats() -> std::map<attributes, std::size_t> {
  return attributes_;
}

auto profession::load() -> void {
  parser_->load();
  const auto& entry{find_profession(parser_->get_data(), name_)};
  attributes_ = attributes_from_json(entry.at("stats"));
}