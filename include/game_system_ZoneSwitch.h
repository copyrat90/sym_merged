#pragma once

#include "game_system_ISystem.h"

#include <bn_optional.h>

#include "game_stage_StageInfo.h"

namespace sym::game::system
{
using ExitInfo = stage::ZoneInfo::ExitInfo;

/**
 * @brief All about switching zones,
 * including checking doors and exits.
 *
 */
class ZoneSwitch final : public ISystem
{
public:
    ZoneSwitch(scene::GameState& state) : ISystem(state)
    {
    }

    void Update() final;

private:
    bn::optional<ExitInfo> nextZone_;

    void InitTransition_();

    bn::optional<ExitInfo> GetNextZone_();

    // wait between phase event handler
    void SwitchToNextZone_();
};

} // namespace sym::game::system
