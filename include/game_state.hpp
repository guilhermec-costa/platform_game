#pragma once

#include "bg_parallax.hpp"
#include "entity_manager.hpp"
#include "game_context.hpp"
#include "ground.hpp"
#include "objects/platform_object.hpp"
#include "objects/player_object.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <memory>

class GameState {
protected:
  GameContext&  context;
  EntityManager entity_manager;

public:
  GameState(const GameState&) = delete;
  GameState(GameContext& ctx) : context(ctx), entity_manager(EntityManager()) {}
  virtual void handle_event(SDL_Event& event) = 0;
  virtual void update(float dt)               = 0;
  virtual void render()                       = 0;
  virtual ~GameState() {}
};

class PlayState : public GameState {
private:
  BackgroundParallax            bg_parallax;
  Ground                        ground;
  std::unique_ptr<PlayerObject> player;
  std::unique_ptr<PlatformObject> platform;

public:
  PlayState(GameContext& ctx);
  void handle_event(SDL_Event& event) override;
  void handle_mouse_click_event(const SDL_MouseButtonEvent& window);
  void handle_window_event(const SDL_WindowEvent& window);
  void handle_keydown(const SDL_KeyboardEvent& key);
  void handle_keyup(const SDL_KeyboardEvent& key);
  bool is_key_down(SDL_Scancode scancode) const;
  void update(float dt) override;
  void render() override;
};