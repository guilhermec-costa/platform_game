#include "../include/level.hpp"

Level Level::from_json(const json& j, float screen_height) {
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
  auto player           = j["player"];
  auto player_position  = player["start_position"];
  auto player_dimension = player["dimension"];
  auto player_attrs     = player["attrs"];

  level.player.start_position = Vector2{
      player_position["x"].get<float>() * world_width,
      player_position["y"].get<float>() * screen_height
  };

  level.player.screen_height_pct = player_position["y"].get<float>();

  level.player.dimension = Vector2{
      player_dimension["width"].get<float>(),
      player_dimension["height"].get<float>()
  };

  level.player.attrs = LevelMetadata::Player::Attributes{
      .move_speed     = player_attrs["move_speed"].get<float>(),
      .jump_force     = player_attrs["jump_force"].get<float>(),
      .gravity        = player_attrs["gravity"].get<float>(),
      .max_fall_speed = player_attrs["max_fall_speed"].get<float>()
  };

  level.player.collision_offset_pct = player["collision_offset_pct"].get<float>();

  // PLATFORMS
  for (const auto& p : j["platforms"]) {
    LevelMetadata::Platform platform{
        .position          = {p["x"].get<float>() * world_width, p["y"].get<float>() * screen_height},
        .dimension         = {p["width"].get<float>(), p["height"].get<float>()},
        .screen_height_pct = p["y"].get<float>()
    };
    level.platforms.push_back(platform);
  }

  return level;
}
