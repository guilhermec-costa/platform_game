#include "../include/game.hpp"

#include "../include/asset_manager/json/json_manager.hpp"
#include "../include/asset_manager/utils.hpp"
#include "../include/level.hpp"
#include "../include/sdl_backend.hpp"

#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <cstdlib>

struct WindowSpecification {
  int width;
  int height;
};

Game::Game() : running(true), current_state(nullptr) {
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
  auto&        tex_manager = TextureManager::get_instance();
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
  json  j     = JSONManager::get_instance().load_file(level_name);
  Level level = Level::from_json(j);
  return level;
}