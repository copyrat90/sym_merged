#include "scene_Game.h"

#include <bn_assert.h>

#include "bn_optional.h"
#include "game_stage_getter.h"

// test
#include <bn_log.h>

namespace sym::scene
{

namespace
{

[[nodiscard]] const game::stage::StageInfo& GetStageInfo(game::stage::Id stageId)
{
    using namespace game;
    // TODO
    switch (stageId)
    {
    case stage::Id::W0_S0:
        return game::stage::Get_W0_S0();
    // case stage::Id::W1_S1:
    //     return game::stage::Get_W1_S1();
    default:
        BN_ERROR("Invalid stage::Id : ", static_cast<int>(stageId));
        break;
    }

    return game::stage::Get_W0_S0();
}

} // namespace

using namespace effect;

Game::Game(game::Status& status)
    : status_(status), stageInfo_(GetStageInfo(status.GetCurrentStage())),
      fadeIn_(Transition::Types::FADE | Transition::Types::BG_MOSAIC | effect::Transition::Types::SPRITE_MOSAIC,
              Transition::Direction::IN, FADE_IN_UPDATE_COUNT),
      fadeOut_(Transition::Types::FADE | Transition::Types::BG_MOSAIC | effect::Transition::Types::SPRITE_MOSAIC,
               Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT)

{
    // test
    BN_LOG(stageInfo_.stageName, ", ", stageInfo_.stageSubName, ", ",
           stageInfo_.zoneInfos[0].zoneBoundary.top_left().y());
}

bn::optional<Type> Game::Update()
{
    return bn::nullopt;
}

} // namespace sym::scene
