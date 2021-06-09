#pragma once

#include "game_system_ISystem.h"

namespace sym::game::system
{

class PlayerMovement final : public ISystem
{
public:
    PlayerMovement(scene::GameState& state);

    void Update() final;

private:
};

} // namespace sym::game::system
