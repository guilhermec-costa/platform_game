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
public:
  Game(const GameSpecification& game_spec);
  Game(const Game&) = delete;

  void set_fps(int fps) {
    target_fps = fps;
    std::cout << "[Game] Game FPS setted to " << fps << "\n";
  }
  void run();
  void quit();
  void change_state(std::unique_ptr<GameState> new_state) { current_state = std::move(new_state); }
  void load_textures();
  SDL_Renderer* create_renderer(Window& window);
  Level         load_level(const std::string& level_name);
  void          init_subsytems();

private:
  void handle_events();
  void update(float dt);
  void render();

private:
  bool                       running;
  int                        target_fps = 60;
  GameContext&               ctx        = GameContext::instance();
  std::unique_ptr<GameState> current_state;
  FPSCounter                 fps_counter;
};
} // namespace Core
