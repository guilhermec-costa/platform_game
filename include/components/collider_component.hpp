#pragma once

#include <memory>

#include <SDL2/SDL_pixels.h>
#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>

#include "../camera.hpp"
#include "../shared.hpp"
#include "../ui/label_element.hpp"

namespace Components {

  class ColliderComponent {
  public:
    ColliderComponent();
    ColliderComponent(Vector2            pos,
                      Vector2            dim,
                      Vector2            collision_offset = {0.0f, 0.0f},
                      const std::string& name             = "test");
    ~ColliderComponent();

    bool     check_collision(const SDL_Rect& other) const;
    SDL_Rect get_rect() const;
    void     set_drawing_color(const SDL_Color color);
    void     render_collision_box(SDL_Renderer*       renderer,
                                  const Core::Camera& camera,
                                  const bool          follow_camera = true);

    void set_position(const Vector2& pos);
    void set_dimension(const Vector2& dim);
    void flip_to_left();
    void flip_to_right();
    void set_scale(const Vector2& scale);

  public:
    Vector2                    position;
    Vector2                    scale{1.0f,1.0f};
    Vector2                    dimension;
    SDL_Rect                   collision_box;
    Vector2                    collision_offset{0, 0};
    std::unique_ptr<UI::Label> label;

  private:
    SDL_Color drawing_color = {255, 0, 0, SDL_ALPHA_OPAQUE};
  };

} // namespace Components
