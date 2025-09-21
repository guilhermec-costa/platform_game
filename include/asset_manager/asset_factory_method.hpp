#pragma once

#include <string>

#include "audio_manager.hpp"
#include "font_manager.hpp"
#include "json_manager.hpp"
#include "texture_manager.hpp"

class AssetFactoryMethod {
public:
  static void* load(const std::string& path) {
    std::string ext = _get_extension(path);

    if (ext == "png" || ext == "jpg" || ext == "bmp") {
      return Managers::TextureManagerSingleton::instance().get_or_load(path);
    } else if (ext == "ttf" || ext == "otf") {
      return Managers::FontManagerSingleton::instance().get_or_load(path);
    } else if (ext == "json") {
      return Managers::JSONManager::instance().get_or_load(path);
    } else if (ext == "wav") {
      return Managers::AudioManager::instance().get_or_load(path);
    }
    return nullptr;
  }

private:
  static std::string _get_extension(const std::string& path) {
    auto dot = path.find_last_of('.');
    if (dot == std::string::npos)
      return "";
    return path.substr(dot + 1);
  }
};
