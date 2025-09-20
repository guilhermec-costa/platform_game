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
    float screen_height_pct;
  };

  struct Player {
    Vector2 start_position;
    Vector2 dimension;
    float   collision_offset_pct;
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

  static Level from_json(const json& j, float screen_height) {
    Level level;

    // METADATA
    level.name = j["name"];

    // WORLD DATA
    auto world                   = j["world"];
    level.world.ground_tile_side = world["ground_tile_side"].get<float>();
    level.world.min_horizontal_x = world["min_horizontal_x"].get<float>();
    level.world.max_horizontal_x = world["max_horizontal_x"].get<float>();

    float world_width = level.world.max_horizontal_x;

    // PLAYER DATA
    auto player                 = j["player"];
    auto player_position        = player["start_position"];
    auto player_dimension       = player["dimension"];
    auto player_attrs           = player["attrs"];
    level.player.start_position = Vector2{player_position["x"].get<float>() * world_width,
                                          player_position["y"].get<float>() * screen_height};

    level.player.dimension =
        Vector2{player_dimension["width"].get<float>(), player_dimension["height"].get<float>()};

    level.player.attrs = LevelMetadata::Player::Attributes{
        .move_speed     = player_attrs["move_speed"].get<float>(),
        .jump_force     = player_attrs["jump_force"].get<float>(),
        .gravity        = player_attrs["gravity"].get<float>(),
        .max_fall_speed = player_attrs["max_fall_speed"].get<float>()};

    level.player.collision_offset_pct = player["collision_offset_pct"].get<float>();

    for (const auto& p : j["platforms"]) {
      LevelMetadata::Platform platform{
          .position = {p["x"].get<float>() * world_width, p["y"].get<float>() * screen_height},
          .dimension = {p["width"].get<float>() * world_width, p["height"].get<float>() * screen_height},
          .screen_height_pct = p["y"].get<float>()};
      level.platforms.push_back(platform);
    }

    return level;
  }
};