#pragma once

#include <memory>
#include <vector>

#include <SDL2/SDL_timer.h>

#include "fps_counter.hpp"
#include "game_context.hpp"
#include "game_layer.hpp"

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
    void          run();
    void          quit();
    void          load_global_assets();
    SDL_Renderer* create_renderer(Window& window);
    LevelData     load_level(const std::string& level_name);
    void          load_game_data();
    void          init_subsytems();
    void          push_layer(std::unique_ptr<GameLayer> layer);

  private:
    void handle_events();
    void update(float dt);
    void render();

  private:
    bool                                    running;
    int                                     target_fps = 60;
    GameContext&                            ctx        = GameContext::instance();
    std::vector<std::unique_ptr<GameLayer>> layers;
    FPSCounter                              fps_counter;
  };
} // namespace Core
