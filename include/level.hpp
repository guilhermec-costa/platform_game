#pragma once

#include "asset_manager/json_alias.hpp"
#include "shared.hpp"

struct LevelData {
  struct PlatformData {
    Vector2 position;
    Vector2 dimension;
    float   screen_height_pct;
  };
  struct MonsterData {
    Vector2 position;
    Vector2 dimension;
    float   speed        = 50.0f;
    float   gravity      = 500.0f;
    float   patrol_min_x = 0.0f;
    float   patrol_max_x = 0.0f;

    MonsterData() = default;
  };

  std::string               name;
  std::vector<PlatformData> platforms;
  std::vector<MonsterData>  monsters;
  static LevelData          from_json(const json& j, float screen_height, int world_width);
};

struct WorldData {
  float            ground_tile_side;
  float            min_horizontal_x;
  float            max_horizontal_x;
  static WorldData from_json(const json& j);
};

struct PlayerData {
  Vector2 start_position;
  Vector2 dimension;
  float   land_offset_pct;
  float   screen_height_pct;
  struct Attributes {
    float move_speed;
    float jump_force;
    float gravity;
    float max_fall_speed;
  } attrs;

  static PlayerData from_json(const json& j, float screen_height, float world_width);
};

struct GameData {
  PlayerData player_data;
  WorldData  world_data;
  LevelData  level_data;
};
