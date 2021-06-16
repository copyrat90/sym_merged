#include "game_system_ButtonInteraction.h"

#include <bn_fixed_rect.h>

#include "bn_sound_items.h"
#include "constant.h"

namespace sym::game::system
{

ButtonInteraction::ButtonInteraction(scene::GameState& state) : ISystem(state)
{
}

void ButtonInteraction::Update()
{
    HoverButtonPlayerInteract_();
    HoverButtonThrownSymbolInteract_();
}

void ButtonInteraction::HoverButtonPlayerInteract_()
{
    if (IsLKeyPressLasts_())
    {
        const bn::fixed_rect leftHand = state_.player.GetLeftButtonInteractRange();

        for (int i = 0; i < state_.hoverButtonsOfZones[state_.currentZoneIdx].size(); ++i)
        {
            auto& hoverButton = state_.hoverButtonsOfZones[state_.currentZoneIdx][i];
            if (!hoverButton.CanButtonBeToggled())
                continue;

            if (leftHand.intersects(hoverButton.GetInteractRange()))
            {
                hoverButton.ToggleButtonOn();
                ToggleOpenedHoverButtonAssociatedOpenables_(i);
                bn::sound_items::sfx_toggle_button.play(constant::volume::sfx_toggle_button);
                ResetLKeyPress_();
                break;
            }
        }
    }

    if (IsRKeyPressLasts_())
    {
        const bn::fixed_rect rightHand = state_.player.GetRightButtonInteractRange();
        for (int i = 0; i < state_.hoverButtonsOfZones[state_.currentZoneIdx].size(); ++i)
        {
            auto& hoverButton = state_.hoverButtonsOfZones[state_.currentZoneIdx][i];
            if (!hoverButton.CanButtonBeToggled())
                continue;

            if (rightHand.intersects(hoverButton.GetInteractRange()))
            {
                hoverButton.ToggleButtonOn();
                ToggleOpenedHoverButtonAssociatedOpenables_(i);
                bn::sound_items::sfx_toggle_button.play();
                ResetRKeyPress_();
                break;
            }
        }
    }
}

void ButtonInteraction::HoverButtonThrownSymbolInteract_()
{
    // TODO
}

bool ButtonInteraction::IsLKeyPressLasts_() const
{
    return state_.lKeyLastingCount >= 0;
}

bool ButtonInteraction::IsRKeyPressLasts_() const
{
    return state_.rKeyLastingCount >= 0;
}

void ButtonInteraction::ResetLKeyPress_()
{
    state_.lKeyLastingCount = -1;
}

void ButtonInteraction::ResetRKeyPress_()
{
    state_.rKeyLastingCount = -1;
}

void ButtonInteraction::ToggleOpenedHoverButtonAssociatedOpenables_(int hoverButtonIdx)
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

} // namespace sym::game::system