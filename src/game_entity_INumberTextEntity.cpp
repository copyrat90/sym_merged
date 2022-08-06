#include "game_entity_INumberTextEntity.h"

#include "bn_sprite_items_font_modern_dos_8x8.h"

namespace sym::game::entity
{

INumberTextEntity::~INumberTextEntity() = default;

INumberTextEntity::INumberTextEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                                     bn::fixed_point relativeNumberTextPosition, const bn::sprite_item* spriteItem)
    : IEntity(position, relativeInteractRange, spriteItem), textNumber_(textNumber),
      relativeNumberTextPosition_(relativeNumberTextPosition)
{
}

INumberTextEntity::INumberTextEntity(INumberTextEntity&& other)
    : IEntity(bn::move(other)), textNumber_(other.textNumber_),
      relativeNumberTextPosition_(other.relativeNumberTextPosition_)
{
}

INumberTextEntity& INumberTextEntity::operator=(INumberTextEntity&& other)
{
    IEntity::operator=(bn::move(other));
    textNumber_ = other.textNumber_;
    relativeNumberTextPosition_ = other.relativeNumberTextPosition_;
    return *this;
}

void INumberTextEntity::FreeGraphicResource()
{
    numberSprite_.reset();
    IEntity::FreeGraphicResource();
}

void INumberTextEntity::AllocateGraphicResource(int z_order)
{
    IEntity::AllocateGraphicResource(z_order);
    numberSprite_ =
        bn::sprite_items::font_modern_dos_8x8.create_sprite(position_ + relativeNumberTextPosition_, textNumber_);
    numberSprite_->set_z_order(z_order);
    numberSprite_->set_blending_enabled(isBlendingEnabled_);
    numberSprite_->set_mosaic_enabled(isMosaicEnabled_);
    numberSprite_->set_visible(isVisible_);
    // TODO: Override SetPalette()
    // if (paletteItem_)
    // SetPalette(*paletteItem_);
}

void INumberTextEntity::SetBlendingEnabled(bool isEnable)
{
    IEntity::SetBlendingEnabled(isEnable);
    if (numberSprite_)
        numberSprite_->set_blending_enabled(isEnable);
}

void INumberTextEntity::SetMosaicEnabled(bool isEnable)
{
    IEntity::SetMosaicEnabled(isEnable);
    if (numberSprite_)
        numberSprite_->set_mosaic_enabled(isEnable);
}

void INumberTextEntity::SetCamera(const bn::camera_ptr& camera)
{
    IEntity::SetCamera(camera);
    numberSprite_->set_camera(camera);
}

} // namespace sym::game::entity
