#pragma once

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../camera.hpp"
#include "../shared.hpp"

namespace Components {

  class ColliderComponent {
  public:
    ColliderComponent();
    ColliderComponent(Vector2 pos, Vector2 dim, Vector2 collision_offset = {0, 0});

    bool     check_collision(const SDL_Rect& other) const;
    SDL_Rect get_rect() const;
    void     render_collision_box(SDL_Renderer*       renderer,
                                  const Core::Camera& camera,
                                  const bool          follow_camera = true);

    void set_position(const Vector2& pos);
    void set_dimension(const Vector2& dim);

  public:
    Vector2  position;
    Vector2  dimension;
    SDL_Rect collision_box;
    Vector2  collision_offset;
  };

} // namespace Components
