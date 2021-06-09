#include "game_system_PlayerMovement.h"

#include <bn_keypad.h>

namespace sym::game::system
{

PlayerMovement::PlayerMovement(scene::GameState& state) : ISystem(state)
{
}

void PlayerMovement::Update()
{
    // Move player
    if (bn::keypad::up_held())
    {
        state_.player.SetY(state_.player.GetY() - 3);
    }
    else if (bn::keypad::down_held())
    {
        state_.player.SetY(state_.player.GetY() + 3);
    }
    if (bn::keypad::left_held())
    {
        state_.player.SetX(state_.player.GetX() - 3);
    }
    else if (bn::keypad::right_held())
    {
        state_.player.SetX(state_.player.GetX() + 3);
    }
}

} // namespace sym::game::system
