#pragma once

#include <bn_sprite_ptr.h>

#include <bn_fixed.h>
#include <bn_size.h>

namespace sym::helper::sprite
{

inline bn::fixed Left(bn::sprite_ptr sprite)
{
    return sprite.x() - sprite.dimensions().width() / 2;
}
inline bn::fixed Right(bn::sprite_ptr sprite)
{
    return sprite.x() + sprite.dimensions().width() / 2;
}
inline bn::fixed Top(bn::sprite_ptr sprite)
{
    return sprite.y() - sprite.dimensions().height() / 2;
}
inline bn::fixed Bottom(bn::sprite_ptr sprite)
{
    return sprite.y() + sprite.dimensions().height() / 2;
}

} // namespace sym::helper::sprite
