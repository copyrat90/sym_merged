#pragma once

#include <bn_assert.h>
#include <bn_fixed.h>
#include <bn_fixed_point.h>

namespace sym::helper::math
{

[[nodiscard]] inline constexpr bn::fixed DistanceSquared(bn::fixed_point p1, bn::fixed_point p2)
{
    return (p2.x() - p1.x()) * (p2.x() - p1.x()) + (p2.y() - p1.y()) * (p2.y() - p1.y());
}

[[nodiscard]] inline constexpr bn::fixed_point operator*(int num, bn::fixed_point vector)
{
    return {num * vector.x(), num * vector.y()};
}

[[nodiscard]] inline constexpr bn::fixed_point operator*(bn::fixed num, bn::fixed_point vector)
{
    return {num * vector.x(), num * vector.y()};
}

} // namespace sym::helper::math
