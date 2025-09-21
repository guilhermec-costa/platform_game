#pragma once

#include "bg_parallax.hpp"
#include "game_context.hpp"
#include "ground.hpp"
#include "objects/platform_object.hpp"
#include "objects/player_object.hpp"
#include "ui/ui_manager.hpp"

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>
#include <memory>

class GameLayer {
protected:
  Core::GameContext& ctx = Core::GameContext::instance();

public:
  GameLayer(const GameLayer&)                       = delete;
  GameLayer()                                       = default;
  virtual void handle_event(const SDL_Event& event) = 0;
  virtual void update(float dt)                     = 0;
  virtual void render()                             = 0;
  void         set_suspended(bool s) { suspended = s; };
  bool         is_suspended() { return suspended; }

  virtual ~GameLayer() {};

protected:
  bool suspended = 0;
};

class PlayLayer : public GameLayer {
private:
  BackgroundParallax                           bg_parallax;
  Ground                                       ground;
  std::unique_ptr<PlayerObject>                player;
  std::vector<std::unique_ptr<PlatformObject>> platforms;

public:
  PlayLayer();
  void handle_event(const SDL_Event& event) override;
  void handle_mouse_click_event(const SDL_MouseButtonEvent& window);
  void handle_window_event(const SDL_WindowEvent& window);
  void handle_keydown(const SDL_KeyboardEvent& key);
  void handle_keyup(const SDL_KeyboardEvent& key);
  bool is_key_down(SDL_Scancode scancode) const;
  void update(float dt) override;
  void render() override;
  void check_player_ground_collision();
  void check_player_window_collision();
  void check_player_platform_collision();
};

class UILayer : public GameLayer {
public:
  UILayer();
  void handle_event(const SDL_Event& event) override;
  void update(float dt) override;
  void render() override;

private:
  UI::UIManager ui_manager;
};