#pragma once

#include <bn_affine_bg_ptr.h>
#include <bn_forward_list.h>
#include <bn_vector.h>

#include "effect_Transition.h"
#include "game_entity_Door.h"
#include "game_entity_HoverButton.h"
#include "game_entity_Player.h"
#include "game_entity_PressureButton.h"
#include "game_entity_Shutter.h"
#include "game_stage_StageInfo.h"
#include "helper_tilemap.h"
#include "scene_Param.h"

namespace sym::scene
{

namespace
{

constexpr int ZONE_MAX_COUNT = 8;
constexpr int STAGE_SYMBOL_MAX_COUNT = 16;
constexpr int ZONE_HOVER_BUTTON_MAX_COUNT = 8;
constexpr int ZONE_PRESSURE_BUTTON_MAX_COUNT = 8;
constexpr int ZONE_DOOR_MAX_COUNT = 8;
constexpr int ZONE_SHUTTER_MAX_COUNT = 8;

} // namespace

/**
 * @brief All scene::Game member variables reside in here.
 * to easily pass states by reference into Systems.
 *
 */
struct GameState
{
    scene::Param& sceneParam_;
    const game::stage::StageInfo& stageInfo;

    effect::Transition fadeIn;
    effect::Transition fadeOut;

    int currentZoneIdx;
    bn::affine_bg_ptr currentMapBg;

    bn::camera_ptr camera;
    bn::fixed_rect zoneBoundary;

    // Movable entities.
    game::entity::Player player;
    bn::vector<bn::forward_list<game::entity::Symbol, STAGE_SYMBOL_MAX_COUNT>, ZONE_MAX_COUNT> symbolsOfZones;
    bn::array<bn::optional<game::entity::Symbol>, 2> symbolsInHands;

    // Fixed entities.
    bn::vector<bn::vector<game::entity::Door, ZONE_DOOR_MAX_COUNT>, ZONE_MAX_COUNT> doorsOfZones;
    bn::vector<bn::vector<game::entity::Shutter, ZONE_SHUTTER_MAX_COUNT>, ZONE_MAX_COUNT> shuttersOfZones;
    bn::vector<bn::vector<game::entity::HoverButton, ZONE_HOVER_BUTTON_MAX_COUNT>, ZONE_MAX_COUNT> hoverButtonsOfZones;
    bn::vector<bn::vector<game::entity::PressureButton, ZONE_PRESSURE_BUTTON_MAX_COUNT>, ZONE_MAX_COUNT>
        pressureButtonsOfZones;

    bool isMenuPopup;

    helper::tilemap::TileInfo currentMapTileInfo;

    static constexpr int KEYPRESS_LASTING_UPDATE_COUNT = 5;
    int lKeyLastingCount;
    int rKeyLastingCount;
    static constexpr int KEYPRESS_COOLDOWN_UPDATE_COUNT = 20;
    int lKeyCooldownCount;
    int rKeyCooldownCount;
};

} // namespace sym::scene