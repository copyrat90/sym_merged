#pragma once

#include <bn_camera_ptr.h>
#include <bn_fixed_point.h>
#include <bn_fixed_rect.h>
#include <bn_size.h>

#include "helper_rect.h"

namespace sym::helper::tilemap
{

/**
 * @brief bg tile(8x8) index rect
 *
 */
using IndexRect = bn::fixed_rect;
using IndexPoint = bn::fixed_point;

[[nodiscard]] constexpr IndexRect MakeIndexRectByTopLeftAndBottomRight(IndexPoint topLeft, IndexPoint bottomRight)
{
    return rect::MakeFixedRectByTopLeftAndBottomRightPosition(topLeft, bottomRight);
}

/**
 * @brief Move `cam` back to fit in `zoneBoundary`
 *
 */
void SnapCameraToZoneBoundary(bn::camera_ptr& cam, IndexRect indexBoundary, const bn::size bgSize);

} // namespace sym::helper::tilemap
