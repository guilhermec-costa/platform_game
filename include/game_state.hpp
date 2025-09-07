#pragma once

#include "bg_parallax.hpp"
#include "entity_manager.hpp"
#include "game_context.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

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
  BackgroundParallax bg_parallax;
public:
  PlayState(GameContext& ctx);
  void handle_event(SDL_Event& event) override;
  void update(float dt) override;
  void render() override;
};