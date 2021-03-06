#include "game_entity_Door.h"

#include <bn_assert.h>
#include <bn_sprite_animate_actions.h>

#include "helper_rect.h"

#include "bn_sprite_items_spr_door.h"

namespace sym::game::entity
{

namespace
{

constexpr bn::fixed SPRITE_WIDTH = bn::sprite_items::spr_door.shape_size().width();
constexpr bn::fixed SPRITE_HEIGHT = bn::sprite_items::spr_door.shape_size().height();

constexpr int WAIT_UPDATES = 15;

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({2 - SPRITE_WIDTH / 2, 1 - SPRITE_HEIGHT / 2}, {28, 30});

constexpr bn::fixed_point RELATIVE_NUMBER_TEXT_POS = {0, -23};

constexpr int OPENED_GRAPHICS_INDEX = 3;
constexpr int CLOSED_GRAPHICS_INDEX = 0;

} // namespace

Door::Door(bn::fixed_point position, bool isOpenedByDefault, int textNumber)
    : IOpenableEntity(position, RELATIVE_INTERACT_RANGE, textNumber, RELATIVE_NUMBER_TEXT_POS, isOpenedByDefault,
                      &bn::sprite_items::spr_door)
{
}

void Door::AllocateGraphicResource(int z_order)
{
    IOpenableEntity::AllocateGraphicResource(z_order);
    sprite_ = spriteItem_->create_sprite(position_, GetOpened() ? OPENED_GRAPHICS_INDEX : CLOSED_GRAPHICS_INDEX);
    sprite_->set_z_order(z_order);
    sprite_->set_blending_enabled(isBlendingEnabled_);
    sprite_->set_mosaic_enabled(isMosaicEnabled_);
    sprite_->set_visible(isVisible_);
    if (paletteItem_)
        SetPalette(*paletteItem_);
}

void Door::FreeGraphicResource()
{
    action_.reset();
    IOpenableEntity::FreeGraphicResource();
}

void Door::Update()
{
    IOpenableEntity::Update();
    if (action_ && !action_->done())
        action_->update();
}

bool Door::ToggleOpened()
{
    const bool isOpenedBefore = GetOpened();
    IOpenableEntity::ToggleOpened();
    if (isOpenedBefore)
        InitDoorCloseAction_();
    else
        InitDoorOpenAction_();
    return !isOpenedBefore;
}

void Door::InitDoorOpenAction_()
{
    BN_ASSERT(sprite_, "Door action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES, bn::sprite_items::spr_door.tiles_item(), 0,
                                                    1, 2, 3);
}

void Door::InitDoorCloseAction_()
{
    BN_ASSERT(sprite_, "Door action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES, bn::sprite_items::spr_door.tiles_item(), 3,
                                                    2, 1, 0);
}

} // namespace sym::game::entity
