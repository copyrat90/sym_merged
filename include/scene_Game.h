#pragma once

#include "scene_IScene.h"

#include <bn_memory.h>
#include <bn_span.h>

#include "effect_Transition.h"
#include "game_Status.h"
#include "game_stage_Id.h"
#include "game_stage_StageInfo.h"

namespace sym::scene
{

class Game final : public IScene
{
public:
    Game(game::Status& status);
    [[nodiscard]] bn::optional<Type> Update() final;

private:
    static constexpr int FADE_IN_UPDATE_COUNT = 30;
    static constexpr int FADE_OUT_UPDATE_COUNT = 30;

    game::Status status_;
    const game::stage::StageInfo& stageInfo_;

    effect::Transition fadeIn_;
    effect::Transition fadeOut_;
};

} // namespace sym::scene
