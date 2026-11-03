#pragma once

#include <cstddef>

enum class attributes : std::size_t {
  level,
  hp,
  mp,
  vigor,
  mana,
  attack,
  defense,
  special_attack,
  special_defense,
  accuracy,
  luck
};