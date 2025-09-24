#pragma once

#include "asset_manager/json_manager.hpp"
#include "game_context.hpp"
#include "level.hpp"

#pragma once

using namespace Managers;

class GameDataManager {
public:
  GameDataManager() = default;

public:
  static LevelData load_level(const std::string& level_name, const WorldData& world) {
    int   window_height = Core::GameContext::instance().window.get_height();
    json* j             = JSONManager::instance().get_asset(level_name);
    return LevelData::from_json(*j);
  }

  static PlayerData load_player(const WorldData& world) {
    Vector2 window_dim = Core::GameContext::instance().window.get_dimension();
    json*   j          = JSONManager::instance().get_asset("assets/phases/player_data.json");
    return PlayerData::from_json(*j);
  }

  static WorldData load_world() {
    json* j = JSONManager::instance().get_asset("assets/phases/world_data.json");
    return WorldData::from_json(*j);
  }
};
