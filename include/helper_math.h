#pragma once

#include "bn_log.h"
#include <bn_assert.h>
#include <bn_fixed.h>

namespace sym::helper::math
{

constexpr bn::fixed EPSILON = 0.01;

[[nodiscard]] inline bn::fixed operator%(bn::fixed a, int b)
{
    bn::fixed result = a - (a / b).floor_integer() * b;
    BN_LOG("operator% : ", result);
    BN_ASSERT(-EPSILON <= result && result < b + EPSILON, "operator% is giving wrong result of ", result);
    return result;
}

} // namespace sym::helper::math
