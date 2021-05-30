#pragma once

#include <bn_fixed_rect.h>
#include <bn_optional.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>

namespace sym::game::entity
{

class Entity
{
public:
    virtual ~Entity() = 0;

    Entity(bn::fixed_point position, bn::fixed_rect collider, const bn::sprite_item* const spriteItem = nullptr);
    Entity(Entity&& other) noexcept;

    void FreeGraphicResource();
    virtual void AllocateGraphicResource();

protected:
    bn::fixed_point position_;
    bn::fixed_rect collider_;
    bn::optional<bn::sprite_ptr> sprite_;
    const bn::sprite_item* const spriteItem_;
};

} // namespace sym::game::entity
