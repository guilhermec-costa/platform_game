#pragma once

#include "../../vendor/json.hpp"

using json = nlohmann::json;

class JSONManager {
public:

  static JSONManager& get_instance() {
    static JSONManager manager;
    return manager;
  };

  bool load_file() {

  };
private:
  JSONManager() {};
  JSONManager(const JSONManager&)            = delete;
  JSONManager& operator=(const JSONManager&) = delete;
};