#pragma once

#include <string>

inline std::string asset_path(const std::string& relative) {
  return std::string(PROJECT_SOURCE_DIR) + relative;
}
