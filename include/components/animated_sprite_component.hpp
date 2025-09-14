#include "../animation.hpp"
#include "texture_component.hpp"

#include <SDL2/SDL_render.h>
#include <unordered_map>

class AnimatedSpriteComponent {
public:
  AnimatedSpriteComponent() = default;
  AnimatedSpriteComponent(TextureComponent texture, int frame_width, int frame_height,
                          float frame_time, Vector2D render_dim)
      : texture(texture), frame_width(frame_width), frame_height(frame_height),
        frame_time(frame_time), current_frame(0), elapsed_time(0.0f), render_dim(render_dim),
        horizontally_flipped(false) {

    int tex_width, tex_height;
    SDL_QueryTexture(texture.get_texture(), nullptr, nullptr, &tex_width, &tex_height);
    num_columns          = tex_width / frame_width;
    num_rows             = tex_height / frame_height;
    num_frames           = num_columns * num_rows;
    current_animaton_idx = -1;
  }

  void update(float dt) {
    if (current_animaton_idx == -1) {
      return;
    }

    elapsed_time += dt;
    Animation& anim = animations[current_animaton_idx];
    if (elapsed_time >= anim.frame_time) {
      elapsed_time = 0.0f;
      current_frame++;
      if (current_frame > anim.end_frame) {
        if (anim.loop) {
          current_frame = anim.start_frame;
        } else {
          current_frame = anim.end_frame;
          elapsed_time  = anim.frame_time;
        }
      }
    }
  }

  void render(SDL_Renderer* renderer, const Vector2D& pos, const Camera& camera) {
    int             row        = current_frame / num_columns;
    int             col        = current_frame % num_columns;
    const Vector2D& camera_pos = camera.get_position();

    SDL_Rect src_rect = {col * frame_width, row * frame_height, frame_width, frame_height};
    SDL_Rect dst_rect = {static_cast<int>(pos.x - camera_pos.x),
                         static_cast<int>(pos.y - camera_pos.y), static_cast<int>(render_dim.x),
                         static_cast<int>(render_dim.y)};

    if (horizontally_flipped) {
      SDL_RenderCopyEx(renderer, texture.get_texture(), &src_rect, &dst_rect, 0, NULL,
                       SDL_FLIP_HORIZONTAL);
    } else {
      SDL_RenderCopy(renderer, texture.get_texture(), &src_rect, &dst_rect);
    }
  }

  void add_animation(int animation_idx, const std::string& name, int start_frame, int end_frame,
                     float frame_time, bool loop = true) {
    animations[animation_idx] = {name, start_frame, end_frame, frame_time, loop};
  }

  bool is_finished() const {
    if (current_animaton_idx == -1) {
      return true;
    }
    const Animation& anim = animations.at(current_animaton_idx);
    return (current_frame == anim.end_frame && elapsed_time >= anim.frame_time);
  }

  void play_animation(int animation_idx) {
    if (current_animaton_idx == animation_idx)
      return;
    current_animaton_idx = animation_idx;
    current_frame        = animations[animation_idx].start_frame;
    elapsed_time         = 0.0;
  }

  void set_flipped(bool flipped) { horizontally_flipped = flipped; }

private:
  TextureComponent                   texture;
  Vector2D                           render_dim;
  std::unordered_map<int, Animation> animations;
  int                                frame_width;
  int                                frame_height;
  int                                num_frames;
  int                                current_frame;
  int                                num_columns;
  int                                num_rows;
  float                              frame_time;
  float                              elapsed_time;
  bool                               horizontally_flipped;
  int                                current_animaton_idx;
};