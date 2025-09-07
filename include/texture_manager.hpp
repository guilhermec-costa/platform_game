#pragma once

#include <SDL2/SDL_image.h>
#include <SDL2/SDL_render.h>
#include <map>
#include <string>
enum class TextureID { Grass, Player, Enemy };

class TextureManager {
private:
  TextureManager()                      = default;
  TextureManager(const TextureManager&) = delete;

private:
  static TextureManager*            manager;
  std::map<TextureID, SDL_Texture*> textures;
  std::map<TextureID, std::string>  texture_paths = {{TextureID::Grass, "assets/grass.png"}};

public:
  static TextureManager& get_instance() {
    static TextureManager* instance = new TextureManager();
    return *instance;
  }

  void load_textures(SDL_Renderer* renderer) {
    for (auto& [id, path] : texture_paths) {
      SDL_Texture* tex = IMG_LoadTexture(renderer, path.c_str());
      if (!tex) {
        SDL_Log("Failed to load %s: %s", path.c_str(), SDL_GetError());
      } else {
        textures[id] = tex;
      }
    }
  }

  SDL_Texture* get(TextureID id) { return textures[id]; }

  void freeTextures() {
    for (auto& [id, tex] : textures) {
      SDL_DestroyTexture(tex);
    }
    textures.clear();
  }
};