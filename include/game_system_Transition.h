#pragma once

#include "game_system_ISystem.h"

#include <functional>

#include <bn_vector.h>

#include "effect_Transition.h"
#include "game_entity_IEntity.h"

namespace sym::game::system
{

/**
 * @brief Deals with blending & mosaic
 *
 */
class Transition final : public ISystem
{
public:
    using Types = effect::Transition::Types;
    using Direction = effect::Transition::Direction;
    enum class FullDirection
    {
        NONE,
        IN,
        OUT,
        IN_AND_OUT,
        OUT_AND_IN
    };
    enum class AppliedItems
    {
        NONE = 0,
        MAP_BG = 1,
        PLAYER = 2,
        SYMBOLS_IN_HANDS = 4,
        SPRITES_OF_ZONES = 8,
        SIGN_MESSAGES = 16,
        MENU_OPTIONS = 32, // fake one, no functionality
        BLACK_HOLE = 64,
        ALL = MAP_BG | PLAYER | SYMBOLS_IN_HANDS | SPRITES_OF_ZONES | SIGN_MESSAGES | MENU_OPTIONS | BLACK_HOLE
    };

    Transition(scene::GameState& state);

    void Update() final;

    void InitInAndOut(Types in, int inUpdateCount, Types out, int outUpdateCount, int waitCountBetween = 0,
                      int waitCountTillDone = 0);
    void InitInAndOut(Types inAndOut, int inUpdateCount, int outUpdateCount, int waitCountBetween = 0,
                      int waitCountTillDone = 0);

    void InitOutAndIn(Types out, int outUpdateCount, Types in, int inUpdateCount, int waitCountBetween = 0,
                      int waitCountTillDone = 0);
    void InitOutAndIn(Types outAndIn, int outUpdateCount, int inUpdateCount, int waitCountBetween = 0,
                      int waitCountTillDone = 0);

    void InitIn(Types in, int updateCount, int waitCountTillDone = 0);
    void InitOut(Types out, int updateCount, int waitCountTillDone = 0);

    [[nodiscard]] Types GetInTypes() const;
    [[nodiscard]] Types GetOutTypes() const;
    [[nodiscard]] FullDirection GetFullDirection() const;

    enum class State
    {
        NOT_STARTED,
        FIRST_TRANSITION,
        WAIT_BETWEEN,
        LAST_TRANSITION,
        WAIT_TILL_DONE,
        DONE = NOT_STARTED
    };
    [[nodiscard]] State GetState() const;
    [[nodiscard]] bool IsOngoing() const;

    [[nodiscard]] AppliedItems GetBlendingAppliedItems() const;
    void SetBlendingAppliedItems(AppliedItems);
    [[nodiscard]] AppliedItems GetMosaicAppliedItems() const;
    void SetMosaicAppliedItems(AppliedItems);

    void SetWaitBetweenEventHandler(std::function<void()> eventHandler);
    void SetLastTransitionEventHandler(std::function<void()> eventHandler);
    void SetWaitTillDoneEventHandler(std::function<void()> eventHandler);
    void SetDoneEventHandler(std::function<void()> eventHandler);

private:
    FullDirection fullDirection_;
    State transitionState_;
    Types inTypes_;
    Types outTypes_;
    int inUpdateCount_;
    int outUpdateCount_;
    int waitCountBetween_;
    int waitCountTillDone_;

    effect::Transition transition_;
    int waitCounter_;

    AppliedItems blendingAppliedItems_ = AppliedItems::ALL;
    AppliedItems mosaicAppliedItems_ = AppliedItems::ALL;

    // Each event handler is invoked right after transitionState_ reaches the same-named phase.
    std::function<void()> waitBetweenEventHandler_;
    std::function<void()> lastTransitionEventHandler_;
    std::function<void()> waitTillDoneEventHandler_;
    std::function<void()> doneEventHandler_;

    void Set_(FullDirection fullDirection, State state, Types inTypes, int inUpdateCount, Types outTypes,
              int outUpdateCount, int waitCountBetween, int waitCountTillDone);

    /**
     * @brief Clears all private members (but not appliedItems_).
     * Automatically called when all transition procedures are done.
     *
     */
    void Clear_();
};

[[nodiscard]] constexpr Transition::AppliedItems operator|(Transition::AppliedItems item1,
                                                           Transition::AppliedItems item2)
{
    return static_cast<Transition::AppliedItems>(static_cast<int>(item1) | static_cast<int>(item2));
}

[[nodiscard]] constexpr Transition::AppliedItems operator&(Transition::AppliedItems item1,
                                                           Transition::AppliedItems item2)
{
    return static_cast<Transition::AppliedItems>(static_cast<int>(item1) & static_cast<int>(item2));
}

[[nodiscard]] constexpr bool operator!(Transition::AppliedItems item)
{
    return item == Transition::AppliedItems::NONE;
}

[[nodiscard]] constexpr Transition::AppliedItems operator~(Transition::AppliedItems item)
{
    return static_cast<Transition::AppliedItems>((static_cast<int>(Transition::AppliedItems::ALL)) &
                                                 (~static_cast<int>(item)));
}

} // namespace sym::game::system
