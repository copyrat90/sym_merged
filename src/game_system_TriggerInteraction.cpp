#include "game_system_TriggerInteraction.h"

#include <bn_algorithm.h>
#include <bn_assert.h>
#include <bn_fixed_rect.h>
#include <bn_keypad.h>
#include <bn_sound.h>

#include "bn_sound_items.h"
#include "constant.h"
#include "helper_math.h"
#include "scene_GameState.h"

#include "bn_sound_items.h"

namespace sym::game::system
{

TriggerInteraction::TriggerInteraction(scene::GameState& state) : ISystem(state)
{
}

void TriggerInteraction::Update()
{
    // WIP
    if (bn::keypad::b_held())
    {
        if (!isMergeOrSplitTriggered)
        {
            if (state_.player.GetAnimationState() != entity::Player::AnimationState::MERGE_START)
            {
                if ((state_.player.GetGrounded() && !state_.isPaused && state_.symbolsInHands[0] &&
                     state_.symbolsInHands[1] &&
                     state_.symbolsInHands[0]->GetType() < entity::Symbol::COMPLEX_SYMBOL_START_NUM &&
                     state_.symbolsInHands[1]->GetType() < entity::Symbol::COMPLEX_SYMBOL_START_NUM) ||
                    (state_.symbolsInHands[0] && !state_.symbolsInHands[1] &&
                     state_.symbolsInHands[0]->GetType() >= entity::Symbol::COMPLEX_SYMBOL_START_NUM) ||
                    (state_.symbolsInHands[1] && !state_.symbolsInHands[0] &&
                     state_.symbolsInHands[1]->GetType() >= entity::Symbol::COMPLEX_SYMBOL_START_NUM))
                {
                    isMergeOrSplitTriggered = true;
                    state_.player.InitMergeStartAction();
                    bn::sound_items::sfx_symbol_merge.play(constant::volume::sfx_symbol_merge);
                }
                else if (bn::keypad::b_pressed())
                {
                    bn::sound_items::sfx_error.play(constant::volume::sfx_error);
                }
            }
        }
    }
    else
    {
        isMergeOrSplitTriggered = false;

        if (state_.player.GetAnimationState() == entity::Player::AnimationState::MERGE_START)
        {
            state_.player.InitMergeEndAction();
            bn::sound::stop_all();
        }

        if (IsLKeyPressLasts_() || IsRKeyPressLasts_())
        {
            if (bn::keypad::down_held())
            {
                if (IsLKeyPressLasts_())
                {
                    PlayerPutsDownSymbol_(Hand::LEFT);
                    ResetLKeyPress_();
                }
                else if (IsRKeyPressLasts_())
                {
                    PlayerPutsDownSymbol_(Hand::RIGHT);
                    ResetRKeyPress_();
                }
            }
            else
            {
                auto symbolIter = state_.symbolsOfZones[state_.currentZoneIdx].end();
                int entityIdx = -1;
                auto [nearestEntityType, interactHand] = GetNearestInteractableFromPlayer_(symbolIter, entityIdx);
                switch (nearestEntityType)
                {
                case EntityType::NONE:
                    break;
                case EntityType::SYMBOL:
                    PlayerPicksUpSymbol_(interactHand, symbolIter);
                    if (interactHand == Hand::LEFT)
                        ResetLKeyPress_();
                    else
                        ResetRKeyPress_();
                    break;
                case EntityType::HOVER_BUTTON:
                    PlayerClicksHoverButton_(entityIdx);
                    if (interactHand == Hand::LEFT)
                        ResetLKeyPress_();
                    else
                        ResetRKeyPress_();
                    break;
                default:
                    BN_ERROR("Invalid TriggerInteraction::EntityType : ", static_cast<int>(nearestEntityType));
                }
            }
        }
    }

    InteractHoverButtonsAndThrownSymbol_();
    InteractPressureButtonsAndEntities_();
}

bn::pair<TriggerInteraction::EntityType, TriggerInteraction::Hand> TriggerInteraction::
    GetNearestInteractableFromPlayer_(bn::ilist<entity::Symbol>::iterator& outSymbolIter, int& outHoverButtonIdx)
{
    bn::fixed_rect buttonInteractRange;
    bn::fixed_rect symbolInteractRange;
    Hand interactHand = Hand::RIGHT;
    if (!state_.symbolsInHands[1] && state_.rKeyLastingCount >= state_.lKeyLastingCount)
    {
        buttonInteractRange = state_.player.GetRightButtonInteractRange();
        symbolInteractRange = state_.player.GetRightButtonInteractRange();
        interactHand = Hand::RIGHT;
    }
    else if (!state_.symbolsInHands[0] && state_.rKeyLastingCount < state_.lKeyLastingCount)
    {
        buttonInteractRange = state_.player.GetLeftButtonInteractRange();
        symbolInteractRange = state_.player.GetLeftSymbolPickupRange();
        interactHand = Hand::LEFT;
    }
    else
    {
        return {EntityType::NONE, interactHand};
    }

    EntityType nearestEntityType = EntityType::NONE;
    auto& symbolsOfZone = state_.symbolsOfZones[state_.currentZoneIdx];
    bn::fixed nearestDistanceSquared = -1;
    for (auto symbol = symbolsOfZone.begin(); symbol != symbolsOfZone.end(); ++symbol)
    {
        if (!symbol->GetPickedUp() && symbol->GetInteractRange().intersects(symbolInteractRange))
        {
            const bn::fixed candidateDistanceSquared =
                helper::math::DistanceSquared(symbol->GetInteractRange().position(), symbolInteractRange.position());
            if (nearestEntityType == EntityType::NONE || candidateDistanceSquared < nearestDistanceSquared)
            {
                nearestEntityType = EntityType::SYMBOL;
                outSymbolIter = symbol;
                nearestDistanceSquared = candidateDistanceSquared;
            }
        }
    }

    for (int i = 0; i < state_.hoverButtonsOfZones[state_.currentZoneIdx].size(); ++i)
    {
        auto& button = state_.hoverButtonsOfZones[state_.currentZoneIdx][i];
        if (button.CanButtonBeToggled() && button.GetInteractRange().intersects(buttonInteractRange))
        {
            const bn::fixed candidateDistanceSquared =
                helper::math::DistanceSquared(button.GetInteractRange().position(), buttonInteractRange.position());
            if (nearestEntityType == EntityType::NONE || candidateDistanceSquared < nearestDistanceSquared)
            {
                nearestEntityType = EntityType::HOVER_BUTTON;
                outHoverButtonIdx = i;
                nearestDistanceSquared = candidateDistanceSquared;
            }
        }
    }

    return {nearestEntityType, interactHand};
}

void TriggerInteraction::PlayerPicksUpSymbol_(Hand interactHand, bn::ilist<entity::Symbol>::iterator symbolIter)
{
    bn::sound_items::sfx_symbol_pickup.play(constant::volume::sfx_symbol_pickup);

    symbolIter->SetPickedUp(true);
    symbolIter->SetGravityEnabled(false);

    if (interactHand == Hand::LEFT)
        state_.symbolsInHands[0] = bn::move(*symbolIter);
    else
        state_.symbolsInHands[1] = bn::move(*symbolIter);
    state_.symbolsOfZones[state_.currentZoneIdx].erase(symbolIter);
}

void TriggerInteraction::PlayerPutsDownSymbol_(Hand hand)
{
    if (hand == Hand::LEFT)
    {
        if (state_.symbolsInHands[0])
        {
            state_.symbolsInHands[0]->SetPickedUp(false);
            state_.symbolsInHands[0]->SetGravityEnabled(true);
            state_.symbolsOfZones[state_.currentZoneIdx].push_back(bn::move(*state_.symbolsInHands[0]));
            state_.symbolsInHands[0].reset();
            bn::sound_items::sfx_symbol_drop.play(constant::volume::sfx_symbol_drop);
        }
    }
    else
    {
        if (state_.symbolsInHands[1])
        {
            state_.symbolsInHands[1]->SetPickedUp(false);
            state_.symbolsInHands[1]->SetGravityEnabled(true);
            state_.symbolsOfZones[state_.currentZoneIdx].push_back(bn::move(*state_.symbolsInHands[1]));
            state_.symbolsInHands[1].reset();
            bn::sound_items::sfx_symbol_drop.play(constant::volume::sfx_symbol_drop);
        }
    }
}

void TriggerInteraction::PlayerClicksHoverButton_(int hoverButtonIdx)
{
    auto& hoverButton = state_.hoverButtonsOfZones[state_.currentZoneIdx][hoverButtonIdx];
    hoverButton.ToggleButtonOn();
    ToggleOpenedHoverButtonAssociatedOpenables_(hoverButtonIdx);
    bn::sound_items::sfx_hover_button_toggle.play(constant::volume::sfx_hover_button_toggle);
}

void TriggerInteraction::InteractHoverButtonsAndThrownSymbol_()
{
    // TODO
}

void TriggerInteraction::InteractPressureButtonsAndEntities_()
{
    const bn::fixed_rect playerCollider = state_.player.GetPhysicsCollider();
    auto& pressureButtons = state_.pressureButtonsOfZones[state_.currentZoneIdx];
    for (int buttonIdx = 0; buttonIdx < pressureButtons.size(); ++buttonIdx)
    {
        auto& button = pressureButtons[buttonIdx];

        if (!button.CanButtonBeToggled())
            continue;

        // player & symbol collision check
        const bool prevButtonOn = button.GetButtonOn();
        bool currentButtonOn = false;

        if (button.GetInteractRange().intersects(playerCollider))
        {
            currentButtonOn = true;
        }
        else
        {
            for (const auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
            {
                if (button.GetInteractRange().intersects(symbol.GetPhysicsCollider()))
                {
                    currentButtonOn = true;
                    break;
                }
            }
        }

        // result: toggles or not
        if ((!prevButtonOn && currentButtonOn) || (prevButtonOn && !currentButtonOn))
        {
            button.ToggleButtonOn();
            bn::sound_items::sfx_pressure_button_toggle.play(constant::volume::sfx_pressure_button_toggle);
            ToggleOpenedPressureButtonAssociatedOpenables_(buttonIdx);
        }
    }
}

bool TriggerInteraction::IsLKeyPressLasts_() const
{
    return state_.lKeyLastingCount >= 0;
}

bool TriggerInteraction::IsRKeyPressLasts_() const
{
    return state_.rKeyLastingCount >= 0;
}

void TriggerInteraction::ResetLKeyPress_()
{
    state_.lKeyLastingCount = -1;
}

void TriggerInteraction::ResetRKeyPress_()
{
    state_.rKeyLastingCount = -1;
}

void TriggerInteraction::ToggleOpenedHoverButtonAssociatedOpenables_(int hoverButtonIdx)
{
    const auto& zoneInfo = state_.stageInfo.zoneInfos[state_.currentZoneIdx];
    const int hoverButtonTextNum = zoneInfo.hoverButtons[hoverButtonIdx].textSpriteNumber;

    for (int i = 0; i < state_.doorsOfZones[state_.currentZoneIdx].size(); ++i)
    {
        if (hoverButtonTextNum == zoneInfo.doors[i].textSpriteNumber)
            state_.doorsOfZones[state_.currentZoneIdx][i].ToggleOpened();
    }
    for (int i = 0; i < state_.shuttersOfZones[state_.currentZoneIdx].size(); ++i)
    {
        if (hoverButtonTextNum == zoneInfo.shutters[i].textSpriteNumber)
            state_.shuttersOfZones[state_.currentZoneIdx][i].ToggleOpened();
    }
}

void TriggerInteraction::ToggleOpenedPressureButtonAssociatedOpenables_(int pressureButtonIdx)
{
    const auto& zoneInfo = state_.stageInfo.zoneInfos[state_.currentZoneIdx];
    const int pressureButtonTextNum = zoneInfo.pressureButtons[pressureButtonIdx].textSpriteNumber;

    for (int i = 0; i < state_.doorsOfZones[state_.currentZoneIdx].size(); ++i)
    {
        if (pressureButtonTextNum == zoneInfo.doors[i].textSpriteNumber)
            state_.doorsOfZones[state_.currentZoneIdx][i].ToggleOpened();
    }
    for (int i = 0; i < state_.shuttersOfZones[state_.currentZoneIdx].size(); ++i)
    {
        if (pressureButtonTextNum == zoneInfo.shutters[i].textSpriteNumber)
            state_.shuttersOfZones[state_.currentZoneIdx][i].ToggleOpened();
    }
}

} // namespace sym::game::system
