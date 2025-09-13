#include "../include/game.hpp"

#include <SDL2/SDL_log.h>
#include <SDL2/SDL_render.h>
#include <cstdlib>

Game::Game(GameContext& ctx) : running(true), current_state(nullptr), context(ctx) {
  load_textures();
  fps_counter = FPSCounter();
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
  SDL_SetRenderDrawColor(context.renderer, 0, 0, 0, 255);
  SDL_RenderClear(context.renderer);
  if (current_state) {
    current_state->render();
  }

  SDL_RenderPresent(context.renderer);
}

SDL_Texture* load_texture_or_die(const std::string& path, SDL_Renderer* renderer) {
  auto&        tex_manager = TextureManager::get_instance();
  SDL_Texture* tex         = tex_manager.load_texture(path, renderer);

  if (!tex) {
    SDL_LogError(SDL_LOG_CATEGORY_APPLICATION, "Failed to load texture: %s", path.c_str());
    std::exit(EXIT_FAILURE);
  }

  return tex;
}

void Game::load_textures() {
  std::vector<std::string> textures = {
      "assets/nigthborne.png",
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-bg.png",
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-far-trees.png",
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-mid-trees.png",
      "assets/parallax_demon_woods_pack/layers/parallax-demon-woods-close-trees.png"};

  for (auto& path : textures) {
    load_texture_or_die(TextureManager::asset_path(path), context.renderer);
  }
}