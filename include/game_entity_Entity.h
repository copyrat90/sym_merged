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

    /**
     * @brief Constructor.
     *
     * Graphic is not allocated in here.
     * You have to manually call AllocateGraphicResource().
     *
     * @param position center position of the Entity
     * @param collider collider for actual collision detection.
     * @param isApplyGravity whether to apply gravity or not
     * @param spriteItem sprite_item
     */
    Entity(bn::fixed_point position, bn::fixed_rect collider, bool isApplyGravity,
           const bn::sprite_item* spriteItem = nullptr);

    /**
     * @brief Move constructor.
     *
     */
    Entity(Entity&& other) noexcept;

    /**
     * @brief Move assignment operator.
     *
     */
    Entity& operator=(Entity&& other) noexcept;

    Entity(const Entity& other) = delete;
    Entity& operator=(const Entity& other) = delete;

    void FreeGraphicResource();
    virtual void AllocateGraphicResource();

    /**
     * @brief Update the Entity.
     *
     * Apply gravity
     */
    virtual void Update();

protected:
    bn::fixed_point position_;
    bn::fixed_rect collider_;
    const bool isApplyGravity_;
    bn::optional<bn::sprite_ptr> sprite_;
    const bn::sprite_item* spriteItem_;
};

} // namespace sym::game::entity
