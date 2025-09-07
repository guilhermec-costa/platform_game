#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_render.h>
class Texture_Component {
public:
  SDL_Rect src_rect;
  SDL_Rect target_rect;

public:
  Texture_Component();
  ~Texture_Component();
  void         render();
  void         scale(float proportion);
  void         rotate(double angle);
  SDL_Texture* m_texture;
};