#include "game_system_Transition.h"

#include <bn_assert.h>

#include "scene_GameState.h"

namespace sym::game::system
{

namespace
{

template <typename NestedIt> void SetBlendingEnabledToEntites_(NestedIt begin, NestedIt end, bool isEnable)
{
    for (NestedIt i = begin; i != end; ++i)
    {
        for (auto j = i->begin(); j != i->end(); ++j)
        {
            j->SetBlendingEnabled(isEnable);
        }
    }
}

template <typename NestedIt> void SetMosaicEnabledToEntites_(NestedIt begin, NestedIt end, bool isEnable)
{
    for (NestedIt i = begin; i != end; ++i)
    {
        for (auto j = i->begin(); j != i->end(); ++j)
        {
            j->SetMosaicEnabled(isEnable);
        }
    }
}

} // namespace

Transition::Transition(scene::GameState& state) : ISystem(state)
{
    Clear_();
}

void Transition::Update()
{
    if (!IsOngoing())
        return;

    switch (transitionState_)
    {
    case State::FIRST_TRANSITION:
        if (transition_.GetState() == effect::Transition::State::DONE)
        {
            waitCounter_ = waitCountBetween_;
            transitionState_ = State::WAIT_BETWEEN;
            if (waitBetweenEventHandler_)
                waitBetweenEventHandler_();
        }
        else
        {
            transition_.Update();
        }
        break;
    case State::WAIT_BETWEEN:
        if (--waitCounter_ < 0)
        {
            const bool isInAndOut = fullDirection_ == FullDirection::IN_AND_OUT;
            transition_.Init(isInAndOut ? outTypes_ : inTypes_, isInAndOut ? Direction::OUT : Direction::IN,
                             isInAndOut ? outUpdateCount_ : inUpdateCount_);
            transitionState_ = State::LAST_TRANSITION;
            if (lastTransitionEventHandler_)
                lastTransitionEventHandler_();
        }
        break;
    case State::LAST_TRANSITION:
        if (transition_.GetState() == effect::Transition::State::DONE)
        {
            waitCounter_ = waitCountTillDone_;
            transitionState_ = State::WAIT_TILL_DONE;
            if (waitTillDoneEventHandler_)
                waitTillDoneEventHandler_();
        }
        else
        {
            transition_.Update();
        }
        break;
    case State::WAIT_TILL_DONE:
        if (--waitCounter_ < 0)
        {
            std::function<void()> doneEventHandler = std::move(doneEventHandler_);
            Clear_();
            transitionState_ = State::DONE;
            if (doneEventHandler)
                doneEventHandler();
        }
        break;
    default:
        BN_ERROR("Invalid transition state: ", static_cast<int>(transitionState_),
                 ", on direction: ", static_cast<int>(fullDirection_));
    }
}

void Transition::InitInAndOut(Types in, int inUpdateCount, Types out, int outUpdateCount, int waitCountBetween,
                              int waitCountTillDone)
{
    BN_ASSERT(!IsOngoing(), "Can't Init transition when it's ongoing");
    Set_(FullDirection::IN_AND_OUT, State::FIRST_TRANSITION, in, inUpdateCount, out, outUpdateCount, waitCountBetween,
         waitCountTillDone);
    transition_.Init(in, Direction::IN, inUpdateCount);
}

void Transition::InitInAndOut(Types inAndOut, int inUpdateCount, int outUpdateCount, int waitCountBetween,
                              int waitCountTillDone)
{
    InitInAndOut(inAndOut, inUpdateCount, inAndOut, outUpdateCount, waitCountBetween, waitCountTillDone);
}

void Transition::InitOutAndIn(Types out, int outUpdateCount, Types in, int inUpdateCount, int waitCountBetween,
                              int waitCountTillDone)
{
    BN_ASSERT(!IsOngoing(), "Can't Init transition when it's ongoing");
    Set_(FullDirection::OUT_AND_IN, State::FIRST_TRANSITION, in, inUpdateCount, out, outUpdateCount, waitCountBetween,
         waitCountTillDone);
    transition_.Init(out, Direction::OUT, outUpdateCount);
}

void Transition::InitOutAndIn(Types outAndIn, int outUpdateCount, int inUpdateCount, int waitCountBetween,
                              int waitCountTillDone)
{
    InitOutAndIn(outAndIn, outUpdateCount, outAndIn, inUpdateCount, waitCountBetween, waitCountTillDone);
}

void Transition::InitIn(Types in, int updateCount, int waitCountTillDone)
{
    BN_ASSERT(!IsOngoing(), "Can't Init transition when it's ongoing");
    Set_(FullDirection::IN, State::LAST_TRANSITION, in, updateCount, Types::NONE, -1, -1, waitCountTillDone);
    transition_.Init(in, Direction::IN, updateCount);
}

void Transition::InitOut(Types out, int updateCount, int waitCountTillDone)
{
    BN_ASSERT(!IsOngoing(), "Can't Init transition when it's ongoing");
    Set_(FullDirection::OUT, State::LAST_TRANSITION, Types::NONE, -1, out, updateCount, -1, waitCountTillDone);
    transition_.Init(out, Direction::OUT, updateCount);
}

Transition::Types Transition::GetInTypes() const
{
    return inTypes_;
}

Transition::Types Transition::GetOutTypes() const
{
    return outTypes_;
}

Transition::State Transition::GetState() const
{
    return transitionState_;
}

bool Transition::IsOngoing() const
{
    return transitionState_ != State::NOT_STARTED;
}

Transition::FullDirection Transition::GetFullDirection() const
{
    return fullDirection_;
}

Transition::AppliedItems Transition::GetBlendingAppliedItems() const
{
    return blendingAppliedItems_;
}

void Transition::SetBlendingAppliedItems(AppliedItems appliedItems)
{
    state_.currentMapBg.set_blending_enabled(!!(appliedItems & AppliedItems::MAP_BG));
    state_.player.SetBlendingEnabled(!!(appliedItems & AppliedItems::PLAYER));
    if (state_.symbolsInHands[0])
        state_.symbolsInHands[0]->SetBlendingEnabled(!!(appliedItems & AppliedItems::SYMBOLS_IN_HANDS));
    if (state_.symbolsInHands[1])
        state_.symbolsInHands[1]->SetBlendingEnabled(!!(appliedItems & AppliedItems::SYMBOLS_IN_HANDS));
    for (auto& textMsg : state_.triggerInteraction.GetSignMessageTextSprites())
        textMsg.set_blending_enabled(!!(appliedItems & AppliedItems::SIGN_MESSAGES));
    if (state_.blackHole)
        state_.blackHole->SetBlendingEnabled(!!(appliedItems & AppliedItems::BLACK_HOLE));
    auto pair = state_.stageNameSplash.GetNameAndSubNameSprites();
    for (auto& textMsg : pair.first)
        textMsg.set_blending_enabled(!!(appliedItems & AppliedItems::STAGE_NAME_SPLASH));
    for (auto& textMsg : pair.second)
        textMsg.set_blending_enabled(!!(appliedItems & AppliedItems::STAGE_NAME_SPLASH));
    // WARN: Menu option is not modified here.

    if (!!(appliedItems & AppliedItems::SPRITES_OF_ZONES) != !!(blendingAppliedItems_ & AppliedItems::SPRITES_OF_ZONES))
    {
        const bool isEnable = !!(appliedItems & AppliedItems::SPRITES_OF_ZONES);
        SetBlendingEnabledToEntites_(state_.doorsOfZones.begin(), state_.doorsOfZones.end(), isEnable);
        SetBlendingEnabledToEntites_(state_.shuttersOfZones.begin(), state_.shuttersOfZones.end(), isEnable);
        SetBlendingEnabledToEntites_(state_.hoverButtonsOfZones.begin(), state_.hoverButtonsOfZones.end(), isEnable);
        SetBlendingEnabledToEntites_(state_.pressureButtonsOfZones.begin(), state_.pressureButtonsOfZones.end(),
                                     isEnable);
        SetBlendingEnabledToEntites_(state_.symbolsOfZones.begin(), state_.symbolsOfZones.end(), isEnable);
        SetBlendingEnabledToEntites_(state_.signsOfZones.begin(), state_.signsOfZones.end(), isEnable);
    }

    blendingAppliedItems_ = appliedItems;
}

Transition::AppliedItems Transition::GetMosaicAppliedItems() const
{
    return mosaicAppliedItems_;
}

void Transition::SetMosaicAppliedItems(AppliedItems appliedItems)
{
    state_.currentMapBg.set_mosaic_enabled(!!(appliedItems & AppliedItems::MAP_BG));
    state_.player.SetMosaicEnabled(!!(appliedItems & AppliedItems::PLAYER));
    if (state_.symbolsInHands[0])
        state_.symbolsInHands[0]->SetMosaicEnabled(!!(appliedItems & AppliedItems::SYMBOLS_IN_HANDS));
    if (state_.symbolsInHands[1])
        state_.symbolsInHands[1]->SetMosaicEnabled(!!(appliedItems & AppliedItems::SYMBOLS_IN_HANDS));
    for (auto& textMsg : state_.triggerInteraction.GetSignMessageTextSprites())
        textMsg.set_mosaic_enabled(!!(appliedItems & AppliedItems::SIGN_MESSAGES));
    if (state_.blackHole)
        state_.blackHole->SetMosaicEnabled(!!(appliedItems & AppliedItems::BLACK_HOLE));
    auto pair = state_.stageNameSplash.GetNameAndSubNameSprites();
    for (auto& textMsg : pair.first)
        textMsg.set_mosaic_enabled(!!(appliedItems & AppliedItems::STAGE_NAME_SPLASH));
    for (auto& textMsg : pair.second)
        textMsg.set_mosaic_enabled(!!(appliedItems & AppliedItems::STAGE_NAME_SPLASH));
    // WARN: Menu option is not modified here.

    if (!!(appliedItems & AppliedItems::SPRITES_OF_ZONES) != !!(mosaicAppliedItems_ & AppliedItems::SPRITES_OF_ZONES))
    {
        const bool isEnable = !!(appliedItems & AppliedItems::SPRITES_OF_ZONES);
        SetMosaicEnabledToEntites_(state_.doorsOfZones.begin(), state_.doorsOfZones.end(), isEnable);
        SetMosaicEnabledToEntites_(state_.shuttersOfZones.begin(), state_.shuttersOfZones.end(), isEnable);
        SetMosaicEnabledToEntites_(state_.hoverButtonsOfZones.begin(), state_.hoverButtonsOfZones.end(), isEnable);
        SetMosaicEnabledToEntites_(state_.pressureButtonsOfZones.begin(), state_.pressureButtonsOfZones.end(),
                                   isEnable);
        SetMosaicEnabledToEntites_(state_.symbolsOfZones.begin(), state_.symbolsOfZones.end(), isEnable);
        SetMosaicEnabledToEntites_(state_.signsOfZones.begin(), state_.signsOfZones.end(), isEnable);
    }

    mosaicAppliedItems_ = appliedItems;
}

void Transition::SetWaitBetweenEventHandler(std::function<void()> eventHandler)
{
    waitBetweenEventHandler_ = std::move(eventHandler);
}

void Transition::SetLastTransitionEventHandler(std::function<void()> eventHandler)
{
    lastTransitionEventHandler_ = std::move(eventHandler);
}

void Transition::SetWaitTillDoneEventHandler(std::function<void()> eventHandler)
{
    waitTillDoneEventHandler_ = std::move(eventHandler);
}

void Transition::SetDoneEventHandler(std::function<void()> eventHandler)
{
    doneEventHandler_ = std::move(eventHandler);
}

void Transition::Set_(FullDirection fullDirection, State state, Types inTypes, int inUpdateCount, Types outTypes,
                      int outUpdateCount, int waitCountBetween, int waitCountTillDone)
{
    fullDirection_ = fullDirection;
    transitionState_ = state;
    inTypes_ = inTypes;
    inUpdateCount_ = inUpdateCount;
    outTypes_ = outTypes;
    outUpdateCount_ = outUpdateCount;
    waitCountBetween_ = waitCountBetween;
    waitCountTillDone_ = waitCountTillDone;
}

void Transition::Clear_()
{
    Set_(FullDirection::NONE, State::NOT_STARTED, Types::NONE, -1, Types::NONE, -1, -1, -1);
    transition_.Destroy();
    waitCounter_ = -1;

    waitBetweenEventHandler_ = nullptr;
    lastTransitionEventHandler_ = nullptr;
    waitTillDoneEventHandler_ = nullptr;
    doneEventHandler_ = nullptr;
}

} // namespace sym::game::system
