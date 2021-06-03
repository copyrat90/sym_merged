#include "game_entity_IGravityEntity.h"

#include "helper_rect.h"

namespace sym::game::entity
{

IGravityEntity::~IGravityEntity() = default;

IGravityEntity::IGravityEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange,
                               bn::fixed_rect relativePhysicsCollider, bool isGravityEnabledByDefault,
                               bn::fixed gravityScale, const bn::sprite_item* spriteItem)
    : IEntity(position, relativeInteractRange, spriteItem), relativePhysicsCollider_(relativePhysicsCollider),
      isGravityEnabled_(isGravityEnabledByDefault), gravityScale_(gravityScale)
{
}

IGravityEntity::IGravityEntity(IGravityEntity&& other)
    : IEntity(other.position_, other.relativeInteractRange_, other.spriteItem_),
      relativePhysicsCollider_(other.relativePhysicsCollider_), isGravityEnabled_(other.isGravityEnabled_),
      gravityScale_(other.gravityScale_)
{
}

IGravityEntity& IGravityEntity::operator=(IGravityEntity&& other)
{
    IEntity::operator=(bn::move(other));
    relativePhysicsCollider_ = other.relativePhysicsCollider_;
    isGravityEnabled_ = other.isGravityEnabled_;
    gravityScale_ = other.gravityScale_;
    return *this;
}

bn::fixed_rect IGravityEntity::GetPhysicsCollider() const
{
    using sym::helper::rect::operator+;
    return position_ + relativePhysicsCollider_;
}

bool IGravityEntity::GetGravityEnabled() const
{
    return isGravityEnabled_;
}

void IGravityEntity::SetGravityEnabled(bool isGravityEnabled)
{
    isGravityEnabled_ = isGravityEnabled;
}

bn::fixed IGravityEntity::GetGravityScale() const
{
    return gravityScale_;
}

void IGravityEntity::SetGravityScale(bn::fixed gravityScale)
{
    gravityScale_ = gravityScale;
}

bool IGravityEntity::ToggleGravityEnabled()
{
    return isGravityEnabled_ = !isGravityEnabled_;
}

} // namespace sym::game::entity
