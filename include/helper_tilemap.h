#pragma once

#include <bn_affine_bg_ptr.h>
#include <bn_assert.h>
#include <bn_camera_ptr.h>
#include <bn_fixed_rect.h>
#include <bn_point.h>
#include <bn_size.h>
#include <bn_vector.h>

#include "helper_rect.h"

namespace sym::helper::tilemap
{

/**
 * @brief bg tile(8x8) index rect.

 *
 */
// using IndexRect = bn::fixed_rect;

/**
 * @brief TopLeft-Anchored bg tile(8x8) index rect.
 *
 * Anchor position {0,0} is bg center.
 * (e.g. IndexRect can be topLeft:{-15,-21}, bottomRight:{15,0},
 * then the center of the IndexRect will be {0,-10.5})
 */
struct IndexRect
{
    int x, y, w, h;

    /**
     * @brief Constructor.
     * Automatically moves anchor to match the center to the bg.
     * (i.e. Converts {0,0} to {-bgTileCountDimentions.width()/2,-bgTileCountDimentions.height()/2})
     *
     * @param bgTileCountSize bg_item.map_item().dimensions()
     */
    constexpr IndexRect(int x_a, int y_a, int w_a, int h_a, bn::size bgTileCountSize)
        : x(x_a - bgTileCountSize.width() / 2), y(y_a - bgTileCountSize.height() / 2), w(w_a), h(h_a)
    {
        BN_ASSERT(w_a >= 30, "Invalid IndexRect width: ", w);
        BN_ASSERT(h_a >= 20, "Invalid IndexRect height: ", h);
    }

    /**
     * @brief Constructor.
     * Automatically moves anchor to match the center to the bg.
     * (i.e. Converts {0,0} to {-bgTileCountDimentions.width()/2,-bgTileCountDimentions.height()/2})
     *
     * @param bgTileCountSize bg_item.map_item().dimensions()
     */
    constexpr IndexRect(bn::point topLeft, bn::size size, bn::size bgTileCountSize)
        : IndexRect(topLeft.x(), topLeft.y(), size.width(), size.height(), bgTileCountSize)
    {
    }

    /**
     * @brief Constructor.
     * Automatically moves anchor to match the center to the bg.
     * (i.e. Converts {0,0} to {-bgTileCountDimentions.width()/2,-bgTileCountDimentions.height()/2})
     *
     * @param bgTileCountSize bg_item.map_item().dimensions()
     */
    constexpr IndexRect(bn::point topLeft, bn::point bottomRight, bn::size bgTileCountSize)
        : IndexRect(topLeft, bn::size{bottomRight.x() - topLeft.x(), bottomRight.y() - topLeft.y()}, bgTileCountSize)
    {
    }

    constexpr bn::point TopLeftPosition() const
    {
        return {x, y};
    }

    /**
     * @brief Returns center position.
     *
     * type `bn::fixed_point` is used.
     * Because center position can hold *.5 element.
     * (e.g. IndexRect can be topLeft:{-15,-21}, bottomRight:{15,0},
     * then the center of the IndexRect will be {0,-10.5})
     *
     * @return `constexpr bn::fixed_point` center position
     */
    constexpr bn::fixed_point CenterPosition() const
    {
        return {x + bn::fixed(w) / 2, y + bn::fixed(h) / 2};
    }

    constexpr bn::size Dimensions() const
    {
        return {w, h};
    }
};

[[nodiscard]] constexpr bn::fixed_rect ConvertIndexRectToPositionRect(const IndexRect& zoneBoundary)
{
    const bn::fixed_point centerPos = zoneBoundary.CenterPosition() * 8;
    const bn::fixed_size size = zoneBoundary.Dimensions() * 8;
    return {centerPos, size};
}

/**
 * @brief Snap camera to zone boundary.
 * This function receives absolute position `zoneBoundary`,
 * so, `ConvertIndexRectToPositionRect()` must be called before.
 *
 * @param cam
 * @param zoneBoundary zoneBoundary in absolute position
 */
void SnapCameraToZoneBoundary(bn::camera_ptr& cam, const bn::fixed_rect& zoneBoundary);

class TileInfo
{
public:
    TileInfo(const bn::affine_bg_ptr& bg);
    void Reset(const bn::affine_bg_ptr& bg);

    TileInfo(const TileInfo&) = delete;
    TileInfo& operator=(const TileInfo&) = delete;

    TileInfo(TileInfo&& other) : bg_(other.bg_), tileFlags_(bn::move(other.tileFlags_))
    {
    }
    TileInfo& operator=(TileInfo&& other)
    {
        bg_ = other.bg_;
        tileFlags_ = bn::move(other.tileFlags_);
        return *this;
    }

    enum class Flags
    {
        EMPTY = 0,
        CEILING = 0x01,
        FLOOR = 0x02,
        LEFT_BLOCKING_WALL = 0x04,
        RIGHT_BLOCKING_WALL = 0x08,
        SPIKE = 0x10,
    };

    Flags GetTileFlagsByPosition(bn::fixed_point position) const;

private:
    static constexpr int TILE_VARIATION_COUNT = 32;
    static constexpr int TILE_GROUP_COUNT = 9;

    const bn::affine_bg_ptr* bg_;
    bn::vector<Flags, TILE_VARIATION_COUNT> tileFlags_;

    Flags At_(int tileVariationId) const;

    /**
     * @brief Get map index which can be used to index bg.map().cells_ref().value()
     *
     */
    int MapIndex_(bn::fixed_point point) const;
};

inline TileInfo::Flags operator|(TileInfo::Flags f1, TileInfo::Flags f2)
{
    return static_cast<TileInfo::Flags>(static_cast<int>(f1) | static_cast<int>(f2));
}

inline TileInfo::Flags operator&(TileInfo::Flags f1, TileInfo::Flags f2)
{
    return static_cast<TileInfo::Flags>(static_cast<int>(f1) & static_cast<int>(f2));
}

inline bool operator!(TileInfo::Flags flags)
{
    return flags == TileInfo::Flags::EMPTY;
}

} // namespace sym::helper::tilemap
