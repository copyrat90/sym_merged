#include "helper_tilemap.h"

#include <bn_affine_bg_map_cell.h>
#include <bn_affine_bg_map_ptr.h>
#include <bn_assert.h>
#include <bn_display.h>
#include <bn_fixed_rect.h>
#include <bn_optional.h>
#include <bn_span.h>

#include <bn_log.h>

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

TileInfo::TileInfo(const bn::affine_bg_ptr& bg) : bg_(&bg)
{
    Reset(bg);
}

void TileInfo::Reset(const bn::affine_bg_ptr& bg)
{
    bg_ = &bg;
    tileFlags_.clear();
    const bn::span<const bn::affine_bg_map_cell>& cells = bg.map().cells_ref().value();
    tileFlags_.push_back(Flags::EMPTY);
    int group = 0;
    int spikeDirection = 0;
    BN_ASSERT(cells[0] == 0, "First cell is not empty. check transparent color.");
    for (int i = 1; group < TILE_GROUP_COUNT; ++i)
    {
        if (cells[i] == !!Flags::EMPTY)
        {
            ++group;
            continue;
        }
        switch (group)
        {
        case 0:
            tileFlags_.push_back(Flags::CEILING);
            break;
        case 1:
            tileFlags_.push_back(Flags::FLOOR);
            break;
        case 2:
            tileFlags_.push_back(Flags::LEFT_BLOCKING_WALL);
            break;
        case 3:
            tileFlags_.push_back(Flags::RIGHT_BLOCKING_WALL);
            break;
        case 4:
            tileFlags_.push_back(Flags::CEILING | Flags::LEFT_BLOCKING_WALL);
            break;
        case 5:
            tileFlags_.push_back(Flags::CEILING | Flags::RIGHT_BLOCKING_WALL);
            break;
        case 6:
            tileFlags_.push_back(Flags::FLOOR | Flags::LEFT_BLOCKING_WALL);
            break;
        case 7:
            tileFlags_.push_back(Flags::FLOOR | Flags::RIGHT_BLOCKING_WALL);
            break;
        case 8:
            switch (spikeDirection)
            {
            case 0:
                tileFlags_.push_back(Flags::SPIKE | Flags::CEILING);
                break;
            case 1:
                tileFlags_.push_back(Flags::SPIKE | Flags::FLOOR);
                break;
            case 2:
                tileFlags_.push_back(Flags::SPIKE | Flags::LEFT_BLOCKING_WALL);
                break;
            case 3:
                tileFlags_.push_back(Flags::SPIKE | Flags::RIGHT_BLOCKING_WALL);
                break;
            default:
                BN_ERROR("There are more than 4 spikes!");
                break;
            }
            ++spikeDirection;
            break;
        default:
            BN_ERROR("Invalid tile group number");
            break;
        }
    }
}

TileInfo::Flags TileInfo::GetTileFlagsByPosition(bn::fixed_point position) const
{
    const auto& cells = bg_->map().cells_ref().value();
    const int tileId = cells[MapIndex_(position)];
    return At_(tileId);
}

TileInfo::Flags TileInfo::At_(int tileVariationId) const
{
    if (tileVariationId == 0 || tileVariationId >= tileFlags_.size())
        return Flags::EMPTY;
    return tileFlags_[tileVariationId];
}

int TileInfo::MapIndex_(bn::fixed_point point) const
{
    const bn::fixed_point bgPos = bg_->position();
    const int bgWidth = bg_->dimensions().width();
    const int bgHeight = bg_->dimensions().height();

    BN_ASSERT(bgWidth % 256 == 0 && bgHeight % 256 == 0, "width or height of bg is not multiple of 256");

    bn::fixed x = point.x() - (bgPos.x() - bgWidth / 2);
    bn::fixed y = point.y() - (bgPos.y() - bgHeight / 2);
    while (x < 0)
        x += bgWidth;
    while (y < 0)
        y += bgHeight;
    int ix = (x / 8).integer();
    int iy = (y / 8).integer();
    const int nRow = bgHeight / 8;
    const int nCol = bgWidth / 8;
    ix = (ix + nCol) % nCol;
    iy = (iy + nRow) % nRow;
    return ix + iy * nCol;
}

} // namespace sym::helper::tilemap
