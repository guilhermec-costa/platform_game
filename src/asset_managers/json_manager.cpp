#include "../../include/asset_manager/json_manager.hpp"

#include <fstream>

namespace Managers {

  JSONManager& JSONManager::instance() {
    static JSONManager manager;
    return manager;
  }

  JSONManager::JSONManager() = default;

  json* JSONManager::get_or_load(const std::string& path) {
    const std::string normalized_path = asset_path(path);
    std::ifstream     file(normalized_path);
    if (!file.is_open()) {
      return nullptr;
    }

    json* _json = get_asset(path);
    if (_json)
      return _json;

    json* parsed              = new json(json::parse(file));
    m_assets[normalized_path] = parsed;
    return parsed;
  }

} // namespace Managers
