#pragma once

#include <bn_affine_bg_item.h>
#include <bn_assert.h>
#include <bn_display.h>
#include <bn_fixed_rect.h>
#include <bn_optional.h>
#include <bn_span.h>
#include <bn_string_view.h>

#include "helper_tilemap.h"

#include "game_entity_Symbol.h"

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
        bn::fixed_point position;
        entity::Symbol::Type symbolType;
        bool isGravityReversedByDefault;
    };

    struct DoorInfo
    {
        bn::fixed_point position;
        bool isOpenedByDefault;
        int destinationZoneIndex;
        int entranceOfZoneIndex;
        int textSpriteNumber;
    };

    struct ShutterInfo
    {
        bn::fixed_point position;
        bool isOpenedByDefault;
        int textSpriteNumber;
    };

    struct ButtonInfo
    {
        bn::fixed_point position;
        bool isOnByDefault;
        int textSpriteNumber;
        bool isVerticallyReversed;
        bn::optional<entity::Symbol::Type> requiredSymbolType;
    };

    struct SignInfo
    {
        bn::fixed_point position;
        bn::span<const bn::string_view> engMessage;
        bn::span<const bn::string_view> korMessage;
    };

    struct ExitInfo
    {
        bn::fixed_rect collider;
        int destinationZoneIndex;
        int entranceOfZoneIndex;
    };

    struct EntranceInfo
    {
        bn::fixed_point position;
    };

    /**
     * @brief Constructor.
     *
     * @param zoneBoundary_a anchor is center of bg
     */
    constexpr ZoneInfo(const bn::affine_bg_item& mapBg_a, bn::span<const SymbolInfo> symbols_a,
                       bn::span<const DoorInfo> doors_a, bn::span<const ShutterInfo> shutters_a,
                       bn::span<const ButtonInfo> hoverButtons_a, bn::span<const ButtonInfo> pressureButtons_a,
                       const helper::tilemap::IndexRect zoneBoundary_a, const bn::span<const SignInfo> signs_a,
                       bn::span<const ExitInfo> exits_a, bn::span<const EntranceInfo> entrances_a)
        : mapBg(mapBg_a), symbols(symbols_a), doors(doors_a), shutters(shutters_a), hoverButtons(hoverButtons_a),
          pressureButtons(pressureButtons_a), zoneBoundary(zoneBoundary_a), signs(signs_a), exits(exits_a),
          entrances(entrances_a)
    {
    }

    const bn::affine_bg_item& mapBg;

    // Mutable things. Only for initialize.
    const bn::span<const SymbolInfo> symbols;
    const bn::span<const DoorInfo> doors;
    const bn::span<const ShutterInfo> shutters;
    const bn::span<const ButtonInfo> hoverButtons;
    const bn::span<const ButtonInfo> pressureButtons;

    // Constant things. Constantly referenced.
    const helper::tilemap::IndexRect zoneBoundary;
    const bn::span<const SignInfo> signs;
    const bn::span<const ExitInfo> exits;
    const bn::span<const EntranceInfo> entrances;
};

} // namespace sym::game::stage
