#include "shared.hpp"

#include <SDL2/SDL_video.h>
#include <iostream>

namespace Core {

struct WindowSpecification {
  std::string title;
  int         width  = 900;
  int         height = 600;
};

class Window {
public:
  Window(const WindowSpecification& spec = WindowSpecification()) {
    m_window = SDL_CreateWindow(spec.title.c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
                                spec.width, spec.height, 0);
    if (!m_window) {
      throw std::runtime_error(std::string("SDL Error: ") + SDL_GetError());
    }
    std::cout << "[Window] SDL Window created\n";
  }

  ~Window() { destroy(); }

  Window(const Window&) = delete;

  Window& operator=(Window&& other) noexcept {
    if (this != &other) {
      destroy();
      m_window       = other.m_window;
      other.m_window = nullptr;
    }
    return *this;
  }

  Vector2 get_dimension() const {
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    return Vector2(w, h);
  }

  int get_width() const { return get_dimension().x; }
  int get_height() const { return get_dimension().y; }

  SDL_Window* get() const { return m_window; }

private:
  void destroy() {
    if (m_window) {
      SDL_DestroyWindow(m_window);
      std::cout << "[GameContext] SDL Window destroyed\n";
      m_window = nullptr;
    }
  }

  SDL_Window* m_window = nullptr;
};

}; // namespace Core