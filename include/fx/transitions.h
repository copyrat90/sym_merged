// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "enum_as_flags.h"

#include <bn_bgs_mosaic_actions.h>
#include <bn_blending_actions.h>
#include <bn_music_actions.h>
#include <bn_optional.h>
#include <bn_sound_actions.h>
#include <bn_sprites_mosaic_actions.h>

#include <cstdint>

namespace sym::fx
{

/// @brief Simple global effects transitions manager.
/// This manages transparency, fade, intensity and mosaic at the same time.
class transitions final
{
public:
    enum class kinds : std::uint8_t
    {
        NONE = 0,

        FADE = (1 << 0),
        TRANSPARENCY = (1 << 1),
        INTENSITY = (1 << 2),
        SPRITES_MOSAIC = (1 << 3),
        BGS_MOSAIC = (1 << 4),
        MUSIC_VOLUME = (1 << 5),
        SOUND_VOLUME = (1 << 6),

        ALL = FADE | TRANSPARENCY | INTENSITY | SPRITES_MOSAIC | BGS_MOSAIC | MUSIC_VOLUME | SOUND_VOLUME
    };

public:
    void update();

public:
    /// @brief Helper function to set the alpha values for many effects once.
    /// @note Keep in mind that the transparency/volume alpha value is inverted.
    /// (`0` being fully visible/audible, `1` being fully transparent/muted.)
    /// @param flags Effect kind(s) to apply the alpha value.
    /// @param alpha Alpha value to apply. `[0..1]`
    void set_alpha(kinds flags, bn::fixed alpha);

    /// @brief Starts a transition from the current alpha value to the final alpha value.
    /// @note Keep in mind that the transparency/volume alpha value is inverted.
    /// (`0` being fully visible/audible, `1` being fully transparent/muted.)
    /// @param flags Effect kind(s) to transit.
    /// @param duration_updates Number of times that the transitions must be updated to complete.
    /// @param final_alpha Final alpha value when the transitions are done. `[0..1]`
    void start(kinds flags, int duration_updates, bn::fixed final_alpha);

    /// @brief Indicates if the transition is completed.
    bool done() const;

    /// @brief Clears the transition.
    /// @note This never resets the alpha value.
    void clear();

private:
    std::uint16_t _current_update = 0;
    std::uint16_t _duration_updates = 0;

    bn::optional<bn::blending_fade_alpha_to_action> _fade_action;
    bn::optional<bn::blending_transparency_alpha_to_action> _transparency_action;
    bn::optional<bn::blending_intensity_alpha_to_action> _intensity_action;
    bn::optional<bn::sprites_mosaic_stretch_to_action> _sprites_mosaic_action;
    bn::optional<bn::bgs_mosaic_stretch_to_action> _bgs_mosaic_action;
    bn::optional<bn::music_volume_to_action> _music_volume_action;
    bn::optional<bn::sound_master_volume_to_action> _sound_volume_action;
};

ENUM_AS_FLAGS(transitions::kinds);

} // namespace sym::fx
