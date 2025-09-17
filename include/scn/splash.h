// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene.h"

#include "ibn_observer_fwd.h"

#include <bn_regular_bg_animate_actions.h>

#include <cstdint>

#include "ldtk_gen_enums.h"

namespace sym::fx
{
class transitions;
}
namespace sym::sys
{
class save_data;
class text_generators;
} // namespace sym::sys

namespace sym::scn
{

class splash final : public scene
{
public:
    splash(fx::transitions&, sys::save_data&, ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed,
           sys::text_generators&);

    bool update(scene_stack&) override;

private:
    void update_fade_in();
    void update_waiting();
    void update_fade_out(scene_stack&);

    void transit_to_waiting();
    void transit_to_fade_out();

private:
    enum class state : std::uint8_t
    {
        FADE_IN,
        WAITING,
        FADE_OUT,
    };

private:
    fx::transitions& _transitions;
    sys::save_data& _save_data;
    ibn::subject<void(ldtk::gen::lang_kind)>& _lang_changed;
    sys::text_generators& _text_generators;

    bn::regular_bg_cached_animate_action<2> _bg_anim;

    state _state = state::FADE_IN;
    bool _key_ever_pressed = false;

    std::uint16_t _waiting_current_update = 0;
};

} // namespace sym::scn
