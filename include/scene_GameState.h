#pragma once

#include <bn_affine_bg_ptr.h>
#include <bn_list.h>
#include <bn_vector.h>

#include "effect_Transition.h"
#include "game_entity_BlackHole.h"
#include "game_entity_Door.h"
#include "game_entity_HoverButton.h"
#include "game_entity_Player.h"
#include "game_entity_PressureButton.h"
#include "game_entity_Shutter.h"
#include "game_entity_Sign.h"
#include "game_stage_StageInfo.h"
#include "game_system_KeyPress.h"
#include "game_system_Menu.h"
#include "game_system_PhysicsMovement.h"
#include "game_system_StageClear.h"
#include "game_system_StageNameSplash.h"
#include "game_system_Transition.h"
#include "game_system_TriggerInteraction.h"
#include "game_system_ZoneSwitch.h"
#include "helper_tilemap.h"
#include "scene_Param.h"
#include "scene_Type.h"

namespace sym::scene
{

/**
 * @brief All scene::Game member variables reside in here.
 * to easily pass states by reference into Systems.
 *
 */
struct GameState
{
    static constexpr int ZONE_MAX_COUNT = 8;
    static constexpr int STAGE_SYMBOL_MAX_COUNT = 16;
    static constexpr int ZONE_HOVER_BUTTON_MAX_COUNT = 8;
    static constexpr int ZONE_PRESSURE_BUTTON_MAX_COUNT = 8;
    static constexpr int ZONE_DOOR_MAX_COUNT = 8;
    static constexpr int ZONE_SHUTTER_MAX_COUNT = 8;
    static constexpr int ZONE_SIGN_MAX_COUNT = 4;

    scene::Param& sceneParam_;
    const game::stage::StageInfo& stageInfo;

    int currentZoneIdx;
    bn::affine_bg_ptr currentMapBg;

    bn::camera_ptr camera;
    bn::fixed_rect zoneBoundary;

    // Movable entities.
    game::entity::Player player;
    bn::vector<bn::list<game::entity::Symbol, STAGE_SYMBOL_MAX_COUNT>, ZONE_MAX_COUNT> symbolsOfZones;
    bn::array<bn::optional<game::entity::Symbol>, 2> symbolsInHands;

    // Fixed entities.
    bn::vector<bn::vector<game::entity::Door, ZONE_DOOR_MAX_COUNT>, ZONE_MAX_COUNT> doorsOfZones;
    bn::vector<bn::vector<game::entity::Shutter, ZONE_SHUTTER_MAX_COUNT>, ZONE_MAX_COUNT> shuttersOfZones;
    bn::vector<bn::vector<game::entity::HoverButton, ZONE_HOVER_BUTTON_MAX_COUNT>, ZONE_MAX_COUNT> hoverButtonsOfZones;
    bn::vector<bn::vector<game::entity::PressureButton, ZONE_PRESSURE_BUTTON_MAX_COUNT>, ZONE_MAX_COUNT>
        pressureButtonsOfZones;
    bn::vector<bn::vector<game::entity::Sign, ZONE_SIGN_MAX_COUNT>, ZONE_MAX_COUNT> signsOfZones;
    int blackHoleZoneIdx;
    bn::optional<game::entity::BlackHole> blackHole;

    // store initial state of current zone
    // to copy back when restarting current zone.
    bool initialPlayerGravityReversed;
    game::stage::ZoneInfo::ExitInfo initialExitInfo;
    bn::list<game::entity::Symbol, STAGE_SYMBOL_MAX_COUNT> initialSymbolsOfCurrentZone;
    bn::array<bn::optional<game::entity::Symbol>, 2> initialSymbolsInHandsOnCurrentZone;
    bn::vector<game::entity::Door, ZONE_DOOR_MAX_COUNT> initialDoorsOfCurrentZone;
    bn::vector<game::entity::Shutter, ZONE_SHUTTER_MAX_COUNT> initialShuttersOfCurrentZone;
    bn::vector<game::entity::HoverButton, ZONE_HOVER_BUTTON_MAX_COUNT> initialHoverButtonsOfCurrentZone;
    bn::vector<game::entity::PressureButton, ZONE_PRESSURE_BUTTON_MAX_COUNT> initialPressureButtonsOfCurrentZone;

    bool isPaused;
    bool isClearOngoing;

    helper::tilemap::TileInfo currentMapTileInfo;

    static constexpr int KEYPRESS_LASTING_UPDATE_COUNT = 5;
    int lKeyLastingCount;
    int rKeyLastingCount;
    // static constexpr int KEYPRESS_COOLDOWN_UPDATE_COUNT = 20;
    // int lKeyCooldownCount;
    // int rKeyCooldownCount;

    game::system::KeyPress keyPress;
    game::system::TriggerInteraction triggerInteraction;
    game::system::PhysicsMovement physicsMovement;
    game::system::ZoneSwitch zoneSwitch;
    game::system::Transition transition;
    game::system::Menu menu;
    game::system::StageNameSplash stageNameSplash;
    game::system::StageClear stageClear;

    bool isMergeOrSplitTriggered = false;
    bn::optional<scene::Type> nextScene = bn::nullopt;
};

} // namespace sym::scene
