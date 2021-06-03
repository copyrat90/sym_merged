#include "helper_tilemap.h"

#include <bn_assert.h>
#include <bn_display.h>
#include <bn_fixed_rect.h>

namespace sym::helper::tilemap
{

void SnapCameraToZoneBoundary(bn::camera_ptr& cam, const bn::fixed_rect& zoneBoundary)
{
    const bn::fixed_rect camRect(cam.position(), {bn::display::width(), bn::display::height()});

    if (camRect.top() < zoneBoundary.top())
    {
        const bn::fixed dy = zoneBoundary.top() - camRect.top();
        cam.set_y(cam.y() + dy);
    }
    else if (camRect.bottom() > zoneBoundary.bottom())
    {
        const bn::fixed dy = camRect.bottom() - zoneBoundary.bottom();
        cam.set_y(cam.y() - dy);
    }
    if (camRect.left() < zoneBoundary.left())
    {
        const bn::fixed dx = zoneBoundary.left() - camRect.left();
        cam.set_x(cam.x() + dx);
    }
    else if (camRect.right() > zoneBoundary.right())
    {
        const bn::fixed dx = camRect.right() - zoneBoundary.right();
        cam.set_x(cam.x() - dx);
    }
}

} // namespace sym::helper::tilemap
