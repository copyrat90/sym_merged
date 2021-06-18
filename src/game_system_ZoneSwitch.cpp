#include "game_system_ZoneSwitch.h"

#include <bn_assert.h>

#include "constant.h"

namespace sym::game::system
{
using namespace sym::effect;

void ZoneSwitch::Update()
{
    if (nextZone_)
        UpdateNextZone_();
    else
    {
        nextZone_ = GetNextZone_();
        if (nextZone_)
        {
            UpdateNextZone_();
        }
    }
}

void ZoneSwitch::UpdateNextZone_()
{
    switch (exitFadeState_)
    {
    case ExitFadeState::NOT_STARTED:
        exitFadeState_ = ExitFadeState::FADE_OUT;
        state_.fadeOut.Init();
        state_.isPlayerControllable = false;
        break;
    case ExitFadeState::FADE_OUT:
        if (state_.fadeOut.GetState() == Transition::State::DONE)
        {
            exitFadeState_ = ExitFadeState::FADE_IN;
            SwitchToNextZone_();
            state_.fadeIn.Init();
            state_.isPlayerControllable = true;
        }
        break;
    case ExitFadeState::FADE_IN:
        if (state_.fadeIn.GetState() == Transition::State::DONE)
        {
            exitFadeState_ = ExitFadeState::NOT_STARTED;
            nextZone_.reset();
        }
        break;
    default:
        BN_ERROR("Invalid ExitState: ", static_cast<int>(exitFadeState_));
        break;
    }
}

bn::optional<ExitInfo> ZoneSwitch::GetNextZone_()
{
    if (state_.fadeOut.GetState() != Transition::State::ONGOING &&
        state_.fadeIn.GetState() != Transition::State::ONGOING && !state_.isPaused)
    {
        const auto playerCollider = state_.player.GetPhysicsCollider();
        const auto exits = state_.stageInfo.zoneInfos[state_.currentZoneIdx].exits;
        for (const auto& exit : exits)
        {
            if (playerCollider.intersects(exit.collider))
            {
                return exit;
            }
        }
    }
    return bn::nullopt;
}

void ZoneSwitch::SwitchToNextZone_()
{
    BN_ASSERT(nextZone_, "SwitchToNextZone_() called when there is no nextZone_");

    // unload prev zone graphics
    for (auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
        symbol.FreeGraphicResource();
    for (auto& door : state_.doorsOfZones[state_.currentZoneIdx])
        door.FreeGraphicResource();
    for (auto& shutter : state_.shuttersOfZones[state_.currentZoneIdx])
        shutter.FreeGraphicResource();
    for (auto& hoverButton : state_.hoverButtonsOfZones[state_.currentZoneIdx])
        hoverButton.FreeGraphicResource();
    for (auto& pressureButton : state_.pressureButtonsOfZones[state_.currentZoneIdx])
        pressureButton.FreeGraphicResource();

    // switch zone
    const int prevZoneIdx = state_.currentZoneIdx;
    state_.currentZoneIdx = nextZone_->destinationZoneIndex;
    const auto& currentZoneInfo = state_.stageInfo.zoneInfos[state_.currentZoneIdx];

    // Set bg and tiles
    if (currentZoneInfo.mapBg != state_.stageInfo.zoneInfos[prevZoneIdx].mapBg)
    {
        state_.currentMapBg = currentZoneInfo.mapBg.create_bg({0, 0});
        state_.currentMapBg.set_wrapping_enabled(false);
        state_.currentMapBg.set_blending_enabled(true);
        state_.currentMapBg.set_mosaic_enabled(true);
        state_.currentMapBg.set_camera(state_.camera);
        state_.currentMapTileInfo = helper::tilemap::TileInfo(state_.currentMapBg);
    }
    state_.zoneBoundary = helper::tilemap::ConvertIndexRectToPositionRect(currentZoneInfo.zoneBoundary);
    state_.player.SetPosition(currentZoneInfo.entrances[nextZone_->entranceOfZoneIndex].position);

    // load current zone graphics
    for (auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
    {
        symbol.AllocateGraphicResource(constant::SYMBOL_Z_ORDER);
        symbol.SetCamera(state_.camera);
    }
    for (auto& door : state_.doorsOfZones[state_.currentZoneIdx])
    {
        door.AllocateGraphicResource(constant::DOOR_Z_ORDER);
        door.SetCamera(state_.camera);
    }
    for (auto& shutter : state_.shuttersOfZones[state_.currentZoneIdx])
    {
        shutter.AllocateGraphicResource(constant::DOOR_Z_ORDER);
        shutter.SetCamera(state_.camera);
    }
    for (auto& hoverButton : state_.hoverButtonsOfZones[state_.currentZoneIdx])
    {
        hoverButton.AllocateGraphicResource(constant::BUTTON_Z_ORDER);
        hoverButton.SetCamera(state_.camera);
    }
    for (auto& pressureButton : state_.pressureButtonsOfZones[state_.currentZoneIdx])
    {
        pressureButton.AllocateGraphicResource(constant::BUTTON_Z_ORDER);
        pressureButton.SetCamera(state_.camera);
    }
}

} // namespace sym::game::system