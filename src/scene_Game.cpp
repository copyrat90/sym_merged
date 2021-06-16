#include "scene_Game.h"

#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_keypad.h>

#include "bn_optional.h"
#include "constant.h"
#include "game_stage_getter.h"
#include "helper_tilemap.h"

namespace sym::scene
{

namespace
{

// Sprites with higher z orders are drawn first
// (and therefore can be covered by later sprites)
constexpr int PLAYER_Z_ORDER = 0;
constexpr int SYMBOL_Z_ORDER = -10;
constexpr int DOOR_Z_ORDER = 10;
constexpr int BUTTON_Z_ORDER = 5;

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

Game::Game(scene::Param& sceneParam)
    : IScene(sceneParam),
      state_{
          sceneParam,
          GetStageInfo(sceneParam.GetCurrentStage()),
          {Transition::Types::FADE | Transition::Types::BG_MOSAIC | effect::Transition::Types::SPRITE_MOSAIC,
           Transition::Direction::IN, FADE_IN_UPDATE_COUNT},
          {Transition::Types::FADE | Transition::Types::BG_MOSAIC | effect::Transition::Types::SPRITE_MOSAIC,
           Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT},
          0,
          state_.stageInfo.zoneInfos[0].mapBg.create_bg({0, 0}),
          bn::camera_ptr::create(0, 0),
          helper::tilemap::ConvertIndexRectToPositionRect(
              state_.stageInfo.zoneInfos[state_.currentZoneIdx].zoneBoundary),
          bn::fixed_point{0, 0},
          {},
          {},
          {},
          {},
          {},
          {},
          true,
          helper::tilemap::TileInfo(state_.currentMapBg),
          -1,
          -1,
          -1,
          -1,
      },
      keyPress_(state_), buttonInteraction_(state_), physicsMovement_(state_)

{
    state_.currentMapBg.set_wrapping_enabled(false);
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

    // Initialize player and camera
    const bn::fixed_point& playerPosition = state_.stageInfo.zoneInfos[0].entrances[0].position;
    state_.player.SetPosition(playerPosition);
    state_.camera.set_position(playerPosition);

    // Load and initialize Entities in zones using ZoneInfo
    for (int i = 0; i < zoneCount; ++i)
    {
        const auto& zoneInfo = state_.stageInfo.zoneInfos[i];

        for (const auto& symbolInfo : zoneInfo.symbols)
            state_.symbolsOfZones[i].emplace_front(symbolInfo.position, symbolInfo.symbolType);
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
                pressureButtonInfo.position, pressureButtonInfo.textSpriteNumber, pressureButtonInfo.isOnByDefault);
    }

    // Allocate all graphics within current zone
    state_.player.AllocateGraphicResource(PLAYER_Z_ORDER);
    state_.player.SetCamera(state_.camera);
    for (auto& symbol : state_.symbolsOfZones[state_.currentZoneIdx])
    {
        symbol.AllocateGraphicResource(SYMBOL_Z_ORDER);
        symbol.SetCamera(state_.camera);
    }
    for (auto& door : state_.doorsOfZones[state_.currentZoneIdx])
    {
        door.AllocateGraphicResource(DOOR_Z_ORDER);
        door.SetCamera(state_.camera);
    }
    for (auto& shutter : state_.shuttersOfZones[state_.currentZoneIdx])
    {
        shutter.AllocateGraphicResource(DOOR_Z_ORDER);
        shutter.SetCamera(state_.camera);
    }
    for (auto& hoverButton : state_.hoverButtonsOfZones[state_.currentZoneIdx])
    {
        hoverButton.AllocateGraphicResource(BUTTON_Z_ORDER);
        hoverButton.SetCamera(state_.camera);
    }
    for (auto& pressureButton : state_.pressureButtonsOfZones[state_.currentZoneIdx])
    {
        pressureButton.AllocateGraphicResource(BUTTON_Z_ORDER);
        pressureButton.SetCamera(state_.camera);
    }

    // Initialize Idle action
    state_.player.InitIdleAction();
}

Game::~Game()
{
    bn::blending::set_fade_alpha(0);
}

bn::optional<Type> Game::Update()
{
    // TODO
    keyPress_.Update();
    buttonInteraction_.Update();
    // symbolInteraction_.Update();

    physicsMovement_.Update();

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

    return bn::nullopt;
}

void Game::SetCurrentZone_(int zoneIdx)
{
    BN_ASSERT(0 <= zoneIdx && zoneIdx < state_.stageInfo.zoneInfos.size(), "Zone index out of range!");

    state_.currentMapBg = state_.stageInfo.zoneInfos[zoneIdx].mapBg.create_bg({0, 0});
    state_.currentMapBg.set_wrapping_enabled(false);
}

} // namespace sym::scene
