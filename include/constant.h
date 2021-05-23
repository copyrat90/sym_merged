#pragma once

#include <bn_color.h>
#include <bn_sprite_text_generator.h>

#include "global.h"

namespace sym::constant
{

constexpr bn::color TRANSPARENT_BG_COLOR(4, 4, 6);
constexpr sym::global::setting::Lang DEFAULT_LANG = sym::global::setting::Lang::ENG;
constexpr auto DEFAULT_TEXT_GEN_ALIGN = bn::sprite_text_generator::alignment_type::CENTER;

} // namespace sym::constant
