#pragma once

#include <map>
#include <cstddef>

enum class attributes : std::size_t;

class statistics final {
public:
  statistics() = default;
  ~statistics() = default;
  auto set(const std::map<attributes, std::size_t>& stats) -> void;
  auto get_attributes() -> std::map<attributes, std::size_t>;
  auto get_level() -> std::size_t;
  auto get_hp() -> std::size_t;
  auto get_mp() -> std::size_t;
  auto get_vigor() -> std::size_t;
  auto get_mana() -> std::size_t;
  auto get_attack() -> std::size_t;
  auto get_defense() -> std::size_t;
  auto get_special_attack() -> std::size_t;
  auto get_special_defense() -> std::size_t;
  auto get_accuracy() -> std::size_t;
  auto get_luck() -> std::size_t;

  auto level_up() -> void;
  auto increase_attributes() -> void;

private:
  std::map<attributes, std::size_t> attributes_;
};