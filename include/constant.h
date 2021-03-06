#pragma once

#include <bn_algorithm.h>
#include <bn_array.h>
#include <bn_assert.h>
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
inline constexpr int PLAYER_Z_ORDER = 0;
inline constexpr int BLACK_HOLE_Z_ORDER = 50;
inline constexpr int SYMBOL_Z_ORDER = -10;
inline constexpr int DOOR_Z_ORDER = 10;
inline constexpr int BUTTON_Z_ORDER = 5;
inline constexpr int SIGN_Z_ORDER = 7;
inline constexpr int SIGN_TEXT_Z_ORDER = -30;
inline constexpr int MENU_Z_ORDER = -100;
inline constexpr int STAGE_NAME_SPLASH_Z_ORDER = -50;

namespace volume
{

inline constexpr bn::fixed MUSIC_DEFAULT = 0.5;
inline constexpr bn::fixed SFX_DEFAULT = 1;

inline constexpr bn::fixed music_oh_well = 0.25;
inline constexpr bn::fixed music_detour = MUSIC_DEFAULT;
inline constexpr bn::fixed music_autang = 0.125;
inline constexpr bn::fixed music_fruity_radioactivity = 0.15;
inline constexpr bn::fixed music_rene = 0.2;
inline constexpr bn::fixed music_past = 0.2;

inline constexpr bn::fixed sfx_splash_intro = SFX_DEFAULT;
inline constexpr bn::fixed sfx_menu_open = SFX_DEFAULT;
inline constexpr bn::fixed sfx_menu_cursor = SFX_DEFAULT;
inline constexpr bn::fixed sfx_menu_select = SFX_DEFAULT;
inline constexpr bn::fixed sfx_player_jump = SFX_DEFAULT;
inline constexpr bn::fixed sfx_symbol_pickup = 0.5;
inline constexpr bn::fixed sfx_symbol_drop = SFX_DEFAULT;
inline constexpr bn::fixed sfx_symbol_wall_bump = SFX_DEFAULT;
inline constexpr bn::fixed sfx_symbol_ground_bump = SFX_DEFAULT;
inline constexpr bn::fixed sfx_symbol_throw = SFX_DEFAULT;
inline constexpr bn::fixed sfx_hover_button_toggle = SFX_DEFAULT;
inline constexpr bn::fixed sfx_pressure_button_toggle = SFX_DEFAULT;
inline constexpr bn::fixed sfx_player_damage = 0.5;
inline constexpr bn::fixed sfx_symbol_merge = 0.75;
inline constexpr bn::fixed sfx_error = SFX_DEFAULT;
inline constexpr bn::fixed sfx_umbrella_open = SFX_DEFAULT;
inline constexpr bn::fixed sfx_symbol_jump = SFX_DEFAULT;
inline constexpr bn::fixed sfx_gravity_reverse = SFX_DEFAULT;
inline constexpr bn::fixed sfx_enter_black_hole = SFX_DEFAULT;
inline constexpr bn::fixed sfx_stage_clear = SFX_DEFAULT;

[[nodiscard]] inline constexpr bn::fixed ClampVolume(bn::fixed volume)
{
    return bn::max(bn::min(volume, bn::fixed(1)), bn::fixed(0));
}

[[nodiscard]] inline constexpr bn::fixed GetVolume(const bn::music_item& music)
{
    if (music == bn::music_items::music_oh_well)
        return volume::music_oh_well;
    if (music == bn::music_items::music_detour)
        return volume::music_detour;
    if (music == bn::music_items::music_autang)
        return volume::music_autang;
    if (music == bn::music_items::music_fruity_radioactivity)
        return volume::music_fruity_radioactivity;
    if (music == bn::music_items::music_rene)
        return volume::music_rene;
    if (music == bn::music_items::music_past)
        return volume::music_past;

    BN_ERROR("Unknown music item: ", music.id());
    return MUSIC_DEFAULT;
}

} // namespace volume

namespace symbol
{

[[nodiscard]] inline constexpr game::entity::Symbol::Type GetMergedSymbolType(game::entity::Symbol::Type sym1,
                                                                              game::entity::Symbol::Type sym2)
{
    using SymType = game::entity::Symbol::Type;
    constexpr SymType SYMBOL_MERGE_LUT[2][2] = {
        {SymType::PLUS, SymType::UP},
        {SymType::UP, SymType::VV},
    };
    return SYMBOL_MERGE_LUT[sym1][sym2];
}

[[nodiscard]] inline constexpr bn::pair<game::entity::Symbol::Type, game::entity::Symbol::Type> GetSplitSymbolTypes(
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
