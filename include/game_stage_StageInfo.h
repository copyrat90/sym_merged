#pragma once

#include <bn_affine_bg_item.h>
#include <bn_music_item.h>
#include <bn_span.h>
#include <bn_string_view.h>

#include "game_stage_ZoneInfo.h"

namespace sym::game::stage
{

struct StageInfo
{
    constexpr StageInfo(bn::string_view stageName_a, bn::string_view stageSubName_a,
                        bool isPlayerGravityReversedByDefault_a, const bn::music_item& backgroundMusic_a,
                        const bn::span<const ZoneInfo> zoneInfos_a)
        : stageName(stageName_a), stageSubName(stageSubName_a), backgroundMusic(backgroundMusic_a),
          isPlayerGravityReversedByDefault(isPlayerGravityReversedByDefault_a), zoneInfos(zoneInfos_a)
    {
    }

    bn::string_view stageName;
    bn::string_view stageSubName;
    const bn::optional<const bn::music_item> backgroundMusic;

    bool isPlayerGravityReversedByDefault;
    const bn::span<const ZoneInfo> zoneInfos;
};

} // namespace sym::game::stage
