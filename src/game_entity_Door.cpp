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
    helper::rect::MakeFixedRectByTopLeftAndSize({5 - SPRITE_WIDTH / 2, 4 - SPRITE_HEIGHT / 2}, {22, 24});

constexpr bn::fixed_point RELATIVE_NUMBER_TEXT_POS = {0, -20};

} // namespace

Door::Door(bn::fixed_point position, bool isOpened, int textNumber)
    : IOpenableEntity(position, RELATIVE_INTERACT_RANGE, textNumber, RELATIVE_NUMBER_TEXT_POS, isOpened,
                      &bn::sprite_items::spr_door)
{
    // TODO: draw door graphic asset
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

void Door::InitDoorOpenAction()
{
    BN_ASSERT(sprite_, "Door action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES, bn::sprite_items::spr_door.tiles_item(), 0,
                                                    1, 2, 3);
}

void Door::InitDoorCloseAction()
{
    BN_ASSERT(sprite_, "Door action cannot be init without allocating graphics!");
    action_ = bn::create_sprite_animate_action_once(*sprite_, WAIT_UPDATES, bn::sprite_items::spr_door.tiles_item(), 3,
                                                    2, 1, 0);
}

} // namespace sym::game::entity
