#pragma once

#include "game_entity_IPhysicsEntity.h"

#include <bn_sprite_animate_actions.h>

#include "scene_GameState_fwd.h"

namespace sym::game::entity
{

class Player final : public IPhysicsEntity
{
public:
    Player(bn::fixed_point position, scene::GameState& gameState);

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

    enum class AnimationState
    {
        IDLE,
        JUMP,
        FALL,
        LAND,
        MERGE_START,
        MERGE_END,
        SPLIT_START = MERGE_START,
        SPLIT_END = MERGE_END
    };

    AnimationState GetAnimationState() const;
    bool GetAnimationDone() const;

    bn::fixed_point GetLastSafePosition() const;
    void SetLastSafePosition(bn::fixed_point safePosition);

private:
    static constexpr int ANIMATION_MAX_FRAMES = 3;

    bn::fixed_point lastSafePosition_;

    bn::optional<bn::sprite_animate_action<ANIMATION_MAX_FRAMES>> spriteAnimation_;
    AnimationState animationState_ = AnimationState::IDLE;

    /**
     * @brief Additional wait update before the InitIdleAction() is called.
     (for MERGE_START, InitMergeEndAction() is called instead).
     * If it is `-1`, InitIdleAction() is not called.
     * (i.e. the last animation frame is used after the action ends.)
     *
     */
    int additionalWaitUpdateCount_ = -1;

    int mergePosDeltaCounter_ = 0;
    int divergeResetCounter_ = 0;
    bn::fixed_point diverge_ = {0, 0};

    bool isControllable_ = true;

    scene::GameState& gameState_;

    /**
     * @brief Updates animation.
     *
     * @return `true` if animation is done, otherwise `false`
     */
    bool UpdateAnimation_();
    void DestroyAnimation_();

    void AddRelativeYPos_(bn::fixed_point& resultPos, bool isFront) const;

    // Something's utterly wrong, why entity::Player deals with this?
    // What a design flaw.  망했어요...
    void MergeSymbolsInHands_();
    void SplitSymbolsInHands_();

    void ResetMergePosDeltaCounter_();
};

} // namespace sym::game::entity
