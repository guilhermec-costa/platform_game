#pragma once

#include "asset_manager/json_alias.hpp"
#include "shared.hpp"

struct LevelMetadata {
  struct World {
    float ground_tile_side;
    float min_horizontal_x;
    float max_horizontal_x;
  };

  struct Platform {
    Vector2 position;
    Vector2 dimension;
    float   screen_height_pct;
  };

  struct Player {
    Vector2 start_position;
    Vector2 dimension;
    float   collision_offset_pct;
    float   screen_height_pct;
    struct Attributes {
      float move_speed;
      float jump_force;
      float gravity;
      float max_fall_speed;
    } attrs;
  };
};

struct Level {
  std::string                          name;
  LevelMetadata::World                 world;
  LevelMetadata::Player                player;
  std::vector<LevelMetadata::Platform> platforms;

  static Level from_json(const json& j, float screen_height);
};