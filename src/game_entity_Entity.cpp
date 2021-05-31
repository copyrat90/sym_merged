#include "game_entity_Entity.h"

namespace sym::game::entity
{

Entity::~Entity() = default;

Entity::Entity(bn::fixed_point position, bn::fixed_rect collider, bool isApplyGravity,
               const bn::sprite_item* const spriteItem)
    : position_(position), collider_(collider), isApplyGravity_(isApplyGravity), spriteItem_(spriteItem)
{
}

Entity::Entity(Entity&& other) noexcept
    : position_(other.position_), collider_(other.collider_), isApplyGravity_(other.isApplyGravity_),
      sprite_(bn::move(other.sprite_)), spriteItem_(other.spriteItem_)
{
}

Entity& Entity::operator=(Entity&& other) noexcept
{
    position_ = other.position_;
    collider_ = other.collider_;
    sprite_ = bn::move(other.sprite_);
    spriteItem_ = other.spriteItem_;
    return *this;
}

void Entity::FreeGraphicResource()
{
    sprite_.reset();
}

void Entity::AllocateGraphicResource()
{
    if (spriteItem_ && !sprite_)
        sprite_ = spriteItem_->create_sprite(position_);
}

void Entity::Update()
{
    // TODO: Apply gravity
}

} // namespace sym::game::entity
