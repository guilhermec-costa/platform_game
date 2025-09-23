#pragma once

#include "../components/collider_component.hpp"
#include "../components/texture_component.hpp"
#include "../level.hpp"
#include "game_object.hpp"

struct RectOverlap {
  int left;
  int right;
  int top;
  int bottom;
};

class PlatformObject : public GameObject {
public:
  PlatformObject(const LevelData::PlatformData metadata);
  void              handle_event();
  void              update(float dt) override;
  void              render(SDL_Renderer* renderer, const Core::Camera& camera) override;
  void              resize();
  const RectOverlap get_overlap(const SDL_Rect& rect) const;

private:
  Components::TextureComponent texture_component;
  LevelData::PlatformData      m_metadata;
};