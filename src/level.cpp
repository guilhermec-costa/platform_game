#include "../include/level.hpp"

LevelData LevelData::from_json(const json& j, float screen_height, int world_width) {
  LevelData level;

  level.name = j["name"];

  for (const auto& p : j["platforms"]) {
    LevelData::PlatformData platform{
        .position  = {p["x"].get<float>() * world_width, p["y"].get<float>() * screen_height},
        .dimension = {p["width"].get<float>(), p["height"].get<float>()},
        .screen_height_pct = p["y"].get<float>()};
    level.platforms.push_back(platform);
  }

  for (const auto& m : j["monsters"]) {
    MonsterData monster;
    monster.position.x   = m.at("position").at(0).get<float>() * world_width;
    monster.position.y   = m.at("position").at(1).get<float>() * screen_height;
    monster.dimension.x  = m.at("dimension").at(0).get<float>();
    monster.dimension.y  = m.at("dimension").at(1).get<float>();
    monster.move_speed   = m.value("speed", 50.0f);
    monster.gravity      = m.value("gravity", 500.0f);
    monster.patrol_min_x = m.value("patrol_min_x", 0.0f);
    monster.patrol_max_x = m.value("patrol_max_x", 0.0f);
    level.monsters.push_back(monster);
  };

  return level;
}

PlayerData PlayerData::from_json(const json& j, float screen_height, float world_width) {
  PlayerData player_data;
  auto       player           = j["player"];
  auto       player_position  = player["position"];
  auto       player_dimension = player["dimension"];
  auto       player_attrs     = player["attrs"];

  player_data.position = Vector2{player_position["x"].get<float>() * world_width,
                                 player_position["y"].get<float>() * screen_height};

  player_data.screen_height_pct = player_position["y"].get<float>();

  player_data.dimension =
      Vector2{player_dimension["width"].get<float>(), player_dimension["height"].get<float>()};

  player_data.attrs =
      PlayerData::Attributes{.move_speed     = player_attrs["move_speed"].get<float>(),
                             .jump_force     = player_attrs["jump_force"].get<float>(),
                             .gravity        = player_attrs["gravity"].get<float>(),
                             .max_fall_speed = player_attrs["max_fall_speed"].get<float>()};

  player_data.land_offset_pct = player["land_offset_pct"].get<float>();
  return player_data;
}

WorldData WorldData::from_json(const json& j) {
  WorldData world_data;
  auto      world             = j["world"];
  world_data.ground_tile_side = world["ground_tile_side"].get<float>();
  world_data.min_horizontal_x = world["min_horizontal_x"].get<float>();
  world_data.max_horizontal_x = world["max_horizontal_x"].get<float>();

  float world_width = world_data.max_horizontal_x;

  return world_data;
}