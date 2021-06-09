#include "game_entity_PressureButton.h"

#include "helper_rect.h"

#include "bn_sprite_items_spr_pressure_button.h"

namespace sym::game::entity
{

namespace
{

constexpr bn::fixed SPRITE_WIDTH = bn::sprite_items::spr_pressure_button.shape_size().width();
constexpr bn::fixed SPRITE_HEIGHT = bn::sprite_items::spr_pressure_button.shape_size().height();

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({1 - SPRITE_WIDTH / 2, 10 - SPRITE_HEIGHT / 2}, {15, 6});

constexpr bn::fixed_point RELATIVE_NUMBER_TEXT_POS = {1, 16};

constexpr int WAIT_UPDATES = 15;

constexpr int BUTTON_ON_GRAPHICS_INDEX = 2;
constexpr int BUTTON_OFF_GRAPHICS_INDEX = 0;

} // namespace

PressureButton::PressureButton(bn::fixed_point position, int textNumber, bool isOnByDefault)
    : IButtonEntity(position, RELATIVE_INTERACT_RANGE, textNumber, RELATIVE_NUMBER_TEXT_POS, isOnByDefault,
                    &bn::sprite_items::spr_pressure_button)
{
}

void PressureButton::AllocateGraphicResource(int z_order)
{
    IButtonEntity::AllocateGraphicResource(z_order);
    sprite_ =
        spriteItem_->create_sprite(position_, GetButtonOn() ? BUTTON_ON_GRAPHICS_INDEX : BUTTON_OFF_GRAPHICS_INDEX);
    sprite_->set_z_order(z_order);
}

void PressureButton::FreeGraphicResource()
{
    action_.reset();
    IButtonEntity::FreeGraphicResource();
}

void PressureButton::Update()
{
    IButtonEntity::Update();
    if (action_ && !action_->done())
        action_->update();
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
        InitButtonOffAction_();
    else
        InitButtonOnAction_();
    return !isButtonOnBefore;
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

} // namespace sym::game::entity
