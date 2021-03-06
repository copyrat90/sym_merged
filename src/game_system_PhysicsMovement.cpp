#include "game_system_PhysicsMovement.h"

#include <bn_affine_bg_map_ptr.h>
#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_keypad.h>
#include <bn_log.h>
#include <bn_math.h>
#include <bn_optional.h>
#include <bn_sound.h>
#include <bn_vector.h>

#include "helper_keypad.h"
#include "helper_math.h"
#include "helper_tilemap.h"
#include "scene_GameState.h"

#include "bn_sound_items.h"
#include "constant.h"

#include "bn_sprite_items_spr_ingame_protagonist_star.h"
#include "bn_sprite_palette_items_pal_ingame_protagonist_star_damage.h"

namespace sym::game::system
{

namespace
{

constexpr bn::fixed_point MAX_ENTITY_VEL = {2, 4};
constexpr bn::fixed PLAYER_UMBRELLA_VEL = 0.65;
constexpr bn::fixed_point PLAYER_DELTA_VEL = {0.3, 0.3};
constexpr bn::fixed PRESS_A_JUMP_VEL = -2.8;
constexpr bn::fixed SYMBOL_JUMP_VEL = -2;
constexpr bn::fixed GRAVITY_REVERSE_INIT_VEL = 2;
constexpr bn::fixed_point SYMBOL_DELTA_VEL = {0.3, 0.3};
constexpr bn::fixed VEL_FRICTION = 1.2;
constexpr bn::fixed COLLISION_DELTA_EPSILON = 0.01;
constexpr int SPIKE_SIZE = 3;

constexpr int PLAYER_DAMAGE_TRANSPARENCY_UPDATE_COUNT = 20;
constexpr int PLAYER_DAMAGE_WAIT_BETWEEN_TRANSPARENCY_AND_FADE = 10;
constexpr int PLAYER_DAMAGE_FADE_UPDATE_COUNT = 20;
constexpr int PLAYER_DAMAGE_WAIT_BETWEEN_FO_AND_FI = 5;

constexpr int COLLISION_LOOP_MAX_COUNT = 100;

constexpr const bn::sprite_palette_item& PLAYER_NORMAL_PAL =
    bn::sprite_items::spr_ingame_protagonist_star.palette_item();
constexpr const bn::sprite_palette_item& PLAYER_DAMAGE_PAL =
    bn::sprite_palette_items::pal_ingame_protagonist_star_damage;

static_assert(bn::abs(PRESS_A_JUMP_VEL) <= MAX_ENTITY_VEL.y());
static_assert(bn::abs(SYMBOL_JUMP_VEL) <= MAX_ENTITY_VEL.y());
static_assert(bn::abs(PLAYER_UMBRELLA_VEL) <= MAX_ENTITY_VEL.y());

enum class PushbackDirection
{
    UP,
    DOWN,
    LEFT,
    RIGHT,
    SPIKE
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
 * TODO: after handling symbol collision resolution properly, isSpikeCollisionDisabled should be removed.
 *
 */
[[nodiscard]] bn::optional<PushbackDirection> PlatformCollisionResolution_(entity::IPhysicsEntity& entity,
                                                                           const helper::tilemap::TileInfo& mapTileInfo,
                                                                           bool isSpikeCollisionDisabled = false)
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

    // TODO: after handling symbol collision resolution properly, isSpikeCollisionDisabled should be removed.
    if (!isSpikeCollisionDisabled)
    {
        // spike detection
        if (!!(topLeftTileFlag & TileInfo::Flags::SPIKE))
        {
            if (!!(topLeftTileFlag & TileInfo::Flags::CEILING))
            {
                if ((collider.top() % 8) <= SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
            else if (!!(topLeftTileFlag & TileInfo::Flags::LEFT_BLOCKING_WALL))
            {
                if ((collider.left() % 8) <= SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
        }
        if (!!(topRightTileFlag & TileInfo::Flags::SPIKE))
        {
            if (!!(topRightTileFlag & TileInfo::Flags::CEILING))
            {
                if ((collider.top() % 8) <= SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
            else if (!!(topRightTileFlag & TileInfo::Flags::RIGHT_BLOCKING_WALL))
            {
                if ((collider.right() % 8) >= 8 - SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
        }
        if (!!(bottomLeftTileFlag & TileInfo::Flags::SPIKE))
        {
            if (!!(bottomLeftTileFlag & TileInfo::Flags::FLOOR))
            {
                if ((collider.bottom() % 8) >= 8 - SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
            else if (!!(bottomLeftTileFlag & TileInfo::Flags::LEFT_BLOCKING_WALL))
            {
                if ((collider.left() % 8) <= SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
        }
        if (!!(bottomRightTileFlag & TileInfo::Flags::SPIKE))
        {
            if (!!(bottomRightTileFlag & TileInfo::Flags::FLOOR))
            {
                if ((collider.bottom() % 8) >= 8 - SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
            else if (!!(bottomRightTileFlag & TileInfo::Flags::RIGHT_BLOCKING_WALL))
            {
                if ((collider.right() % 8) >= 8 - SPIKE_SIZE)
                    return PushbackDirection::SPIKE;
            }
        }
    }

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

/**
 * @brief Detect and Resolve colision.
 * You need to call this once again, if if returns value other than `bn::nullopt`.
 *
 */
[[nodiscard]] bn::optional<PushbackDirection> ShuttersCollisionResolution_(
    entity::IPhysicsEntity& entity, bn::vector<entity::Shutter, scene::GameState::ZONE_SHUTTER_MAX_COUNT>& shutters)
{
    for (const auto& shutter : shutters)
    {
        bn::fixed_rect entityCollider = entity.GetPhysicsCollider();
        const bn::fixed_rect shutterCollider = shutter.GetInteractRange();
        if (shutter.GetOpened())
            continue;
        if (!entityCollider.intersects(shutterCollider))
            continue;

        if (entityCollider.position().x() <= shutterCollider.position().x())
        {
            const bn::fixed dx = entityCollider.right() - shutterCollider.left();
            entity.SetX(entity.GetX() - dx);
            entity.SetXVelocity(0);
            return PushbackDirection::LEFT;
        }
        else
        {
            const bn::fixed dx = shutterCollider.right() - entityCollider.left();
            entity.SetX(entity.GetX() + dx);
            entity.SetXVelocity(0);
            return PushbackDirection::RIGHT;
        }
    }
    return bn::nullopt;
}

void ApplyGravity_(entity::IPhysicsEntity& entity)
{
    if (!entity.GetGravityEnabled())
        return;

    bn::fixed_point velocity = entity.GetVelocity();
    // this approach give fairly inaccurate result
    // const int direction = entity.GetGravityReversed() ? -1 : 1;
    // velocity.set_y(velocity.y() + direction * entity.GetGravityScale());
    if (entity.GetGravityReversed())
        velocity.set_y(velocity.y() - entity.GetGravityScale());
    else
        velocity.set_y(velocity.y() + entity.GetGravityScale());
    entity.SetVelocity(velocity);
}

void ClampVelocity_(entity::IPhysicsEntity& entity)
{
    bn::fixed_point velocity = entity.GetVelocity();

    if (velocity.x() > MAX_ENTITY_VEL.x())
        velocity.set_x(MAX_ENTITY_VEL.x());
    else if (velocity.x() < -MAX_ENTITY_VEL.x())
        velocity.set_x(-MAX_ENTITY_VEL.x());
    if (velocity.y() > MAX_ENTITY_VEL.y())
        velocity.set_y(MAX_ENTITY_VEL.y());
    else if (velocity.y() < -MAX_ENTITY_VEL.y())
        velocity.set_y(-MAX_ENTITY_VEL.y());

    entity.SetVelocity(velocity);
}

} // namespace

PhysicsMovement::PhysicsMovement(scene::GameState& state) : ISystem(state)
{
}

void PhysicsMovement::Update()
{
    if (state_.isClearOngoing)
        return;

    UpdatePlayer_();
    UpdateSymbols_();
}

void PhysicsMovement::UpdatePlayer_()
{
    if (state_.player.GetControllable())
    {
        ApplyGravity_(state_.player);
        PlayerKeyboardHandle_();

        ClampVelocity_(state_.player);
        if (umbrellaApplied_)
        {
            if (state_.player.GetGravityReversed())
            {
                if (state_.player.GetVelocity().y() < -PLAYER_UMBRELLA_VEL)
                    state_.player.SetYVelocity(-PLAYER_UMBRELLA_VEL);
            }
            else
            {
                if (state_.player.GetVelocity().y() > PLAYER_UMBRELLA_VEL)
                    state_.player.SetYVelocity(PLAYER_UMBRELLA_VEL);
            }
        }
        state_.player.SetPosition(state_.player.GetPosition() + state_.player.GetVelocity());

        PlayerCollision_();
        helper::tilemap::SnapEntityToZoneBoundary(state_.player, state_.zoneBoundary);
    }
    PlayerAnimation_();
}

void PhysicsMovement::PlayerKeyboardHandle_()
{
    if (!state_.player.GetControllable())
        return;
    const bool mergeOngoing = state_.player.GetAnimationState() == entity::Player::AnimationState::MERGE_START ||
                              state_.player.GetAnimationState() == entity::Player::AnimationState::MERGE_END;

    bn::fixed_point velocity = state_.player.GetVelocity();

    auto symbolAbilityPress = [&](entity::Symbol& symbol) {
        using SymType = entity::Symbol::Type;
        using AbilityState = entity::Symbol::AbilityState;
        switch (symbol.GetType())
        {
        case SymType::PLUS:
            if (!symbolJumping_)
            {
                symbolJumping_ = true;
                SetAbilityStateOfCertainTypeSymbols_(SymType::PLUS, AbilityState::NOT_READY);
                // state_.player.SetGrounded(false);
                state_.player.InitJumpAction();
                bn::sound_items::sfx_symbol_jump.play(constant::volume::sfx_symbol_jump);
                if (state_.player.GetGravityReversed())
                {
                    if (-SYMBOL_JUMP_VEL > velocity.y())
                        velocity.set_y(-SYMBOL_JUMP_VEL);
                }
                else
                {
                    if (SYMBOL_JUMP_VEL < velocity.y())
                        velocity.set_y(SYMBOL_JUMP_VEL);
                }
            }
            break;
        case SymType::VV:
            if (symbol.GetAbilityState() == AbilityState::READY_TO_USE)
            {
                SetAbilityStateOfCertainTypeSymbols_(SymType::VV, AbilityState::NOT_READY);
                bn::sound_items::sfx_gravity_reverse.play(constant::volume::sfx_gravity_reverse);
                state_.player.SetGravityReversed(!state_.player.GetGravityReversed());
                if (state_.symbolsInHands[0])
                    state_.symbolsInHands[0]->ToggleGravityReversed();
                if (state_.symbolsInHands[1])
                    state_.symbolsInHands[1]->ToggleGravityReversed();
                velocity = {velocity.x(),
                            state_.player.GetGravityReversed() ? -GRAVITY_REVERSE_INIT_VEL : GRAVITY_REVERSE_INIT_VEL};
            }
            break;
        case SymType::UP:
            if (!umbrellaApplied_)
            {
                umbrellaApplied_ = true;
                SetAbilityStateOfCertainTypeSymbols_(SymType::UP, AbilityState::USING);
                bn::sound_items::sfx_umbrella_open.play(constant::volume::sfx_umbrella_open);
            }
            break;
        default:
            BN_ERROR("Invalid complex SymType: ", static_cast<int>(state_.symbolsInHands[0]->GetType()));
            break;
        }
    };

    auto symbolAbilityRelease = [&](entity::Symbol& symbol) {
        using SymType = entity::Symbol::Type;
        using AbilityState = entity::Symbol::AbilityState;
        switch (symbol.GetType())
        {
        case SymType::UP:
            if (umbrellaApplied_)
            {
                umbrellaApplied_ = false;
                SetAbilityStateOfCertainTypeSymbols_(SymType::UP, AbilityState::READY_TO_USE);
                bn::sound::stop_all();
            }
            break;

        case SymType::VV:
        case SymType::PLUS:
            // Disabling ability is handled somewhere else,
            // hence no need to do anything here.
            break;
        default:
            BN_ERROR("Invalid complex SymType: ", static_cast<int>(state_.symbolsInHands[0]->GetType()));
            break;
        }
    };

    // left symbol ability: Press
    if (!mergeOngoing && state_.triggerInteraction.IsLKeyPressLasts() && state_.symbolsInHands[0])

    {
        if (state_.symbolsInHands[0]->IsComplexSymbol())
        {
            if (state_.symbolsInHands[0]->GetAbilityState() == entity::Symbol::AbilityState::READY_TO_USE)
            {
                symbolAbilityPress(*state_.symbolsInHands[0]);
            }
            else if (state_.symbolsInHands[0]->GetAbilityState() == entity::Symbol::AbilityState::NOT_READY)
            {
                bn::sound_items::sfx_error.play(constant::volume::sfx_error);
            }
        }
        else
        {
            bn::sound_items::sfx_error.play(constant::volume::sfx_error);
        }
        state_.triggerInteraction.ResetLKeyPress();
    }
    // right symbol ability: Press
    if (!mergeOngoing && state_.triggerInteraction.IsRKeyPressLasts() && state_.symbolsInHands[1])
    {
        if (state_.symbolsInHands[1]->IsComplexSymbol())
        {
            if (state_.symbolsInHands[1]->GetAbilityState() == entity::Symbol::AbilityState::READY_TO_USE)
            {
                symbolAbilityPress(*state_.symbolsInHands[1]);
            }
            else if (state_.symbolsInHands[1]->GetAbilityState() == entity::Symbol::AbilityState::NOT_READY)
            {
                bn::sound_items::sfx_error.play(constant::volume::sfx_error);
            }
        }
        else
        {
            bn::sound_items::sfx_error.play(constant::volume::sfx_error);
        }
        state_.triggerInteraction.ResetRKeyPress();
    }
    // left symbol ability: Release
    if (!mergeOngoing && bn::keypad::l_released() && state_.symbolsInHands[0] &&
        state_.symbolsInHands[0]->GetAbilityState() == entity::Symbol::AbilityState::USING)
    {
        symbolAbilityRelease(*state_.symbolsInHands[0]);
    }
    // right symbol ability: Release
    if (!mergeOngoing && bn::keypad::r_released() && state_.symbolsInHands[1] &&
        state_.symbolsInHands[1]->GetAbilityState() == entity::Symbol::AbilityState::USING)
    {
        symbolAbilityRelease(*state_.symbolsInHands[1]);
    }
    // left symbol ability: Cancel
    if ((mergeOngoing || bn::keypad::start_pressed()) && state_.symbolsInHands[0] &&
        state_.symbolsInHands[0]->GetAbilityState() == entity::Symbol::AbilityState::USING)
    {
        symbolAbilityRelease(*state_.symbolsInHands[0]);
    }
    // right symbol ability: Cancel
    if ((mergeOngoing || bn::keypad::start_pressed()) && state_.symbolsInHands[1] &&
        state_.symbolsInHands[1]->GetAbilityState() == entity::Symbol::AbilityState::USING)
    {
        symbolAbilityRelease(*state_.symbolsInHands[1]);
    }

    if (!mergeOngoing && bn::keypad::a_pressed() && state_.player.GetGrounded() && !pressAJumping_)
    {
        pressAJumping_ = true;
        // state_.player.SetGrounded(false);
        state_.player.InitJumpAction();
        bn::sound_items::sfx_player_jump.play(constant::volume::sfx_player_jump);
        if (state_.player.GetGravityReversed())
        {
            if (-PRESS_A_JUMP_VEL > velocity.y())
                velocity.set_y(-PRESS_A_JUMP_VEL);
        }
        else
        {
            if (PRESS_A_JUMP_VEL < velocity.y())
                velocity.set_y(PRESS_A_JUMP_VEL);
        }
    }
    if (!mergeOngoing && bn::keypad::left_held())
    {
        velocity.set_x(velocity.x() - PLAYER_DELTA_VEL.x());
    }
    else if (!mergeOngoing && bn::keypad::right_held())
    {
        velocity.set_x(velocity.x() + PLAYER_DELTA_VEL.x());
    }
    else
    {
        velocity.set_x(velocity.x() / VEL_FRICTION);
    }

    state_.player.SetVelocity(velocity);
}

void PhysicsMovement::PlayerCollision_()
{
    bn::optional<PushbackDirection> collisionResult;
    bool nextGrounded = false;
    bool nextSafe = true;

    for (int i = 0; i < COLLISION_LOOP_MAX_COUNT; ++i)
    {
        // resolve collision
        collisionResult = PlatformCollisionResolution_(state_.player, state_.currentMapTileInfo);
        if (!collisionResult)
        {
            collisionResult =
                ShuttersCollisionResolution_(state_.player, state_.shuttersOfZones[state_.currentZoneIdx]);
            if (!collisionResult)
                break;
        }

        // additional work to do when collision is detected
        switch (*collisionResult)
        {
        case PushbackDirection::SPIKE:
            state_.player.SetControllable(false);
            state_.player.SetGravityEnabled(false);
            state_.player.SetVelocity({0, 0});
            state_.player.SetPalette(PLAYER_DAMAGE_PAL);
            bn::sound_items::sfx_player_damage.play(constant::volume::sfx_player_damage);
            state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::PLAYER |
                                                      Transition::AppliedItems::SYMBOLS_IN_HANDS);
            state_.transition.InitOut(Transition::Types::TRANSPARENCY, PLAYER_DAMAGE_TRANSPARENCY_UPDATE_COUNT,
                                      PLAYER_DAMAGE_WAIT_BETWEEN_TRANSPARENCY_AND_FADE);
            state_.transition.SetDoneEventHandler([this] {
                ResetAbilitiesToReadyInHands_();
                state_.player.SetVisible(false);
                if (state_.symbolsInHands[0])
                    state_.symbolsInHands[0]->SetVisible(false);
                if (state_.symbolsInHands[1])
                    state_.symbolsInHands[1]->SetVisible(false);
                bn::blending::set_transparency_alpha(1);
                state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::ALL);
                state_.transition.InitOutAndIn(Transition::Types::FADE, PLAYER_DAMAGE_FADE_UPDATE_COUNT,
                                               PLAYER_DAMAGE_FADE_UPDATE_COUNT, PLAYER_DAMAGE_WAIT_BETWEEN_FO_AND_FI);
                state_.transition.SetWaitBetweenEventHandler([this] {
                    state_.player.SetPalette(PLAYER_NORMAL_PAL);
                    state_.player.SetVisible(true);
                    if (state_.symbolsInHands[0])
                        state_.symbolsInHands[0]->SetVisible(true);
                    if (state_.symbolsInHands[1])
                        state_.symbolsInHands[1]->SetVisible(true);
                    state_.player.SetPosition(state_.player.GetLastSafePosition());
                    if (state_.player.GetGravityReversed() != state_.player.GetLastSafeGravityIsReversed())
                    {
                        state_.player.SetGravityReversed(state_.player.GetLastSafeGravityIsReversed());
                        if (state_.symbolsInHands[0])
                            state_.symbolsInHands[0]->ToggleGravityReversed();
                        if (state_.symbolsInHands[1])
                            state_.symbolsInHands[1]->ToggleGravityReversed();
                    }
                });
                state_.transition.SetDoneEventHandler([this] {
                    state_.player.SetControllable(true);
                    state_.player.SetGravityEnabled(true);
                });
            });
            break;
        case PushbackDirection::UP:
            if (!nextGrounded && !state_.player.GetGravityReversed())
                nextGrounded = true;
            break;
        case PushbackDirection::DOWN:
            if (!nextGrounded && state_.player.GetGravityReversed())
                nextGrounded = true;
            break;
        case PushbackDirection::LEFT:
            break;
        case PushbackDirection::RIGHT:
            break;
        default:
            BN_ERROR("Invalid PushbackDirection: ", static_cast<int>(*collisionResult));
        }

        if (collisionResult == PushbackDirection::SPIKE)
        {
            nextSafe = false;
            break;
        }

        if (i == COLLISION_LOOP_MAX_COUNT - 1)
            BN_LOG("[WARN] Collision detection loop max count reached!");
    }

    if (!state_.player.GetGrounded() && nextGrounded)
    {
        state_.player.InitLandAction();
        pressAJumping_ = false;
        symbolJumping_ = false;
        SetAbilityStateOfCertainTypeSymbols_(entity::Symbol::Type::PLUS, entity::Symbol::AbilityState::READY_TO_USE);
        SetAbilityStateOfCertainTypeSymbols_(entity::Symbol::Type::VV, entity::Symbol::AbilityState::READY_TO_USE);
    }
    else if (state_.player.GetGrounded() && !nextGrounded)
    {
        SetAbilityStateOfCertainTypeSymbols_(entity::Symbol::Type::VV, entity::Symbol::AbilityState::NOT_READY);
    }
    state_.player.SetGrounded(nextGrounded);

    if (nextGrounded && nextSafe && state_.player.GetControllable())
    {
        state_.player.SetLastSafePosition(state_.player.GetPosition());
        state_.player.SetLastSafeGravityIsReversed(state_.player.GetGravityReversed());
    }
}

void PhysicsMovement::PlayerAnimation_()
{
    using entity::IPhysicsEntity;
    bn::fixed_point velocity = state_.player.GetVelocity();
    const entity::Player::AnimationState prevPlayerActionState = state_.player.GetAnimationState();

    if (velocity.y() > IPhysicsEntity::EPSILON_VEL)
    {
        if (prevPlayerActionState != entity::Player::AnimationState::FALL)
            state_.player.InitFallAction();
    }
    else if (bn::keypad::left_held())
    {
        state_.player.SetHorizontalFlip(true);
        if (prevPlayerActionState == entity::Player::AnimationState::IDLE &&
            state_.player.GetVelocity().x() < -IPhysicsEntity::EPSILON_VEL)
            state_.player.InitFallAction();
    }
    else if (bn::keypad::right_held())
    {
        state_.player.SetHorizontalFlip(false);
        if (prevPlayerActionState == entity::Player::AnimationState::IDLE &&
            state_.player.GetVelocity().x() > IPhysicsEntity::EPSILON_VEL)
            state_.player.InitFallAction();
    }
    else if (state_.player.GetGrounded() && prevPlayerActionState == entity::Player::AnimationState::FALL &&
             bn::abs(velocity.x()) < IPhysicsEntity::EPSILON_VEL && bn::abs(velocity.y()) < IPhysicsEntity::EPSILON_VEL)
    {
        if (prevPlayerActionState != entity::Player::AnimationState::IDLE)
            state_.player.InitIdleAction();
    }
}

void PhysicsMovement::UpdateSymbols_()
{
    UpdateSymbolsInHands_();
    UpdateSymbolsOfCurrentZone_();
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

void PhysicsMovement::UpdateSymbolsOfCurrentZone_()
{
    for (auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
    {
        ApplyGravity_(symbol);

        ClampVelocity_(symbol);
        symbol.SetPosition(symbol.GetPosition() + symbol.GetVelocity());

        SymbolCollision_(symbol);
    }
}

void PhysicsMovement::SymbolCollision_(entity::Symbol& symbol)
{
    bn::optional<PushbackDirection> collisionResult;
    bool nextGrounded = false;
    for (int i = 0; i < COLLISION_LOOP_MAX_COUNT; ++i)
    {
        // resolve collision
        collisionResult = PlatformCollisionResolution_(symbol, state_.currentMapTileInfo, true);
        if (!collisionResult)
        {
            collisionResult = ShuttersCollisionResolution_(symbol, state_.shuttersOfZones[state_.currentZoneIdx]);
            if (!collisionResult)
                break;
        }

        // additional work to do when collision is detected
        switch (*collisionResult)
        {
        case PushbackDirection::SPIKE:
            // TODO
            break;
        case PushbackDirection::UP:
            if (!nextGrounded && !symbol.GetGravityReversed())
                nextGrounded = true;
            break;
        case PushbackDirection::DOWN:
            if (!nextGrounded && symbol.GetGravityReversed())
                nextGrounded = true;
            break;
        case PushbackDirection::LEFT:
        case PushbackDirection::RIGHT:
            if (!symbol.GetGravityEnabled())
            {
                symbol.SetThrown(false);
                symbol.SetGravityEnabled(true);
                bn::sound_items::sfx_symbol_wall_bump.play(constant::volume::sfx_symbol_wall_bump);
            }
            break;
        default:
            BN_ERROR("Invalid PushbackDirection: ", static_cast<int>(*collisionResult));
        }
        if (i == COLLISION_LOOP_MAX_COUNT - 1)
            BN_LOG("[WARN] Collision detection loop max count reached!");
    }

    if (!symbol.GetGrounded() && nextGrounded)
    {
        if (!state_.transition.IsOngoing())
            bn::sound_items::sfx_symbol_ground_bump.play(constant::volume::sfx_symbol_ground_bump);
    }
    symbol.SetGrounded(nextGrounded);
}

void PhysicsMovement::UpdateSymbolsThrown_()
{
}

void PhysicsMovement::SetAbilityStateOfCertainTypeSymbols_(entity::Symbol::Type type,
                                                           entity::Symbol::AbilityState newState)
{
    if (state_.symbolsInHands[0] && state_.symbolsInHands[0]->GetType() == type)
        state_.symbolsInHands[0]->SetAbilityState(newState);
    if (state_.symbolsInHands[1] && state_.symbolsInHands[1]->GetType() == type)
        state_.symbolsInHands[1]->SetAbilityState(newState);
    for (auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
    {
        if (symbol.GetType() == type)
            symbol.SetAbilityState(newState);
    }
}

void PhysicsMovement::ResetAbilitiesToReadyInHands_()
{
    using SymType = entity::Symbol::Type;
    using AbilityState = entity::Symbol::AbilityState;

    auto resetOneHand = [this](entity::Symbol& symbol) {
        SetAbilityStateOfCertainTypeSymbols_(symbol.GetType(), AbilityState::READY_TO_USE);
        switch (symbol.GetType())
        {
        case SymType::PLUS:
            symbolJumping_ = false;
            break;
        case SymType::UP:
            umbrellaApplied_ = false;
            break;
        case SymType::VV:
            break;
        default:
            BN_ERROR("Invalid SymType: ", static_cast<int>(symbol.GetType()), " to reset");
        }
    };

    if (state_.symbolsInHands[0] && state_.symbolsInHands[0]->IsComplexSymbol() &&
        state_.symbolsInHands[0]->GetAbilityState() != entity::Symbol::AbilityState::NOT_READY)
    {
        resetOneHand(*state_.symbolsInHands[0]);
    }

    if (state_.symbolsInHands[1] && state_.symbolsInHands[1]->IsComplexSymbol() &&
        state_.symbolsInHands[1]->GetAbilityState() != entity::Symbol::AbilityState::NOT_READY)
    {
        resetOneHand(*state_.symbolsInHands[1]);
    }
}

} // namespace sym::game::system
