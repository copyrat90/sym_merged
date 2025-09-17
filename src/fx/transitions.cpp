// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "fx/transitions.h"

#include <bn_assert.h>

namespace sym::fx
{

void transitions::update()
{
    if (!done())
    {
        if (_fade_action.has_value())
            _fade_action->update();
        else
        {
            if (_transparency_action.has_value())
                _transparency_action->update();
            if (_intensity_action.has_value())
                _intensity_action->update();
        }
        if (_sprites_mosaic_action.has_value())
            _sprites_mosaic_action->update();
        if (_bgs_mosaic_action.has_value())
            _bgs_mosaic_action->update();
        if (_music_volume_action.has_value())
            _music_volume_action->update();
        if (_sound_volume_action.has_value())
            _sound_volume_action->update();

        ++_current_update;

        if (done())
            clear();
    }
}

void transitions::set_alpha(kinds flags, bn::fixed alpha)
{
    if (!!(flags & kinds::FADE))
        bn::blending::set_fade_alpha(alpha);
    if (!!(flags & kinds::TRANSPARENCY))
        bn::blending::set_transparency_alpha(1 - alpha); // inverted
    if (!!(flags & kinds::INTENSITY))
        bn::blending::set_intensity_alpha(alpha);
    if (!!(flags & kinds::SPRITES_MOSAIC))
        bn::sprites_mosaic::set_stretch(alpha);
    if (!!(flags & kinds::BGS_MOSAIC))
        bn::bgs_mosaic::set_stretch(alpha);
    if (!!(flags & kinds::MUSIC_VOLUME))
        bn::music::set_volume(1 - alpha); // inverted
    if (!!(flags & kinds::SOUND_VOLUME))
        bn::sound::set_master_volume(1 - alpha); // inverted
}

void transitions::start(kinds flags, int duration_updates, bn::fixed final_alpha)
{
    BN_ASSERT(!(!!(flags & kinds::FADE) && (!!(flags & kinds::TRANSPARENCY) || !!(kinds::INTENSITY))),
              "Fade and other blendings can't be enabled at the same time");

    clear();

    if (!!(flags & kinds::FADE))
        _fade_action.emplace(duration_updates, final_alpha);
    else
    {
        if (!!(flags & kinds::TRANSPARENCY))
            _transparency_action.emplace(duration_updates, 1 - final_alpha); // inverted
        if (!!(flags & kinds::INTENSITY))
            _intensity_action.emplace(duration_updates, final_alpha);
    }
    if (!!(flags & kinds::SPRITES_MOSAIC))
        _sprites_mosaic_action.emplace(duration_updates, final_alpha);
    if (!!(flags & kinds::BGS_MOSAIC))
        _bgs_mosaic_action.emplace(duration_updates, final_alpha);
    if (!!(flags & kinds::MUSIC_VOLUME))
        _music_volume_action.emplace(duration_updates, 1 - final_alpha); // inverted
    if (!!(flags & kinds::SOUND_VOLUME))
        _sound_volume_action.emplace(duration_updates, 1 - final_alpha); // inverted

    _current_update = 0;
    _duration_updates = duration_updates;
}

bool transitions::done() const
{
    return _current_update == _duration_updates;
}

void transitions::clear()
{
    _current_update = 0;
    _duration_updates = 0;

    _fade_action.reset();
    _transparency_action.reset();
    _intensity_action.reset();
    _sprites_mosaic_action.reset();
    _bgs_mosaic_action.reset();
    _music_volume_action.reset();
    _sound_volume_action.reset();
}

} // namespace sym::fx
