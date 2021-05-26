#pragma once

#include <bn_affine_bg_ptr.h>
#include <bn_optional.h>
#include <bn_vector.h>

#include "game_entity_Symbol.h"
#include "game_stage_ZoneInfo.h"

namespace sym::game
{

class Zone
{
public:
    Zone(const stage::ZoneInfo& zoneInfo);

    enum NextState
    {
        MOVE_TO_OTHER_ZONE,
        STAGE_CLEAR,
        GO_BACK_TO_STAGE_SELECT,
        SPECIAL_CUTSCENE
    };

    /**
     * @brief Update everything within this zone.
     *
     * @return bn::optional<NextState>
     */
    [[nodiscard]] bn::optional<NextState> Update();

    void FreeGraphicResources();
    void AllocateGraphicResources();

private:
    const stage::ZoneInfo& zoneInfo_;
    bool isGraphicAllocated_;

    bn::optional<bn::affine_bg_ptr> bg_;
    bn::vector<entity::Symbol, 32> symbols_;
};

} // namespace sym::game
