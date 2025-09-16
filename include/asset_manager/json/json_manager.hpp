#pragma once

#include "json_alias.hpp"

#include <fstream>

class JSONManager {
public:
  static JSONManager& get_instance() {
    static JSONManager manager;
    return manager;
  };

  json load_file(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
      std::runtime_error("Cannot open JSON file: " + path);
    }
    return json::parse(file);
  };

private:
  JSONManager()                              = default;
  JSONManager(const JSONManager&)            = delete;
  JSONManager& operator=(const JSONManager&) = delete;
};