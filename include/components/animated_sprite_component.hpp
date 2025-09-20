#pragma once

#include "../camera.hpp"
#include "../shared.hpp"
#include "texture_component.hpp"

#include <SDL2/SDL.h>
#include <string>
#include <unordered_map>

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
  AnimatedSpriteComponent(TextureComponent texture, int frame_width, int frame_height,
                          float frame_time, Vector2 render_dim);

  void update(float dt);
  void render(SDL_Renderer* renderer, const Vector2& pos, const Core::Camera& camera);

  void add_animation(int animation_idx, const std::string& name, int start_frame, int end_frame,
                     float frame_time, bool loop = true);

  bool is_finished() const;
  void play_animation(int animation_idx);

  void set_flipped(bool flipped);

private:
  TextureComponent                   texture;
  Vector2                            render_dim;
  std::unordered_map<int, Animation> animations;
  int                                frame_width          = 0;
  int                                frame_height         = 0;
  int                                num_frames           = 0;
  int                                current_frame        = 0;
  int                                num_columns          = 0;
  int                                num_rows             = 0;
  float                              frame_time           = 0.0f;
  float                              elapsed_time         = 0.0f;
  bool                               horizontally_flipped = false;
  int                                current_animaton_idx = -1;
};

} // namespace Components
