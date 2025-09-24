#pragma once

#include <memory>
#include "collider_component.hpp"

class ComponentFactory {
public:
    static std::unique_ptr<Components::ColliderComponent> make_collider(
        const Vector2& pos, 
        const Vector2& dim, 
        const Vector2& collision_offset = {0,0}, 
        const std::string& name = "test"
    ) {
        return std::make_unique<Components::ColliderComponent>(pos, dim, collision_offset, name);
    }
};
