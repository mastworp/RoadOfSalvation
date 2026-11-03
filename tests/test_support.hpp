#pragma once

#include <gtest/gtest.h>

#include <array>
#include <filesystem>
#include <fstream>
#include <nlohmann/json.hpp>
#include <ostream>
#include <random>
#include <string>
#include <string_view>
#include <system_error>
#include <utility>

#include "RoadOfSalvation/character/attributes.hpp"
#include "RoadOfSalvation/character/professions.hpp"
#include "RoadOfSalvation/json/attributes_json.hpp"
#include "RoadOfSalvation/json/professions_json.hpp"

inline auto PrintTo(const attributes value, std::ostream* os) -> void {
  *os << nlohmann::json(value).get<std::string>();
}

inline auto PrintTo(const professions value, std::ostream* os) -> void {
  *os << nlohmann::json(value).get<std::string>();
}

inline constexpr std::array<std::pair<attributes, std::string_view>, 11>
    attribute_names{{{attributes::level, "level"},
                     {attributes::hp, "hp"},
                     {attributes::mp, "mp"},
                     {attributes::vigor, "vigor"},
                     {attributes::mana, "mana"},
                     {attributes::attack, "attack"},
                     {attributes::defense, "defense"},
                     {attributes::special_attack, "special_attack"},
                     {attributes::special_defense, "special_defense"},
                     {attributes::accuracy, "accuracy"},
                     {attributes::luck, "luck"}}};

inline constexpr std::array<std::pair<professions, std::string_view>, 4>
    profession_names{{{professions::crusader, "crusader"},
                      {professions::alchemist, "alchemist"},
                      {professions::scout, "scout"},
                      {professions::none, "none"}}};

struct enum_param_name {
  template <typename Enum>
  auto operator()(
      const testing::TestParamInfo<std::pair<Enum, std::string_view>>& info)
      const -> std::string {
    return std::string{info.param.second};
  }
};

class temporary_directory final {
 public:
  temporary_directory()
      : path_{std::filesystem::temp_directory_path() /
              ("RoadOfSalvation-" + std::to_string(std::random_device{}()))} {
    std::filesystem::create_directories(path_);
  }

  ~temporary_directory() {
    std::error_code ignored;
    std::filesystem::remove_all(path_, ignored);
  }

  temporary_directory(const temporary_directory&) = delete;
  temporary_directory(temporary_directory&&) = delete;
  auto operator=(const temporary_directory&) -> temporary_directory& = delete;
  auto operator=(temporary_directory&&) -> temporary_directory& = delete;

  [[nodiscard]] auto path() const -> const std::filesystem::path& {
    return path_;
  }

  auto write(const std::string_view name, const std::string_view content) const
      -> std::filesystem::path {
    const auto file = path_ / name;
    std::ofstream stream{file, std::ios::trunc};
    stream << content;
    return file;
  }

 private:
  std::filesystem::path path_;
};
