#include "bn_affine_bg_item.h"
#include "game_stage_getter.h"

#include "bn_affine_bg_items_bg_w0_s0_0.h"
#include <bn_fixed_rect.h>

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
    {{227 - MAP_DIMENSIONS[0].width() / 2, 927 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR},
    {{254 - MAP_DIMENSIONS[0].width() / 2, 927 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR},
};

constexpr ZoneInfo::DoorInfo Z0_doorInfos_[] = {
    {{87 - MAP_DIMENSIONS[0].width() / 2, 849 - MAP_DIMENSIONS[0].height() / 2}, false, -1, -1, 1},
    {{55 - MAP_DIMENSIONS[0].width() / 2, 849 - MAP_DIMENSIONS[0].height() / 2}, true, -1, -1, 1},
    {{87 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2}, false, -1, -1, 2},
    {{55 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2}, true, -1, -1, 2},
};

constexpr ZoneInfo::ShutterInfo Z0_shutterInfos_[] = {
    {{256 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{135 - MAP_DIMENSIONS[0].width() / 2, 849 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{119 - MAP_DIMENSIONS[0].width() / 2, 849 - MAP_DIMENSIONS[0].height() / 2}, true, 1},
    {{135 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2}, false, 2},
    {{119 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2}, true, 2},
};

constexpr ZoneInfo::ButtonInfo Z0_hoverButtonInfos_[] = {
    {{92 - MAP_DIMENSIONS[0].width() / 2, 962 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{167 - MAP_DIMENSIONS[0].width() / 2, 849 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
    {{167 - MAP_DIMENSIONS[0].width() / 2, 792 - MAP_DIMENSIONS[0].height() / 2}, false, 2},
};

constexpr ZoneInfo::ButtonInfo Z0_pressureButtonInfos_[] = {
    {{136 - MAP_DIMENSIONS[0].width() / 2, 984 - MAP_DIMENSIONS[0].height() / 2}, false, 3},
};

constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {
    {{57 - MAP_DIMENSIONS[0].width() / 2, 950 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
             Z0_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 93}, bn::size{39, 35}, bg_w0_s0_0.map_item().dimensions()),
             bn::span<ZoneInfo::ExitInfo>(), Z0_entranceInfos_),
};

constexpr StageInfo w0_s0_("W0-0", "Test stage", zoneInfos_);

} // namespace

const StageInfo& Get_W0_S0()
{
    return w0_s0_;
}

} // namespace sym::game::stage
