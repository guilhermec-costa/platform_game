#include <SDL2/SDL_events.h>
#include <SDL2/SDL_render.h>

#include "../../include/asset_manager/font_manager.hpp"
#include "../../include/game_context.hpp"
#include "../../include/game_layer.hpp"
#include "../../include/ui/label_element.hpp"

UILayer::UILayer() :
  level_label(ctx.get_active_level().name,
              FontManager::instance().get_asset("assets/fonts/YoungSerif-Regular.ttf"),
              Vector2{100, 100},
              Vector2{80, 80}),
  GameLayer() {}

void UILayer::handle_event(const SDL_Event& event) {}

void UILayer::update(float dt) {}

void UILayer::render() {
  level_label.render(ctx.renderer);
}