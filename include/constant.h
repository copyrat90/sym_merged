#pragma once

#include <bn_algorithm.h>
#include <bn_array.h>
#include <bn_color.h>
#include <bn_fixed.h>
#include <bn_sprite_text_generator.h>

#include "bn_music_items.h"
#include "game_entity_Symbol.h"
#include "global.h"

namespace sym::constant
{

inline constexpr bn::color TRANSPARENT_BG_COLOR(0, 0, 0);
inline constexpr sym::global::setting::Lang DEFAULT_LANG = sym::global::setting::Lang::ENG;
inline constexpr auto DEFAULT_TEXT_GEN_ALIGN = bn::sprite_text_generator::alignment_type::CENTER;

// Sprites with higher z orders are drawn first
// (and therefore can be covered by later sprites)
constexpr int PLAYER_Z_ORDER = 0;
constexpr int SYMBOL_Z_ORDER = -10;
constexpr int DOOR_Z_ORDER = 10;
constexpr int BUTTON_Z_ORDER = 5;

namespace volume
{

constexpr bn::fixed MUSIC_DEFAULT = 0.5;
constexpr bn::fixed SFX_DEFAULT = 1;

constexpr bn::fixed music_detour = MUSIC_DEFAULT;
constexpr bn::fixed music_autang = 0.125;
constexpr bn::fixed music_fruity_radioactivity = 0.15;
constexpr bn::fixed music_rene = 0.2;

constexpr bn::fixed sfx_menu_cursor = SFX_DEFAULT;
constexpr bn::fixed sfx_menu_select = SFX_DEFAULT;
constexpr bn::fixed sfx_player_jump = SFX_DEFAULT;
constexpr bn::fixed sfx_symbol_pickup = 0.5;
constexpr bn::fixed sfx_symbol_drop = SFX_DEFAULT;
constexpr bn::fixed sfx_symbol_wall_bump = SFX_DEFAULT;
constexpr bn::fixed sfx_symbol_ground_bump = SFX_DEFAULT;
constexpr bn::fixed sfx_symbol_throw = SFX_DEFAULT;
constexpr bn::fixed sfx_hover_button_toggle = SFX_DEFAULT;
constexpr bn::fixed sfx_pressure_button_toggle = SFX_DEFAULT;
constexpr bn::fixed sfx_player_damage = 0.5;
constexpr bn::fixed sfx_symbol_merge = SFX_DEFAULT;
constexpr bn::fixed sfx_error = SFX_DEFAULT;

constexpr bn::fixed ClampVolume(bn::fixed volume)
{
    return bn::max(bn::min(volume, bn::fixed(1)), bn::fixed(0));
}

constexpr bn::fixed GetVolume(const bn::music_item& music)
{
    constexpr bn::fixed NON_INIT_VAL = -999;
    bn::fixed volume = NON_INIT_VAL;
    if (music == bn::music_items::music_detour)
        return volume::music_detour;
    if (music == bn::music_items::music_autang)
        return volume::music_autang;
    if (music == bn::music_items::music_fruity_radioactivity)
        return volume::music_fruity_radioactivity;
    if (music == bn::music_items::music_rene)
        return volume::music_rene;

    BN_ASSERT(volume == NON_INIT_VAL, "Unknown music item: ", music.id());
    ClampVolume(volume);
    return volume;
}

} // namespace volume

namespace symbol
{

[[nodiscard]] constexpr game::entity::Symbol::Type GetMergedSymbolType(game::entity::Symbol::Type sym1,
                                                                       game::entity::Symbol::Type sym2)
{
    using SymType = game::entity::Symbol::Type;
    constexpr SymType SYMBOL_MERGE_LUT[2][2] = {
        {SymType::PLUS, SymType::UP},
        {SymType::UP, SymType::VV},
    };
    return SYMBOL_MERGE_LUT[sym1][sym2];
}

[[nodiscard]] constexpr bn::pair<game::entity::Symbol::Type, game::entity::Symbol::Type> GetSplitSymbolTypes(
    game::entity::Symbol::Type sym)
{
    using SymType = game::entity::Symbol::Type;
    constexpr bn::pair<SymType, SymType> SYMBOL_SPLIT_LUT[3] = {
        {SymType::BAR, SymType::XOR},
        {SymType::XOR, SymType::XOR},
        {SymType::BAR, SymType::BAR},
    };
    return SYMBOL_SPLIT_LUT[sym - game::entity::Symbol::COMPLEX_SYMBOL_START_NUM];
}

} // namespace symbol

} // namespace sym::constant
