// #include <bn_affine_bg_item.h>
// #include <bn_array.h>
// #include <bn_fixed_rect.h>

// #include "bn_affine_bg_items_bg_w1_s2_0.h"
// #include "bn_affine_bg_items_bg_w1_s2_1.h"
// #include "bn_music_items.h"
// #include "bn_string_view.h"
// #include "game_stage_getter.h"

// namespace sym::game::stage
// {

// namespace
// {

// using namespace bn::affine_bg_items;

// constexpr bn::size MAP_DIMENSIONS[] = {
//     {
//         bg_w1_s2_0.map_item().dimensions().width() * 8,
//         bg_w1_s2_0.map_item().dimensions().height() * 8,
//     },
//     {
//         bg_w1_s2_1.map_item().dimensions().width() * 8,
//         bg_w1_s2_1.map_item().dimensions().height() * 8,
//     },
// };

// // Zone 0

// constexpr bn::span<ZoneInfo::SymbolInfo> Z0_symbolInfos_ = {

// };

// constexpr bn::span<ZoneInfo::DoorInfo> Z0_doorInfos_ = {

// };

// constexpr ZoneInfo::ShutterInfo Z0_shutterInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z0_hoverButtonInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::ButtonInfo> Z0_pressureButtonInfos_ = {

// };

// constexpr ZoneInfo::SignInfo Z0_signInfos_[] = {

// };

// constexpr ZoneInfo::ExitInfo Z0_exitInfos_[] = {

// };

// constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {

// };

// // Zone 1

// constexpr ZoneInfo::SymbolInfo Z1_symbolInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::DoorInfo> Z1_doorInfos_ = {

// };

// constexpr ZoneInfo::ShutterInfo Z1_shutterInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z1_hoverButtonInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z1_pressureButtonInfos_[] = {

// };

// constexpr ZoneInfo::SignInfo Z1_signInfos_[] = {

// };

// constexpr ZoneInfo::ExitInfo Z1_exitInfos_[] = {

// };

// constexpr ZoneInfo::EntranceInfo Z1_entranceInfos_[] = {

// };

// // Zone 2

// constexpr ZoneInfo::SymbolInfo Z2_symbolInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::DoorInfo> Z2_doorInfos_ = {

// };

// constexpr ZoneInfo::ShutterInfo Z2_shutterInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::ButtonInfo> Z2_hoverButtonInfos_ = {

// };

// constexpr ZoneInfo::ButtonInfo Z2_pressureButtonInfos_[] = {

// };

// constexpr ZoneInfo::SignInfo Z2_signInfos_[] = {

// };

// constexpr ZoneInfo::ExitInfo Z2_exitInfos_[] = {

// };

// constexpr ZoneInfo::EntranceInfo Z2_entranceInfos_[] = {

// };

// // Zone 3

// constexpr ZoneInfo::SymbolInfo Z3_symbolInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::DoorInfo> Z3_doorInfos_ = {

// };

// constexpr ZoneInfo::ShutterInfo Z3_shutterInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::ButtonInfo> Z3_hoverButtonInfos_ = {

// };

// constexpr ZoneInfo::ButtonInfo Z3_pressureButtonInfos_[] = {

// };

// constexpr bn::array<bn::string_view, 3> Z3_signTexts_English[] = {

// };

// constexpr bn::array<bn::string_view, 3> Z3_signTexts_Korean[] = {

// };

// constexpr ZoneInfo::SignInfo Z3_signInfos_[] = {

// };

// constexpr ZoneInfo::ExitInfo Z3_exitInfos_[] = {

// };

// constexpr ZoneInfo::EntranceInfo Z3_entranceInfos_[] = {

// };

// // Zone 4

// constexpr bn::span<ZoneInfo::SymbolInfo> Z4_symbolInfos_ = {

// };

// constexpr bn::span<ZoneInfo::DoorInfo> Z4_doorInfos_ = {

// };

// constexpr ZoneInfo::ShutterInfo Z4_shutterInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z4_hoverButtonInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::ButtonInfo> Z4_pressureButtonInfos_ = {

// };

// constexpr ZoneInfo::SignInfo Z4_signInfos_[] = {

// };

// constexpr ZoneInfo::ExitInfo Z4_exitInfos_[] = {

// };

// constexpr ZoneInfo::EntranceInfo Z4_entranceInfos_[] = {

// };

// // Zone 5

// // constexpr ZoneInfo::SymbolInfo Z5_symbolInfos_[] = {

// // };

// // constexpr bn::span<ZoneInfo::DoorInfo> Z5_doorInfos_ = {

// // };

// // constexpr ZoneInfo::ShutterInfo Z5_shutterInfos_[] = {

// // };

// // constexpr ZoneInfo::ButtonInfo Z5_hoverButtonInfos_[] = {

// // };

// // constexpr ZoneInfo::ButtonInfo Z5_pressureButtonInfos_[] = {

// // };

// // constexpr bn::array<bn::string_view, 3> Z5_signTexts_English[] = {

// // };

// // constexpr bn::array<bn::string_view, 3> Z5_signTexts_Korean[] = {

// // };

// // constexpr ZoneInfo::SignInfo Z5_signInfos_[] = {

// // };

// // constexpr ZoneInfo::ExitInfo Z5_exitInfos_[] = {

// // };

// // constexpr ZoneInfo::EntranceInfo Z5_entranceInfos_[] = {

// // };

// // End of Zone

// constexpr ZoneInfo::BlackHoleInfo blackHole = {
//     {784 - MAP_DIMENSIONS[0].width() / 2, 976 - MAP_DIMENSIONS[0].height() / 2},
// };

// constexpr ZoneInfo zoneInfos_[] = {
//     ZoneInfo(bn::affine_bg_items::bg_w1_s2_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
//              Z0_pressureButtonInfos_,
//              helper::tilemap::IndexRect({0, 2}, bn::size{54, 94}, bg_w1_s2_0.map_item().dimensions()), Z0_signInfos_,
//              Z0_exitInfos_, Z0_entranceInfos_),
//     ZoneInfo(bn::affine_bg_items::bg_w1_s2_0, Z1_symbolInfos_, Z1_doorInfos_, Z1_shutterInfos_, Z1_hoverButtonInfos_,
//              Z1_pressureButtonInfos_,
//              helper::tilemap::IndexRect({0, 108}, bn::size{30, 20}, bg_w1_s2_0.map_item().dimensions()),
//              Z1_signInfos_, Z1_exitInfos_, Z1_entranceInfos_),
//     ZoneInfo(bn::affine_bg_items::bg_w1_s2_1, Z2_symbolInfos_, Z2_doorInfos_, Z2_shutterInfos_, Z2_hoverButtonInfos_,
//              Z2_pressureButtonInfos_,
//              helper::tilemap::IndexRect({0, 2}, bn::size{120, 70}, bg_w1_s2_1.map_item().dimensions()),
//              Z2_signInfos_, Z2_exitInfos_, Z2_entranceInfos_),
//     ZoneInfo(bn::affine_bg_items::bg_w1_s2_1, Z3_symbolInfos_, Z3_doorInfos_, Z3_shutterInfos_, Z3_hoverButtonInfos_,
//              Z3_pressureButtonInfos_,
//              helper::tilemap::IndexRect({0, 75}, bn::size{63, 53}, bg_w1_s2_1.map_item().dimensions()),
//              Z3_signInfos_, Z3_exitInfos_, Z3_entranceInfos_),
//     ZoneInfo(bn::affine_bg_items::bg_w1_s2_0, Z4_symbolInfos_, Z4_doorInfos_, Z4_shutterInfos_, Z4_hoverButtonInfos_,
//              Z4_pressureButtonInfos_,
//              helper::tilemap::IndexRect({85, 0}, bn::size{43, 128}, bg_w1_s2_0.map_item().dimensions()),
//              Z4_signInfos_, Z4_exitInfos_, Z4_entranceInfos_, blackHole),
// };

// constexpr int blackHoleCount_ = [] {
//     int count = 0;
//     for (ZoneInfo zone : zoneInfos_)
//     {
//         if (zone.blackHole)
//             ++count;
//     }
//     return count;
// }();

// static_assert(blackHoleCount_ <= 1, "There are more than one blackHole!");

// constexpr StageInfo w1_s2_(stage::Id::W1_S2, "Stage 2", "Downside Up", "스테이지 2", "아래쪽이 위쪽", false,
//                            bn::music_items::music_autang, zoneInfos_);

// } // namespace

// const StageInfo& Get_W1_S2()
// {
//     return w1_s2_;
// }

// } // namespace sym::game::stage
