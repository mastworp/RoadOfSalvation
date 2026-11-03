#pragma once

#include "character.hpp"
#include "profession.hpp"
#include "statistics.hpp"

class player final : public character {
 public:
  player(std::string_view name, std::unique_ptr<statistics> stats,
         std::unique_ptr<profession> prof);
  ~player() override = default;
  [[nodiscard]] auto name() const -> std::string override;
  [[nodiscard]] auto get_profession() const -> professions;
  [[nodiscard]] auto get_statistics() const
      -> std::map<attributes, std::size_t>;

 private:
  std::string name_;
  std::unique_ptr<statistics> statistics_;
  std::unique_ptr<profession> profession_;
};