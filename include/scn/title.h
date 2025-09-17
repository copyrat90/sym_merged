// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene.h"

#include "ibn_observer.h"

#include "ldtk_gen_enums.h"

#include <bn_array.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>
#include <bn_vector.h>

#include <cstdint>

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

class title final : public scene
{
public:
    title(fx::transitions&, sys::save_data&, ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed,
          sys::text_generators&);

    bool update(scene_stack&) override;

private:
    void update_fade_in();
    void update_ready();
    void update_fade_out(scene_stack&);

    void transit_to_ready();
    void transit_to_fade_out();

private:
    void handle_up_down_press();
    void handle_a_press();

private:
    void redraw_texts(ldtk::gen::lang_kind);
    void redraw_cursors(ldtk::gen::lang_kind);

private:
    void on_lang_changed(ldtk::gen::lang_kind);

public:
    enum class menu_option : std::uint8_t
    {
        START,
        LANGUAGE,
        THANKS,

        MAX_COUNT
    };

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

    ibn::observer<void(ldtk::gen::lang_kind)> _lang_changed_observer;

    state _state = state::FADE_IN;
    menu_option _menu_option = menu_option::START;

    bn::regular_bg_ptr _bg;

    bn::array<bn::sprite_ptr, 2> _cursor_sprites;
    bn::array<bn::vector<bn::sprite_ptr, 4>, (int)menu_option::MAX_COUNT> _options_text_sprites;
};

} // namespace sym::scn
