#pragma once

#include "collider_component.hpp"
#include "texture_component.hpp"

class TileComponent {
public:
    TileComponent(SDL_Texture* texture, Vector2D pos, Vector2D dim)
        : texture_comp(texture, pos, dim),
          collider_comp(pos, dim)
    {}

    void render(SDL_Renderer* renderer, const Camera& camera) {
        texture_comp.render(renderer, camera);
    }

    bool check_collision(const Vector2D& player_pos, const Vector2D& player_dim) {
        return collider_comp.check_collision(player_pos, player_dim);
    }

    void set_position(Vector2D pos) {
        texture_comp.set_position(pos);
        collider_comp.position = pos;
    }

private:
    TextureComponent texture_comp;
    ColliderComponent collider_comp;
};