#include "../include/game.hpp"

#include "../include/asset_manager/font_manager.hpp"
#include "../include/asset_manager/json_manager.hpp"
#include "../include/asset_manager/texture_manager.hpp"
#include "../include/asset_manager/utils.hpp"
#include "../include/level.hpp"

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>

namespace Core {

Game::Game(const GameSpecification& game_spec) : running(true) {
  init_subsytems();

  Level l = load_level(asset_path("assets/phases/level1.json"));
  ctx.set_level(l);

  GameContext::instance().init(game_spec.window_spec);
  load_textures();

  TTF_Font* font = Managers::FontManager::load_font("assets/fonts/YoungSerif-Regular.ttf", 22);
  if (!font) {
    std::runtime_error("Failed to load font");
  }
  ctx.set_font(font);
  ctx.ui_manager = UI::UIManager();
  fps_counter    = FPSCounter();
}

void Game::handle_events() {
  SDL_Event event;
  while (SDL_PollEvent(&event)) {
    if (event.type == SDL_QUIT) {
      quit();
    }
    for (const auto& layer : layers) {
      layer->handle_event(event);
    }
  }
}

void Game::update(float dt) {
  for (const auto& layer : layers) {
    layer->update(dt);
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
  SDL_RenderClear(ctx.renderer);
  for (const auto& layer : layers) {
    layer->render();
  }
  ctx.ui_manager.render(ctx.renderer);
  SDL_RenderPresent(ctx.renderer);
}

SDL_Texture* load_texture_or_die(const std::string& path, SDL_Renderer* renderer) {
  auto&        tex_manager = Managers::TextureManager::get_instance();
  SDL_Texture* tex         = tex_manager.get_or_load(path, renderer);

  if (!tex) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", path.c_str());
    std::exit(EXIT_FAILURE);
  }

  return tex;
}

void Game::load_textures() {
  std::vector<std::string> textures = {
      "assets/images/nigthborne.png",           "assets/images/parallax/bg.png",
      "assets/images/parallax/far-trees.png",   "assets/images/parallax/mid-trees.png",
      "assets/images/parallax/close-trees.png", "assets/images/grass.png"};

  for (const auto& path : textures)
    load_texture_or_die(path, ctx.renderer);
}

Level Game::load_level(const std::string& level_name) {
  json  j     = Managers::JSONManager::get_instance().load_file(level_name);
  Level level = Level::from_json(j, ctx.window.get_height());
  return level;
}

void Game::init_subsytems() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error("[Game] SDL Video subsystem initialization failed: " +
                             std::string(SDL_GetError()));
  }
  std::cout << "[Game] SDL Video subsystem initialized\n";

  if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
    throw std::runtime_error("[Game] SDL Image subsystem initialization failed: " +
                             std::string(IMG_GetError()));
  }
  std::cout << "[Game] SDL Image subsystem initialized\n";

  if (TTF_Init() < 0) {
    throw std::runtime_error("[Game] SDL TTF subsystem initialization failed: " +
                             std::string(TTF_GetError()));
  }
  std::cout << "[Game] SDL TTF subsystem initialized\n";

  std::cout << "[Game] Finished initializing SDL subsystems\n";
}

void Game::run() {
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
void Game::quit() {
  running = false;
  std::cout << "[Game] Quitting the game\n";
  // for(const auto& layer : layers) {
  //   layer->get;
  // }
  // current_state.reset();
  ctx.end();
}

void Game::push_layer(std::unique_ptr<GameLayer> layer) {
  layers.push_back(std::move(layer));
}
} // namespace Core