#include "game_entity_Player.h"

#include <bn_assert.h>
#include <bn_sprite_animate_actions.h>

#include "bn_sprite_items_spr_ingame_protagonist_star.h"

namespace sym::game::entity
{

Player::Player(bn::fixed_point position)
    : IGravityEntity(position, RELATIVE_INTERACT_RANGE, RELATIVE_PHYSICS_COLLIDER, IS_GRAVITY_ENABLED_BY_DEFAULT,
                     GRAVITY_SCALE, &bn::sprite_items::spr_ingame_protagonist_star)
{
}

Player::Player(Player&& other) : IGravityEntity(bn::move(other))
{
}

Player& Player::operator=(Player&& other)
{
    IGravityEntity::operator=(bn::move(other));
    return *this;
}

void Player::FreeGraphicResource()
{
    DestroyActions_();
    IGravityEntity::FreeGraphicResource();
}

void Player::Update()
{
    bool isActionDone = true;
    if (action2_)
    {
        if (!action2_->done())
        {
            action2_->update();
            isActionDone = false;
        }
    }
    else if (action3_)
    {
        if (!action3_->done())
        {
            action3_->update();
            isActionDone = false;
        }
    }

    if (isActionDone && additionalWaitUpdateCount >= 0)
    {
        if (additionalWaitUpdateCount-- == 0)
        {
            InitIdleAction();
        }
    }
}

void Player::InitIdleAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    action2_ = bn::create_sprite_animate_action_forever(
        *sprite_, IDLE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 0, 1);
}

void Player::InitJumpAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, IDLE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 2, 3, 4);
}

void Player::InitFallAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    action2_ = bn::create_sprite_animate_action_once(*sprite_, IDLE_ACTION_WAIT_UPDATE,
                                                     bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 5, 6);
}

void Player::InitLandAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, IDLE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 7, 8, 0);
    additionalWaitUpdateCount = OTHER_ACTIONS_WAIT_UPDATE;
}

void Player::InitMergeStartAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, IDLE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 9, 10, 11);
}

void Player::InitMergeEndAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, IDLE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 10, 9, 0);
    additionalWaitUpdateCount = OTHER_ACTIONS_WAIT_UPDATE;
}

void Player::DestroyActions_()
{
    additionalWaitUpdateCount = -1;
    action2_.reset();
    action3_.reset();
}

} // namespace sym::game::entity
