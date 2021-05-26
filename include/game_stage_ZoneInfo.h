#pragma once

#include <bn_affine_bg_item.h>
#include <bn_assert.h>
#include <bn_display.h>
#include <bn_fixed_point.h>
#include <bn_fixed_rect.h>
#include <bn_span.h>

#include "helper_tilemap.h"

namespace sym::game::stage
{

/**
 * @brief Stores zone entity informations.
 * for generating constexpr info.
 *
 */
struct ZoneInfo
{
    struct SymbolInfo
    {
        /**
         * @brief Symbol types
         *
         * If value >= 100, it is a complex symbol.
         * otherwise, it is a basic symbol.
         */
        enum Type
        {
            BAR,
            XOR,
            UP = 100,
            VV,
            PLUS
        };

        bn::fixed_point position;
        Type symbolType;
    };

    struct ButtonInfo
    {
        bn::fixed_point position;
        bn::span<int> openWhenOnShutterIndexes;
        bn::span<int> openWhenOffShutterIndexes;
    };

    struct DoorInfo
    {
        bn::fixed_point position;
        int destinationZoneIndex;
        int entranceOfZoneIndex;
    };

    struct ExitInfo
    {
        bn::fixed_rect collider;
        int destinationZoneIndex;
        int entranceOfZoneIndex;
    };

    /**
     * @brief Constructor.
     *
     * @param zoneBoundary_a anchor is center of bg
     */
    constexpr ZoneInfo(const bn::affine_bg_item& mapBg_a, helper::tilemap::IndexRect zoneBoundary_a,
                       bn::span<SymbolInfo> symbols_a, bn::span<ButtonInfo> hoverButtons_a,
                       bn::span<ButtonInfo> pressureButtons_a, bn::span<DoorInfo> doors_a, bn::span<ExitInfo> exits_a,
                       bn::span<bn::fixed_point> entrances_a)
        : mapBg(mapBg_a), symbols(symbols_a), hoverButtons(hoverButtons_a), pressureButtons(pressureButtons_a),
          zoneBoundary(zoneBoundary_a), doors(doors_a), exits(exits_a), entrances(entrances_a)
    {
    }

    const bn::affine_bg_item& mapBg;

    // Mutable things. Only for initialize.
    const bn::span<SymbolInfo> symbols;
    const bn::span<ButtonInfo> hoverButtons;
    const bn::span<ButtonInfo> pressureButtons;

    // Constant things. Constantly referenced.
    const bn::fixed_rect zoneBoundary;
    const bn::span<DoorInfo> doors;
    const bn::span<ExitInfo> exits;
    const bn::span<bn::fixed_point> entrances;
};

} // namespace sym::game::stage
