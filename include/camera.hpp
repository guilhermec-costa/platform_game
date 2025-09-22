#pragma once

#include <SDL2/SDL_render.h>
#include "shared.hpp"

#pragma once

#include "shared.hpp"

namespace Core {

  class Camera {
  public:
    Camera() = default;
    Camera(Vector2 position, Vector2 dimension, float left_margin, float right_margin);

    void follow(const Vector2& target_pos);
    void update(float min_horizontal, float max_horizontal);
    void resize(float screen_width);
    void render(SDL_Renderer* renderer);
    void set_dimension(const Vector2& dim) { dimension = dim; };
    void set_position(const Vector2& pos) { dimension = pos; };

    const Vector2& get_position() const;

  private:
    Vector2 position;
    Vector2 dimension;
    float   left_margin  = 0;
    float   right_margin = 0;

  public:
    static constexpr float LEFT_MARGIN_PCT  = 0.15f;
    static constexpr float RIGHT_MARGIN_PCT = 0.65f;
  };
} // namespace Core
