#pragma once

#include "bn_sprite_items_font_m6x11.h"

#include <bn_span.h>
#include <bn_sprite_font.h>
#include <bn_string_view.h>
#include <bn_utf8_characters_map.h>

namespace sym::font
{

inline constexpr bn::utf8_character m6x11_utf8_chars[] = {"★", "언", "어"};

inline constexpr int8_t m6x11_widths[] = {
    6, 4, 7, 8, 8, 8, 8, 4, 6, 6, 7, 8, 4, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 4, 4, 7, 8, 7, 8, 8,
    8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 6, 8, 8, 4, 8,
    8, 8, 8, 8, 8, 8, 8, 4, 6, 8, 4, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 4, 7, 8, 8, 8, 8,
};

inline constexpr bn::span<const bn::utf8_character> m6x11_utf8_chars_span(m6x11_utf8_chars);

inline constexpr auto m6x11_utf8_chars_span_map = bn::utf8_characters_map<m6x11_utf8_chars_span>();

inline constexpr bn::sprite_font m6x11(bn::sprite_items::font_m6x11, m6x11_utf8_chars_span_map.reference(),
                                       m6x11_widths);

} // namespace sym::font
