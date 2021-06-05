#include "scene_Game.h"

#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_keypad.h>

#include "bn_optional.h"
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

Game::Game(game::Status& status)
    : status_(status), stageInfo_(GetStageInfo(status.GetCurrentStage())),
      fadeIn_(Transition::Types::FADE | Transition::Types::BG_MOSAIC | effect::Transition::Types::SPRITE_MOSAIC,
              Transition::Direction::IN, FADE_IN_UPDATE_COUNT),
      fadeOut_(Transition::Types::FADE | Transition::Types::BG_MOSAIC | effect::Transition::Types::SPRITE_MOSAIC,
               Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT),
      currentMapBg_(stageInfo_.zoneInfos[0].mapBg.create_bg({0, 0})), camera_(bn::camera_ptr::create(0, 0)),
      player_({0, 0})

{
    currentZoneIdx_ = 0;
    currentMapBg_.set_wrapping_enabled(false);
    currentMapBg_.set_camera(camera_);
    zoneBoundary_ = helper::tilemap::ConvertIndexRectToPositionRect(stageInfo_.zoneInfos[currentZoneIdx_].zoneBoundary);

    // Resize vectors according to zone count
    const int zoneCount = stageInfo_.zoneInfos.size();
    symbolsOfZones_.resize(zoneCount);
    doorsOfZones_.resize(zoneCount);
    hoverButtonsOfZones_.resize(zoneCount);
    pressureButtonsOfZones_.resize(zoneCount);
    shuttersOfZones_.resize(zoneCount);

    // Initialize player and camera
    const bn::fixed_point& playerPosition = stageInfo_.zoneInfos[0].entrances[0].position;
    player_.SetPosition(playerPosition);
    camera_.set_position(playerPosition);

    // Load and initialize Entities in zones using ZoneInfo
    for (int i = 0; i < zoneCount; ++i)
    {
        const auto& zoneInfo = stageInfo_.zoneInfos[i];

        for (const auto& symbolInfo : zoneInfo.symbols)
            symbolsOfZones_[i].emplace_front(symbolInfo.position, symbolInfo.symbolType);
        for (const auto& doorInfo : zoneInfo.doors)
            doorsOfZones_[i].emplace_back(doorInfo.position, doorInfo.isOpenedByDefault, doorInfo.textSpriteNumber);
        for (const auto& shutterInfo : zoneInfo.shutters)
            shuttersOfZones_[i].emplace_back(shutterInfo.position, shutterInfo.isOpenedByDefault,
                                             shutterInfo.textSpriteNumber);
        for (const auto& hoverButtonInfo : zoneInfo.hoverButtons)
            hoverButtonsOfZones_[i].emplace_back(hoverButtonInfo.position, hoverButtonInfo.textSpriteNumber,
                                                 hoverButtonInfo.isOnByDefault);
        for (const auto& pressureButtonInfo : zoneInfo.pressureButtons)
            pressureButtonsOfZones_[i].emplace_back(pressureButtonInfo.position, pressureButtonInfo.textSpriteNumber,
                                                    pressureButtonInfo.isOnByDefault);
    }

    // Allocate all graphics within current zone
    player_.AllocateGraphicResource(PLAYER_Z_ORDER);
    player_.SetCamera(camera_);
    for (auto& symbol : symbolsOfZones_[currentZoneIdx_])
    {
        symbol.AllocateGraphicResource(SYMBOL_Z_ORDER);
        symbol.SetCamera(camera_);
    }
    for (auto& door : doorsOfZones_[currentZoneIdx_])
    {
        door.AllocateGraphicResource(DOOR_Z_ORDER);
        door.SetCamera(camera_);
    }
    for (auto& shutter : shuttersOfZones_[currentZoneIdx_])
    {
        shutter.AllocateGraphicResource(DOOR_Z_ORDER);
        shutter.SetCamera(camera_);
    }
    for (auto& hoverButton : hoverButtonsOfZones_[currentZoneIdx_])
    {
        hoverButton.AllocateGraphicResource(BUTTON_Z_ORDER);
        hoverButton.SetCamera(camera_);
    }
    for (auto& pressureButton : pressureButtonsOfZones_[currentZoneIdx_])
    {
        pressureButton.AllocateGraphicResource(BUTTON_Z_ORDER);
        pressureButton.SetCamera(camera_);
    }

    // Initialize Idle action
    player_.InitIdleAction();
}

Game::~Game()
{
    bn::blending::set_fade_alpha(0);
}

bn::optional<Type> Game::Update()
{
    // TODO

    // Move player
    if (bn::keypad::up_held())
    {
        player_.SetY(player_.GetY() - 3);
    }
    else if (bn::keypad::down_held())
    {
        player_.SetY(player_.GetY() + 3);
    }
    if (bn::keypad::left_held())
    {
        player_.SetX(player_.GetX() - 3);
    }
    else if (bn::keypad::right_held())
    {
        player_.SetX(player_.GetX() + 3);
    }

    // Update sprite graphics
    player_.Update();
    for (auto& symbol : symbolsOfZones_[currentZoneIdx_])
        symbol.Update();
    for (auto& door : doorsOfZones_[currentZoneIdx_])
        door.Update();
    // for (auto& shutter : shuttersOfZones_[currentZoneIdx_])
    //     shutter.Update();

    // Move camera (follows player)
    camera_.set_position(player_.GetPosition());
    helper::tilemap::SnapCameraToZoneBoundary(camera_, zoneBoundary_);

    return bn::nullopt;
}

void Game::SetCurrentZone_(int zoneIdx)
{
    BN_ASSERT(0 <= zoneIdx && zoneIdx < stageInfo_.zoneInfos.size(), "Zone index out of range!");

    currentMapBg_ = stageInfo_.zoneInfos[zoneIdx].mapBg.create_bg({0, 0});
    currentMapBg_.set_wrapping_enabled(false);
}

} // namespace sym::scene
