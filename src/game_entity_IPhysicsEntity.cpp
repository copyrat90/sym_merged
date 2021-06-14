#include "game_entity_IPhysicsEntity.h"

#include "helper_rect.h"

namespace sym::game::entity
{

IPhysicsEntity::~IPhysicsEntity() = default;

IPhysicsEntity::IPhysicsEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange,
                               bn::fixed_rect relativePhysicsCollider, bool isGravityEnabledByDefault,
                               bn::fixed gravityScale, const bn::sprite_item* spriteItem)
    : IEntity(position, relativeInteractRange, spriteItem), relativePhysicsCollider_(relativePhysicsCollider),
      isGravityEnabled_(isGravityEnabledByDefault), gravityScale_(gravityScale)
{
}

IPhysicsEntity::IPhysicsEntity(IPhysicsEntity&& other)
    : IEntity(other.position_, other.relativeInteractRange_, other.spriteItem_),
      relativePhysicsCollider_(other.relativePhysicsCollider_), isGravityEnabled_(other.isGravityEnabled_),
      gravityScale_(other.gravityScale_)
{
}

IPhysicsEntity& IPhysicsEntity::operator=(IPhysicsEntity&& other)
{
    IEntity::operator=(bn::move(other));
    relativePhysicsCollider_ = other.relativePhysicsCollider_;
    isGravityEnabled_ = other.isGravityEnabled_;
    gravityScale_ = other.gravityScale_;
    return *this;
}

bn::fixed_rect IPhysicsEntity::GetPhysicsCollider() const
{
    using sym::helper::rect::operator+;
    return position_ + relativePhysicsCollider_;
}

bool IPhysicsEntity::GetGravityEnabled() const
{
    return isGravityEnabled_;
}

void IPhysicsEntity::SetGravityEnabled(bool isGravityEnabled)
{
    isGravityEnabled_ = isGravityEnabled;
}

bn::fixed IPhysicsEntity::GetGravityScale() const
{
    return gravityScale_;
}

void IPhysicsEntity::SetGravityScale(bn::fixed gravityScale)
{
    gravityScale_ = gravityScale;
}

bool IPhysicsEntity::ToggleGravityEnabled()
{
    return isGravityEnabled_ = !isGravityEnabled_;
}

bn::fixed_point IPhysicsEntity::GetVelocity() const
{
    return velocity_;
}

void IPhysicsEntity::SetVelocity(bn::fixed_point velocity)
{
    velocity_ = velocity;
}

void IPhysicsEntity::SetXVelocity(bn::fixed xVel)
{
    velocity_.set_x(xVel);
}

void IPhysicsEntity::SetYVelocity(bn::fixed yVel)
{
    velocity_.set_y(yVel);
}

bool IPhysicsEntity::GetGrounded() const
{
    return isGrounded_;
}

void IPhysicsEntity::SetGrounded(bool isGrounded)
{
    isGrounded_ = isGrounded;
}

IPhysicsEntity::MoveDirections IPhysicsEntity::GetMoveDirections() const
{
    int result = 0;
    if (velocity_.x() > EPSILON_VEL)
        result |= static_cast<int>(MoveDirections::RIGHT);
    else if (velocity_.x() < -EPSILON_VEL)
        result |= static_cast<int>(MoveDirections::LEFT);
    if (velocity_.y() > EPSILON_VEL)
        result |= static_cast<int>(MoveDirections::DOWN);
    else if (velocity_.y() < EPSILON_VEL)
        result |= static_cast<int>(MoveDirections::UP);

    return static_cast<MoveDirections>(result);
}

} // namespace sym::game::entity
