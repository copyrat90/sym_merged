#include "scene_Game.h"

#include <bn_assert.h>
#include <bn_blending.h>

#include "bn_optional.h"
#include "game_stage_getter.h"

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
               Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT),
      currentMapBg_(stageInfo_.zoneInfos[0].mapBg.create_bg({0, 0}))

{
    currentZoneIdx_ = 0;
    currentMapBg_.set_wrapping_enabled(false);

    symbolsOfZones_.resize(stageInfo_.zoneInfos.size());

    // Load and initialize Entities in zones using ZoneInfo
    for (int i = 0; i < stageInfo_.zoneInfos.size(); ++i)
    {
        for (auto& symbolInfo : stageInfo_.zoneInfos[i].symbols)
        {
            symbolsOfZones_[i].emplace_front(symbolInfo.position, symbolInfo.symbolType);
        }
        // for (auto& doorInfo : stageInfo_.zoneInfos[i].doors)
        // {
        //     doorsOfZones_[i].emplace_front(...);
        // }
    }

    // Allocate all graphics within current zone
    for (auto& symbol : symbolsOfZones_[currentZoneIdx_])
        symbol.AllocateGraphicResource();
    // for (auto& door : doorsOfZones_[currentZoneIdx_])
    //     door.AllocateGraphicResource();
}

Game::~Game()
{
    bn::blending::set_fade_alpha(0);
}

bn::optional<Type> Game::Update()
{
    // TODO
    return bn::nullopt;
}

void Game::SetCurrentZone_(int zoneIdx)
{
    BN_ASSERT(0 <= zoneIdx && zoneIdx < stageInfo_.zoneInfos.size(), "Zone index out of range!");

    currentMapBg_ = stageInfo_.zoneInfos[zoneIdx].mapBg.create_bg({0, 0});
    currentMapBg_.set_wrapping_enabled(false);
}

} // namespace sym::scene
