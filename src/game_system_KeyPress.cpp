#include "game_system_KeyPress.h"

#include <bn_keypad.h>

namespace sym::game::system
{

KeyPress::KeyPress(scene::GameState& state) : ISystem(state)
{
}

void KeyPress::Update()
{
    UpdateLRKeyLastingCount_();
}

void KeyPress::UpdateLRKeyLastingCount_()
{
    if (state_.lKeyLastingCount >= 0)
        --state_.lKeyLastingCount;
    if (state_.rKeyLastingCount >= 0)
        --state_.rKeyLastingCount;
    if (state_.lKeyCooldownCount >= 0)
        --state_.lKeyCooldownCount;
    if (state_.rKeyCooldownCount >= 0)
        --state_.rKeyCooldownCount;

    if (state_.lKeyCooldownCount < 0 && bn::keypad::l_pressed())
    {
        state_.lKeyLastingCount = state_.KEYPRESS_LASTING_UPDATE_COUNT;
        state_.lKeyCooldownCount = state_.KEYPRESS_COOLDOWN_UPDATE_COUNT;
    }
    if (state_.rKeyCooldownCount < 0 && bn::keypad::r_pressed())
    {
        state_.rKeyLastingCount = state_.KEYPRESS_LASTING_UPDATE_COUNT;
        state_.rKeyCooldownCount = state_.KEYPRESS_COOLDOWN_UPDATE_COUNT;
    }
}

} // namespace sym::game::system
