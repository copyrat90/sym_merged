#pragma once

#include <bn_fixed_rect.h>

namespace sym::helper::rect
{

[[nodiscard]] constexpr bn::fixed_rect MakeFixedRectByTopLeftAndSize(bn::fixed_point topLeft, bn::fixed_size size)
{
    const bn::fixed_point centerPos = topLeft + bn::fixed_point(size.width() / 2, size.height() / 2);
    return bn::fixed_rect(centerPos, size);
}

[[nodiscard]] constexpr bn::fixed_rect MakeFixedRectByTopLeftAndBottomRightPosition(bn::fixed_point topLeft,
                                                                                    bn::fixed_point bottomRight)
{
    const bn::fixed_size size(bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
    return MakeFixedRectByTopLeftAndSize(topLeft, size);
}

[[nodiscard]] constexpr bn::fixed_rect operator+(const bn::fixed_rect& rect, const bn::fixed_point& point)
{
    return {rect.position() + point, rect.dimensions()};
}

[[nodiscard]] constexpr bn::fixed_rect operator+(const bn::fixed_point& point, const bn::fixed_rect& rect)
{
    return operator+(rect, point);
}

} // namespace sym::helper::rect
