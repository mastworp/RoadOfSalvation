#include "RoadOfSalvation/json/json_parser.hpp"

#include <filesystem>
#include <fstream>
#include <string>
#if defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

using json = nlohmann::json;

namespace {
std::filesystem::path executable_dir() {
#if defined(__APPLE__)
  uint32_t size = 0;
  _NSGetExecutablePath(nullptr, &size);
  std::string buf(size, '\0');
  _NSGetExecutablePath(buf.data(), &size);
  return std::filesystem::canonical(buf.c_str()).parent_path();
#elif defined(_WIN32)
  wchar_t buf[MAX_PATH];
  GetModuleFileNameW(nullptr, buf, MAX_PATH);
  return std::filesystem::path(buf).parent_path();
#else
  return std::filesystem::canonical("/proc/self/exe").parent_path();
#endif
}

std::filesystem::path data_dir() {
  return (executable_dir() / ROADOFSALVATION_DATA_REL_PATH).lexically_normal();
}
}  // namespace

json_parser::json_parser(const std::filesystem::path& file)
    : file_(file.is_relative() ? data_dir() / file : file) {}

auto json_parser::load() -> void {
  std::ifstream f{file_};
  data_ = json::parse(f);
}

auto json_parser::get_data() -> const nlohmann::json& { return data_; }