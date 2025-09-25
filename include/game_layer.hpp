#pragma once

#include <cstdint>
#include <memory>
#include <unordered_map>

#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_video.h>

#include "bg_parallax.hpp"
#include "game_context.hpp"
#include "objects/monster_object.hpp"
#include "objects/platform_object.hpp"
#include "objects/player_object.hpp"
#include "ui/label_element.hpp"

class GameLayer {
protected:
  Core::GameContext& ctx = Core::GameContext::instance();

public:
  GameLayer(const GameLayer&)                       = delete;
  GameLayer()                                       = default;
  virtual void handle_event(const SDL_Event& event) = 0;
  virtual void update(float dt)                     = 0;
  virtual void render()                             = 0;
  void         set_suspended(bool s) {
    suspended = s;
  };
  bool is_suspended() {
    return suspended;
  }

  virtual ~GameLayer() {};

protected:
  bool suspended = false;
};

class PlayLayer : public GameLayer {
private:
  int                                          walk_channel = -1;
  BackgroundParallax                           bg_parallax;
  std::unique_ptr<PlayerObject>                player;
  std::vector<std::unique_ptr<PlatformObject>> platforms;
  std::vector<std::unique_ptr<MonsterObject>>  monsters;

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
  void update_level();
  void resolve_platform_collision(CharacterObject& obj, const PlatformObject& platform);
  void resolve_global_ground_collision(CharacterObject& obj);
  void check_objects_ground_collision();
  void check_objects_platforms_collision();
  void change_level(int level_id);

private:
  std::unordered_map<uint8_t, std::string> level_asset_mapping{
      {1, "assets/phases/level1.json"},
      {2, "assets/phases/level2.json"},
  };
};

class UILayer : public GameLayer {
public:
  UILayer();
  void handle_event(const SDL_Event& event) override;
  void update(float dt) override;
  void render() override;

private:
  UI::Label level_label;
};