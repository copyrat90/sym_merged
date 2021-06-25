#pragma once

#include "game_entity_IPhysicsEntity.h"

#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class Player final : public IPhysicsEntity
{
public:
    Player(bn::fixed_point position);

    Player(Player&& other) = delete;
    Player& operator=(Player&& other) = delete;

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

    [[nodiscard]] bn::fixed_rect GetLeftSymbolPickupRange() const;
    [[nodiscard]] bn::fixed_rect GetRightSymbolPickupRange() const;
    [[nodiscard]] bn::fixed_rect GetLeftButtonInteractRange() const;
    [[nodiscard]] bn::fixed_rect GetRightButtonInteractRange() const;
    [[nodiscard]] bn::fixed_point GetLeftSymbolPosition() const;
    [[nodiscard]] bn::fixed_point GetRightSymbolPosition() const;
    [[nodiscard]] bn::fixed_point GetMergeSymbolPosition() const;

    bool GetControllable() const;
    void SetControllable(bool isControllable);

    enum class ActionState
    {
        IDLE,
        JUMP,
        FALL,
        LAND,
        MERGE_START,
        MERGE_END
    };

    ActionState GetActionState() const;

private:
    bn::optional<bn::sprite_animate_action<2>> action2_;
    bn::optional<bn::sprite_animate_action<3>> action3_;
    ActionState actionState_ = ActionState::IDLE;
    /**
     * @brief Additional wait update before the InitIdleAction() is called.
     * If it is `-1`, InitIdleAction() is not called.
     * (i.e. the last animation frame is used after the action ends.)
     *
     */
    int additionalWaitUpdateCount_ = -1;

    bool isControllable_ = true;

    /**
     * @brief Updates action.
     *
     * @return `true` if action is done, otherwise `false`
     */
    bool UpdateAction_();
    void DestroyActions_();

    void AddRelativeYPos_(bn::fixed_point& resultPos, bool isFront) const;
};

} // namespace sym::game::entity
