// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

// This supports newline character (`\n`)
#include "ibn_sprite_text_generator.h"

#include "ldtk_gen_enums.h"

#include <bn_array.h>

#include <cstdint>

namespace sym::sys
{

class text_generators final
{
public:
    text_generators();

    auto get(ldtk::gen::lang_kind) -> ibn::sprite_text_generator&;

public:
    enum class text_color : std::uint8_t
    {
        WHITE,
        GRAY,
        CYAN,
        YELLOW,

        MAX_COUNT
    };

    static auto get_palette(text_color) -> const bn::sprite_palette_item&;

private:
    bn::array<ibn::sprite_text_generator, (int)ldtk::gen::lang_kind::max_count> _generators;
};

} // namespace sym::sys
