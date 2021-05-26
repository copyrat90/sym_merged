#pragma once

#include <bn_affine_bg_item.h>
#include <bn_span.h>
#include <bn_string_view.h>

#include "game_stage_ZoneInfo.h"

namespace sym::game::stage
{

struct StageInfo
{
    constexpr StageInfo(bn::string_view stageName_a, bn::string_view stageSubName_a,
                        const bn::span<const ZoneInfo> zoneInfos_a)
        : stageName(stageName_a), stageSubName(stageSubName_a), zoneInfos(zoneInfos_a)
    {
    }

    bn::string_view stageName;
    bn::string_view stageSubName;

    const bn::span<const ZoneInfo> zoneInfos;
};

} // namespace sym::game::stage
