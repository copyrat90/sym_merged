#include "game_system_PhysicsMovement.h"

#include <bn_affine_bg_map_ptr.h>
#include <bn_assert.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_math.h>
#include <bn_optional.h>

#include "helper_keypad.h"
#include "helper_math.h"
#include "helper_tilemap.h"

#include "bn_sound_items.h"
#include "constant.h"

namespace sym::game::system
{

namespace
{

constexpr bn::fixed_point MAX_PLAYER_VEL = {2, 3};
constexpr bn::fixed_point PLAYER_DELTA_VEL = {0.3, 0.3};
constexpr bn::fixed JUMP_VEL = -2.8;
constexpr bn::fixed_point MAX_SYMBOL_VEL = {3, 3};
constexpr bn::fixed_point SYMBOL_DELTA_VEL = {0.3, 0.3};
constexpr bn::fixed VEL_FRICTION = 1.2;
constexpr bn::fixed COLLISION_DELTA_EPSILON = 0.01;

static_assert(bn::abs(JUMP_VEL) <= MAX_PLAYER_VEL.y());

enum class PushbackDirection
{
    UP,
    DOWN,
    LEFT,
    RIGHT
};

[[nodiscard]] bn::optional<PushbackDirection> FloorCollisionResolution_(entity::IPhysicsEntity& entity)
{
    using namespace helper::math;
    const bn::fixed dy = -(entity.GetPhysicsCollider().bottom() % 8) - COLLISION_DELTA_EPSILON;
    entity.SetY(entity.GetY() + dy);
    entity.SetYVelocity(0);
    return PushbackDirection::UP;
}

[[nodiscard]] bn::optional<PushbackDirection> LeftWallCollisionResolution_(entity::IPhysicsEntity& entity)
{
    using namespace helper::math;
    const bn::fixed dx = 8 - (entity.GetPhysicsCollider().left() % 8) + COLLISION_DELTA_EPSILON;
    entity.SetX(entity.GetX() + dx);
    entity.SetXVelocity(0);
    return PushbackDirection::RIGHT;
}

[[nodiscard]] bn::optional<PushbackDirection> RightWallCollisionResolution_(entity::IPhysicsEntity& entity)
{
    using namespace helper::math;
    const bn::fixed dx = -(entity.GetPhysicsCollider().right() % 8) - COLLISION_DELTA_EPSILON;
    entity.SetX(entity.GetX() + dx);
    entity.SetXVelocity(0);
    return PushbackDirection::LEFT;
}

[[nodiscard]] bn::optional<PushbackDirection> CeilingCollisionResolution_(entity::IPhysicsEntity& entity)
{
    using namespace helper::math;
    const bn::fixed dy = 8 - (entity.GetPhysicsCollider().top() % 8) + COLLISION_DELTA_EPSILON;
    entity.SetY(entity.GetY() + dy);
    entity.SetYVelocity(0);
    return PushbackDirection::DOWN;
}

/**
 * @brief Detect and Resolve colision.
 * You need to call this once again, if if returns value other than `bn::nullopt`.
 *
 */
[[nodiscard]] bn::optional<PushbackDirection> PlatformCollisionResolution_(entity::IPhysicsEntity& entity,
                                                                           const helper::tilemap::TileInfo& mapTileInfo)
{
    using namespace helper::tilemap;
    using namespace helper::math;
    using MoveDirections = entity::IPhysicsEntity::MoveDirections;

    const bn::fixed_rect collider = entity.GetPhysicsCollider();

    const TileInfo::Flags topLeftTileFlag = mapTileInfo.GetTileFlagsByPosition(collider.top_left());
    const TileInfo::Flags topRightTileFlag = mapTileInfo.GetTileFlagsByPosition(collider.top_right());
    const TileInfo::Flags bottomLeftTileFlag = mapTileInfo.GetTileFlagsByPosition(collider.bottom_left());
    const TileInfo::Flags bottomRightTileFlag = mapTileInfo.GetTileFlagsByPosition(collider.bottom_right());

    const MoveDirections moveDirs = entity.GetMoveDirections();

    // diagonal approach to a corner
    if (!!(moveDirs & MoveDirections::DOWN) && !!(moveDirs & MoveDirections::RIGHT))
    {
        if (!!(bottomRightTileFlag & TileInfo::Flags::FLOOR) &&
            !!(bottomRightTileFlag & TileInfo::Flags::RIGHT_BLOCKING_WALL) &&
            !(bottomRightTileFlag & TileInfo::Flags::SPIKE))
        {
            const bn::fixed cellX = collider.right() % 8;
            const bn::fixed cellY = collider.bottom() % 8;
            if (cellX >= cellY)
                return FloorCollisionResolution_(entity);
            else
                return RightWallCollisionResolution_(entity);
        }
    }
    else if (!!(moveDirs & MoveDirections::DOWN) && !!(moveDirs & MoveDirections::LEFT))
    {
        if (!!(bottomLeftTileFlag & TileInfo::Flags::FLOOR) &&
            !!(bottomLeftTileFlag & TileInfo::Flags::LEFT_BLOCKING_WALL) &&
            !(bottomLeftTileFlag & TileInfo::Flags::SPIKE))
        {
            const bn::fixed cellX = collider.left() % 8;
            const bn::fixed cellY = 8 - collider.bottom() % 8;
            if (cellX > cellY)
                return LeftWallCollisionResolution_(entity);
            else
                return FloorCollisionResolution_(entity);
        }
    }
    else if (!!(moveDirs & MoveDirections::UP) && !!(moveDirs & MoveDirections::LEFT))
    {
        if (!!(topLeftTileFlag & TileInfo::Flags::CEILING) &&
            !!(topLeftTileFlag & TileInfo::Flags::LEFT_BLOCKING_WALL) && !(topLeftTileFlag & TileInfo::Flags::SPIKE))
        {
            const bn::fixed cellX = collider.left() % 8;
            const bn::fixed cellY = collider.top() % 8;
            if (cellX >= cellY)
                return LeftWallCollisionResolution_(entity);
            else
                return CeilingCollisionResolution_(entity);
        }
    }
    else if (!!(moveDirs & MoveDirections::UP) && !!(moveDirs & MoveDirections::RIGHT))
    {
        if (!!(topRightTileFlag & TileInfo::Flags::CEILING) &&
            !!(topRightTileFlag & TileInfo::Flags::RIGHT_BLOCKING_WALL) && !(topRightTileFlag & TileInfo::Flags::SPIKE))
        {
            const bn::fixed cellX = 8 - collider.right() % 8;
            const bn::fixed cellY = collider.top() % 8;
            if (cellX >= cellY)
                return RightWallCollisionResolution_(entity);
            else
                return CeilingCollisionResolution_(entity);
        }
    }

    // general collision
    if (!!(moveDirs & MoveDirections::DOWN))
    {
        if ((!!(bottomLeftTileFlag & TileInfo::Flags::FLOOR) && !(bottomLeftTileFlag & TileInfo::Flags::SPIKE)) ||
            (!!(bottomRightTileFlag & TileInfo::Flags::FLOOR) && !(bottomRightTileFlag & TileInfo::Flags::SPIKE)))
        {
            return FloorCollisionResolution_(entity);
        }
    }
    if (!!(moveDirs & MoveDirections::LEFT))
    {
        if ((!!(topLeftTileFlag & TileInfo::Flags::LEFT_BLOCKING_WALL) &&
             !(topLeftTileFlag & TileInfo::Flags::SPIKE)) ||
            (!!(bottomLeftTileFlag & TileInfo::Flags::LEFT_BLOCKING_WALL) &&
             !(bottomLeftTileFlag & TileInfo::Flags::SPIKE)))
        {
            return LeftWallCollisionResolution_(entity);
        }
    }
    if (!!(moveDirs & MoveDirections::RIGHT))
    {
        if ((!!(topRightTileFlag & TileInfo::Flags::RIGHT_BLOCKING_WALL) &&
             !(topRightTileFlag & TileInfo::Flags::SPIKE)) ||
            (!!(bottomRightTileFlag & TileInfo::Flags::RIGHT_BLOCKING_WALL) &&
             !(bottomRightTileFlag & TileInfo::Flags::SPIKE)))
        {
            return RightWallCollisionResolution_(entity);
        }
    }
    if (!!(moveDirs & MoveDirections::UP))
    {
        if ((!!(topLeftTileFlag & TileInfo::Flags::CEILING) && !(topLeftTileFlag & TileInfo::Flags::SPIKE)) ||
            (!!(topRightTileFlag & TileInfo::Flags::CEILING) && !(topRightTileFlag & TileInfo::Flags::SPIKE)))
        {
            return CeilingCollisionResolution_(entity);
        }
    }

    return bn::nullopt;
}

} // namespace

PhysicsMovement::PhysicsMovement(scene::GameState& state) : ISystem(state)
{
}

void PhysicsMovement::Update()
{
    UpdatePlayer_();
    UpdateSymbols_();
}

void PhysicsMovement::UpdatePlayer_()
{
    PlayerGravity_();
    PlayerKeyboardHandle_();

    PlayerClampVelocity_();
    state_.player.SetPosition(state_.player.GetPosition() + state_.player.GetVelocity());

    PlayerCollision_();
    helper::tilemap::SnapEntityToZoneBoundary(state_.player, state_.zoneBoundary);
    PlayerAnimation_();
}

void PhysicsMovement::PlayerKeyboardHandle_()
{
    if (!state_.isPlayerControllable)
        return;

    bn::fixed_point velocity = state_.player.GetVelocity();

    if (bn::keypad::a_pressed() && state_.player.GetGrounded() && playerJumpCount > 0)
    {
        state_.player.SetGrounded(false);
        state_.player.InitJumpAction();
        --playerJumpCount;
        bn::sound_items::sfx_player_jump.play(constant::volume::sfx_player_jump);
        velocity.set_y(JUMP_VEL);
    }
    if (bn::keypad::left_held())
    {
        velocity.set_x(velocity.x() - PLAYER_DELTA_VEL.x());
    }
    else if (bn::keypad::right_held())
    {
        velocity.set_x(velocity.x() + PLAYER_DELTA_VEL.x());
    }
    else
    {
        velocity.set_x(velocity.x() / VEL_FRICTION);
    }

    state_.player.SetVelocity(velocity);
}

void PhysicsMovement::PlayerGravity_()
{
    // if (state_.player.GetGrounded())
    //     return;

    bn::fixed_point velocity = state_.player.GetVelocity();
    velocity.set_y(velocity.y() + state_.player.GetGravityScale());
    state_.player.SetVelocity(velocity);
}

void PhysicsMovement::PlayerClampVelocity_()
{
    bn::fixed_point velocity = state_.player.GetVelocity();

    if (velocity.x() > MAX_PLAYER_VEL.x())
        velocity.set_x(MAX_PLAYER_VEL.x());
    else if (velocity.x() < -MAX_PLAYER_VEL.x())
        velocity.set_x(-MAX_PLAYER_VEL.x());
    if (velocity.y() > MAX_PLAYER_VEL.y())
        velocity.set_y(MAX_PLAYER_VEL.y());
    else if (velocity.y() < -MAX_PLAYER_VEL.y())
        velocity.set_y(-MAX_PLAYER_VEL.y());

    state_.player.SetVelocity(velocity);
}

void PhysicsMovement::PlayerCollision_()
{
    bn::optional<PushbackDirection> collisionResult;
    bool nextGrounded = false;
    for (int i = 0; i < 100; ++i)
    {
        collisionResult = PlatformCollisionResolution_(state_.player, state_.currentMapTileInfo);
        if (!collisionResult)
            break;
        switch (*collisionResult)
        {
        case PushbackDirection::UP:
            if (!nextGrounded)
            {
                nextGrounded = true;
                if (!state_.player.GetGrounded())
                {
                    state_.player.InitLandAction();
                }
            }
            playerJumpCount = MAX_PLAYER_JUMP_COUNT;
            break;
        case PushbackDirection::DOWN:
            break;
        case PushbackDirection::LEFT:
            break;
        case PushbackDirection::RIGHT:
            break;
        default:
            BN_ERROR("Invalid PushbackDirection: ", static_cast<int>(*collisionResult));
        }

        if (i == 99)
            BN_LOG("[WARN] Collision detection max loop reached!");
    }
    state_.player.SetGrounded(nextGrounded);
}

void PhysicsMovement::PlayerAnimation_()
{
    using entity::IPhysicsEntity;
    bn::fixed_point velocity = state_.player.GetVelocity();

    if (velocity.y() > IPhysicsEntity::EPSILON_VEL)
    {
        if (state_.player.GetActionState() != entity::Player::ActionState::FALL)
            state_.player.InitFallAction();
    }
    else if (bn::keypad::left_held())
    {
        state_.player.SetHorizontalFlip(true);
        if (state_.player.GetActionState() == entity::Player::ActionState::IDLE &&
            state_.player.GetVelocity().x() < -IPhysicsEntity::EPSILON_VEL)
            state_.player.InitFallAction();
    }
    else if (bn::keypad::right_held())
    {
        state_.player.SetHorizontalFlip(false);
        if (state_.player.GetActionState() == entity::Player::ActionState::IDLE &&
            state_.player.GetVelocity().x() > IPhysicsEntity::EPSILON_VEL)
            state_.player.InitFallAction();
    }
    else if (state_.player.GetGrounded() && state_.player.GetActionState() == entity::Player::ActionState::FALL &&
             bn::abs(velocity.x()) < IPhysicsEntity::EPSILON_VEL && bn::abs(velocity.y()) < IPhysicsEntity::EPSILON_VEL)
    {
        if (state_.player.GetActionState() != entity::Player::ActionState::IDLE)
            state_.player.InitIdleAction();
    }
}

void PhysicsMovement::UpdateSymbols_()
{
    UpdateSymbolsInHands_();
    UpdateSymbolsOnFloor_();
    UpdateSymbolsThrown_();
}

void PhysicsMovement::UpdateSymbolsInHands_()
{
    if (state_.symbolsInHands[0])
    {
        state_.symbolsInHands[0]->SetPosition(state_.player.GetLeftSymbolPosition());
    }
    if (state_.symbolsInHands[1])
    {
        state_.symbolsInHands[1]->SetPosition(state_.player.GetRightSymbolPosition());
    }
}

void PhysicsMovement::UpdateSymbolsOnFloor_()
{
}

void PhysicsMovement::UpdateSymbolsThrown_()
{
}

} // namespace sym::game::system
