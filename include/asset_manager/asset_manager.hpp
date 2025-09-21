#pragma once

#include <string>
#include <unordered_map>

#include "utils.hpp"

namespace Managers {

  template <typename T>
  class AssetManager {
  public:
    AssetManager()          = default;
    virtual ~AssetManager() = default;

    virtual T get_or_load(const std::string& file) = 0;

    T get_asset(const std::string& file) {
      auto it = m_assets.find(asset_path(file));
      if (it != m_assets.end()) {
        return it->second;
      }
      return nullptr;
    }

  protected:
    std::unordered_map<std::string, T> m_assets;
  };
}; // namespace Managers