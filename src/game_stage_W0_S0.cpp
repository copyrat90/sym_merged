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
    {{180 - MAP_DIMENSIONS[0].width() / 2, 974 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{205 - MAP_DIMENSIONS[0].width() / 2, 870 - MAP_DIMENSIONS[0].height() / 2}, false, 2},
};

constexpr ZoneInfo::ButtonInfo Z0_pressureButtonInfos_[] = {
    {{257 - MAP_DIMENSIONS[0].width() / 2, 856 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{210 - MAP_DIMENSIONS[0].width() / 2, 936 - MAP_DIMENSIONS[0].height() / 2}, false, 3},
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
    {{136 - MAP_DIMENSIONS[0].width() / 2, 984 - MAP_DIMENSIONS[0].height() / 2}, false, 3},
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
};

constexpr ZoneInfo::EntranceInfo Z1_entranceInfos_[] = {
    {{345 - MAP_DIMENSIONS[0].width() / 2, 927 - MAP_DIMENSIONS[0].height() / 2}},
    {{332 - MAP_DIMENSIONS[0].width() / 2, 791 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
             Z0_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 93}, bn::size{39, 35}, bg_w0_s0_0.map_item().dimensions()), Z0_signInfos_,
             Z0_exitInfos_, Z0_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z1_symbolInfos_, Z1_doorInfos_, Z1_shutterInfos_, Z1_hoverButtonInfos_,
             Z1_pressureButtonInfos_,
             helper::tilemap::IndexRect({39, 93}, bn::size{30, 29}, bg_w0_s0_0.map_item().dimensions()), Z1_signInfos_,
             Z1_exitInfos_, Z1_entranceInfos_),
};

constexpr StageInfo w0_s0_("W0-0", "Test stage", false, bn::music_items::music_detour, zoneInfos_);

} // namespace

const StageInfo& Get_W0_S0()
{
    return w0_s0_;
}

} // namespace sym::game::stage
