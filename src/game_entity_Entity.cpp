#include "game_entity_Entity.h"

namespace sym::game::entity
{

Entity::~Entity() = default;

Entity::Entity(bn::fixed_point position, bn::fixed_rect collider, const bn::sprite_item* const spriteItem)
    : position_(position), collider_(collider), spriteItem_(spriteItem)
{
}

Entity::Entity(Entity&& other) noexcept
    : collider_(other.collider_), sprite_(bn::move(other.sprite_)), spriteItem_(other.spriteItem_)
{
}

void Entity::FreeGraphicResource()
{
    sprite_.reset();
}

void Entity::AllocateGraphicResource()
{
    if (spriteItem_)
        sprite_ = spriteItem_->create_sprite(collider_.position());
}

} // namespace sym::game::entity
