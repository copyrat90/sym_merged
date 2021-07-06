#include "scene_Game.h"

#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_keypad.h>
#include <bn_log.h>

#include "bn_optional.h"
#include "constant.h"
#include "game_stage_getter.h"
#include "helper_tilemap.h"

namespace sym::scene
{

namespace
{

constexpr int FADE_IN_UPDATE_COUNT = 30;

[[nodiscard]] const game::stage::StageInfo& GetStageInfo(game::stage::Id stageId)
{
    using namespace game;
    // TODO
    switch (stageId)
    {
    case stage::Id::W0_S0: // test stage
        return game::stage::Get_W0_S0();
    case stage::Id::W1_S0: // tutorial
        return game::stage::Get_W1_S0();
    default:
        BN_ERROR("Invalid stage::Id : ", static_cast<int>(stageId));
        break;
    }

    return game::stage::Get_W0_S0();
}

} // namespace

using namespace effect;

Game::Game(scene::Param& sceneParam)
    : IScene(sceneParam), state_{
                              sceneParam,
                              GetStageInfo(sceneParam.GetCurrentStage()),
                              0,
                              state_.stageInfo.zoneInfos[0].mapBg.create_bg({0, 0}),
                              bn::camera_ptr::create(0, 0),
                              helper::tilemap::ConvertIndexRectToPositionRect(
                                  state_.stageInfo.zoneInfos[state_.currentZoneIdx].zoneBoundary),
                              {bn::fixed_point{0, 0}, state_.stageInfo.isPlayerGravityReversedByDefault, state_},
                              {},
                              {},
                              {},
                              {},
                              {},
                              {},
                              {},
                              -1,
                              bn::nullopt,
                              {{0, 0, 0, 0}, 0, 0},
                              {},
                              {},
                              {},
                              {},
                              {},
                              {},
                              false,
                              false,
                              helper::tilemap::TileInfo(state_.currentMapBg),
                              -1,
                              -1,
                              {state_},
                              {state_},
                              {state_},
                              {state_},
                              {state_},
                              {state_},
                              {state_},
                              {state_},
                          }
{
    // Set bg
    state_.currentMapBg.set_wrapping_enabled(false);
    state_.currentMapBg.set_blending_enabled(true);
    state_.currentMapBg.set_mosaic_enabled(true);
    state_.currentMapBg.set_camera(state_.camera);

    if (state_.stageInfo.backgroundMusic)
        state_.stageInfo.backgroundMusic->play(constant::volume::GetVolume(*state_.stageInfo.backgroundMusic));

    // Resize vectors according to zone count
    const int zoneCount = state_.stageInfo.zoneInfos.size();
    state_.symbolsOfZones.resize(zoneCount);
    state_.doorsOfZones.resize(zoneCount);
    state_.hoverButtonsOfZones.resize(zoneCount);
    state_.pressureButtonsOfZones.resize(zoneCount);
    state_.shuttersOfZones.resize(zoneCount);
    state_.signsOfZones.resize(zoneCount);

    // Initialize player and camera
    const bn::fixed_point& playerPosition = state_.stageInfo.zoneInfos[0].entrances[0].position;
    state_.player.SetPosition(playerPosition);
    state_.camera.set_position(playerPosition);

    // Load and initialize Entities in zones using ZoneInfo
    for (int i = 0; i < zoneCount; ++i)
    {
        const auto& zoneInfo = state_.stageInfo.zoneInfos[i];

        if (zoneInfo.blackHole)
        {
            state_.blackHoleZoneIdx = i;
            state_.blackHole.emplace(zoneInfo.blackHole->position);
        }
        for (const auto& symbolInfo : zoneInfo.symbols)
            state_.symbolsOfZones[i].emplace_front(symbolInfo.position, symbolInfo.symbolType,
                                                   symbolInfo.isGravityReversedByDefault);
        for (const auto& doorInfo : zoneInfo.doors)
            state_.doorsOfZones[i].emplace_back(doorInfo.position, doorInfo.isOpenedByDefault,
                                                doorInfo.textSpriteNumber);
        for (const auto& shutterInfo : zoneInfo.shutters)
            state_.shuttersOfZones[i].emplace_back(shutterInfo.position, shutterInfo.isOpenedByDefault,
                                                   shutterInfo.textSpriteNumber);
        for (const auto& hoverButtonInfo : zoneInfo.hoverButtons)
            state_.hoverButtonsOfZones[i].emplace_back(hoverButtonInfo.position, hoverButtonInfo.textSpriteNumber,
                                                       hoverButtonInfo.isOnByDefault);
        for (const auto& pressureButtonInfo : zoneInfo.pressureButtons)
            state_.pressureButtonsOfZones[i].emplace_back(
                pressureButtonInfo.position, pressureButtonInfo.textSpriteNumber, pressureButtonInfo.isOnByDefault,
                pressureButtonInfo.isVerticallyReversed, pressureButtonInfo.requiredSymbolType);
        for (const auto& signInfo : zoneInfo.signs)
            state_.signsOfZones[i].emplace_back(signInfo.position);
    }
    if (!state_.blackHole)
        BN_LOG("[WARN] No BlackHole available in this stage!");

    // Copy initial state of current zone
    // to copy back when restarting current zone.
    state_.initialSymbolsOfCurrentZone.clear();
    state_.initialDoorsOfCurrentZone.clear();
    state_.initialShuttersOfCurrentZone.clear();
    state_.initialHoverButtonsOfCurrentZone.clear();
    state_.initialPressureButtonsOfCurrentZone.clear();

    for (const auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
    {
        state_.initialSymbolsOfCurrentZone.push_back(symbol);
        state_.initialSymbolsOfCurrentZone.back().FreeGraphicResource();
    }
    // state_.initialSymbolsInHandsOnCurrentZone[0] = state_.symbolsInHands[0];
    // if (state_.initialSymbolsInHandsOnCurrentZone[0])
    //     state_.initialSymbolsInHandsOnCurrentZone[0]->FreeGraphicResource();
    // state_.initialSymbolsInHandsOnCurrentZone[1] = state_.symbolsInHands[1];
    // if (state_.initialSymbolsInHandsOnCurrentZone[1])
    //     state_.initialSymbolsInHandsOnCurrentZone[1]->FreeGraphicResource();
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

    // Allocate all graphics within current zone
    state_.player.AllocateGraphicResource(constant::PLAYER_Z_ORDER);
    state_.player.SetCamera(state_.camera);
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

    // Initialize Idle action
    state_.player.InitIdleAction();

    // Init Fade-in effect
    state_.transition.InitIn(Transition::Types::FADE | Transition::Types::BG_MOSAIC | Transition::Types::SPRITE_MOSAIC,
                             FADE_IN_UPDATE_COUNT);
}

Game::~Game()
{
    bn::blending::set_fade_alpha(0);
}

bn::optional<Type> Game::Update()
{
    // TODO
    if (!state_.isPaused)
    {
        state_.keyPress.Update();
        state_.triggerInteraction.Update();
        state_.physicsMovement.Update();
        state_.zoneSwitch.Update();
        state_.stageNameSplash.Update();
        state_.stageClear.Update();
    }
    state_.transition.Update();
    state_.menu.Update();

    if (!state_.isPaused)
    {
        // Update sprite graphics
        state_.player.Update();
        for (auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
            symbol.Update();
        for (auto& door : state_.doorsOfZones[state_.currentZoneIdx])
            door.Update();
        for (auto& shutter : state_.shuttersOfZones[state_.currentZoneIdx])
            shutter.Update();
        for (auto& hoverButton : state_.hoverButtonsOfZones[state_.currentZoneIdx])
            hoverButton.Update();
        for (auto& pressureButton : state_.pressureButtonsOfZones[state_.currentZoneIdx])
            pressureButton.Update();

        // Move camera (follows player)
        state_.camera.set_position(state_.player.GetPosition());
        helper::tilemap::SnapCameraToZoneBoundary(state_.camera, state_.zoneBoundary);
    }

    return state_.nextScene;
}

void Game::SetCurrentZone_(int zoneIdx)
{
    BN_ASSERT(0 <= zoneIdx && zoneIdx < state_.stageInfo.zoneInfos.size(), "Zone index out of range!");

    state_.currentMapBg = state_.stageInfo.zoneInfos[zoneIdx].mapBg.create_bg({0, 0});
    state_.currentMapBg.set_wrapping_enabled(false);
}

} // namespace sym::scene
