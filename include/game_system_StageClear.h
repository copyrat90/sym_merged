#pragma once

#include "game_system_ISystem.h"

#include <bn_array.h>
#include <bn_fixed_point.h>
#include <bn_optional.h>
#include <bn_sprite_actions.h>
#include <bn_vector.h>

namespace sym::game::system
{

class StageClear final : public ISystem
{
public:
    StageClear(scene::GameState& state);

    void Update() final;

private:
    enum class ActionState
    {
        NOT_STARTED,
        ENTER_BLACK_HOLE,
        SPRITE_ACTION_DONE
    };
    ActionState clearState_ = ActionState::NOT_STARTED;

    int updateCountDown_ = -1;
    bn::fixed_point playerDeltaPos_;
    bn::fixed_point leftSymbolDeltaPos_;
    bn::fixed_point rightSymbolDeltaPos_;
    bn::optional<bn::sprite_scale_to_action> playerScaleAction_;
    bn::optional<bn::sprite_scale_to_action> leftSymbolScaleAction_;
    bn::optional<bn::sprite_scale_to_action> rightSymbolScaleAction_;
    bn::array<bn::vector<bn::sprite_ptr, 7>, 2> textSprites_;

    bool CheckCollideWithBlackHole_();
};

} // namespace sym::game::system
