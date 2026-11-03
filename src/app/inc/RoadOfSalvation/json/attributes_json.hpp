#pragma once
#include <cstddef>
#include <map>
#include <nlohmann/json.hpp>

#include "RoadOfSalvation/character/attributes.hpp"

NLOHMANN_JSON_SERIALIZE_ENUM(attributes,
                             {
                                 {attributes::level, "level"},
                                 {attributes::hp, "hp"},
                                 {attributes::mp, "mp"},
                                 {attributes::vigor, "vigor"},
                                 {attributes::mana, "mana"},
                                 {attributes::attack, "attack"},
                                 {attributes::defense, "defense"},
                                 {attributes::special_attack, "special_attack"},
                                 {attributes::special_defense,
                                  "special_defense"},
                                 {attributes::accuracy, "accuracy"},
                                 {attributes::luck, "luck"},
                             })

auto attributes_from_json(const nlohmann::json& stats)
    -> std::map<attributes, std::size_t>;