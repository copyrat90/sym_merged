#include "game_entity_PressureButton.h"

#include "helper_rect.h"

#include "bn_sprite_items_spr_basic_symbols.h"
#include "bn_sprite_items_spr_complex_symbols.h"
#include "bn_sprite_items_spr_pressure_button.h"
#include "bn_sprite_palette_items_pal_symbol_placeholder.h"

namespace sym::game::entity
{

namespace
{

constexpr bn::fixed SPRITE_WIDTH = bn::sprite_items::spr_pressure_button.shape_size().width();
constexpr bn::fixed SPRITE_HEIGHT = bn::sprite_items::spr_pressure_button.shape_size().height();

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({1 - SPRITE_WIDTH / 2, 10 - SPRITE_HEIGHT / 2}, {15, 6});
constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE_CEIL =
    helper::rect::MakeFixedRectByTopLeftAndSize({1 - SPRITE_WIDTH / 2, 0 - SPRITE_HEIGHT / 2}, {15, 6});

constexpr bn::fixed_point RELATIVE_NUMBER_TEXT_POS = {1, 16};
constexpr bn::fixed_point RELATIVE_REQUIRED_SYMBOL_PLACEHOLDER_POS = {0, -5};

constexpr int WAIT_UPDATES = 15;

constexpr int BUTTON_ON_GRAPHICS_INDEX = 2;
constexpr int BUTTON_OFF_GRAPHICS_INDEX = 0;

} // namespace

PressureButton::PressureButton(bn::fixed_point position, int textNumber, bool isOnByDefault, bool isVerticallyReversed,
                               bn::optional<entity::Symbol::Type> requiredSymbolType)
    : IButtonEntity(position, RELATIVE_INTERACT_RANGE, textNumber,
                    isVerticallyReversed ? -RELATIVE_NUMBER_TEXT_POS : RELATIVE_NUMBER_TEXT_POS, isOnByDefault,
                    &bn::sprite_items::spr_pressure_button),
      isOnCeil_(isVerticallyReversed), requiredSymbolType_(requiredSymbolType)
{
}

bn::fixed_rect PressureButton::GetInteractRange() const
{
    using helper::rect::operator+;
    return isOnCeil_ ? position_ + RELATIVE_INTERACT_RANGE_CEIL : position_ + RELATIVE_INTERACT_RANGE;
}

void PressureButton::AllocateGraphicResource(int z_order)
{
    IButtonEntity::AllocateGraphicResource(z_order);
    sprite_ =
        spriteItem_->create_sprite(position_, GetButtonOn() ? BUTTON_ON_GRAPHICS_INDEX : BUTTON_OFF_GRAPHICS_INDEX);
    sprite_->set_z_order(z_order);
    sprite_->set_blending_enabled(isBlendingEnabled_);
    sprite_->set_mosaic_enabled(isMosaicEnabled_);
    sprite_->set_visible(isVisible_);
    sprite_->set_vertical_flip(isOnCeil_);
    if (paletteItem_)
        SetPalette(*paletteItem_);

    AllocateRequiredSymbolPlaceholderGraphic_(z_order);
}

void PressureButton::FreeGraphicResource()
{
    action_.reset();
    requiredSymbolSprite_.reset();
    IButtonEntity::FreeGraphicResource();
}

void PressureButton::Update()
{
    IButtonEntity::Update();
    if (action_ && !action_->done())
        action_->update();
}

void PressureButton::SetBlendingEnabled(bool isEnable)
{
    IButtonEntity::SetBlendingEnabled(isEnable);
    if (requiredSymbolSprite_)
        requiredSymbolSprite_->set_blending_enabled(isEnable);
}

void PressureButton::SetMosaicEnabled(bool isEnable)
{
    IButtonEntity::SetMosaicEnabled(isEnable);
    if (requiredSymbolSprite_)
        requiredSymbolSprite_->set_mosaic_enabled(isEnable);
}

void PressureButton::SetCamera(const bn::camera_ptr& camera)
{
    IButtonEntity::SetCamera(camera);
    if (requiredSymbolSprite_)
        requiredSymbolSprite_->set_camera(camera);
}

bool PressureButton::CanButtonBeToggled()
{
    return !action_ || action_->done();
}

bool PressureButton::ToggleButtonOn()
{
    const bool isButtonOnBefore = GetButtonOn();
    IButtonEntity::ToggleButtonOn();
    if (isButtonOnBefore)
    {
        if (requiredSymbolSprite_)
            requiredSymbolSprite_->set_visible(true);
        InitButtonOffAction_();
    }
    else
    {
        if (requiredSymbolSprite_)
            requiredSymbolSprite_->set_visible(false);
        InitButtonOnAction_();
    }
    return !isButtonOnBefore;
}

bn::optional<entity::Symbol::Type> PressureButton::GetRequiredSymbolType() const
{
    return requiredSymbolType_;
}

void PressureButton::InitButtonOnAction_()
{
    BN_ASSERT(sprite_, "PressureButton action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES,
                                                    bn::sprite_items::spr_pressure_button.tiles_item(), 1, 2);
}

void PressureButton::InitButtonOffAction_()
{
    BN_ASSERT(sprite_, "PressureButton action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES,
                                                    bn::sprite_items::spr_pressure_button.tiles_item(), 1, 0);
}

void PressureButton::AllocateRequiredSymbolPlaceholderGraphic_(int z_order)
{
    if (requiredSymbolType_)
    {
        const bool isComplex = static_cast<int>(*requiredSymbolType_) >= entity::Symbol::COMPLEX_SYMBOL_START_NUM;
        const int spriteIdx = isComplex
                                  ? static_cast<int>(*requiredSymbolType_) - entity::Symbol::COMPLEX_SYMBOL_START_NUM
                                  : static_cast<int>(*requiredSymbolType_);
        const auto& symbolSpriteItem =
            isComplex ? bn::sprite_items::spr_complex_symbols : bn::sprite_items::spr_basic_symbols;
        requiredSymbolSprite_ =
            symbolSpriteItem.create_sprite(position_ + (isOnCeil_ ? -RELATIVE_REQUIRED_SYMBOL_PLACEHOLDER_POS
                                                                  : RELATIVE_REQUIRED_SYMBOL_PLACEHOLDER_POS),
                                           spriteIdx);
        requiredSymbolSprite_->set_palette(bn::sprite_palette_items::pal_symbol_placeholder);

        requiredSymbolSprite_->set_z_order(z_order);
        requiredSymbolSprite_->set_blending_enabled(isBlendingEnabled_);
        requiredSymbolSprite_->set_mosaic_enabled(isMosaicEnabled_);
        requiredSymbolSprite_->set_visible(!GetButtonOn());
        requiredSymbolSprite_->set_vertical_flip(isOnCeil_);
    }
}

} // namespace sym::game::entity
