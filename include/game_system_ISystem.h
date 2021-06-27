#pragma once

#include "scene_GameState_fwd.h"

namespace sym::game::system
{

class ISystem
{
public:
    virtual ~ISystem() = default;
    ISystem(scene::GameState& state) : state_(state)
    {
    }

    virtual void Update() = 0;

protected:
    scene::GameState& state_;
};

} // namespace sym::game::system
