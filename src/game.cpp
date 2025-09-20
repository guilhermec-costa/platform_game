#include "../include/game.hpp"

#include "../include/asset_manager/json/json_manager.hpp"
#include "../include/asset_manager/utils.hpp"
#include "../include/level.hpp"
#include "../include/sdl_backend.hpp"

#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>

namespace Core {

Game::Game(const GameSpecification& game_spec) : running(true), current_state(nullptr) {
  init_subsytems();
  GameContext::instance().init(game_spec.window_spec);
  load_textures();
  Level l = load_level(asset_path("assets/phases/level1.json"));
  ctx.set_level(l);

  TTF_Font* font =
      SDLBackend::load_font(asset_path("assets/fonts/YoungSerif-Regular.ttf").c_str(), 22);
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
    if (current_state) {
      current_state->handle_event(event);
    }
  }
}

void Game::update(float dt) {
  if (current_state) {
    current_state->update(dt);
  }
}

void Game::render() {
  SDL_SetRenderDrawColor(ctx.renderer, 0, 0, 0, 255);
  SDL_RenderClear(ctx.renderer);
  if (current_state) {
    current_state->render();
  }
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

  for (auto& path : textures) {
    load_texture_or_die(asset_path(path), ctx.renderer);
  }
}

Level Game::load_level(const std::string& level_name) {
  json  j     = Managers::JSONManager::get_instance().load_file(level_name);
  Level level = Level::from_json(j);
  return level;
}

void Game::init_subsytems() {
  if (SDL_Init(SDL_INIT_VIDEO) < 0) {
    throw std::runtime_error("[SDLBackend] SDL Video subsystem initialization failed: " +
                             std::string(SDL_GetError()));
  }
  std::cout << "[SDLBackend] SDL Video subsystem initialized\n";

  if (!(IMG_Init(IMG_INIT_JPG) & IMG_INIT_JPG)) {
    throw std::runtime_error("[SDLBackend] SDL Image subsystem initialization failed: " +
                             std::string(IMG_GetError()));
  }
  std::cout << "[SDLBackend] SDL Image subsystem initialized\n";

  if (TTF_Init() < 0) {
    throw std::runtime_error("[SDLBackend] SDL TTF subsystem initialization failed: " +
                             std::string(TTF_GetError()));
  }
  std::cout << "[SDLBackend] SDL TTF subsystem initialized\n";

  std::cout << "[SDLBackend] Finished initializing SDL subsystems\n";
}
} // namespace Core