#pragma once

#include "game_entity_IEntity.h"

namespace sym::game::entity
{

class IPhysicsEntity : public IEntity
{
public:
    virtual ~IPhysicsEntity() = 0;

    IPhysicsEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange,
                   bn::fixed_rect relativePhysicsCollider, bool isGravityEnabledByDefault, bn::fixed gravityScale,
                   const bn::sprite_item* spriteItem);

    IPhysicsEntity(IPhysicsEntity&& other);
    IPhysicsEntity& operator=(IPhysicsEntity&& other);

    IPhysicsEntity(const IPhysicsEntity& other) = delete;
    IPhysicsEntity& operator=(const IPhysicsEntity& other) = delete;

    /**
     * @brief Get the physics collider.
     * Absolute Coordinate.
     *
     * @return `bn::fixed_rect` collider
     */
    [[nodiscard]] bn::fixed_rect GetPhysicsCollider() const;

    [[nodiscard]] bool GetGravityEnabled() const;
    void SetGravityEnabled(bool isGravityEnabled);

    [[nodiscard]] bn::fixed GetGravityScale() const;
    void SetGravityScale(bn::fixed gravityScale);
    [[maybe_unused]] bool ToggleGravityEnabled();

    [[nodiscard]] bn::fixed_point GetVelocity() const;
    void SetVelocity(bn::fixed_point velocity);
    void SetXVelocity(bn::fixed xVel);
    void SetYVelocity(bn::fixed yVel);

    [[nodiscard]] bool GetGrounded() const;
    void SetGrounded(bool);

    enum class MoveDirections
    {
        UP = 1,
        DOWN = 2,
        LEFT = 4,
        RIGHT = 8
    };
    MoveDirections GetMoveDirections() const;

    static constexpr bn::fixed EPSILON_VEL = 0.01;

protected:
    /**
     * @brief saves collider relative to the position_.
     *
     */
    bn::fixed_rect relativePhysicsCollider_;
    bool isGravityEnabled_;
    bn::fixed gravityScale_;

    bn::fixed_point velocity_ = {0, 0};
    bool isGrounded_ = false;
};

inline IPhysicsEntity::MoveDirections operator|(IPhysicsEntity::MoveDirections d1, IPhysicsEntity::MoveDirections d2)
{
    using MoveDirections = IPhysicsEntity::MoveDirections;
    return static_cast<MoveDirections>(static_cast<int>(d1) | static_cast<int>(d2));
}

inline IPhysicsEntity::MoveDirections operator&(IPhysicsEntity::MoveDirections d1, IPhysicsEntity::MoveDirections d2)
{
    using MoveDirections = IPhysicsEntity::MoveDirections;
    return static_cast<MoveDirections>(static_cast<int>(d1) & static_cast<int>(d2));
}

inline bool operator!(IPhysicsEntity::MoveDirections dir)
{
    using MoveDirections = IPhysicsEntity::MoveDirections;
    return dir == static_cast<MoveDirections>(0);
}

} // namespace sym::game::entity
