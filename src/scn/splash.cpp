// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/splash.h"

#include "fx/transitions.h"
#include "scn/scene_stack.h"
#include "scn/title.h"

#include "bn_dmg_music_items_dmg_music_signature_sound.h"
#include "bn_regular_bg_items_bg_splash.h"

#include <bn_assert.h>
#include <bn_dmg_music.h>
#include <bn_keypad.h>
#include <bn_music.h>
#include <bn_regular_bg_builder.h>

#include <cstdint>

namespace sym::scn
{

namespace
{

constexpr int FADE_IN_DURATION_UPDATES = 60;
constexpr int WAITING_DURATION_UPDATES = 240;
constexpr int FADE_OUT_DURATION_UPDATES = 60;

auto create_bg_anim() -> bn::regular_bg_cached_animate_action<2>
{
    static constexpr const auto& bg_splash = bn::regular_bg_items::bg_splash;
    static constexpr std::uint16_t MAP_IDXES[2] = {0, 1};

    bn::regular_bg_builder builder(bg_splash);
    builder.set_blending_top_enabled(true);

    return bn::regular_bg_cached_animate_action<2>::forever(builder.build(), 29, bg_splash.map_item(), MAP_IDXES);
}

} // namespace

splash::splash(fx::transitions& transitions, sys::save_data& save_data,
               ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed, sys::text_generators& text_generators)
    : _transitions(transitions), _save_data(save_data), _lang_changed(lang_changed), _text_generators(text_generators),
      _bg_anim(create_bg_anim())
{
    bn::music::stop();
    bn::dmg_music::stop();

    bn::dmg_music::set_master_volume(bn::dmg_music_master_volume::HALF);
    bn::dmg_music_items::dmg_music_signature_sound.play(1, false);

    _transitions.set_alpha(fx::transitions::kinds::TRANSPARENCY, 1);
    _transitions.start(fx::transitions::kinds::TRANSPARENCY, FADE_IN_DURATION_UPDATES, 0);
}

bool splash::update(scene_stack& scene_stack_)
{
    if (!_bg_anim.done())
        _bg_anim.update();

    _key_ever_pressed |= bn::keypad::any_pressed();

    switch (_state)
    {
    case state::FADE_IN:
        update_fade_in();
        break;
    case state::WAITING:
        update_waiting();
        break;
    case state::FADE_OUT:
        update_fade_out(scene_stack_);
        break;
    default:
        BN_ERROR("Invalid state: ", (int)_state);
    }

    return false;
}

void splash::update_fade_in()
{
    if (_transitions.done())
    {
        if (_key_ever_pressed)
            transit_to_fade_out();
        else
            transit_to_waiting();
    }
}

void splash::update_waiting()
{
    if (_key_ever_pressed || ++_waiting_current_update == WAITING_DURATION_UPDATES)
        transit_to_fade_out();
}

void splash::update_fade_out(scene_stack& scene_stack_)
{
    if (_transitions.done())
    {
        scene_stack_.reserve_pop();
        scene_stack_.reserve_push<title>(_transitions, _save_data, _lang_changed, _text_generators);
    }
}

void splash::transit_to_waiting()
{
    _state = state::WAITING;
}

void splash::transit_to_fade_out()
{
    _transitions.start(fx::transitions::kinds::TRANSPARENCY, FADE_OUT_DURATION_UPDATES, 1);

    _state = state::FADE_OUT;
}

} // namespace sym::scn
