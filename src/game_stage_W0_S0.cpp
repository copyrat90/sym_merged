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
    {{186 - MAP_DIMENSIONS[0].width() / 2, 954 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR},
    {{254 - MAP_DIMENSIONS[0].width() / 2, 927 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR},
};

constexpr ZoneInfo::DoorInfo Z0_doorInfos_[] = {
    {{115 - MAP_DIMENSIONS[0].width() / 2, 860 - MAP_DIMENSIONS[0].height() / 2}, true, -1, -1},
};

constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {
    {{57 - MAP_DIMENSIONS[0].width() / 2, 980 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, Z0_symbolInfos_, Z0_doorInfos_, bn::span<ZoneInfo::ShutterInfo>(),
             bn::span<ZoneInfo::ButtonInfo>(), bn::span<ZoneInfo::ButtonInfo>(),
             helper::tilemap::IndexRect({0, 82}, bn::size{39, 46}, bg_w0_s0_0.map_item().dimensions()),
             bn::span<ZoneInfo::ExitInfo>(), Z0_entranceInfos_),
};

constexpr StageInfo w0_s0_("W0-0", "Test stage", zoneInfos_);

} // namespace

const StageInfo& Get_W0_S0()
{
    return w0_s0_;
}

} // namespace sym::game::stage
