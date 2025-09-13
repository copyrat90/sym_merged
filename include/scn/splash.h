// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene.h"

#include <bn_regular_bg_animate_actions.h>

#include <cstdint>

namespace sym::fx
{
class transitions;
}
namespace sym::sys
{
class save_data;
}

namespace sym::scn
{

class splash final : public scene
{
public:
    splash(fx::transitions&, sys::save_data&);

    bool update(scene_stack&) override;

private:
    void update_fade_in();
    void update_waiting();
    void update_fade_out(scene_stack&);

private:
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

    bn::regular_bg_cached_animate_action<2> _bg_anim;

    state _state = state::FADE_IN;
    bool _key_ever_pressed = false;

    std::uint16_t _waiting_current_update = 0;
};

} // namespace sym::scn
