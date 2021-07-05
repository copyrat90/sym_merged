#include "game_entity_Player.h"

#include <bn_assert.h>
#include <bn_sound.h>
#include <bn_sprite_animate_actions.h>
#include <bn_utility.h>

#include "bn_sprite_items_spr_ingame_protagonist_star.h"
#include "constant.h"
#include "global.h"
#include "helper_math.h"
#include "helper_rect.h"
#include "scene_GameState.h"

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

constexpr bn::fixed_rect RELATIVE_PHYSICS_COLLIDER = {{0, 0}, {26, 26}};
constexpr bool IS_GRAVITY_ENABLED_BY_DEFAULT = true;
constexpr bn::fixed GRAVITY_SCALE = 0.1;

constexpr int IDLE_ACTION_WAIT_UPDATE = 30;
constexpr int OTHER_ACTIONS_WAIT_UPDATE = 5;
constexpr int MERGE_ACTION_WAIT_UPDATE = OTHER_ACTIONS_WAIT_UPDATE;
constexpr int MERGE_START_TO_MERGED_UPDATE_COUNT = 90;
constexpr int MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT = 3 * MERGE_ACTION_WAIT_UPDATE;
constexpr int WAIT_UPDATE_BETWEEN_MERGE_START_AND_END =
    MERGE_START_TO_MERGED_UPDATE_COUNT + MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT - MERGE_ACTION_WAIT_UPDATE;

constexpr bn::fixed_point LEFT_SYMBOL_MERGE_END_POS = {-1, -24};
constexpr bn::fixed_point RIGHT_SYMBOL_MERGE_END_POS = {1, -24};
constexpr bn::fixed_point LEFT_SYMBOL_MERGE_START_POS = {-12, -3};
constexpr bn::fixed_point RIGHT_SYMBOL_MERGE_START_POS = {12, -3};
constexpr bn::fixed_point LEFT_SYMBOL_MERGE_POS_DELTA = {
    (LEFT_SYMBOL_MERGE_END_POS.x() - LEFT_SYMBOL_MERGE_START_POS.x()) / MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT,
    (LEFT_SYMBOL_MERGE_END_POS.y() - LEFT_SYMBOL_MERGE_START_POS.y()) / MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT};
constexpr bn::fixed_point RIGHT_SYMBOL_MERGE_POS_DELTA = {
    (RIGHT_SYMBOL_MERGE_END_POS.x() - RIGHT_SYMBOL_MERGE_START_POS.x()) / MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT,
    (RIGHT_SYMBOL_MERGE_END_POS.y() - RIGHT_SYMBOL_MERGE_START_POS.y()) / MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT};

constexpr bn::fixed_point LEFT_SYMBOL_MERGE_END_POS_REVERSED = {LEFT_SYMBOL_MERGE_END_POS.x(),
                                                                -LEFT_SYMBOL_MERGE_END_POS.y()};
constexpr bn::fixed_point RIGHT_SYMBOL_MERGE_END_POS_REVERSED = {RIGHT_SYMBOL_MERGE_END_POS.x(),
                                                                 -RIGHT_SYMBOL_MERGE_END_POS.y()};
constexpr bn::fixed_point LEFT_SYMBOL_MERGE_START_POS_REVERSED = {LEFT_SYMBOL_MERGE_START_POS.x(),
                                                                  -LEFT_SYMBOL_MERGE_START_POS.y()};
constexpr bn::fixed_point RIGHT_SYMBOL_MERGE_START_POS_REVERSED = {RIGHT_SYMBOL_MERGE_START_POS.x(),
                                                                   -RIGHT_SYMBOL_MERGE_START_POS.y()};
constexpr bn::fixed_point LEFT_SYMBOL_MERGE_POS_DELTA_REVERSED = {
    (LEFT_SYMBOL_MERGE_END_POS_REVERSED.x() - LEFT_SYMBOL_MERGE_START_POS_REVERSED.x()) /
        MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT,
    (LEFT_SYMBOL_MERGE_END_POS_REVERSED.y() - LEFT_SYMBOL_MERGE_START_POS_REVERSED.y()) /
        MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT};
constexpr bn::fixed_point RIGHT_SYMBOL_MERGE_POS_DELTA_REVERSED = {
    (RIGHT_SYMBOL_MERGE_END_POS_REVERSED.x() - RIGHT_SYMBOL_MERGE_START_POS_REVERSED.x()) /
        MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT,
    (RIGHT_SYMBOL_MERGE_END_POS_REVERSED.y() - RIGHT_SYMBOL_MERGE_START_POS_REVERSED.y()) /
        MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT};

constexpr int DIVERGE_RESET_UPDATE_COUNT = 2;

} // namespace

Player::Player(bn::fixed_point position, bool isGravityReversedByDefault, scene::GameState& gameState)
    : IPhysicsEntity(position, RELATIVE_INTERACT_RANGE, RELATIVE_PHYSICS_COLLIDER, IS_GRAVITY_ENABLED_BY_DEFAULT,
                     isGravityReversedByDefault, GRAVITY_SCALE, &bn::sprite_items::spr_ingame_protagonist_star),
      gameState_(gameState)
{
}

void Player::FreeGraphicResource()
{
    DestroyAnimation_();
    IPhysicsEntity::FreeGraphicResource();
}

void Player::AllocateGraphicResource(int z_order)
{
    IPhysicsEntity::AllocateGraphicResource(z_order);
    if (isGravityReversed_)
        sprite_->set_vertical_flip(true);
}

void Player::Update()
{
    IPhysicsEntity::Update();

    bool isAnimationDone = UpdateAnimation_();

    if (animationState_ == AnimationState::MERGE_START)
    {
        if (mergePosDeltaCounter_ < MERGE_SYMBOL_POS_MOVE_UPDATE_COUNT)
            ++mergePosDeltaCounter_;
        else
        {
            if (++divergeResetCounter_ > DIVERGE_RESET_UPDATE_COUNT)
            {
                diverge_ = {static_cast<int>(global::GetRandomNumber() % 3) - 1,
                            static_cast<int>(global::GetRandomNumber() % 3) - 1};
                divergeResetCounter_ = 0;
            }
        }
    }
    else if (animationState_ == AnimationState::MERGE_END)
    {
        if (mergePosDeltaCounter_ > 0)
            --mergePosDeltaCounter_;
    }

    if (isAnimationDone && additionalWaitUpdateCount_ >= 0)
    {
        if (additionalWaitUpdateCount_-- == 0)
        {
            if (animationState_ == AnimationState::MERGE_START)
            {
                if (gameState_.symbolsInHands[0] && gameState_.symbolsInHands[1])
                    MergeSymbolsInHands_();
                else if ((gameState_.symbolsInHands[0] && !gameState_.symbolsInHands[1]) ||
                         (!gameState_.symbolsInHands[0] && gameState_.symbolsInHands[1]))
                    SplitSymbolsInHands_();
                else
                    BN_ERROR("No symbols in hands but merge or split actions ongoing");
                InitMergeEndAction();
            }
            else
            {
                InitIdleAction();
            }
        }
    }
}

void Player::InitIdleAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    if (animationState_ == AnimationState::MERGE_START)
        bn::sound::stop_all();
    DestroyAnimation_();
    ResetMergePosDeltaCounter_();
    animationState_ = AnimationState::IDLE;
    spriteAnimation_ = bn::sprite_animate_action<ANIMATION_MAX_FRAMES>::forever(
        *sprite_, IDLE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(),
        bn::array<uint16_t, 2>{0, 1});
}

void Player::InitJumpAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    if (animationState_ == AnimationState::MERGE_START)
        bn::sound::stop_all();
    DestroyAnimation_();
    ResetMergePosDeltaCounter_();
    animationState_ = AnimationState::JUMP;
    spriteAnimation_ = bn::create_sprite_animate_action_once(
        *sprite_, OTHER_ACTIONS_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 2, 3, 4);
}

void Player::InitFallAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    if (animationState_ == AnimationState::MERGE_START)
        bn::sound::stop_all();
    DestroyAnimation_();
    ResetMergePosDeltaCounter_();
    animationState_ = AnimationState::FALL;
    spriteAnimation_ = bn::sprite_animate_action<ANIMATION_MAX_FRAMES>::once(
        *sprite_, OTHER_ACTIONS_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(),
        bn::array<uint16_t, 2>{5, 6});
}

void Player::InitLandAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    if (animationState_ == AnimationState::MERGE_START)
        bn::sound::stop_all();
    DestroyAnimation_();
    ResetMergePosDeltaCounter_();
    animationState_ = AnimationState::LAND;
    spriteAnimation_ = bn::create_sprite_animate_action_once(
        *sprite_, OTHER_ACTIONS_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 7, 8, 0);
    additionalWaitUpdateCount_ = OTHER_ACTIONS_WAIT_UPDATE;
}

void Player::InitMergeStartAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyAnimation_();
    animationState_ = AnimationState::MERGE_START;
    spriteAnimation_ = bn::create_sprite_animate_action_once(
        *sprite_, MERGE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 9, 10, 11);
    additionalWaitUpdateCount_ = WAIT_UPDATE_BETWEEN_MERGE_START_AND_END;
}

void Player::InitMergeEndAction()
{
    BN_ASSERT(sprite_, "Player action cannot be init without allocating graphics!");
    DestroyAnimation_();
    animationState_ = AnimationState::MERGE_END;
    spriteAnimation_ = bn::create_sprite_animate_action_once(
        *sprite_, MERGE_ACTION_WAIT_UPDATE, bn::sprite_items::spr_ingame_protagonist_star.tiles_item(), 10, 9, 0);
    additionalWaitUpdateCount_ = MERGE_ACTION_WAIT_UPDATE;
}

void Player::SetGravityReversed(bool isGravityReversed)
{
    IPhysicsEntity::SetGravityReversed(isGravityReversed);
    if (sprite_)
        sprite_->set_vertical_flip(isGravityReversed);
}

bool Player::ToggleGravityReversed()
{
    IPhysicsEntity::ToggleGravityReversed();
    if (sprite_)
        sprite_->set_vertical_flip(isGravityReversed_);
    return isGravityReversed_;
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
    if (animationState_ == AnimationState::MERGE_START || animationState_ == AnimationState::MERGE_END)
    {
        using helper::math::operator*;
        if (isGravityReversed_)
            return resultPos + LEFT_SYMBOL_MERGE_START_POS_REVERSED +
                   mergePosDeltaCounter_ * LEFT_SYMBOL_MERGE_POS_DELTA_REVERSED + diverge_;
        else
            return resultPos + LEFT_SYMBOL_MERGE_START_POS + mergePosDeltaCounter_ * LEFT_SYMBOL_MERGE_POS_DELTA +
                   diverge_;
    }
    resultPos.set_x(resultPos.x() + RELATIVE_LEFT_SYMBOL_X_POS);
    const bool leftIsFront = GetHorizontalFlip();
    AddRelativeYPos_(resultPos, leftIsFront);
    return resultPos;
}

bn::fixed_point Player::GetRightSymbolPosition() const
{
    bn::fixed_point resultPos = position_;
    if (animationState_ == AnimationState::MERGE_START || animationState_ == AnimationState::MERGE_END)
    {
        using helper::math::operator*;

        if (isGravityReversed_)
            return resultPos + RIGHT_SYMBOL_MERGE_START_POS_REVERSED +
                   mergePosDeltaCounter_ * RIGHT_SYMBOL_MERGE_POS_DELTA_REVERSED + diverge_;
        else
            return resultPos + RIGHT_SYMBOL_MERGE_START_POS + mergePosDeltaCounter_ * RIGHT_SYMBOL_MERGE_POS_DELTA +
                   diverge_;
    }
    resultPos.set_x(resultPos.x() + RELATIVE_RIGHT_SYMBOL_X_POS);
    const bool rightIsFront = !GetHorizontalFlip();
    AddRelativeYPos_(resultPos, rightIsFront);
    return resultPos;
}

bn::fixed_point Player::GetMergeSymbolPosition() const
{
    return position_ + (isGravityReversed_ ? -LEFT_SYMBOL_MERGE_END_POS : LEFT_SYMBOL_MERGE_END_POS);
}

bool Player::GetControllable() const
{
    return isControllable_;
}

void Player::SetControllable(bool isControllable)
{

    isControllable_ = isControllable;
}

Player::AnimationState Player::GetAnimationState() const
{
    return animationState_;
}

bool Player::GetAnimationDone() const
{
    if (spriteAnimation_)
        return spriteAnimation_->done();
    BN_ERROR("GetAnimationDone() is called when there is no animation");
    return true;
}

bn::fixed_point Player::GetLastSafePosition() const
{
    return lastSafePosition_;
}

void Player::SetLastSafePosition(bn::fixed_point safePosition)
{
    lastSafePosition_ = safePosition;
}

bool Player::GetLastSafeGravityIsReversed() const
{
    return lastSafeGravityIsReversed_;
}

void Player::SetLastSafeGravityIsReversed(bool lastSafeGravityIsReversed)
{
    lastSafeGravityIsReversed_ = lastSafeGravityIsReversed;
}

bool Player::UpdateAnimation_()
{
    bool isActionDone = true;
    if (spriteAnimation_)
    {
        if (!spriteAnimation_->done())
        {
            spriteAnimation_->update();
            isActionDone = false;
        }
    }
    return isActionDone;
}

void Player::DestroyAnimation_()
{
    additionalWaitUpdateCount_ = -1;
    spriteAnimation_.reset();
}

void Player::AddRelativeYPos_(bn::fixed_point& resultPos, bool isFront) const
{
    switch (animationState_)
    {
    case AnimationState::IDLE:
        if (isFront)
            if (isGravityReversed_)
                resultPos.set_y(resultPos.y() - IDLE_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
            else
                resultPos.set_y(resultPos.y() + IDLE_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else if (isGravityReversed_)
            resultPos.set_y(resultPos.y() - IDLE_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + IDLE_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        break;
    case AnimationState::JUMP:
        if (isFront)
            if (isGravityReversed_)
                resultPos.set_y(resultPos.y() - JUMP_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
            else
                resultPos.set_y(resultPos.y() + JUMP_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else if (isGravityReversed_)
            resultPos.set_y(resultPos.y() - JUMP_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + JUMP_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        break;
    case AnimationState::FALL:
        if (isFront)
            if (isGravityReversed_)
                resultPos.set_y(resultPos.y() - FALL_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
            else
                resultPos.set_y(resultPos.y() + FALL_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else if (isGravityReversed_)
            resultPos.set_y(resultPos.y() - FALL_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + FALL_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        break;
    case AnimationState::LAND:
        if (isFront)
            if (isGravityReversed_)
                resultPos.set_y(resultPos.y() - LAND_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
            else
                resultPos.set_y(resultPos.y() + LAND_FRONT_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else if (isGravityReversed_)
            resultPos.set_y(resultPos.y() - LAND_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        else
            resultPos.set_y(resultPos.y() + LAND_BACK_SYMBOL_Y_POSITIONS[spriteAnimation_->current_index()]);
        break;
    default:
        BN_ERROR("Invalid Player::AnimationState : ", static_cast<int>(animationState_));
        break;
    }
}

void Player::MergeSymbolsInHands_()
{
    BN_ASSERT(gameState_.symbolsInHands[0] && gameState_.symbolsInHands[1],
              "Player::MergeSymbolsInHands_() called without holding 2 symbols");
    const auto mergedSymbol = constant::symbol::GetMergedSymbolType(gameState_.symbolsInHands[0]->GetType(),
                                                                    gameState_.symbolsInHands[1]->GetType());
    gameState_.symbolsInHands[0].reset();
    gameState_.symbolsInHands[1] =
        Symbol(position_ + ((isGravityReversed_) ? -LEFT_SYMBOL_MERGE_END_POS : LEFT_SYMBOL_MERGE_END_POS),
               mergedSymbol, gameState_.player.isGravityReversed_);
    gameState_.symbolsInHands[1]->AllocateGraphicResource(constant::SYMBOL_Z_ORDER);
    gameState_.symbolsInHands[1]->SetCamera(gameState_.camera);
}

void Player::SplitSymbolsInHands_()
{
    bool isRightHand = false;
    if (gameState_.symbolsInHands[0] && !gameState_.symbolsInHands[1])
        isRightHand = false;
    else if (gameState_.symbolsInHands[1] && !gameState_.symbolsInHands[0])
        isRightHand = true;
    else
        BN_ERROR("Player::SplitSymbolsInHands_() called without holding 1 symbol");

    auto syms = constant::symbol::GetSplitSymbolTypes(gameState_.symbolsInHands[isRightHand]->GetType());
    gameState_.symbolsInHands[0] =
        Symbol(position_ + (isGravityReversed_ ? -LEFT_SYMBOL_MERGE_END_POS : LEFT_SYMBOL_MERGE_END_POS), syms.first,
               gameState_.player.isGravityReversed_);
    gameState_.symbolsInHands[1] =
        Symbol(position_ + (isGravityReversed_ ? -RIGHT_SYMBOL_MERGE_END_POS : RIGHT_SYMBOL_MERGE_END_POS), syms.second,
               gameState_.player.isGravityReversed_);
    gameState_.symbolsInHands[0]->AllocateGraphicResource(constant::SYMBOL_Z_ORDER);
    gameState_.symbolsInHands[0]->SetCamera(gameState_.camera);
    gameState_.symbolsInHands[1]->AllocateGraphicResource(constant::SYMBOL_Z_ORDER);
    gameState_.symbolsInHands[1]->SetCamera(gameState_.camera);
}

void Player::ResetMergePosDeltaCounter_()
{
    mergePosDeltaCounter_ = 0;
}

} // namespace sym::game::entity
