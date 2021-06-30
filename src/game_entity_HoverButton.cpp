#include "game_entity_HoverButton.h"

#include "helper_rect.h"

#include "bn_sprite_items_spr_hover_button.h"

namespace sym::game::entity
{

namespace
{

constexpr bn::fixed SPRITE_WIDTH = bn::sprite_items::spr_hover_button.shape_size().width();
constexpr bn::fixed SPRITE_HEIGHT = bn::sprite_items::spr_hover_button.shape_size().height();

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({0 - SPRITE_WIDTH / 2, -1 - SPRITE_HEIGHT / 2}, {16, 18});

constexpr bn::fixed_point RELATIVE_NUMBER_TEXT_POS = {0, -13};

constexpr int WAIT_UPDATES = 15;

constexpr int BUTTON_ON_GRAPHICS_INDEX = 3;
constexpr int BUTTON_OFF_GRAPHICS_INDEX = 0;

} // namespace

HoverButton::HoverButton(bn::fixed_point position, int textNumber, bool isOnByDefault)
    : IButtonEntity(position, RELATIVE_INTERACT_RANGE, textNumber, RELATIVE_NUMBER_TEXT_POS, isOnByDefault,
                    &bn::sprite_items::spr_hover_button)
{
}

void HoverButton::AllocateGraphicResource(int z_order)
{
    IButtonEntity::AllocateGraphicResource(z_order);
    sprite_ =
        spriteItem_->create_sprite(position_, GetButtonOn() ? BUTTON_ON_GRAPHICS_INDEX : BUTTON_OFF_GRAPHICS_INDEX);
    sprite_->set_z_order(z_order);
    sprite_->set_blending_enabled(isBlendingEnabled_);
    sprite_->set_mosaic_enabled(isMosaicEnabled_);
    sprite_->set_visible(isVisible_);
    if (paletteItem_)
        SetPalette(*paletteItem_);
}

void HoverButton::FreeGraphicResource()
{
    action_.reset();
    IButtonEntity::FreeGraphicResource();
}

void HoverButton::Update()
{
    IButtonEntity::Update();
    if (action_ && !action_->done())
        action_->update();
}

bool HoverButton::CanButtonBeToggled()
{
    return !action_ || action_->done();
}

bool HoverButton::ToggleButtonOn()
{
    const bool isButtonOnBefore = GetButtonOn();
    IButtonEntity::ToggleButtonOn();
    if (isButtonOnBefore)
        InitButtonOffAction_();
    else
        InitButtonOnAction_();
    return !isButtonOnBefore;
}

void HoverButton::InitButtonOnAction_()
{
    BN_ASSERT(sprite_, "HoverButton action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES,
                                                    bn::sprite_items::spr_hover_button.tiles_item(), 2, 3);
}

void HoverButton::InitButtonOffAction_()
{
    BN_ASSERT(sprite_, "HoverButton action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES,
                                                    bn::sprite_items::spr_hover_button.tiles_item(), 1, 0);
}

} // namespace sym::game::entity
