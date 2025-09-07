#pragma once

#include "objects/game_object.hpp"

#include <vector>

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
  void updateAll(float dt) {
    for (auto& e : entities)
      e->update(dt);
  }
  void renderAll() {
    for (auto& e : entities)
      e->render();
  }
};