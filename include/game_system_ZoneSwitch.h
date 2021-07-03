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

    /**
     * @brief Init transition
     *
     * @param `nextZone` the zone to switch
     * @param `isRestartCurrentZone` indicate if it is restart current zone
     *
     * @return `true` if new transition started.
     * @return `false` if there is another transition ongoing.
     */
    bool InitTransition(ExitInfo nextZone, bool isRestartCurrentZone = false);

private:
    bn::optional<ExitInfo> nextZone_;

    bn::optional<ExitInfo> GetNextZone_();

    // wait between phase event handler
    void SwitchToNextZone_(bool isRestartCurrentZone);
};

} // namespace sym::game::system
