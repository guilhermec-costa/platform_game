#pragma once

#include <memory>
#include <vector>

#include "ui_element.hpp"

namespace UI {
  class UIManager {
  public:
    UIManager() = default;

    void add_element(std::unique_ptr<UIElement> element) {
      elements.push_back(std::move(element));
    }
    void render(SDL_Renderer* renderer) {
      for (const auto& elem : elements)
        elem->render(renderer);
    }

    void handle_event(const SDL_Event& e) {
      for (const auto& elem : elements)
        elem->handle_event(e);
    }

    void update(float dt) {
      for (const auto& elem : elements)
        elem->update(dt);
    }

    std::vector<std::unique_ptr<UIElement>> elements;
  };
} // namespace UI