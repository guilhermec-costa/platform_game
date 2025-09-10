#include "texture_component.hpp"

class AnimatedSpriteComponent {
public:
  AnimatedSpriteComponent() = default;
  AnimatedSpriteComponent(TextureComponent texture, int frame_width, int frame_height,
                          int num_frames, int num_columns, float frame_time, Vector2D render_dim)
      : texture(texture), frame_width(frame_width), frame_height(frame_height),
        num_frames(num_frames), num_columns(num_columns), frame_time(frame_time), current_frame(0),
        elapsed_time(0.0f), render_dim(render_dim) {}

  void update(float dt) {
    elapsed_time += dt;
    if (elapsed_time >= frame_time) {
      elapsed_time  = 0;
      current_frame = (current_frame + 1) % num_frames;
    }
  }

  void render(SDL_Renderer* renderer, const Vector2D& pos, const Camera& camera) {
    int row = current_frame / num_columns;
    int col = current_frame % num_columns;

    SDL_Rect src_rect = {col * frame_width, row * frame_height, frame_width, frame_height};
    SDL_Rect dst_rect = {static_cast<int>(pos.x - camera.x), static_cast<int>(pos.y - camera.y),
                         static_cast<int>(render_dim.x), static_cast<int>(render_dim.y)};

    SDL_RenderCopy(renderer, texture.get_texture(), &src_rect, &dst_rect);
  }

private:
  TextureComponent texture;
  Vector2D         render_dim;
  int              frame_width;
  int              frame_height;
  int              num_frames;
  float            frame_time;
  int              current_frame;
  float            elapsed_time;
  int              num_columns;
};