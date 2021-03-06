#include "game_entity_IEntity.h"

#include "helper_rect.h"

namespace sym::game::entity
{

IEntity::~IEntity() = default;

IEntity::IEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange,
                 const bn::sprite_item* const spriteItem)
    : position_(position), relativeInteractRange_(relativeInteractRange), spriteItem_(spriteItem),
      paletteItem_(&spriteItem->palette_item())
{
}

IEntity::IEntity(IEntity&& other)
    : position_(other.position_), relativeInteractRange_(other.relativeInteractRange_),
      sprite_(bn::move(other.sprite_)), spriteItem_(other.spriteItem_), paletteItem_(other.paletteItem_),
      isBlendingEnabled_(other.isBlendingEnabled_), isMosaicEnabled_(other.isMosaicEnabled_),
      isVisible_(other.isVisible_)
{
}

IEntity& IEntity::operator=(IEntity&& other)
{
    position_ = other.position_;
    relativeInteractRange_ = other.relativeInteractRange_;
    sprite_ = bn::move(other.sprite_);
    spriteItem_ = other.spriteItem_;
    paletteItem_ = other.paletteItem_;
    isBlendingEnabled_ = other.isBlendingEnabled_;
    isMosaicEnabled_ = other.isMosaicEnabled_;
    isVisible_ = other.isVisible_;
    return *this;
}

void IEntity::Update()
{
}

bn::fixed_rect IEntity::GetInteractRange() const
{
    using helper::rect::operator+;
    return position_ + relativeInteractRange_;
}

void IEntity::FreeGraphicResource()
{
    sprite_.reset();
}

void IEntity::AllocateGraphicResource(int z_order)
{
    if (spriteItem_)
    {
        sprite_ = spriteItem_->create_sprite(position_);
        sprite_->set_z_order(z_order);
        sprite_->set_blending_enabled(isBlendingEnabled_);
        sprite_->set_mosaic_enabled(isMosaicEnabled_);
        sprite_->set_visible(isVisible_);
        sprite_->set_scale(scale_);
        if (paletteItem_)
            SetPalette(*paletteItem_);
    }
}

void IEntity::SetBlendingEnabled(bool isEnable)
{
    isBlendingEnabled_ = isEnable;
    if (sprite_)
        sprite_->set_blending_enabled(isEnable);
}

bool IEntity::GetBlendingEnabled() const
{
    return isBlendingEnabled_;
}

void IEntity::SetMosaicEnabled(bool isEnable)
{
    isMosaicEnabled_ = isEnable;
    if (sprite_)
        sprite_->set_mosaic_enabled(isEnable);
}

bool IEntity::GetMosaicEnabled() const
{
    return isMosaicEnabled_;
}

void IEntity::SetVisible(bool isVisible)
{
    isVisible_ = isVisible;
    if (sprite_)
        sprite_->set_visible(isVisible);
}

bool IEntity::GetVisible() const
{
    return isVisible_;
}

void IEntity::SetScale(bn::fixed scale)
{
    scale_ = scale;
    if (sprite_)
        sprite_->set_scale(scale);
}

bn::fixed IEntity::GetScale() const
{
    return scale_;
}

void IEntity::SetPalette(const bn::sprite_palette_item& paletteItem)
{
    paletteItem_ = &paletteItem;
    if (sprite_)
        sprite_->set_palette(paletteItem);
}

void IEntity::SetCamera(const bn::camera_ptr& camera)
{
    BN_ASSERT(sprite_, "cannot attach camera to null sprite");
    sprite_->set_camera(camera);
}

bn::fixed_point IEntity::GetPosition() const
{
    return position_;
}

void IEntity::SetPosition(const bn::fixed_point& position)
{
    position_ = position;
    SyncSpritePositionToPosition_();
}

bn::fixed IEntity::GetX() const
{
    return position_.x();
}

bn::fixed IEntity::GetY() const
{
    return position_.y();
}

void IEntity::SetX(bn::fixed x)
{
    position_.set_x(x);
    SyncSpritePositionToPosition_();
}

void IEntity::SetY(bn::fixed y)
{
    position_.set_y(y);
    SyncSpritePositionToPosition_();
}

bn::optional<bn::sprite_ptr> IEntity::GetSprite()
{
    return sprite_;
}

void IEntity::SetHorizontalFlip(bool flip)
{
    if (sprite_)
        sprite_->set_horizontal_flip(flip);
}

bool IEntity::GetHorizontalFlip() const
{
    if (sprite_)
        return sprite_->horizontal_flip();
    return false;
}

void IEntity::SetVerticalFlip(bool flip)
{
    if (sprite_)
        sprite_->set_vertical_flip(flip);
}

bool IEntity::GetVerticalFlip() const
{
    if (sprite_)
        return sprite_->vertical_flip();
    return false;
}

void IEntity::SyncSpritePositionToPosition_()
{
    if (sprite_)
        sprite_->set_position(position_);
}

} // namespace sym::game::entity
