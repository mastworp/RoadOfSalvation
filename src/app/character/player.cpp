#include "RoadOfSalvation/character/player.hpp"

#include <string>

player::player(const std::string_view name, std::unique_ptr<statistics> stats,
               std::unique_ptr<profession> prof)
    : name_(name), statistics_(std::move(stats)), profession_(std::move(prof)) {
  statistics_->set(profession_->get_base_stats());
}

auto player::name() const -> std::string { return name_; }

auto player::get_profession() const -> professions {
  return profession_->name();
}

auto player::get_statistics() const -> std::map<attributes, std::size_t> {
  return statistics_->get_attributes();
}