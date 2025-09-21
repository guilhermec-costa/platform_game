#pragma once

#include <string>

#include "asset_manager.hpp"
#include "json_alias.hpp"

namespace Managers {

  class JSONManager : public AssetManager<json*> {
  public:
    static JSONManager& instance();

    json* get_or_load(const std::string& path);

  private:
    JSONManager();
    JSONManager(const JSONManager&)            = delete;
    JSONManager& operator=(const JSONManager&) = delete;
  };

} // namespace Managers