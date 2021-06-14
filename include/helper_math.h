#pragma once

#include "bn_log.h"
#include <bn_assert.h>
#include <bn_fixed.h>

namespace sym::helper::math
{

[[nodiscard]] inline bn::fixed operator%(bn::fixed a, int b)
{
    bn::fixed result = a - (a / b).floor_integer() * b;
    BN_LOG("operator% : ", result);
    BN_ASSERT(0 <= result && result < b, "operator% is giving wrong result");
    return result;
}

} // namespace sym::helper::math
