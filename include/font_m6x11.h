#pragma once

#include <bn_span.h>
#include <bn_sprite_font.h>

#include "bn_sprite_items_font_m6x11.h"

namespace sym::font
{

constexpr int8_t variable_8x16_m6x11_widths[] = {6, 3, 7, 8, 8, 8, 8, 3, 6, 6, 7, 8, 3, 8, 3, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                                 8, 8, 3, 3, 7, 8, 7, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8,
                                                 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 6, 8, 6, 8, 8, 3, 8, 8, 8, 8, 8, 8, 8,
                                                 8, 3, 6, 8, 3, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 8, 7, 3, 7, 8};

constexpr bn::sprite_font variable_8x16_m6x11(bn::sprite_items::font_m6x11, bn::span<bn::string_view>(),
                                              variable_8x16_m6x11_widths);

} // namespace sym::font
