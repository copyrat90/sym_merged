#pragma once

#include "game_entity_IGravityEntity.h"

#include <bn_any.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class Player final : public IGravityEntity
{
public:
    Player(bn::fixed_point position);

    Player(Player&& other);
    Player& operator=(Player&& other);

    Player(const Player& other) = delete;
    Player& operator=(const Player& other) = delete;

    void FreeGraphicResource() final;

    void Update() final;

    void InitIdleAction();
    void InitJumpAction();
    void InitFallAction();
    void InitLandAction();
    void InitMergeStartAction();
    void InitMergeEndAction();

private:
    static constexpr bn::fixed_rect RELATIVE_INTERACT_RANGE = {{0, 0}, {32, 32}};
    static constexpr bn::fixed_rect RELATIVE_PHYSICS_COLLIDER = {{0, 0}, {26, 26}};
    static constexpr bool IS_GRAVITY_ENABLED_BY_DEFAULT = true;
    static constexpr bn::fixed GRAVITY_SCALE = 1;

    static constexpr int IDLE_ACTION_WAIT_UPDATE = 30;
    static constexpr int OTHER_ACTIONS_WAIT_UPDATE = 10;

    bn::optional<bn::sprite_animate_action<2>> action2_;
    bn::optional<bn::sprite_animate_action<3>> action3_;
    /**
     * @brief Additional wait update before the InitIdleAction() is called.
     * If it is `-1`, InitIdleAction() is not called.
     * (i.e. the last animation frame is used after the action ends.)
     *
     */
    int additionalWaitUpdateCount = -1;

    void DestroyActions_();
};

} // namespace sym::game::entity
