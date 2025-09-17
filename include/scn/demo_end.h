// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene.h"

#include "ibn_observer.h"

#include <bn_sprite_ptr.h>
#include <bn_vector.h>

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

class demo_end final : public scene
{
public:
    demo_end(fx::transitions&, sys::save_data&, ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed,
             sys::text_generators&);

    bool update(scene_stack&) override;

private:
    void update_fade_in();
    void update_ready();
    void update_fade_out(scene_stack&);

    void transit_to_ready();
    void transit_to_fade_out();

private:
    void draw_texts(ldtk::gen::lang_kind);

private:
    enum class state : std::uint8_t
    {
        FADE_IN,
        READY,
        FADE_OUT,
    };

private:
    fx::transitions& _transitions;
    sys::save_data& _save_data;
    ibn::subject<void(ldtk::gen::lang_kind)>& _lang_changed;
    sys::text_generators& _text_generators;

    state _state = state::FADE_IN;

    bool _restart_game = false;

    bn::vector<bn::sprite_ptr, 32> _text_sprites;
};

} // namespace sym::scn
