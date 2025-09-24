#include "../../include/components/animated_sprite_component.hpp"

namespace Components {

  AnimatedSpriteComponent::AnimatedSpriteComponent(TextureComponent texture,
                                                   int              frame_width,
                                                   int              frame_height,
                                                   float            frame_time,
                                                   Vector2          render_dim) :
    texture(texture),
    frame_width(frame_width),
    frame_height(frame_height),
    frame_time(frame_time),
    render_dim(render_dim),
    current_animaton_idx(-1) {

    int tex_width, tex_height;
    SDL_QueryTexture(texture.get_texture(), nullptr, nullptr, &tex_width, &tex_height);
    num_columns = tex_width / frame_width;
    num_rows    = tex_height / frame_height;
    num_frames  = num_columns * num_rows;
  }

  void AnimatedSpriteComponent::update(float dt) {
    if (current_animaton_idx == -1) {
      return;
    }

    elapsed_time += dt;
    Animation& anim = animations[current_animaton_idx];
    if (elapsed_time >= anim.default_frame_time) {
      elapsed_time = 0.0f;
      current_frame++;
      if (current_frame > anim.end_frame) {
        if (anim.loop) {
          current_frame = anim.start_frame;
        } else {
          current_frame = anim.end_frame;
          elapsed_time  = anim.default_frame_time;
        }
      }
    }
  }

  void AnimatedSpriteComponent::render(SDL_Renderer*       renderer,
                                       const Vector2&      pos,
                                       const Core::Camera& camera) {
    int            row        = current_frame / num_columns;
    int            col        = current_frame % num_columns;
    const Vector2& camera_pos = camera.get_position();

    SDL_Rect src_rect = {col * frame_width, row * frame_height, frame_width, frame_height};
    SDL_Rect dst_rect = {static_cast<int>(pos.x - camera_pos.x),
                         static_cast<int>(pos.y - camera_pos.y),
                         static_cast<int>(render_dim.x),
                         static_cast<int>(render_dim.y)};

    if (horizontally_flipped) {
      SDL_RenderCopyEx(
          renderer, texture.get_texture(), &src_rect, &dst_rect, 0, nullptr, SDL_FLIP_HORIZONTAL);
    } else {
      SDL_RenderCopy(renderer, texture.get_texture(), &src_rect, &dst_rect);
    }
  }

  void AnimatedSpriteComponent::add_animation(int                animation_idx,
                                              const std::string& name,
                                              int                start_frame,
                                              int                end_frame,
                                              float              frame_time,
                                              bool               loop) {
    animations[animation_idx] = {name, start_frame, end_frame, frame_time, loop};
  }

  bool AnimatedSpriteComponent::is_finished() const {
    if (current_animaton_idx == -1) {
      return true;
    }
    const Animation& anim = animations.at(current_animaton_idx);
    return (current_frame == anim.end_frame && elapsed_time >= anim.default_frame_time);
  }

  void AnimatedSpriteComponent::play_animation(int animation_idx) {
    if (current_animaton_idx == animation_idx) {
      return;
    }
    current_animaton_idx = animation_idx;
    current_frame        = animations[animation_idx].start_frame;
    elapsed_time         = 0.0f;
  }

  void AnimatedSpriteComponent::set_flipped(bool flipped) {
    horizontally_flipped = flipped;
  }

  void AnimatedSpriteComponent::set_scale(const Vector2& s) {
    scale = s;
    render_dim *= s;
  }

} // namespace Components
