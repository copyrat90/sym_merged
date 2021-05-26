#pragma once

#include <bn_camera_ptr.h>
#include <bn_fixed_point.h>
#include <bn_fixed_rect.h>
#include <bn_size.h>

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
    const bn::fixed_size size(bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y());
    const IndexPoint centerPos = topLeft + IndexPoint(size.width() / 2, size.height() / 2);
    return {centerPos, size};
}

/**
 * @brief Move `cam` back to fit in `zoneBoundary`
 *
 */
void SnapCameraToZoneBoundary(bn::camera_ptr& cam, IndexRect indexBoundary, const bn::size bgSize);

} // namespace sym::helper::tilemap
