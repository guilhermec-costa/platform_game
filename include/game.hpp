#pragma once

#include "fps_counter.hpp"
#include "game_context.hpp"
#include "game_state.hpp"

#include <SDL2/SDL_timer.h>
#include <memory>

namespace Core {

struct GameSpecification {
  WindowSpecification window_spec;
};

class Game {
private:
  bool                       running;
  int                        target_fps = 60;
  GameContext&               ctx        = GameContext::instance();
  std::unique_ptr<GameState> current_state;
  FPSCounter                 fps_counter;

public:
  Game(const GameSpecification& game_spec);
  Game(const Game&) = delete;

  void set_fps(int fps) {
    target_fps = fps;
    std::cout << "[Game] Game FPS setted to " << fps << "\n";
  }
  void run() {
    Uint32    frame_start;
    Uint32    last_time = SDL_GetTicks();
    int       frame_time;
    const int MS_FRAME_DELAY = 1000 / target_fps;

    while (running) {
      frame_start = SDL_GetTicks();
      float dt    = (frame_start - last_time) / 1000.0f;
      last_time   = frame_start;
      handle_events();
      update(dt);
      render();

      frame_time = SDL_GetTicks() - frame_start;
      if (MS_FRAME_DELAY > frame_time) {
        SDL_Delay(MS_FRAME_DELAY - frame_time);
      }
      fps_counter.frame_rendered();
    }
  }

  void quit() {
    running = false;
    std::cout << "[Game] Quitting the game\n";
    current_state.reset();
    ctx.end();
  }

  void change_state(std::unique_ptr<GameState> new_state) { current_state = std::move(new_state); }
  void load_textures();
  SDL_Renderer* create_renderer(Window& window);
  Level         load_level(const std::string& level_name);
  void          init_subsytems();

private:
  void handle_events();
  void update(float dt);
  void render();
};
} // namespace Core
