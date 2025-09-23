#pragma once

#include <vector>

#include "camera.hpp"
#include "objects/game_object.hpp"

class EntityManager {
private:
  EntityManager(const EntityManager&) = delete;

private:
  std::vector<GameObject*> entities;

public:
  EntityManager() = default;
  void add_entity(GameObject* object) {
    entities.push_back(object);
  };
  void remove_entity(GameObject* object) {
    // TODO
  }
  void updateAll(float dt) {
    for (auto& e : entities)
      e->update(dt);
  }
  void renderAll(SDL_Renderer* renderer, const Core::Camera& camera) {
    for (auto& e : entities)
      e->render(renderer, camera);
  }
};