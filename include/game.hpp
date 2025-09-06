#include "fps_counter.hpp"
#include "game_context.hpp"
#include "game_state.hpp"

class Game {
private:
  bool         running;
  GameContext& context;
  GameState*   current_state;
  FPSCounter   fps_counter;

public:
  Game(GameContext& ctx);

  void set_fps(int fps) {}
  void run() {
    while (running) {
      handle_events();
      update();
      render();
    }
  }

  void quit() { running = false; }

  void change_state(GameState* new_state) {
    delete current_state;
    current_state = new_state;
  }

private:
  void handle_events();
  void update();
  void render();
};
