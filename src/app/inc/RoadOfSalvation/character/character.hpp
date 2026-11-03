#pragma once
#include <string>

class character {
 public:
  virtual ~character() = default;
  [[nodiscard]] virtual auto name() const -> std::string = 0;
};