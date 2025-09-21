#include <SDL2/SDL_events.h>

#include "../../include/asset_manager/font_manager.hpp"
#include "../../include/game_context.hpp"
#include "../../include/game_layer.hpp"
#include "../../include/ui/label_element.hpp"

UILayer::UILayer() : ui_manager(), GameLayer() {
  TTF_Font* font =
      Managers::FontManagerSingleton::instance().get_asset("assets/fonts/YoungSerif-Regular.ttf");
  auto label =
      std::make_unique<UI::Label>(ctx.get_level().name, font, Vector2{100, 100}, Vector2{130, 130});
  ui_manager.add_element(std::move(label));
}

void UILayer::handle_event(const SDL_Event& event) {
  ui_manager.handle_event(event);
}

void UILayer::update(float dt) {
  ui_manager.update(dt);
}

void UILayer::render() {
  ui_manager.render(ctx.renderer);
}