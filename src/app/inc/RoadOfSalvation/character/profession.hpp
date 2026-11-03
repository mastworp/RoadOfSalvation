#pragma once

#include <map>

#include "RoadOfSalvation/json/json_parser.hpp"

enum class attributes : std::size_t;
enum class professions : std::size_t;

class profession final {
 public:
  profession(professions name, std::unique_ptr<json_parser> parser);
  ~profession() = default;
  auto name() const -> professions;
  auto get_base_stats() -> std::map<attributes, std::size_t>;

 private:
  auto load() -> void;
  std::map<attributes, std::size_t> attributes_;
  professions name_;
  std::unique_ptr<json_parser> parser_;
};