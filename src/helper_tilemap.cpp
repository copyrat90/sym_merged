#include "helper_tilemap.h"

#include <bn_assert.h>
#include <bn_display.h>
#include <bn_fixed_rect.h>

namespace sym::helper::tilemap
{

namespace
{

[[nodiscard]] bn::fixed_rect ConvertIndexRectToPosRect_(IndexRect zoneBoundary, const bn::size bgSize)
{
    const bn::fixed_point centerPos = zoneBoundary.position() * 8 - bn::point(bgSize.width() / 2, bgSize.height() / 2);
    const bn::fixed_size size = zoneBoundary.dimensions() * 8;
    return {centerPos, size};
}

} // namespace

void SnapCameraToZoneBoundary(bn::camera_ptr& cam, IndexRect indexBoundary, const bn::size bgSize)
{
    BN_ASSERT(bgSize.width() % 256 == 0 && bgSize.height() % 256 == 0, "Invalid bgSize");

    bn::fixed_rect fixedBoundary = ConvertIndexRectToPosRect_(indexBoundary, bgSize);

    bn::fixed_rect camRect(cam.position(), {bn::display::width(), bn::display::height()});

    if (camRect.top() < fixedBoundary.top())
    {
        const bn::fixed dy = fixedBoundary.top() - camRect.top();
        cam.set_y(cam.y() + dy);
    }
    else if (camRect.bottom() > fixedBoundary.bottom())
    {
        const bn::fixed dy = camRect.bottom() - fixedBoundary.bottom();
        cam.set_y(cam.y() - dy);
    }
    if (camRect.left() < fixedBoundary.left())
    {
        const bn::fixed dx = fixedBoundary.left() - camRect.left();
        cam.set_x(cam.x() + dx);
    }
    else if (camRect.right() > fixedBoundary.right())
    {
        const bn::fixed dx = camRect.right() - fixedBoundary.right();
        cam.set_x(cam.x() - dx);
    }
}

} // namespace sym::helper::tilemap
