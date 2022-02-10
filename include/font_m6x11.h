#pragma once

#include <bn_span.h>
#include <bn_sprite_font.h>
#include <bn_string_view.h>
#include <bn_utf8_characters_map.h>

#include "bn_sprite_items_font_m6x11.h"

namespace sym::font
{

namespace
{

inline constexpr bn::utf8_character variable_8x16_m6x11_utf8_chars[] = {"★", "언", "어"};

inline constexpr int8_t variable_8x16_m6x11_widths[] = {
    6, 4, 7, 8, 8, 8, 8, 4, 6, 6, 7, 8, 4, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 7, 8, 7, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 6, 8, 8, 4, 8,
    8, 8, 8, 8, 8, 8, 8, 4, 6, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 4, 7, 8, 8, 8, 8};

inline constexpr bn::span<const bn::utf8_character> variable_8x16_m6x11_utf8_chars_span(variable_8x16_m6x11_utf8_chars);

inline constexpr auto variable_8x16_m6x11_utf8_chars_span_map =
    bn::utf8_characters_map<variable_8x16_m6x11_utf8_chars_span>();

} // anonymous namespace

inline constexpr bn::sprite_font variable_8x16_m6x11(bn::sprite_items::font_m6x11,
                                                     variable_8x16_m6x11_utf8_chars_span_map.reference(),
                                                     variable_8x16_m6x11_widths);

} // namespace sym::font
