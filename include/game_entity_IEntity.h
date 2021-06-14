#pragma once

#include <bn_camera_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_optional.h>
#include <bn_sprite_item.h>
#include <bn_sprite_ptr.h>

namespace sym::game::entity
{

class IEntity
{
public:
    virtual ~IEntity() = 0;

    /**
     * @brief Constructor.
     *
     * Graphic is not allocated in here.
     * You have to manually call AllocateGraphicResource().
     *
     * @param position center position of the Entity
     * @param relativeInteractRange rect range to interact with player, relative to position.
     * @param spriteItem sprite_item
     */
    IEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange,
            const bn::sprite_item* spriteItem = nullptr);

    /**
     * @brief Move constructor.
     *
     */
    IEntity(IEntity&& other);

    /**
     * @brief Move assignment operator.
     *
     */
    IEntity& operator=(IEntity&& other);

    IEntity(const IEntity& other) = delete;
    IEntity& operator=(const IEntity& other) = delete;

    virtual void Update();

    /**
     * @brief Get the interactive range.
     * Absolute coordinate.
     *
     */
    bn::fixed_rect GetInteractRange() const;

    virtual void FreeGraphicResource();
    virtual void AllocateGraphicResource(int z_order);

    virtual void SetCamera(const bn::camera_ptr& camera);

    bn::fixed_point GetPosition() const;
    void SetPosition(const bn::fixed_point& position);
    bn::fixed GetX() const;
    bn::fixed GetY() const;
    void SetX(bn::fixed x);
    void SetY(bn::fixed y);

    void SetHorizontalFlip(bool flip);
    bool GetHorizontalFlip() const;
    void SetVerticalFlip(bool flip);
    bool GetVerticalFlip() const;

protected:
    bn::fixed_point position_;
    bn::fixed_rect relativeInteractRange_;
    bn::optional<bn::sprite_ptr> sprite_;
    const bn::sprite_item* spriteItem_;

    void SyncSpritePositionToPosition_();
};

} // namespace sym::game::entity
