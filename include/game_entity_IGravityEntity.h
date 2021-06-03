#pragma once

#include "game_entity_IEntity.h"

namespace sym::game::entity
{

class IGravityEntity : public IEntity
{
public:
    virtual ~IGravityEntity() = 0;

    IGravityEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange,
                   bn::fixed_rect relativePhysicsCollider, bool isGravityEnabledByDefault, bn::fixed gravityScale,
                   const bn::sprite_item* spriteItem);

    IGravityEntity(IGravityEntity&& other);
    IGravityEntity& operator=(IGravityEntity&& other);

    IGravityEntity(const IGravityEntity& other) = delete;
    IGravityEntity& operator=(const IGravityEntity& other) = delete;

    /**
     * @brief Get the physics collider.
     * Absolute Coordinate.
     *
     * @return `bn::fixed_rect` collider
     */
    bn::fixed_rect GetPhysicsCollider() const;

    bool GetGravityEnabled() const;
    void SetGravityEnabled(bool isGravityEnabled);

    bn::fixed GetGravityScale() const;
    void SetGravityScale(bn::fixed gravityScale);
    [[maybe_unused]] bool ToggleGravityEnabled();

protected:
    /**
     * @brief saves collider relative to the position_.
     *
     */
    bn::fixed_rect relativePhysicsCollider_;
    bool isGravityEnabled_;
    bn::fixed gravityScale_;
};

} // namespace sym::game::entity
