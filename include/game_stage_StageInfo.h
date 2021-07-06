#pragma once

#include <bn_affine_bg_item.h>
#include <bn_music_item.h>
#include <bn_span.h>
#include <bn_string_view.h>

#include "game_stage_Id.h"
#include "game_stage_ZoneInfo.h"

namespace sym::game::stage
{

struct StageInfo
{
    constexpr StageInfo(stage::Id stageId_a, bn::string_view stageNameEng_a, bn::string_view stageSubNameEng_a,
                        bn::string_view stageNameKor_a, bn::string_view stageSubNameKor_a,
                        bool isPlayerGravityReversedByDefault_a, const bn::music_item& backgroundMusic_a,
                        const bn::span<const ZoneInfo> zoneInfos_a)
        : stageId(stageId_a), stageNameEng(stageNameEng_a), stageSubNameEng(stageSubNameEng_a),
          stageNameKor(stageNameKor_a), stageSubNameKor(stageSubNameKor_a), backgroundMusic(backgroundMusic_a),
          isPlayerGravityReversedByDefault(isPlayerGravityReversedByDefault_a), zoneInfos(zoneInfos_a)
    {
    }

    stage::Id stageId;

    bn::string_view stageNameEng;
    bn::string_view stageSubNameEng;
    bn::string_view stageNameKor;
    bn::string_view stageSubNameKor;

    const bn::optional<const bn::music_item> backgroundMusic;

    bool isPlayerGravityReversedByDefault;
    const bn::span<const ZoneInfo> zoneInfos;
};

} // namespace sym::game::stage
