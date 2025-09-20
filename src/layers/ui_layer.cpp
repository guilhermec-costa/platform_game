#include "../../include/game_layer.hpp"
#include "../../include/game_context.hpp"
#include "../../include/ui/label_element.hpp"
#include <SDL2/SDL_events.h>

UILayer::UILayer(): ui_manager(), GameLayer() {
  auto label = std::make_unique<UI::Label>(ctx.get_level().name, ctx.font, Vector2{100, 100},
                                           Vector2{100, 100});
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