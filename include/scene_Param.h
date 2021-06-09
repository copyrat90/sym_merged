#pragma once

#include "game_stage_Id.h"

namespace sym::scene
{

class Param
{
public:
    void SetCurrentStage(game::stage::Id stage)
    {
        currentStage_ = stage;
    }
    game::stage::Id GetCurrentStage()
    {
        return currentStage_;
    }

private:
    game::stage::Id currentStage_;
};

} // namespace sym::scene
