#include "game_stage_getter.h"

#include "bn_affine_bg_items_bg_w0_s0_0.h"
#include <bn_fixed_rect.h>

namespace sym::game::stage
{

namespace
{

constexpr ZoneInfo::SymbolInfo symbolInfos_[] = {{{0, 0}, entity::Symbol::Type::BAR},
                                                 {{30, 30}, entity::Symbol::Type::XOR}};

constexpr ZoneInfo zoneInfos_[] = {ZoneInfo(bn::affine_bg_items::bg_w0_s0_0, symbolInfos_,
                                            bn::span<ZoneInfo::DoorInfo>(), bn::span<ZoneInfo::ShutterInfo>(),
                                            bn::span<ZoneInfo::ButtonInfo>(), bn::span<ZoneInfo::ButtonInfo>(),
                                            helper::tilemap::MakeIndexRectByTopLeftAndBottomRight({0, 82}, {127, 127}),
                                            bn::span<ZoneInfo::ExitInfo>(), bn::span<ZoneInfo::EntranceInfo>())};

constexpr StageInfo w0_s0_("W0-0", "Test stage", zoneInfos_);

} // namespace

const StageInfo& Get_W0_S0()
{
    return w0_s0_;
}

} // namespace sym::game::stage
