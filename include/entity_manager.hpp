#pragma once

#include <vector>
#include "objects/game_object.hpp"

class EntityManager {
private:
  EntityManager(const EntityManager&) = delete;
private:
  std::vector<GameObject*> entities;

public:
  EntityManager() = default;
  void add_entity(GameObject* object) { entities.push_back(object); };
  void remove_entity(GameObject* object) {
    // TODO
  }
};