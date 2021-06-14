#pragma once

#include "game_system_ISystem.h"

namespace sym::game::system
{

class KeyPress final : public ISystem
{
public:
    KeyPress(scene::GameState& state);

    void Update() final;

private:
    void UpdateLRKeyLastingCount_();
};

} // namespace sym::game::system
