#include "../include/level.hpp"

LevelData LevelData::from_json(const json& j) {
  LevelData level;

  level.name = j["name"];

  for (const auto& p : j["platforms"]) {
    LevelData::PlatformData platform{
        .position          = {p["x"].get<float>(), p["y"].get<float>()},
        .dimension         = {p["width"].get<float>(), p["height"].get<float>()},
        .screen_height_pct = p["y"].get<float>()};
    level.platforms.push_back(platform);
  }

  for (const auto& m : j["monsters"]) {
    LevelData::MonsterData monster;
    monster.position.x   = m.at("position").at(0).get<float>();
    monster.position.y   = m.at("position").at(1).get<float>();
    monster.dimension.x  = m.at("dimension").at(0).get<float>();
    monster.dimension.y  = m.at("dimension").at(1).get<float>();
    monster.move_speed   = m.value("speed", 50.0f);
    monster.gravity      = m.value("gravity", 500.0f);
    monster.patrol_min_x = m.value("patrol_min_x", 0.0f);
    monster.patrol_max_x = m.value("patrol_max_x", 0.0f);

    if (m.contains("collider")) {
      const auto& c                = m["collider"];
      monster.collider.position.x  = c.value("x", monster.position.x);
      monster.collider.position.y  = c.value("y", monster.position.y);
      monster.collider.dimension.x = c.value("width", monster.dimension.x);
      monster.collider.dimension.y = c.value("height", monster.dimension.y);
    } else {
      monster.collider.position  = monster.position;
      monster.collider.dimension = monster.dimension;
    }

    level.monsters.push_back(monster);
  }

  return level;
}

PlayerData PlayerData::from_json(const json& j) {
  PlayerData player_data;
  const auto& player           = j.at("player");
  const auto& player_position  = player.at("position");
  const auto& player_dimension = player.at("dimension");
  const auto& player_attrs     = player.at("attrs");

  // Posição inicial absoluta
  player_data.position = {
      player_position.at("x").get<float>(),
      player_position.at("y").get<float>()};

  player_data.dimension = {
      player_dimension.at("width").get<float>(),
      player_dimension.at("height").get<float>()};

  player_data.attrs = PlayerData::Attributes{
      .move_speed     = player_attrs.at("move_speed").get<float>(),
      .jump_force     = player_attrs.at("jump_force").get<float>(),
      .gravity        = player_attrs.at("gravity").get<float>(),
      .max_fall_speed = player_attrs.at("max_fall_speed").get<float>()};

  player_data.land_offset_pct = player.value("land_offset_pct", 0.0f);

  if (player.contains("collider")) {
    const auto& c = player["collider"];
    player_data.collider.position.x  = c.value("x", player_data.position.x);
    player_data.collider.position.y  = c.value("y", player_data.position.y);
    player_data.collider.dimension.x = c.value("width", player_data.dimension.x);
    player_data.collider.dimension.y = c.value("height", player_data.dimension.y);
  } else {
    // fallback: collider = dimensão do player
    player_data.collider.position  = player_data.position;
    player_data.collider.dimension = player_data.dimension;
  }

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