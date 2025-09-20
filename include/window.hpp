#pragma once

#include "shared.hpp"

#include <SDL2/SDL_video.h>
#include <string>

namespace Core {

struct WindowSpecification {
  std::string title;
  int         width  = 900;
  int         height = 600;
};

class Window {
public:
  Window(const WindowSpecification& spec = WindowSpecification());
  ~Window();

  Window(const Window&) = delete;
  Window& operator=(Window&& other) noexcept;

  Vector2     get_dimension() const;
  int         get_width() const;
  int         get_height() const;
  SDL_Window* get() const;

private:
  void destroy();

  SDL_Window* m_window = nullptr;
};

} // namespace Core
