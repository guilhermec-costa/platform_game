#include "../include/window.hpp"

#include <iostream>

#include <SDL2/SDL.h>
#include <SDL2/SDL_video.h>

#include "../include/logger.hpp"

namespace Core {

  Window::Window(const WindowSpecification& spec) {
    m_window = SDL_CreateWindow(spec.title.c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                spec.width,
                                spec.height,
                                0);

    SDL_SetWindowFullscreen(m_window, 0);
    if (!m_window) {
      throw std::runtime_error(std::string("SDL Error: ") + SDL_GetError());
    }
    LOG_INFO("[Window] SDL Window created");
  }

  Window::~Window() {
    destroy();
  }

  Window& Window::operator=(Window&& other) noexcept {
    if (this != &other) {
      destroy();
      m_window       = other.m_window;
      other.m_window = nullptr;
    }
    return *this;
  }

  Vector2 Window::get_dimension() const {
    int w, h;
    SDL_GetWindowSize(m_window, &w, &h);
    return Vector2(w, h);
  }

  int Window::get_width() const {
    return get_dimension().x;
  }

  int Window::get_height() const {
    return get_dimension().y;
  }

  SDL_Window* Window::get() const {
    return m_window;
  }

  void Window::destroy() {
    if (m_window) {
      SDL_DestroyWindow(m_window);
      LOG_INFO("[GameContext] SDL Window destroyed");
      m_window = nullptr;
    }
  }

} // namespace Core
