#include <bn_affine_bg_item.h>
#include <bn_array.h>
#include <bn_fixed_rect.h>

#include "bn_affine_bg_items_bg_w1_s1_0.h"
#include "bn_affine_bg_items_bg_w1_s1_1.h"
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
        bg_w1_s1_0.map_item().dimensions().width() * 8,
        bg_w1_s1_0.map_item().dimensions().height() * 8,
    },
    {
        bg_w1_s1_1.map_item().dimensions().width() * 8,
        bg_w1_s1_1.map_item().dimensions().height() * 8,
    },
};

constexpr ZoneInfo::SymbolInfo Z0_symbolInfos_[] = {
    {{211 - MAP_DIMENSIONS[0].width() / 2, 128 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{194 - MAP_DIMENSIONS[0].width() / 2, 128 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{459 - MAP_DIMENSIONS[0].width() / 2, 116 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z0_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z0_shutterInfos_[] = {
    {{80 - MAP_DIMENSIONS[0].width() / 2, 64 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{400 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z0_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z0_pressureButtonInfos_[] = {
    {{112 - MAP_DIMENSIONS[0].width() / 2, 72 - MAP_DIMENSIONS[0].height() / 2},
     false,
     0,
     false,
     entity::Symbol::Type::UP},
    {{459 - MAP_DIMENSIONS[0].width() / 2, 120 - MAP_DIMENSIONS[0].height() / 2}, false, 1, false, bn::nullopt},
};

constexpr bn::span<ZoneInfo::SignInfo> Z0_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z0_exitInfos_[] = {
    {{48 - MAP_DIMENSIONS[0].width() / 2, 176 - MAP_DIMENSIONS[0].height() / 2, 32, 2}, 1, 0},
};

constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {
    {{145 - MAP_DIMENSIONS[0].width() / 2, 95 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::SymbolInfo Z1_symbolInfos_[] = {
    {{47 - MAP_DIMENSIONS[0].width() / 2, 271 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{278 - MAP_DIMENSIONS[0].width() / 2, 421 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::PLUS, false},
    {{607 - MAP_DIMENSIONS[0].width() / 2, 417 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{932 - MAP_DIMENSIONS[0].width() / 2, 363 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
    {{539 - MAP_DIMENSIONS[0].width() / 2, 660 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z1_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z1_shutterInfos_[] = {
    {{328 - MAP_DIMENSIONS[0].width() / 2, 416 - MAP_DIMENSIONS[0].height() / 2}, false, 2},
    {{568 - MAP_DIMENSIONS[0].width() / 2, 416 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{864 - MAP_DIMENSIONS[0].width() / 2, 360 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{592 - MAP_DIMENSIONS[0].width() / 2, 656 - MAP_DIMENSIONS[0].height() / 2}, false, 3},
    {{926 - MAP_DIMENSIONS[0].width() / 2, 656 - MAP_DIMENSIONS[0].height() / 2}, false, 4},
    {{936 - MAP_DIMENSIONS[0].width() / 2, 656 - MAP_DIMENSIONS[0].height() / 2}, false, 5},
    {{946 - MAP_DIMENSIONS[0].width() / 2, 656 - MAP_DIMENSIONS[0].height() / 2}, false, 6},
};

constexpr ZoneInfo::ButtonInfo Z1_hoverButtonInfos_[] = {
    {{566 - MAP_DIMENSIONS[0].width() / 2, 598 - MAP_DIMENSIONS[0].height() / 2}, false, 4, false, bn::nullopt},
};

constexpr ZoneInfo::ButtonInfo Z1_pressureButtonInfos_[] = {
    {{278 - MAP_DIMENSIONS[0].width() / 2, 424 - MAP_DIMENSIONS[0].height() / 2},
     false,
     2,
     false,
     entity::Symbol::Type::PLUS},
    {{511 - MAP_DIMENSIONS[0].width() / 2, 424 - MAP_DIMENSIONS[0].height() / 2}, false, 1, false, bn::nullopt},
    {{932 - MAP_DIMENSIONS[0].width() / 2, 368 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
    {{539 - MAP_DIMENSIONS[0].width() / 2, 664 - MAP_DIMENSIONS[0].height() / 2}, false, 3, false, bn::nullopt},
    {{956 - MAP_DIMENSIONS[0].width() / 2, 608 - MAP_DIMENSIONS[0].height() / 2}, false, 5, false, bn::nullopt},
    {{868 - MAP_DIMENSIONS[0].width() / 2, 664 - MAP_DIMENSIONS[0].height() / 2}, false, 6, false, bn::nullopt},
};

constexpr bn::span<ZoneInfo::SignInfo> Z1_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z1_exitInfos_[] = {
    {{972 - MAP_DIMENSIONS[0].width() / 2, 704 - MAP_DIMENSIONS[0].height() / 2, 40, 2}, 2, 0},
};

constexpr ZoneInfo::EntranceInfo Z1_entranceInfos_[] = {
    {{440 - MAP_DIMENSIONS[0].width() / 2, 366 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::SymbolInfo Z2_symbolInfos_[] = {
    {{614 - MAP_DIMENSIONS[1].width() / 2, 145 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::BAR, false},
    {{383 - MAP_DIMENSIONS[1].width() / 2, 90 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::BAR, false},
    {{45 - MAP_DIMENSIONS[1].width() / 2, 290 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::XOR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z2_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z2_shutterInfos_[] = {
    {{664 - MAP_DIMENSIONS[1].width() / 2, 144 - MAP_DIMENSIONS[1].height() / 2}, false, 0},
    {{735 - MAP_DIMENSIONS[1].width() / 2, 320 - MAP_DIMENSIONS[1].height() / 2}, true, 0},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z2_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z2_pressureButtonInfos_[] = {
    {{614 - MAP_DIMENSIONS[1].width() / 2, 152 - MAP_DIMENSIONS[1].height() / 2}, false, 0, false, bn::nullopt},
};

constexpr bn::span<ZoneInfo::SignInfo> Z2_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z2_exitInfos_[] = {
    {{1024 - MAP_DIMENSIONS[1].width() / 2, 208 - MAP_DIMENSIONS[1].height() / 2, 2, 32}, 3, 0},
};

constexpr ZoneInfo::EntranceInfo Z2_entranceInfos_[] = {
    {{812 - MAP_DIMENSIONS[1].width() / 2, 213 - MAP_DIMENSIONS[1].height() / 2}},
    {{992 - MAP_DIMENSIONS[1].width() / 2, 208 - MAP_DIMENSIONS[1].height() / 2}},
};

constexpr bn::span<ZoneInfo::SymbolInfo> Z3_symbolInfos_ = {

};

constexpr bn::span<ZoneInfo::DoorInfo> Z3_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z3_shutterInfos_[] = {
    {{401 - MAP_DIMENSIONS[0].width() / 2, 808 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{411 - MAP_DIMENSIONS[0].width() / 2, 808 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z3_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z3_pressureButtonInfos_[] = {
    {{181 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2},
     false,
     0,
     false,
     entity::Symbol::Type::BAR},
    {{360 - MAP_DIMENSIONS[0].width() / 2, 816 - MAP_DIMENSIONS[0].height() / 2},
     false,
     1,
     false,
     entity::Symbol::Type::BAR},
};

constexpr bn::span<ZoneInfo::SignInfo> Z3_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z3_exitInfos_[] = {
    {{0 - MAP_DIMENSIONS[0].width() / 2, 976 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 2, 1},
    {{484 - MAP_DIMENSIONS[0].width() / 2, 704 - MAP_DIMENSIONS[0].height() / 2, 56, 2}, 4, 0},
};

constexpr ZoneInfo::EntranceInfo Z3_entranceInfos_[] = {
    {{32 - MAP_DIMENSIONS[0].width() / 2, 976 - MAP_DIMENSIONS[0].height() / 2}},
    {{458 - MAP_DIMENSIONS[0].width() / 2, 750 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::SymbolInfo Z4_symbolInfos_[] = {
    {{328 - MAP_DIMENSIONS[1].width() / 2, 978 - MAP_DIMENSIONS[1].height() / 2}, entity::Symbol::Type::XOR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z4_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z4_shutterInfos_[] = {
    {{264 - MAP_DIMENSIONS[1].width() / 2, 976 - MAP_DIMENSIONS[1].height() / 2}, false, 0},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z4_hoverButtonInfos_ = {

};

constexpr ZoneInfo::ButtonInfo Z4_pressureButtonInfos_[] = {
    {{328 - MAP_DIMENSIONS[1].width() / 2, 984 - MAP_DIMENSIONS[1].height() / 2}, false, 0, false, bn::nullopt},
};

constexpr bn::span<ZoneInfo::SignInfo> Z4_signInfos_ = {

};

constexpr ZoneInfo::ExitInfo Z4_exitInfos_[] = {
    {{60 - MAP_DIMENSIONS[1].width() / 2, 1024 - MAP_DIMENSIONS[1].height() / 2, 56, 2}, 3, 1},
};

constexpr ZoneInfo::EntranceInfo Z4_entranceInfos_[] = {
    {{92 - MAP_DIMENSIONS[1].width() / 2, 977 - MAP_DIMENSIONS[1].height() / 2}},
};

// constexpr ZoneInfo::SymbolInfo Z5_symbolInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::DoorInfo> Z5_doorInfos_ = {

// };

// constexpr ZoneInfo::ShutterInfo Z5_shutterInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z5_hoverButtonInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z5_pressureButtonInfos_[] = {

// };

// constexpr bn::array<bn::string_view, 3> Z5_signTexts_English[] = {

// };

// constexpr bn::array<bn::string_view, 3> Z5_signTexts_Korean[] = {

// };

// constexpr ZoneInfo::SignInfo Z5_signInfos_[] = {

// };

// constexpr ZoneInfo::ExitInfo Z5_exitInfos_[] = {

// };

// constexpr ZoneInfo::EntranceInfo Z5_entranceInfos_[] = {

// };

constexpr ZoneInfo::BlackHoleInfo blackHole = {
    {840 - MAP_DIMENSIONS[1].width() / 2, 672 - MAP_DIMENSIONS[1].height() / 2},
};

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w1_s1_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
             Z0_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 2}, bn::size{65, 20}, bg_w1_s1_0.map_item().dimensions()), Z0_signInfos_,
             Z0_exitInfos_, Z0_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s1_0, Z1_symbolInfos_, Z1_doorInfos_, Z1_shutterInfos_, Z1_hoverButtonInfos_,
             Z1_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 23}, bn::size{128, 65}, bg_w1_s1_0.map_item().dimensions()), Z1_signInfos_,
             Z1_exitInfos_, Z1_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s1_1, Z2_symbolInfos_, Z2_doorInfos_, Z2_shutterInfos_, Z2_hoverButtonInfos_,
             Z2_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 2}, bn::size{128, 47}, bg_w1_s1_1.map_item().dimensions()), Z2_signInfos_,
             Z2_exitInfos_, Z2_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s1_0, Z3_symbolInfos_, Z3_doorInfos_, Z3_shutterInfos_, Z3_hoverButtonInfos_,
             Z3_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 88}, bn::size{68, 40}, bg_w1_s1_0.map_item().dimensions()), Z3_signInfos_,
             Z3_exitInfos_, Z3_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s1_1, Z4_symbolInfos_, Z4_doorInfos_, Z4_shutterInfos_, Z4_hoverButtonInfos_,
             Z4_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 78}, bn::size{115, 50}, bg_w1_s1_1.map_item().dimensions()), Z4_signInfos_,
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

constexpr StageInfo w1_s1_(stage::Id::W1_S1, "Stage 1", "Recombination", "스테이지 1", "재조합", false,
                           bn::music_items::music_oh_well, zoneInfos_);

} // namespace

const StageInfo& Get_W1_S1()
{
    return w1_s1_;
}

} // namespace sym::game::stage
