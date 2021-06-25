#include "game_entity_Player.h"

#include <bn_assert.h>
#include <bn_sprite_animate_actions.h>

#include "bn_sprite_items_spr_ingame_protagonist_star.h"
#include "helper_rect.h"

namespace sym::game::entity
{

namespace
{

constexpr int SPRITE_WIDTH = bn::sprite_items::spr_ingame_protagonist_star.shape_size().width();
constexpr int SPRITE_HEIGHT = bn::sprite_items::spr_ingame_protagonist_star.shape_size().height();

constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE = {{0, 0}, {32, 32}};
constexpr bn::fixed_rect RELATIVE_LEFT_SYMBOL_PICKUP_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({-4 - SPRITE_WIDTH / 2, 3 - SPRITE_HEIGHT / 2}, {27, 26});
constexpr bn::fixed_rect RELATIVE_RIGHT_SYMBOL_PICKUP_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({9 - SPRITE_WIDTH / 2, 3 - SPRITE_HEIGHT / 2}, {27, 26});
constexpr bn::fixed_rect RELATIVE_LEFT_BUTTON_INTERACT_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({-1 - SPRITE_WIDTH / 2, 6 - SPRITE_HEIGHT / 2}, {15, 17});
constexpr bn::fixed_rect RELATIVE_RIGHT_BUTTON_INTERACT_RANGE =
    helper::rect::MakeFixedRectByTopLeftAndSize({18 - SPRITE_WIDTH / 2, 6 - SPRITE_HEIGHT / 2}, {15, 17});

constexpr bn::fixed UNUSED = 0;
constexpr bn::fixed RELATIVE_LEFT_SYMBOL_X_POS = -12;
constexpr bn::fixed RELATIVE_RIGHT_SYMBOL_X_POS = 12;
// index 0 is the final index for animate_action_forever
constexpr bn::fixed IDLE_FRONT_SYMBOL_Y_POSITIONS[] = {-2, -3};
constexpr bn::fixed IDLE_BACK_SYMBOL_Y_POSITIONS[] = {-1, -2};
// index 0 is unused for animate_action_once
constexpr bn::fixed JUMP_FRONT_SYMBOL_Y_POSITIONS[] = {UNUSED, 0, -3, -4};
constexpr bn::fixed JUMP_BACK_SYMBOL_Y_POSITIONS[] = {UNUSED, 1, -2, -2};
constexpr bn::fixed FALL_FRONT_SYMBOL_Y_POSITIONS[] = {UNUSED, -2, -3};
constexpr bn::fixed FALL_BACK_SYMBOL_Y_POSITIONS[] = {UNUSED, -2, -4};
constexpr bn::fixed LAND_FRONT_SYMBOL_Y_POSITIONS[] = {UNUSED, -1, 0, IDLE_FRONT_SYMBOL_Y_POSITIONS[1]};
constexpr bn::fixed LAND_BACK_SYMBOL_Y_POSITIONS[] = {UNUSED, -2, 1, IDLE_BACK_SYMBOL_Y_POSITIONS[1]};

constexpr bn::fixed_point RELATIVE_MERGE_SYMBOL_POS = {0, -21};

constexpr bn::fixed_rect RELATIVE_PHYSICS_COLLIDER = {{0, 0}, {26, 26}};
constexpr bool IS_GRAVITY_ENABLED_BY_DEFAULT = true;
constexpr bn::fixed GRAVITY_SCALE = 0.1;

constexpr int IDLE_ACTION_WAIT_UPDATE = 30;
constexpr int OTHER_ACTIONS_WAIT_UPDATE = 5;

} // namespace

Player::Player(bn::fixed_point position)
    : IPhysicsEntity(position, RELATIVE_INTERACT_RANGE, RELATIVE_PHYSICS_COLLIDER, IS_GRAVITY_ENABLED_BY_DEFAULT,
                     GRAVITY_SCALE, &bn::sprite_items::spr_ingame_protagonist_star)
{
}

void Player::FreeGraphicResource()
{
    DestroyActions_();
    IPhysicsEntity::FreeGraphicResource();
}

void Player::Update()
{
    IPhysicsEntity::Update();

    bool isActionDone = UpdateAction_();

    if (isActionDone && additionalWaitUpdateCount_ >= 0)
    {
        if (additionalWaitUpdateCount_-- == 0)
        {
            InitIdleAction();
        }
    }
}

void Player::InitIdleAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    actionState_ = ActionState::IDLE;
    action2_ = bn::create_sprite_animate_action_forever(
        *sprite_, IDLE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 0, 1);
}

void Player::InitJumpAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    actionState_ = ActionState::JUMP;
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, OTHER_ACTIONS_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 2, 3, 4);
}

void Player::InitFallAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    actionState_ = ActionState::FALL;
    action2_ = bn::create_sprite_animate_action_once(*sprite_, OTHER_ACTIONS_WAIT_UPDATE,
                                                     bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 5, 6);
}

void Player::InitLandAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    actionState_ = ActionState::LAND;
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, OTHER_ACTIONS_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 7, 8, 0);
    additionalWaitUpdateCount_ = OTHER_ACTIONS_WAIT_UPDATE;
}

void Player::InitMergeStartAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    actionState_ = ActionState::MERGE_START;
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, OTHER_ACTIONS_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 9, 10, 11);
}

void Player::InitMergeEndAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyActions_();
    actionState_ = ActionState::MERGE_END;
    action3_ = bn::create_sprite_animate_action_once(
        *sprite_, OTHER_ACTIONS_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 10, 9, 0);
    additionalWaitUpdateCount_ = OTHER_ACTIONS_WAIT_UPDATE;
}

bn::fixed_rect Player::GetLeftSymbolPickupRange() const
{
    using helper::rect::operator+;
    return position_ + RELATIVE_LEFT_SYMBOL_PICKUP_RANGE;
}

bn::fixed_rect Player::GetRightSymbolPickupRange() const
{
    using helper::rect::operator+;
    return position_ + RELATIVE_RIGHT_SYMBOL_PICKUP_RANGE;
}

bn::fixed_rect Player::GetLeftButtonInteractRange() const
{
    using helper::rect::operator+;
    return position_ + RELATIVE_LEFT_BUTTON_INTERACT_RANGE;
}

bn::fixed_rect Player::GetRightButtonInteractRange() const
{
    using helper::rect::operator+;
    return position_ + RELATIVE_RIGHT_BUTTON_INTERACT_RANGE;
}

bn::fixed_point Player::GetLeftSymbolPosition() const
{
    bn::fixed_point resultPos = position_;
    resultPos.set_x(resultPos.x() + RELATIVE_LEFT_SYMBOL_X_POS);
    const bool leftIsFront = GetHorizontalFlip();
    AddRelativeYPos_(resultPos, leftIsFront);
    return resultPos;
}

bn::fixed_point Player::GetRightSymbolPosition() const
{
    bn::fixed_point resultPos = position_;
    resultPos.set_x(resultPos.x() + RELATIVE_RIGHT_SYMBOL_X_POS);
    const bool rightIsFront = !GetHorizontalFlip();
    AddRelativeYPos_(resultPos, rightIsFront);
    return resultPos;
}

bn::fixed_point Player::GetMergeSymbolPosition() const
{
    return position_ + RELATIVE_MERGE_SYMBOL_POS;
}

bool Player::GetControllable() const
{
    return isControllable_;
}

void Player::SetControllable(bool isControllable)
{
    isControllable_ = isControllable;
}

Player::ActionState Player::GetActionState() const
{
    return actionState_;
}

bool Player::UpdateAction_()
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
    return isActionDone;
}

void Player::DestroyActions_()
{
    additionalWaitUpdateCount_ = -1;
    action2_.reset();
    action3_.reset();
}

void Player::AddRelativeYPos_(bn::fixed_point& resultPos, bool isFront) const
{
    switch (actionState_)
    {
    case ActionState::IDLE:
        if (isFront)
            resultPos.set_y(resultPos.y() + IDLE_FRONT_SYMBOL_Y_POSITIONS[action2_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + IDLE_BACK_SYMBOL_Y_POSITIONS[action2_->current_index()]);
        break;
    case ActionState::JUMP:
        if (isFront)
            resultPos.set_y(resultPos.y() + JUMP_FRONT_SYMBOL_Y_POSITIONS[action3_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + JUMP_BACK_SYMBOL_Y_POSITIONS[action3_->current_index()]);
        break;
    case ActionState::FALL:
        if (isFront)
            resultPos.set_y(resultPos.y() + FALL_FRONT_SYMBOL_Y_POSITIONS[action2_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + FALL_BACK_SYMBOL_Y_POSITIONS[action2_->current_index()]);
        break;
    case ActionState::LAND:
        if (isFront)
            resultPos.set_y(resultPos.y() + LAND_FRONT_SYMBOL_Y_POSITIONS[action3_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + LAND_BACK_SYMBOL_Y_POSITIONS[action3_->current_index()]);
        break;
        break;
    case ActionState::MERGE_START:
    case ActionState::MERGE_END:
        BN_ERROR("Merge Symbol Position not implemented!");
        break;
    default:
        BN_ERROR("Invalid Player::ActionState : ", static_cast<int>(actionState_));
        break;
    }
}

} // namespace sym::game::entity
