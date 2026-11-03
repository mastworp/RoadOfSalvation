#pragma once

#include <nlohmann/json.hpp>

class json_parser {
 public:
  json_parser(const std::filesystem::path& file);
  ~json_parser() = default;

  auto load() -> void;
  auto get_data() -> const nlohmann::json&;

 private:
  std::filesystem::path file_;
  nlohmann::json data_;
};