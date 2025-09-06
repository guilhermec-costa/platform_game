#pragma once

#include <SDL2/SDL_render.h>
#include <SDL2/SDL_ttf.h>

class GameContext {
private:
  SDL_Window*   m_window   = nullptr;
  SDL_Renderer* m_renderer = nullptr;
  TTF_Font*     font       = nullptr;

public:
  GameContext()             = default;
  GameContext(GameContext&) = delete;
  GameContext(SDL_Window* window, SDL_Renderer* renderer)
      : m_window(window), m_renderer(renderer) {};
  inline SDL_Window*   window() const { return m_window; }
  inline SDL_Renderer* renderer() const { return m_renderer; }
};