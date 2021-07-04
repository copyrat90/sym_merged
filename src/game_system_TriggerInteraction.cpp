#include "game_system_TriggerInteraction.h"

#include <bn_algorithm.h>
#include <bn_assert.h>
#include <bn_display.h>
#include <bn_fixed_rect.h>
#include <bn_keypad.h>
#include <bn_sound.h>

#include "bn_log.h"
#include "constant.h"
#include "helper_math.h"
#include "scene_GameState.h"

#include "bn_sound_items.h"
#include "bn_sprite_palette_items_pal_sign_text.h"

namespace sym::game::system
{

namespace
{

constexpr bn::fixed RELATIVE_TOOLTIP_TEXT_POS_DY = 20;

} // namespace

TriggerInteraction::TriggerInteraction(scene::GameState& state) : ISystem(state)
{
}

void TriggerInteraction::Update()
{
    if (state_.isPaused)
        return;
    if (!state_.player.GetControllable())
        return;

    // WIP
    if (bn::keypad::b_held())
    {
        if (!state_.isMergeOrSplitTriggered)
        {
            if (state_.player.GetAnimationState() != entity::Player::AnimationState::MERGE_START)
            {
                const bool mergeCheck =
                    (state_.player.GetGrounded() && !state_.isPaused) &&
                    ((state_.symbolsInHands[0] && state_.symbolsInHands[1] &&
                      state_.symbolsInHands[0]->GetType() < entity::Symbol::COMPLEX_SYMBOL_START_NUM &&
                      state_.symbolsInHands[1]->GetType() < entity::Symbol::COMPLEX_SYMBOL_START_NUM) ||
                     (state_.symbolsInHands[0] && !state_.symbolsInHands[1] &&
                      state_.symbolsInHands[0]->GetType() >= entity::Symbol::COMPLEX_SYMBOL_START_NUM) ||
                     (state_.symbolsInHands[1] && !state_.symbolsInHands[0] &&
                      state_.symbolsInHands[1]->GetType() >= entity::Symbol::COMPLEX_SYMBOL_START_NUM));
                if (mergeCheck)
                {
                    state_.isMergeOrSplitTriggered = true;
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
        if (state_.isMergeOrSplitTriggered)
        {
            state_.isMergeOrSplitTriggered = false;
            if (state_.player.GetAnimationState() == entity::Player::AnimationState::MERGE_START)
            {
                state_.player.InitMergeEndAction();
                bn::sound::stop_all();
            }
        }

        if (IsLKeyPressLasts() || IsRKeyPressLasts())
        {
            if (bn::keypad::down_held())
            {
                if (IsLKeyPressLasts())
                {
                    PlayerPutsDownSymbol_(Hand::LEFT);
                    ResetLKeyPress();
                }
                else if (IsRKeyPressLasts())
                {
                    PlayerPutsDownSymbol_(Hand::RIGHT);
                    ResetRKeyPress();
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
                        ResetLKeyPress();
                    else
                        ResetRKeyPress();
                    break;
                case EntityType::HOVER_BUTTON:
                    PlayerClicksHoverButton_(entityIdx);
                    if (interactHand == Hand::LEFT)
                        ResetLKeyPress();
                    else
                        ResetRKeyPress();
                    break;
                default:
                    BN_ERROR("Invalid TriggerInteraction::EntityType : ", static_cast<int>(nearestEntityType));
                }
            }
        }
    }

    UpdateSignTooltipMessages_();
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
        auto requiredSymbolType = button.GetRequiredSymbolType();

        if (button.GetInteractRange().intersects(playerCollider) && !requiredSymbolType)
        {
            currentButtonOn = true;
        }
        else
        {
            for (const auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
            {
                if (requiredSymbolType && (*requiredSymbolType != symbol.GetType()))
                    continue;

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

bool TriggerInteraction::IsLKeyPressLasts() const
{
    return state_.lKeyLastingCount >= 0;
}

bool TriggerInteraction::IsRKeyPressLasts() const
{
    return state_.rKeyLastingCount >= 0;
}

void TriggerInteraction::ResetLKeyPress()
{
    state_.lKeyLastingCount = -1;
}

void TriggerInteraction::ResetRKeyPress()
{
    state_.rKeyLastingCount = -1;
}

bn::vector<bn::sprite_ptr, 20>& TriggerInteraction::GetSignMessageTextSprites()
{
    return tooltipTextSprites_;
}

void TriggerInteraction::RedrawSignMessages()
{
    tooltipTextSprites_.clear();
    const stage::ZoneInfo& currentZoneInfo = state_.stageInfo.zoneInfos[state_.currentZoneIdx];
    for (int i = 0; i < state_.signsOfZones[state_.currentZoneIdx].size(); ++i)
    {
        entity::Sign& sign = state_.signsOfZones[state_.currentZoneIdx][i];
        if (sign.GetInteractRange().intersects(state_.player.GetInteractRange()))
        {
            const bool isEnglish = global::setting::GetLang() == global::setting::Lang::ENG;
            auto* const textGen = global::GetTextGen();
            auto prevAlignment = textGen->alignment();
            auto prevPalette = textGen->palette_item();
            textGen->set_alignment(bn::sprite_text_generator::alignment_type::CENTER);
            textGen->set_palette_item(bn::sprite_palette_items::pal_sign_text);
            auto generateText = [&](bn::span<const bn::string_view> rawMsgs) {
                using helper::math::operator*;
                const bool isMsgShownUp = state_.camera.position().y() < sign.GetPosition().y();
                if (isMsgShownUp)
                {
                    for (int j = 0; j < rawMsgs.size(); ++j)
                    {
                        if (rawMsgs[j].empty())
                            break;
                        textGen->generate(0, -bn::display::height() / 2 + (j + 1) * RELATIVE_TOOLTIP_TEXT_POS_DY,
                                          rawMsgs[j], tooltipTextSprites_);
                    }
                }
                else
                {
                    int lineCount = 0;
                    for (int j = rawMsgs.size() - 1; j >= 0; --j)
                    {
                        if (rawMsgs[j].empty())
                            continue;
                        textGen->generate(0, bn::display::height() / 2 - (++lineCount) * RELATIVE_TOOLTIP_TEXT_POS_DY,
                                          rawMsgs[j], tooltipTextSprites_);
                    }
                }
            };

            if (isEnglish)
                generateText(currentZoneInfo.signs[i].engMessage);
            else
                generateText(currentZoneInfo.signs[i].korMessage);
            for (auto& text : tooltipTextSprites_)
            {
                text.set_blending_enabled(
                    !!(state_.transition.GetBlendingAppliedItems() & Transition::AppliedItems::SIGN_MESSAGES));
                text.set_mosaic_enabled(
                    !!(state_.transition.GetMosaicAppliedItems() & Transition::AppliedItems::SIGN_MESSAGES));
            }
            textGen->set_alignment(prevAlignment);
            textGen->set_palette_item(prevPalette);

            collidedSign_ = &sign;
            break;
        }
    }
}

void TriggerInteraction::UpdateSignTooltipMessages_()
{
    if (collidedSign_)
    {
        if (!collidedSign_->GetInteractRange().intersects(state_.player.GetInteractRange()))
        {
            tooltipTextSprites_.clear();
            collidedSign_ = nullptr;
        }
    }
    else
    {
        RedrawSignMessages();
    }
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
