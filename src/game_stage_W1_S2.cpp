#include <bn_affine_bg_item.h>
#include <bn_array.h>
#include <bn_fixed_rect.h>

#include "bn_affine_bg_items_bg_w1_s2_0.h"
#include "bn_affine_bg_items_bg_w1_s2_1.h"
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
        bg_w1_s2_0.map_item().dimensions().width() * 8,
        bg_w1_s2_0.map_item().dimensions().height() * 8,
    },
    {
        bg_w1_s2_1.map_item().dimensions().width() * 8,
        bg_w1_s2_1.map_item().dimensions().height() * 8,
    },
};

// Zone 0

constexpr ZoneInfo::SymbolInfo Z0_symbolInfos_[] = {
    {{112 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
    {{369 - MAP_DIMENSIONS[0].width() / 2, 273 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{351 - MAP_DIMENSIONS[0].width() / 2, 612 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z0_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z0_shutterInfos_[] = {
    {{393 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{93 - MAP_DIMENSIONS[0].width() / 2, 448 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{232 - MAP_DIMENSIONS[0].width() / 2, 504 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{216 - MAP_DIMENSIONS[0].width() / 2, 608 - MAP_DIMENSIONS[0].height() / 2}, true, 2},
    {{336 - MAP_DIMENSIONS[0].width() / 2, 528 - MAP_DIMENSIONS[0].height() / 2}, false, 2},
};

constexpr ZoneInfo::ButtonInfo Z0_hoverButtonInfos_[] = {
    {{299 - MAP_DIMENSIONS[0].width() / 2, 404 - MAP_DIMENSIONS[0].height() / 2}, false, 1, false, bn::nullopt},
};

constexpr ZoneInfo::ButtonInfo Z0_pressureButtonInfos_[] = {
    {{369 - MAP_DIMENSIONS[0].width() / 2, 216 - MAP_DIMENSIONS[0].height() / 2},
     false,
     0,
     true,
     entity::Symbol::Type::BAR},
    {{260 - MAP_DIMENSIONS[0].width() / 2, 616 - MAP_DIMENSIONS[0].height() / 2},
     false,
     2,
     false,
     entity::Symbol::Type::BAR},
};

constexpr bn::span<ZoneInfo::SignInfo> Z0_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z0_exitInfos_[] = {
    {{432 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 0},
};

constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {
    {{65 - MAP_DIMENSIONS[0].width() / 2, 111 - MAP_DIMENSIONS[0].height() / 2}},
    {{380 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2}},
};

// Zone 1

constexpr bn::span<ZoneInfo::SymbolInfo> Z1_symbolInfos_ = {

};

constexpr bn::span<ZoneInfo::DoorInfo> Z1_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z1_shutterInfos_[] = {
    {{194 - MAP_DIMENSIONS[0].width() / 2, 896 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{46 - MAP_DIMENSIONS[0].width() / 2, 992 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z1_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z1_pressureButtonInfos_[] = {
    {{144 - MAP_DIMENSIONS[0].width() / 2, 888 - MAP_DIMENSIONS[0].height() / 2},
     false,
     0,
     true,
     entity::Symbol::Type::BAR},
    {{95 - MAP_DIMENSIONS[0].width() / 2, 1000 - MAP_DIMENSIONS[0].height() / 2},
     false,
     1,
     false,
     entity::Symbol::Type::PLUS},
};

constexpr bn::span<ZoneInfo::SignInfo> Z1_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z1_exitInfos_[] = {
    {{0 - MAP_DIMENSIONS[0].width() / 2, 896 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 1},
    {{240 - MAP_DIMENSIONS[0].width() / 2, 992 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 2, 0},
    {{240 - MAP_DIMENSIONS[0].width() / 2, 896 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 3, 0},
    {{0 - MAP_DIMENSIONS[0].width() / 2, 992 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 4, 0},
};

constexpr ZoneInfo::EntranceInfo Z1_entranceInfos_[] = {
    {{40 - MAP_DIMENSIONS[0].width() / 2, 898 - MAP_DIMENSIONS[0].height() / 2}},
    {{215 - MAP_DIMENSIONS[0].width() / 2, 992 - MAP_DIMENSIONS[0].height() / 2}},
    {{215 - MAP_DIMENSIONS[0].width() / 2, 898 - MAP_DIMENSIONS[0].height() / 2}},
    {{24 - MAP_DIMENSIONS[0].width() / 2, 992 - MAP_DIMENSIONS[0].height() / 2}},
};

// Zone 2

constexpr ZoneInfo::SymbolInfo Z2_symbolInfos_[] = {
    {{52 - MAP_DIMENSIONS[1].width() / 2, 328 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::BAR, true},
    {{85 - MAP_DIMENSIONS[1].width() / 2, 328 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::BAR, true},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z2_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z2_shutterInfos_[] = {
    {{119 - MAP_DIMENSIONS[1].width() / 2, 384 - MAP_DIMENSIONS[1].height() / 2}, false, 0},
    {{129 - MAP_DIMENSIONS[1].width() / 2, 384 - MAP_DIMENSIONS[1].height() / 2}, false, 1},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z2_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z2_pressureButtonInfos_[] = {
    {{52 - MAP_DIMENSIONS[1].width() / 2, 320 - MAP_DIMENSIONS[1].height() / 2}, false, 0, true, bn::nullopt},
    {{85 - MAP_DIMENSIONS[1].width() / 2, 320 - MAP_DIMENSIONS[1].height() / 2}, false, 1, true, bn::nullopt},
};

constexpr bn::span<ZoneInfo::SignInfo> Z2_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z2_exitInfos_[] = {
    {{0 - MAP_DIMENSIONS[1].width() / 2, 104 - MAP_DIMENSIONS[1].height() / 2, 2, 32}, 1, 1},
};

constexpr ZoneInfo::EntranceInfo Z2_entranceInfos_[] = {
    {{56 - MAP_DIMENSIONS[1].width() / 2, 104 - MAP_DIMENSIONS[1].height() / 2}},
};

// Zone 3

constexpr ZoneInfo::SymbolInfo Z3_symbolInfos_[] = {
    {{65 - MAP_DIMENSIONS[1].width() / 2, 978 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::XOR, false},
    {{398 - MAP_DIMENSIONS[1].width() / 2, 918 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::XOR, true},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z3_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z3_shutterInfos_[] = {
    {{129 - MAP_DIMENSIONS[1].width() / 2, 976 - MAP_DIMENSIONS[1].height() / 2}, true, 0},
    {{315 - MAP_DIMENSIONS[1].width() / 2, 800 - MAP_DIMENSIONS[1].height() / 2}, false, 1},
    {{325 - MAP_DIMENSIONS[1].width() / 2, 800 - MAP_DIMENSIONS[1].height() / 2}, false, 2},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z3_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z3_pressureButtonInfos_[] = {
    {{65 - MAP_DIMENSIONS[1].width() / 2, 984 - MAP_DIMENSIONS[1].height() / 2}, false, 0, false, bn::nullopt},
    {{188 - MAP_DIMENSIONS[1].width() / 2, 840 - MAP_DIMENSIONS[1].height() / 2}, false, 1, false, bn::nullopt},
    {{398 - MAP_DIMENSIONS[1].width() / 2, 912 - MAP_DIMENSIONS[1].height() / 2}, false, 2, true, bn::nullopt},
};

constexpr bn::span<ZoneInfo::SignInfo> Z3_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z3_exitInfos_[] = {
    {{0 - MAP_DIMENSIONS[1].width() / 2, 976 - MAP_DIMENSIONS[1].height() / 2, 2, 32}, 1, 2},
};

constexpr ZoneInfo::EntranceInfo Z3_entranceInfos_[] = {
    {{31 - MAP_DIMENSIONS[1].width() / 2, 976 - MAP_DIMENSIONS[1].height() / 2}},
};

// Zone 4

constexpr ZoneInfo::SymbolInfo Z4_symbolInfos_[] = {
    {{885 - MAP_DIMENSIONS[0].width() / 2, 991 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z4_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z4_shutterInfos_[] = {
    {{760 - MAP_DIMENSIONS[0].width() / 2, 752 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{760 - MAP_DIMENSIONS[0].width() / 2, 784 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{760 - MAP_DIMENSIONS[0].width() / 2, 816 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{760 - MAP_DIMENSIONS[0].width() / 2, 848 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z4_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z4_pressureButtonInfos_[] = {
    {{808 - MAP_DIMENSIONS[0].width() / 2, 24 - MAP_DIMENSIONS[0].height() / 2},
     false,
     0,
     true,
     entity::Symbol::Type::XOR},
};

constexpr bn::span<ZoneInfo::SignInfo> Z4_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z4_exitInfos_[] = {
    {{1024 - MAP_DIMENSIONS[0].width() / 2, 48 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 3},
};

constexpr ZoneInfo::EntranceInfo Z4_entranceInfos_[] = {
    {{972 - MAP_DIMENSIONS[0].width() / 2, 48 - MAP_DIMENSIONS[0].height() / 2}},
};

// End of Zone

constexpr ZoneInfo::BlackHoleInfo blackHole = {
    {756 - MAP_DIMENSIONS[0].width() / 2, 976 - MAP_DIMENSIONS[0].height() / 2},
};

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w1_s2_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
             Z0_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 2}, bn::size{54, 80}, bg_w1_s2_0.map_item().dimensions()), Z0_signInfos_,
             Z0_exitInfos_, Z0_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s2_0, Z1_symbolInfos_, Z1_doorInfos_, Z1_shutterInfos_, Z1_hoverButtonInfos_,
             Z1_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 108}, bn::size{30, 20}, bg_w1_s2_0.map_item().dimensions()), Z1_signInfos_,
             Z1_exitInfos_, Z1_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s2_1, Z2_symbolInfos_, Z2_doorInfos_, Z2_shutterInfos_, Z2_hoverButtonInfos_,
             Z2_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 2}, bn::size{120, 70}, bg_w1_s2_1.map_item().dimensions()), Z2_signInfos_,
             Z2_exitInfos_, Z2_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s2_1, Z3_symbolInfos_, Z3_doorInfos_, Z3_shutterInfos_, Z3_hoverButtonInfos_,
             Z3_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 94}, bn::size{63, 34}, bg_w1_s2_1.map_item().dimensions()), Z3_signInfos_,
             Z3_exitInfos_, Z3_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s2_0, Z4_symbolInfos_, Z4_doorInfos_, Z4_shutterInfos_, Z4_hoverButtonInfos_,
             Z4_pressureButtonInfos_,
             helper::tilemap::IndexRect({79, 0}, bn::size{49, 128}, bg_w1_s2_0.map_item().dimensions()), Z4_signInfos_,
             Z4_exitInfos_, Z4_entranceInfos_, blackHole),
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

constexpr StageInfo w1_s2_(stage::Id::W1_S2, "Stage 2", "Downside Up", "스테이지 2", "아래쪽이 위쪽", false,
                           bn::music_items::music_autang, zoneInfos_);

} // namespace

const StageInfo& Get_W1_S2()
{
    return w1_s2_;
}

} // namespace sym::game::stage
