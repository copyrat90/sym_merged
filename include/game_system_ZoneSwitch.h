#pragma once

#include "game_system_ISystem.h"

#include <bn_optional.h>

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
    enum class ExitFadeState
    {
        NOT_STARTED,
        FADE_OUT,
        FADE_IN
    };

    ExitFadeState exitFadeState_ = ExitFadeState::NOT_STARTED;
    bn::optional<ExitInfo> nextZone_;

    void UpdateNextZone_();

    bn::optional<ExitInfo> GetNextZone_();
    void SwitchToNextZone_();
};

} // namespace sym::game::system
