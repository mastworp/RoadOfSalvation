#pragma once
#include <nlohmann/json.hpp>

#include "RoadOfSalvation/character/professions.hpp"

NLOHMANN_JSON_SERIALIZE_ENUM(professions,
                             {
                                 {professions::none, "none"},
                                 {professions::alchemist, "alchemist"},
                                 {professions::crusader, "crusader"},
                                 {professions::scout, "scout"},
                             })

auto find_profession(const nlohmann::json& doc, professions id)
    -> const nlohmann::json&;