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
    helper::rect::MakeFixedRectByTopLeftAndSize({3 - SPRITE_WIDTH / 2, 2 - SPRITE_HEIGHT / 2}, {10, 12});

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

void HoverButton::InitButtonOnAction()
{
    BN_ASSERT(sprite_, "HoverButton action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES,
                                                    bn::sprite_items::spr_hover_button.tiles_item(), 2, 3);
}

void HoverButton::InitButtonOffAction()
{
    BN_ASSERT(sprite_, "HoverButton action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES,
                                                    bn::sprite_items::spr_hover_button.tiles_item(), 1, 0);
}

} // namespace sym::game::entity
