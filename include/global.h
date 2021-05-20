#pragma once

#include <bn_sprite_text_generator.h>

namespace sym::global
{

struct TextGen
{
    bn::sprite_text_generator* hangeul;
    bn::sprite_text_generator* latin;

    TextGen(bn::sprite_text_generator* hg, bn::sprite_text_generator* lt) : hangeul(hg), latin(lt)
    {
    }
};

extern TextGen* textGenPtr_g;

} // namespace sym::global
