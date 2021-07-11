#include <bn_affine_bg_item.h>
#include <bn_array.h>
#include <bn_fixed_rect.h>

#include "bn_affine_bg_items_bg_w1_s3_0.h"
#include "bn_music_items.h"
#include "bn_string_view.h"
#include "game_stage_getter.h"

namespace sym::game::stage
{

namespace
{

using namespace bn::affine_bg_items;

constexpr bn::size MAP_DIMENSIONS[] = {
    {
        bg_w1_s3_0.map_item().dimensions().width() * 8,
        bg_w1_s3_0.map_item().dimensions().height() * 8,
    },
};

// Zone 0

constexpr bn::span<ZoneInfo::SymbolInfo> Z0_symbolInfos_ = {

};

constexpr bn::span<ZoneInfo::DoorInfo> Z0_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z0_shutterInfos_[] = {
    {{184 - MAP_DIMENSIONS[0].width() / 2, 448 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{496 - MAP_DIMENSIONS[0].width() / 2, 160 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
};

constexpr ZoneInfo::ButtonInfo Z0_hoverButtonInfos_[] = {
    {{141 - MAP_DIMENSIONS[0].width() / 2, 448 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z0_pressureButtonInfos_ = {

};

constexpr bn::array<bn::string_view, 1> Z0_signTexts_English[] = {
    {"Restart is your friend."},
};

constexpr bn::array<bn::string_view, 1> Z0_signTexts_Korean[] = {
    {"재시작은 여러분의 친구다."},
};

constexpr ZoneInfo::SignInfo Z0_signInfos_[] = {
    {{448 - MAP_DIMENSIONS[0].width() / 2, 254 - MAP_DIMENSIONS[0].height() / 2},
     Z0_signTexts_English[0],
     Z0_signTexts_Korean[0]},
};

constexpr ZoneInfo::ExitInfo Z0_exitInfos_[] = {
    {{552 - MAP_DIMENSIONS[0].width() / 2, 216 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 0},
    {{552 - MAP_DIMENSIONS[0].width() / 2, 320 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 3, 0},
    {{8 - MAP_DIMENSIONS[0].width() / 2, 448 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 3, 1},
    {{352 - MAP_DIMENSIONS[0].width() / 2, 16 - MAP_DIMENSIONS[0].height() / 2, 32, 2}, 3, 2},
    {{8 - MAP_DIMENSIONS[0].width() / 2, 252 - MAP_DIMENSIONS[0].height() / 2, 2, 40}, 2, 0},
};

constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {
    {{395 - MAP_DIMENSIONS[0].width() / 2, 247 - MAP_DIMENSIONS[0].height() / 2}},
    {{514 - MAP_DIMENSIONS[0].width() / 2, 216 - MAP_DIMENSIONS[0].height() / 2}},
    {{515 - MAP_DIMENSIONS[0].width() / 2, 320 - MAP_DIMENSIONS[0].height() / 2}},
    {{69 - MAP_DIMENSIONS[0].width() / 2, 448 - MAP_DIMENSIONS[0].height() / 2}},
    {{364 - MAP_DIMENSIONS[0].width() / 2, 52 - MAP_DIMENSIONS[0].height() / 2}},
    {{37 - MAP_DIMENSIONS[0].width() / 2, 253 - MAP_DIMENSIONS[0].height() / 2}},
};

// Zone 1

constexpr ZoneInfo::SymbolInfo Z1_symbolInfos_[] = {
    {{860 - MAP_DIMENSIONS[0].width() / 2, 138 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
    {{896 - MAP_DIMENSIONS[0].width() / 2, 80 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
    {{936 - MAP_DIMENSIONS[0].width() / 2, 80 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, true},
    {{860 - MAP_DIMENSIONS[0].width() / 2, 19 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, true},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z1_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z1_shutterInfos_[] = {
    {{791 - MAP_DIMENSIONS[0].width() / 2, 104 - MAP_DIMENSIONS[0].height() / 2}, false, 2},
    {{801 - MAP_DIMENSIONS[0].width() / 2, 104 - MAP_DIMENSIONS[0].height() / 2}, false, 3},
    {{916 - MAP_DIMENSIONS[0].width() / 2, 136 - MAP_DIMENSIONS[0].height() / 2}, false, 2},
    {{875 - MAP_DIMENSIONS[0].width() / 2, 80 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{957 - MAP_DIMENSIONS[0].width() / 2, 80 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{916 - MAP_DIMENSIONS[0].width() / 2, 24 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z1_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z1_pressureButtonInfos_[] = {
    {{833 - MAP_DIMENSIONS[0].width() / 2, 136 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
    {{1007 - MAP_DIMENSIONS[0].width() / 2, 136 - MAP_DIMENSIONS[0].height() / 2},
     false,
     3,
     false,
     entity::Symbol::Type::BAR},
    {{833 - MAP_DIMENSIONS[0].width() / 2, 24 - MAP_DIMENSIONS[0].height() / 2}, false, 1, true, bn::nullopt},
    {{999 - MAP_DIMENSIONS[0].width() / 2, 24 - MAP_DIMENSIONS[0].height() / 2}, false, 2, true, bn::nullopt},
};

constexpr bn::span<ZoneInfo::SignInfo> Z1_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z1_exitInfos_[] = {
    {{784 - MAP_DIMENSIONS[0].width() / 2, 104 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 1},
};

constexpr ZoneInfo::EntranceInfo Z1_entranceInfos_[] = {
    {{812 - MAP_DIMENSIONS[0].width() / 2, 104 - MAP_DIMENSIONS[0].height() / 2}},
};

// Zone 2

constexpr bn::span<ZoneInfo::SymbolInfo> Z2_symbolInfos_ = {

};

constexpr bn::span<ZoneInfo::DoorInfo> Z2_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z2_shutterInfos_[] = {
    {{668 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z2_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z2_pressureButtonInfos_[] = {
    {{791 - MAP_DIMENSIONS[0].width() / 2, 224 - MAP_DIMENSIONS[0].height() / 2}, false, 0, true, bn::nullopt},
};

constexpr bn::array<bn::string_view, 1> Z2_signTexts_English[] = {
    {"Restart is your friend."},
};

constexpr bn::array<bn::string_view, 1> Z2_signTexts_Korean[] = {
    {"재시작은 여러분의 친구다."},
};

constexpr ZoneInfo::SignInfo Z2_signInfos_[] = {
    {{950 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2},
     Z2_signTexts_English[0],
     Z2_signTexts_Korean[0]},
};

constexpr ZoneInfo::ExitInfo Z2_exitInfos_[] = {
    {{1024 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 5},
};

constexpr ZoneInfo::EntranceInfo Z2_entranceInfos_[] = {
    {{1000 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2}},
};

// Zone 3

constexpr ZoneInfo::SymbolInfo Z3_symbolInfos_[] = {
    {{723 - MAP_DIMENSIONS[0].width() / 2, 838 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::VV, true},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z3_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z3_shutterInfos_[] = {
    {{802 - MAP_DIMENSIONS[0].width() / 2, 368 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{800 - MAP_DIMENSIONS[0].width() / 2, 464 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{852 - MAP_DIMENSIONS[0].width() / 2, 632 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{586 - MAP_DIMENSIONS[0].width() / 2, 368 - MAP_DIMENSIONS[0].height() / 2}, true, 1},
};

constexpr ZoneInfo::ButtonInfo Z3_hoverButtonInfos_[] = {
    {{754 - MAP_DIMENSIONS[0].width() / 2, 843 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
};

constexpr ZoneInfo::ButtonInfo Z3_pressureButtonInfos_[] = {
    {{762 - MAP_DIMENSIONS[0].width() / 2, 608 - MAP_DIMENSIONS[0].height() / 2},
     false,
     1,
     true,
     entity::Symbol::Type::VV},
};

constexpr bn::span<ZoneInfo::SignInfo> Z3_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z3_exitInfos_[] = {
    {{560 - MAP_DIMENSIONS[0].width() / 2, 368 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 2},
    {{920 - MAP_DIMENSIONS[0].width() / 2, 368 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 3},
    {{980 - MAP_DIMENSIONS[0].width() / 2, 976 - MAP_DIMENSIONS[0].height() / 2, 56, 2}, 0, 4},
};

constexpr ZoneInfo::EntranceInfo Z3_entranceInfos_[] = {
    {{600 - MAP_DIMENSIONS[0].width() / 2, 368 - MAP_DIMENSIONS[0].height() / 2}},
    {{867 - MAP_DIMENSIONS[0].width() / 2, 368 - MAP_DIMENSIONS[0].height() / 2}},
    {{960 - MAP_DIMENSIONS[0].width() / 2, 932 - MAP_DIMENSIONS[0].height() / 2}},
};

// End of Zone

constexpr ZoneInfo::BlackHoleInfo blackHole = {
    {624 - MAP_DIMENSIONS[0].width() / 2, 248 - MAP_DIMENSIONS[0].height() / 2},
};

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w1_s3_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
             Z0_pressureButtonInfos_,
             helper::tilemap::IndexRect({1, 2}, bn::size{68, 59}, bg_w1_s3_0.map_item().dimensions()), Z0_signInfos_,
             Z0_exitInfos_, Z0_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s3_0, Z1_symbolInfos_, Z1_doorInfos_, Z1_shutterInfos_, Z1_hoverButtonInfos_,
             Z1_pressureButtonInfos_,
             helper::tilemap::IndexRect({98, 0}, bn::size{30, 20}, bg_w1_s3_0.map_item().dimensions()), Z1_signInfos_,
             Z1_exitInfos_, Z1_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s3_0, Z2_symbolInfos_, Z2_doorInfos_, Z2_shutterInfos_, Z2_hoverButtonInfos_,
             Z2_pressureButtonInfos_,
             helper::tilemap::IndexRect({70, 21}, bn::size{58, 20}, bg_w1_s3_0.map_item().dimensions()), Z2_signInfos_,
             Z2_exitInfos_, Z2_entranceInfos_, blackHole),
    ZoneInfo(bn::affine_bg_items::bg_w1_s3_0, Z3_symbolInfos_, Z3_doorInfos_, Z3_shutterInfos_, Z3_hoverButtonInfos_,
             Z3_pressureButtonInfos_,
             helper::tilemap::IndexRect({70, 42}, bn::size{58, 80}, bg_w1_s3_0.map_item().dimensions()), Z3_signInfos_,
             Z3_exitInfos_, Z3_entranceInfos_),
};

constexpr int blackHoleCount_ = [] {
    int count = 0;
    for (ZoneInfo zone : zoneInfos_)
    {
        if (zone.blackHole)
            ++count;
    }
    return count;
}();

static_assert(blackHoleCount_ <= 1, "There are more than one blackHole!");

constexpr StageInfo w1_s3_(stage::Id::W1_S3, "Final Stage", "Once Again", "마지막 스테이지", "다시 한번", false,
                           bn::music_items::music_fruity_radioactivity, zoneInfos_);

} // namespace

const StageInfo& Get_W1_S3()
{
    return w1_s3_;
}

} // namespace sym::game::stage
