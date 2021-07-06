#include "game_system_ZoneSwitch.h"

#include <bn_assert.h>

#include "constant.h"
#include "scene_GameState.h"

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

    auto nextZone = GetNextZone_();
    if (nextZone)
        InitTransition(*nextZone);
}

bool ZoneSwitch::InitTransition(ExitInfo nextZone, bool isRestartCurrentZone)
{
    if (state_.transition.IsOngoing())
        return false;

    nextZone_ = nextZone;
    state_.initialExitInfo = nextZone;
    state_.player.SetControllable(false);

    state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::ALL);
    state_.transition.SetMosaicAppliedItems(Transition::AppliedItems::ALL);
    state_.transition.InitOutAndIn(TRANSITION_TYPES, FADE_OUT_UPDATE_COUNT, FADE_IN_UPDATE_COUNT);
    state_.transition.SetWaitBetweenEventHandler(
        [this, isRestartCurrentZone] { SwitchToNextZone_(isRestartCurrentZone); });
    state_.transition.SetLastTransitionEventHandler([this] { state_.player.SetControllable(true); });
    state_.transition.SetDoneEventHandler([this] { nextZone_.reset(); });
    return true;
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

void ZoneSwitch::SwitchToNextZone_(bool isRestartCurrentZone)
{
    BN_ASSERT(nextZone_, "SwitchToNextZone_() called when there is no nextZone_");
    const int prevZoneIdx = state_.currentZoneIdx;

    state_.stageNameSplash.Destroy();

    if ((prevZoneIdx != nextZone_->destinationZoneIndex) || isRestartCurrentZone)
    {
        // unload prev zone graphics
        if (state_.blackHole && state_.blackHoleZoneIdx == prevZoneIdx)
            state_.blackHole->FreeGraphicResource();
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
        for (auto& sign : state_.signsOfZones[state_.currentZoneIdx])
            sign.FreeGraphicResource();
    }

    // switch zone
    state_.currentZoneIdx = nextZone_->destinationZoneIndex;
    const auto& currentZoneInfo = state_.stageInfo.zoneInfos[state_.currentZoneIdx];

    // Reset initial state of current zone
    // to copy back when restarting current zone.
    if (!isRestartCurrentZone)
    {
        state_.initialSymbolsOfCurrentZone.clear();
        state_.initialDoorsOfCurrentZone.clear();
        state_.initialShuttersOfCurrentZone.clear();
        state_.initialHoverButtonsOfCurrentZone.clear();
        state_.initialPressureButtonsOfCurrentZone.clear();

        for (const auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
        {
            state_.initialSymbolsOfCurrentZone.push_back(symbol);
            // state_.initialSymbolsOfCurrentZone.back().FreeGraphicResource();
        }
        state_.initialSymbolsInHandsOnCurrentZone[0] = state_.symbolsInHands[0];
        if (state_.initialSymbolsInHandsOnCurrentZone[0])
            state_.initialSymbolsInHandsOnCurrentZone[0]->FreeGraphicResource();
        state_.initialSymbolsInHandsOnCurrentZone[1] = state_.symbolsInHands[1];
        if (state_.initialSymbolsInHandsOnCurrentZone[1])
            state_.initialSymbolsInHandsOnCurrentZone[1]->FreeGraphicResource();
        for (const auto& door : state_.doorsOfZones[state_.currentZoneIdx])
        {
            state_.initialDoorsOfCurrentZone.push_back(door);
            // state_.initialDoorsOfCurrentZone.back().FreeGraphicResource();
        }
        for (const auto& shutter : state_.shuttersOfZones[state_.currentZoneIdx])
        {
            state_.initialShuttersOfCurrentZone.push_back(shutter);
            // state_.initialShuttersOfCurrentZone.back().FreeGraphicResource();
        }
        for (const auto& button : state_.hoverButtonsOfZones[state_.currentZoneIdx])
        {
            state_.initialHoverButtonsOfCurrentZone.push_back(button);
            // state_.initialHoverButtonsOfCurrentZone.back().FreeGraphicResource();
        }
        for (const auto& button : state_.pressureButtonsOfZones[state_.currentZoneIdx])
        {
            state_.initialPressureButtonsOfCurrentZone.push_back(button);
            // state_.initialPressureButtonsOfCurrentZone.back().FreeGraphicResource();
        }
    }
    // Restart current zone
    else
    {
        // WIP: Add symbols of zones, symbols in hands
        state_.symbolsOfZones[state_.currentZoneIdx].clear();
        state_.doorsOfZones[state_.currentZoneIdx].clear();
        state_.shuttersOfZones[state_.currentZoneIdx].clear();
        state_.hoverButtonsOfZones[state_.currentZoneIdx].clear();
        state_.pressureButtonsOfZones[state_.currentZoneIdx].clear();

        for (const auto& symbol : state_.initialSymbolsOfCurrentZone)
            state_.symbolsOfZones[state_.currentZoneIdx].push_back(symbol);
        state_.symbolsInHands[0] = state_.initialSymbolsInHandsOnCurrentZone[0];
        if (state_.symbolsInHands[0])
            state_.symbolsInHands[0]->AllocateGraphicResource(constant::SYMBOL_Z_ORDER);
        state_.symbolsInHands[1] = state_.initialSymbolsInHandsOnCurrentZone[1];
        if (state_.symbolsInHands[1])
            state_.symbolsInHands[1]->AllocateGraphicResource(constant::SYMBOL_Z_ORDER);
        for (const auto& door : state_.initialDoorsOfCurrentZone)
            state_.doorsOfZones[state_.currentZoneIdx].push_back(door);
        for (const auto& shutter : state_.initialShuttersOfCurrentZone)
            state_.shuttersOfZones[state_.currentZoneIdx].push_back(shutter);
        for (const auto& button : state_.initialHoverButtonsOfCurrentZone)
            state_.hoverButtonsOfZones[state_.currentZoneIdx].push_back(button);
        for (const auto& button : state_.initialPressureButtonsOfCurrentZone)
            state_.pressureButtonsOfZones[state_.currentZoneIdx].push_back(button);
    }

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
    if (state_.blackHole && state_.blackHoleZoneIdx == state_.currentZoneIdx)
    {
        state_.blackHole->AllocateGraphicResource(constant::BLACK_HOLE_Z_ORDER);
        state_.blackHole->SetCamera(state_.camera);
    }
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
    for (auto& sign : state_.signsOfZones[state_.currentZoneIdx])
    {
        sign.AllocateGraphicResource(constant::SIGN_Z_ORDER);
        sign.SetCamera(state_.camera);
    }
}

} // namespace sym::game::system
