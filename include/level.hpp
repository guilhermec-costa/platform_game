#pragma once

#include "asset_manager/json/json_alias.hpp"

#include "shared.hpp"

struct Level {
  Vector2 player_start;

  static Level from_json(const json& j) {
    Level level;
    level.player_start = Vector2(j["player_start"]["x"], j["player_start"]["y"]);
    return level;
  }
};