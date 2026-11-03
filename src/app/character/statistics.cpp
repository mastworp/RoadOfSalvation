#include "RoadOfSalvation/character/statistics.hpp"

#include <algorithm>
#include <ranges>

#include "RoadOfSalvation/character/attributes.hpp"

auto statistics::set(const std::map<attributes, std::size_t>& stats) -> void {
  attributes_ = stats;
}

auto statistics::get_attributes() -> std::map<attributes, std::size_t> {
  return attributes_;
}

auto statistics::get_level() -> std::size_t {
  return attributes_[attributes::level];
}

auto statistics::get_hp() -> std::size_t { return attributes_[attributes::hp]; }

auto statistics::get_mp() -> std::size_t { return attributes_[attributes::mp]; }

auto statistics::get_vigor() -> std::size_t {
  return attributes_[attributes::vigor];
}

auto statistics::get_mana() -> std::size_t {
  return attributes_[attributes::mana];
}

auto statistics::get_attack() -> std::size_t {
  return attributes_[attributes::attack];
}

auto statistics::get_defense() -> std::size_t {
  return attributes_[attributes::defense];
}

auto statistics::get_special_attack() -> std::size_t {
  return attributes_[attributes::special_attack];
}

auto statistics::get_special_defense() -> std::size_t {
  return attributes_[attributes::special_defense];
}

auto statistics::get_accuracy() -> std::size_t {
  return attributes_[attributes::accuracy];
}

auto statistics::get_luck() -> std::size_t {
  return attributes_[attributes::luck];
}

auto statistics::level_up() -> void { ++attributes_[attributes::level]; }

auto statistics::increase_attributes() -> void {
  for (auto& val : attributes_ | std::views::values) {
    ++val;
  }
}