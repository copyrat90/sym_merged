#pragma once

#include "scene_IScene.h"

#include <bn_affine_bg_ptr.h>
#include <bn_array.h>
#include <bn_camera_ptr.h>
#include <bn_fixed_point.h>
#include <bn_forward_list.h>
#include <bn_memory.h>
#include <bn_optional.h>
#include <bn_span.h>
#include <bn_vector.h>

#include "effect_Transition.h"
#include "game_Status.h"
#include "game_entity_Door.h"
#include "game_entity_HoverButton.h"
#include "game_entity_Player.h"
#include "game_entity_PressureButton.h"
#include "game_entity_Shutter.h"
#include "game_entity_Symbol.h"
#include "game_stage_Id.h"
#include "game_stage_StageInfo.h"

namespace sym::scene
{

class Game final : public IScene
{
public:
    Game(game::Status& status);
    ~Game();

    [[nodiscard]] bn::optional<Type> Update() final;

private:
    static constexpr int FADE_IN_UPDATE_COUNT = 30;
    static constexpr int FADE_OUT_UPDATE_COUNT = 30;

    static constexpr int ZONE_MAX_COUNT = 8;
    static constexpr int STAGE_SYMBOL_MAX_COUNT = 16;
    static constexpr int ZONE_HOVER_BUTTON_MAX_COUNT = 4;
    static constexpr int ZONE_PRESSURE_BUTTON_MAX_COUNT = 4;
    static constexpr int ZONE_DOOR_MAX_COUNT = 4;
    static constexpr int ZONE_SHUTTER_MAX_COUNT = 4;

    game::Status& status_;
    const game::stage::StageInfo& stageInfo_;

    effect::Transition fadeIn_;
    effect::Transition fadeOut_;

    int currentZoneIdx_;
    bn::affine_bg_ptr currentMapBg_;

    bn::camera_ptr camera_;
    bn::fixed_rect zoneBoundary_;

    // Movable entities.
    game::entity::Player player_;
    bn::vector<bn::forward_list<game::entity::Symbol, STAGE_SYMBOL_MAX_COUNT>, ZONE_MAX_COUNT> symbolsOfZones_;
    bn::array<bn::optional<game::entity::Symbol>, 2> symbolsInHands_;

    // Fixed entities.
    bn::vector<bn::vector<game::entity::Door, ZONE_DOOR_MAX_COUNT>, ZONE_MAX_COUNT> doorsOfZones_;
    bn::vector<bn::vector<game::entity::Shutter, ZONE_SHUTTER_MAX_COUNT>, ZONE_MAX_COUNT> shuttersOfZones_;
    bn::vector<bn::vector<game::entity::HoverButton, ZONE_HOVER_BUTTON_MAX_COUNT>, ZONE_MAX_COUNT> hoverButtonsOfZones_;
    bn::vector<bn::vector<game::entity::PressureButton, ZONE_PRESSURE_BUTTON_MAX_COUNT>, ZONE_MAX_COUNT>
        pressureButtonsOfZones_;

    void SetCurrentZone_(int zoneIdx);
};

} // namespace sym::scene
