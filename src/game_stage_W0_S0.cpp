#include "game_stage_getter.h"

#include "bn_affine_bg_items_bg_w0_s0_0.h"
#include <bn_fixed_rect.h>

namespace sym::game::stage
{

namespace
{

constexpr ZoneInfo zoneInfos_[1] = {ZoneInfo(
    bn::affine_bg_items::bg_w0_s0_0, helper::tilemap::MakeIndexRectByTopLeftAndBottomRight({0, 82}, {127, 127}),
    bn::span<ZoneInfo::SymbolInfo>(), bn::span<ZoneInfo::ButtonInfo>(), bn::span<ZoneInfo::ButtonInfo>(),
    bn::span<ZoneInfo::DoorInfo>(), bn::span<ZoneInfo::ExitInfo>(), bn::span<bn::fixed_point>())};

constexpr StageInfo w0_s0_("W0-0", "Test stage", zoneInfos_);

} // namespace

const StageInfo& Get_W0_S0()
{
    return w0_s0_;
}

} // namespace sym::game::stage
