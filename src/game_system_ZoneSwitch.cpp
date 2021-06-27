#include "game_system_ZoneSwitch.h"

#include <bn_assert.h>

#include "constant.h"
#include "scene_GameState.h"

#include "bn_log.h"

namespace sym::game::system
{

namespace
{

constexpr Transition::Types TRANSITION_TYPES =
    Transition::Types::FADE | Transition::Types::BG_MOSAIC | Transition::Types::SPRITE_MOSAIC;

constexpr int FADE_IN_UPDATE_COUNT = 30;
constexpr int FADE_OUT_UPDATE_COUNT = 30;

} // namespace

void ZoneSwitch::Update()
{
    if (nextZone_)
        return;

    nextZone_ = GetNextZone_();
    if (nextZone_)
        InitTransition_();
}

void ZoneSwitch::InitTransition_()
{
    state_.player.SetControllable(false);

    state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::ALL);
    state_.transition.SetMosaicAppliedItems(Transition::AppliedItems::ALL);
    state_.transition.InitOutAndIn(TRANSITION_TYPES, FADE_OUT_UPDATE_COUNT, FADE_IN_UPDATE_COUNT);
    state_.transition.SetWaitBetweenEventHandler([this] { SwitchToNextZone_(); });
    state_.transition.SetLastTransitionEventHandler([this] { state_.player.SetControllable(true); });
    state_.transition.SetDoneEventHandler([this] { nextZone_.reset(); });
}

bn::optional<ExitInfo> ZoneSwitch::GetNextZone_()
{
    if (!state_.transition.IsOngoing() && !state_.isPaused)
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
        const bool isBlendingEnabled = state_.currentMapBg.blending_enabled();
        const bool isMosaicEnabled = state_.currentMapBg.mosaic_enabled();
        state_.currentMapBg = currentZoneInfo.mapBg.create_bg({0, 0});
        state_.currentMapBg.set_wrapping_enabled(false);
        state_.currentMapBg.set_blending_enabled(isBlendingEnabled);
        state_.currentMapBg.set_mosaic_enabled(isMosaicEnabled);
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
