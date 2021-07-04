#include <bn_affine_bg_item.h>
#include <bn_array.h>
#include <bn_fixed_rect.h>

#include "bn_affine_bg_items_bg_w1_s0_0.h"
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
        bg_w1_s0_0.map_item().dimensions().width() * 8,
        bg_w1_s0_0.map_item().dimensions().height() * 8,
    },
};

constexpr bn::span<ZoneInfo::SymbolInfo> Z0_symbolInfos_ = {

};

constexpr bn::span<ZoneInfo::DoorInfo> Z0_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z0_shutterInfos_[] = {
    {{281 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
};

constexpr ZoneInfo::ButtonInfo Z0_hoverButtonInfos_[] = {
    {{247 - MAP_DIMENSIONS[0].width() / 2, 109 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
};

constexpr bn::span<ZoneInfo::ButtonInfo> Z0_pressureButtonInfos_ = {

};

constexpr bn::array<bn::string_view, 2> Z0_signTexts_English[] = {
    {"Arrow: Move"},
    {"A: Jump"},
    {"L/R: Use Left/Right hand"},
    {"L/R: Toggle a switch"},
};

constexpr bn::array<bn::string_view, 2> Z0_signTexts_Korean[] = {
    {"방향키:이동"},
    {"A:점프"},
    {"L/R:왼/오른손 사용"},
    {"L/R:스위치 누르기"},
};

constexpr ZoneInfo::SignInfo Z0_signInfos_[] = {
    {{50 - MAP_DIMENSIONS[0].width() / 2, 133 - MAP_DIMENSIONS[0].height() / 2},
     Z0_signTexts_English[0],
     Z0_signTexts_Korean[0]},
    {{105 - MAP_DIMENSIONS[0].width() / 2, 133 - MAP_DIMENSIONS[0].height() / 2},
     Z0_signTexts_English[1],
     Z0_signTexts_Korean[1]},
    {{165 - MAP_DIMENSIONS[0].width() / 2, 113 - MAP_DIMENSIONS[0].height() / 2},
     Z0_signTexts_English[2],
     Z0_signTexts_Korean[2]},
    {{218 - MAP_DIMENSIONS[0].width() / 2, 113 - MAP_DIMENSIONS[0].height() / 2},
     Z0_signTexts_English[3],
     Z0_signTexts_Korean[3]},
};

constexpr ZoneInfo::ExitInfo Z0_exitInfos_[] = {
    {{303 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 0},
};

constexpr ZoneInfo::EntranceInfo Z0_entranceInfos_[] = {
    {{43 - MAP_DIMENSIONS[0].width() / 2, 102 - MAP_DIMENSIONS[0].height() / 2}},
    {{257 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::SymbolInfo Z1_symbolInfos_[] = {
    {{409 - MAP_DIMENSIONS[0].width() / 2, 113 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
};

constexpr bn::span<ZoneInfo::DoorInfo> Z1_doorInfos_ = {

};

constexpr ZoneInfo::ShutterInfo Z1_shutterInfos_[] = {
    {{531 - MAP_DIMENSIONS[0].width() / 2, 56 - MAP_DIMENSIONS[0].height() / 2}, false, 0},
    {{541 - MAP_DIMENSIONS[0].width() / 2, 56 - MAP_DIMENSIONS[0].height() / 2}, false, 1},
};

constexpr ZoneInfo::ButtonInfo Z1_hoverButtonInfos_[] = {
    {{358 - MAP_DIMENSIONS[0].width() / 2, 56 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
};

constexpr ZoneInfo::ButtonInfo Z1_pressureButtonInfos_[] = {
    {{490 - MAP_DIMENSIONS[0].width() / 2, 64 - MAP_DIMENSIONS[0].height() / 2}, false, 1, false, bn::nullopt},
};

constexpr bn::array<bn::string_view, 3> Z1_signTexts_English[] = {
    {"L/R: Pick up a symbol"},
    {"Switches open the doors", "with the same number."},
    {"You can't use the hand", "holding a symbol.", "Try using the other hand."},
    {"Down+L/R: Put the symbol down"},
};

constexpr bn::array<bn::string_view, 3> Z1_signTexts_Korean[] = {
    {"L/R:문양 집기"},
    {"스위치의 번호와 같은", "번호의 문이 열린다."},
    {"문양을 든 손으론", "스위치를 누르지 못한다.", "반대쪽 손을 사용하자."},
    {"아래+L/R:내려놓기"},
};

constexpr ZoneInfo::SignInfo Z1_signInfos_[] = {
    {{384 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2},
     Z1_signTexts_English[0],
     Z1_signTexts_Korean[0]},
    {{444 - MAP_DIMENSIONS[0].width() / 2, 80 - MAP_DIMENSIONS[0].height() / 2},
     Z1_signTexts_English[1],
     Z1_signTexts_Korean[1]},
    {{379 - MAP_DIMENSIONS[0].width() / 2, 56 - MAP_DIMENSIONS[0].height() / 2},
     Z1_signTexts_English[2],
     Z1_signTexts_Korean[2]},
    {{470 - MAP_DIMENSIONS[0].width() / 2, 56 - MAP_DIMENSIONS[0].height() / 2},
     Z1_signTexts_English[3],
     Z1_signTexts_Korean[3]},
};

constexpr ZoneInfo::ExitInfo Z1_exitInfos_[] = {
    {{303 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 0, 1},
    {{559 - MAP_DIMENSIONS[0].width() / 2, 56 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 2, 0},
};

constexpr ZoneInfo::EntranceInfo Z1_entranceInfos_[] = {
    {{332 - MAP_DIMENSIONS[0].width() / 2, 112 - MAP_DIMENSIONS[0].height() / 2}},
    {{513 - MAP_DIMENSIONS[0].width() / 2, 56 - MAP_DIMENSIONS[0].height() / 2}},
};

constexpr ZoneInfo::SymbolInfo Z2_symbolInfos_[] = {
    {{186 - MAP_DIMENSIONS[0].width() / 2, 356 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::BAR, false},
    {{209 - MAP_DIMENSIONS[0].width() / 2, 356 - MAP_DIMENSIONS[0].height() / 2}, entity::Symbol::Type::XOR, false},
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

constexpr ZoneInfo::ButtonInfo Z2_pressureButtonInfos_[] = {
    {{233 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2}, false, 0, false, bn::nullopt},
    {{278 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2}, false, 1, false, bn::nullopt},
};

constexpr bn::array<bn::string_view, 3> Z2_signTexts_English[] = {
    {"Hold L/R: Use merged symbol's", "ability (Fall down slowly)"},
    {"Hold B: Merge 2 symbols in hands"},
    {"Hold B: Split the symbol"},
    {"Start: Open the menu", "If you mess up, you can always", "open the menu and restart."},
};

constexpr bn::array<bn::string_view, 3> Z2_signTexts_Korean[] = {
    {"L/R꾹눌러 합친 문양의", "능력 사용(낙하 속력 감소)"},
    {"B꾹눌러 양손 문양 합치기"},
    {"B꾹눌러 문양 분리하기"},
    {"스타트:메뉴 열기", "망친 경우,메뉴를 열어", "재시작 할 수 있다."},
};

constexpr ZoneInfo::SignInfo Z2_signInfos_[] = {
    {{96 - MAP_DIMENSIONS[0].width() / 2, 311 - MAP_DIMENSIONS[0].height() / 2},
     Z2_signTexts_English[0],
     Z2_signTexts_Korean[0]},
    {{146 - MAP_DIMENSIONS[0].width() / 2, 352 - MAP_DIMENSIONS[0].height() / 2},
     Z2_signTexts_English[1],
     Z2_signTexts_Korean[1]},
    {{256 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2},
     Z2_signTexts_English[2],
     Z2_signTexts_Korean[2]},
    {{68 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2},
     Z2_signTexts_English[3],
     Z2_signTexts_Korean[3]},
};

constexpr ZoneInfo::ExitInfo Z2_exitInfos_[] = {
    {{-1 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 1, 1},
    {{343 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2, 2, 32}, 3, 0},
};

constexpr ZoneInfo::EntranceInfo Z2_entranceInfos_[] = {
    {{33 - MAP_DIMENSIONS[0].width() / 2, 272 - MAP_DIMENSIONS[0].height() / 2}},
    {{292 - MAP_DIMENSIONS[0].width() / 2, 264 - MAP_DIMENSIONS[0].height() / 2}},
};

// constexpr ZoneInfo::SymbolInfo Z3_symbolInfos_[] = {

// };

// constexpr bn::span<ZoneInfo::DoorInfo> Z3_doorInfos_ = {

// };

// constexpr ZoneInfo::ShutterInfo Z3_shutterInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z3_hoverButtonInfos_[] = {

// };

// constexpr ZoneInfo::ButtonInfo Z3_pressureButtonInfos_[] = {

// };

// constexpr ZoneInfo::SignInfo Z3_signInfos_[] = {

// };

// constexpr ZoneInfo::ExitInfo Z3_exitInfos_[] = {

// };

// constexpr ZoneInfo::EntranceInfo Z3_entranceInfos_[] = {

// };

constexpr ZoneInfo zoneInfos_[] = {
    ZoneInfo(bn::affine_bg_items::bg_w1_s0_0, Z0_symbolInfos_, Z0_doorInfos_, Z0_shutterInfos_, Z0_hoverButtonInfos_,
             Z0_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 2}, bn::size{38, 20}, bg_w1_s0_0.map_item().dimensions()), Z0_signInfos_,
             Z0_exitInfos_, Z0_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s0_0, Z1_symbolInfos_, Z1_doorInfos_, Z1_shutterInfos_, Z1_hoverButtonInfos_,
             Z1_pressureButtonInfos_,
             helper::tilemap::IndexRect({38, 2}, bn::size{32, 20}, bg_w1_s0_0.map_item().dimensions()), Z1_signInfos_,
             Z1_exitInfos_, Z1_entranceInfos_),
    ZoneInfo(bn::affine_bg_items::bg_w1_s0_0, Z2_symbolInfos_, Z2_doorInfos_, Z2_shutterInfos_, Z2_hoverButtonInfos_,
             Z2_pressureButtonInfos_,
             helper::tilemap::IndexRect({0, 23}, bn::size{43, 25}, bg_w1_s0_0.map_item().dimensions()), Z2_signInfos_,
             Z2_exitInfos_, Z2_entranceInfos_),
};

constexpr StageInfo w1_s0_("Tutorial", "Welcome to sym_merged!", "튜토리얼", "환영하오,낯선이여.", false,
                           bn::music_items::music_detour, zoneInfos_);

} // namespace

const StageInfo& Get_W1_S0()
{
    return w1_s0_;
}

} // namespace sym::game::stage
