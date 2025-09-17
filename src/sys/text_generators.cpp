// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "sys/text_generators.h"

#include "font/hamchorom.h"
#include "font/m6x11.h"

#include <bn_assert.h>

#include "bn_sprite_palette_items_pal_text_cyan.h"
#include "bn_sprite_palette_items_pal_text_gray.h"

namespace sym::sys
{

namespace
{

constexpr const bn::sprite_palette_item* PALETTES_LUT[(int)text_generators::text_color::MAX_COUNT] = {
    &bn::sprite_items::font_m6x11.palette_item(),
    &bn::sprite_palette_items::pal_text_gray,
    &bn::sprite_palette_items::pal_text_cyan,
};

} // namespace

text_generators::text_generators()
    : _generators{
          bn::sprite_text_generator(font::m6x11),
          bn::sprite_text_generator(font::hamchorom),
      }
{
    for (auto& gen : _generators)
    {
        // Center alignment by default
        gen.set_center_alignment();

        // Enable blending & mosaic by default
        gen.set_blending_enabled(true);
        gen.set_mosaic_enabled(true);
    }
}

auto text_generators::get(ldtk::gen::lang_kind lang) -> bn::sprite_text_generator&
{
    BN_ASSERT(lang < ldtk::gen::lang_kind::max_count, "Invalid lang: ", (int)lang);

    return _generators[(int)lang];
}

auto text_generators::get_palette(text_color color) -> const bn::sprite_palette_item&
{
    BN_ASSERT(color < text_generators::text_color::MAX_COUNT, "Invalid color: ", (int)color);

    return *PALETTES_LUT[(int)color];
}

} // namespace sym::sys
