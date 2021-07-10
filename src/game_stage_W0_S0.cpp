#include <bn_affine_bg_item.h>
#include <bn_fixed_rect.h>

#include "bn_affine_bg_items_bg_w0_s0_0.h"
#include "bn_music_items.h"
#include "game_stage_getter.h"

namespace sym::game::stage
{

namespace
{

using namespace bn::affine_bg_items;

constexpr bn::size MAP_DIMENSIONS[] = {
    {
        bg_w0_s0_0.map_item().dimensions().width() * 8,
        bg_w0_s0_0.map_item().dimensions().height() * 8,
    },
};

constexpr ZoneInfo::SymbolInfo Z0_symbolInfos_[] = {
    {{106 - MAP_DIMENSIONS[0].width() / 2, 974 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{127 - MAP_DIMENSIONS[0].width() / 2, 974 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, true},
};

constexpr ZoneInfo::DoorInfo Z0_doorInfos_[] = {
    {{124 - MAP_DIMENSIONS[0].width() / 2, 872 - MAP_DIMENSIONS[0].height() / 2}, false, -1, -1, 2},
};

constexpr ZoneInfo::ShutterInfo Z0_shutterInfos_[] = {
    {{252 - MAP_DIMENSIONS[0].width() / 2, 928 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{154 - MAP_DIMENSIONS[0].width() / 2, 872 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{262 - MAP_DIMENSIONS[0].width() / 2, 928 - MAP_DIMENSIONS[0].height() / 2}, false, 3},
};

constexpr ZoneInfo::ButtonInfo Z0_hoverButtonInfos_[] = {
    {{180 - MAP_DIMENSIONS[0].width() / 2, 974 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
    {{205 - MAP_DIMENSIONS[0].width() / 2, 870 - MAP_DIMENSIONS[0].height() / 2}, false, 2, false, bn::nullopt},
};

constexpr ZoneInfo::ButtonInfo Z0_pressureButtonInfos_[] = {
    {{257 - MAP_DIMENSIONS[0].width() / 2, 856 - MAP_DIMENSIONS[0].height() / 2}, false, 1, false, bn::nullopt},
    {{210 - MAP_DIMENSIONS[0].width() / 2, 920 - MAP_DIMENSIONS[0].height() / 2},
     false,
     3,
     true,                       // test
     entity::Symbol::Type::XOR}, // test
};

constexpr bn::span<ZoneInfo::SignInfo> Z0_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z0_exitInfos_[] = {
    {{312 - MAP_DIMENSIONS[0].width() / 2, 928 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 0},
    {{312 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 1},
};

constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {
    {{57 - MAP_DIMENSIONS[0].width() / 2, 950 - MAP_DIMENSIONS[0].height() / 2}},
    {{283 - MAP_DIMENSIONS[0].width() / 2, 928 - MAP_DIMENSIONS[0].height() / 2}},
    {{289 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::SymbolInfo Z1_symbolInfos_[] = {
    {{472 - MAP_DIMENSIONS[0].width() / 2, 886 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{493 - MAP_DIMENSIONS[0].width() / 2, 886 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z1_doorInfos_ = {

};

constexpr bn::span<ZoneInfo::ShutterInfo> Z1_shutterInfos_ = {

};

constexpr bn::span<ZoneInfo::ButtonInfo> Z1_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z1_pressureButtonInfos_[] = {
    {{136 - MAP_DIMENSIONS[0].width() / 2, 984 - MAP_DIMENSIONS[0].height() / 2}, false, 3, false, bn::nullopt},
};

constexpr bn::array<const bn::string_view, 1> Z1_signTexts_English[] = {
    {"Hello, world!"},
};

constexpr bn::array<const bn::string_view, 1> Z1_signTexts_Korean[] = {
    {"안녕,세상아!"},
};

constexpr ZoneInfo::SignInfo Z1_signInfos_[] = {
    {{409 - MAP_DIMENSIONS[0].width() / 2, 832 - MAP_DIMENSIONS[0].height() / 2},
     Z1_signTexts_English[0],
     Z1_signTexts_Korean[0]},
};

constexpr ZoneInfo::ExitInfo Z1_exitInfos_[] = {
    {{312 - MAP_DIMENSIONS[0].width() / 2, 928 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 1},
    {{312 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 2},
    {{552 - MAP_DIMENSIONS[0].width() / 2, 888 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 2, 0},
    {{504 - MAP_DIMENSIONS[0].width() / 2, 744 - MAP_DIMENSIONS[0].height() / 2, 32, 2}, 2, 1},
};

constexpr ZoneInfo::EntranceInfo Z1_entranceInfos_[] = {
    {{345 - MAP_DIMENSIONS[0].width() / 2, 927 - MAP_DIMENSIONS[0].height() / 2}},
    {{332 - MAP_DIMENSIONS[0].width() / 2, 791 - MAP_DIMENSIONS[0].height() / 2}},
    {{520 - MAP_DIMENSIONS[0].width() / 2, 888 - MAP_DIMENSIONS[0].height() / 2}},
    {{488 - MAP_DIMENSIONS[0].width() / 2, 793 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::SymbolInfo Z2_symbolInfos_[] = {
    {{88 - MAP_DIMENSIONS[0].width() / 2, 686 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{109 - MAP_DIMENSIONS[0].width() / 2, 686 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z2_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z2_shutterInfos_[] = {
    {{319 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{329 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{12 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2}, true, 0},
    {{22 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2}, true, 1},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z2_hoverButtonInfos_ = {

};

constexpr bn::span<ZoneInfo::ButtonInfo> Z2_pressureButtonInfos_ = {

};

constexpr bn::span<ZoneInfo::SignInfo> Z2_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z2_exitInfos_[] = {
    {{0 - MAP_DIMENSIONS[0].width() / 2, 688 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 2},
    {{712 - MAP_DIMENSIONS[0].width() / 2, 736 - MAP_DIMENSIONS[0].height() / 2, 32, 2}, 1, 3},
    {{1024 - MAP_DIMENSIONS[0].width() / 2, 480 - MAP_DIMENSIONS[0].height() / 2, 2, 224}, 3, 0},
};

constexpr ZoneInfo::EntranceInfo Z2_entranceInfos_[] = {
    {{33 - MAP_DIMENSIONS[0].width() / 2, 688 - MAP_DIMENSIONS[0].height() / 2}},
    {{728 - MAP_DIMENSIONS[0].width() / 2, 686 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr bn::span<ZoneInfo::SymbolInfo> Z3_symbolInfos_ = {

};

constexpr bn::span<ZoneInfo::DoorInfo> Z3_doorInfos_ = {

};

constexpr bn::span<ZoneInfo::ShutterInfo> Z3_shutterInfos_ = {

};

constexpr bn::span<ZoneInfo::ButtonInfo> Z3_hoverButtonInfos_ = {

};

constexpr bn::span<ZoneInfo::ButtonInfo> Z3_pressureButtonInfos_ = {

};

constexpr bn::span<ZoneInfo::SignInfo> Z3_signInfos_ = {

};

constexpr bn::span<ZoneInfo::ExitInfo> Z3_exitInfos_ = {

};

constexpr ZoneInfo::EntranceInfo Z3_entranceInfos_[] = {
    {{19 - MAP_DIMENSIONS[0].width() / 2, 258 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::BlackHoleInfo blackHole = {
    {64 - MAP_DIMENSIONS[0].width() / 2, 856 - MAP_DIMENSIONS[0].height() / 2},
};

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
             Z0_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 93}, bn::size{39, 35}, bg_w0_s0_0.map_item().dimensions()), Z0_signInfos_,
             Z0_exitInfos_, Z0_entranceInfos_, blackHole),
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z1_symbolInfos_, Z1_doorInfos_, Z1_shutterInfos_, Z1_hoverButtonInfos_,
             Z1_pressureButtonInfos_,
             helper::tilemap::IndexRect({39, 93}, bn::size{30, 29}, bg_w0_s0_0.map_item().dimensions()), Z1_signInfos_,
             Z1_exitInfos_, Z1_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z2_symbolInfos_, Z2_doorInfos_, Z2_shutterInfos_, Z2_hoverButtonInfos_,
             Z2_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 50}, bn::size{128, 42}, bg_w0_s0_0.map_item().dimensions()), Z2_signInfos_,
             Z2_exitInfos_, Z2_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z3_symbolInfos_, Z3_doorInfos_, Z3_shutterInfos_, Z3_hoverButtonInfos_,
             Z3_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 20}, bn::size{30, 20}, bg_w0_s0_0.map_item().dimensions()), Z3_signInfos_,
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

constexpr StageInfo w0_s0_(stage::Id::W0_S0, "W0-0", "Test stage", "W0-0", "테스트 스테이지", false,
                           bn::music_items::music_oh_well, zoneInfos_);

} // namespace

const StageInfo& Get_W0_S0()
{
    return w0_s0_;
}

} // namespace sym::game::stage
