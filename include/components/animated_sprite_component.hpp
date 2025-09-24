#pragma once

#include <string>
#include <unordered_map>

#include <SDL2/SDL.h>

#include "../camera.hpp"
#include "../shared.hpp"
#include "texture_component.hpp"

namespace Components {

  struct Animation {
    std::string name;
    int         start_frame;
    int         end_frame;
    float       default_frame_time;
    bool        loop;
  };

  class AnimatedSpriteComponent {
  public:
    AnimatedSpriteComponent() = default;
    AnimatedSpriteComponent(TextureComponent texture,
                            int              frame_width,
                            int              frame_height,
                            float            frame_time,
                            Vector2          render_dim);

    void update(float dt);
    void render(SDL_Renderer* renderer, const Vector2& pos, const Core::Camera& camera);

    void add_animation(const std::string& name,
                       int                start_frame,
                       int                end_frame,
                       float              frame_time,
                       bool               loop = true);

    bool is_finished() const;
    void play_animation(const std::string& name);

    void set_flipped(bool flipped);
    void set_scale(const Vector2& scale);

  private:
    TextureComponent                           texture;
    Vector2                                    render_dim;
    std::unordered_map<std::string, Animation> animations;
    int                                        frame_width          = 0;
    int                                        frame_height         = 0;
    int                                        num_frames           = 0;
    int                                        current_frame        = 0;
    int                                        num_columns          = 0;
    int                                        num_rows             = 0;
    float                                      frame_time           = 0.0f;
    float                                      elapsed_time         = 0.0f;
    bool                                       horizontally_flipped = false;
    std::string                                current_animation_name;
    Vector2                                    scale{0.0f, 0.0f};
  };

} // namespace Components
