#pragma once
#include "RoadOfSalvation/character/professions.hpp"

#include <nlohmann/json.hpp>

NLOHMANN_JSON_SERIALIZE_ENUM(professions, {
                             {professions::none, "none"},
                             {professions::alchemist, "alchemist"},
                             {professions::crusader, "crusader"},
                             {professions::scout, "scout"},
                             })

auto find_profession(const nlohmann::json& doc,
                     professions id) -> const nlohmann::json&;