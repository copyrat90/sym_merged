// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/demo_end.h"

#include "fx/transitions.h"
#include "scn/game.h"
#include "scn/scene_stack.h"
#include "scn/splash.h"
#include "sys/configs.h"
#include "sys/music.h"
#include "sys/save_data.h"
#include "sys/text_generators.h"

#include <bn_dmg_music.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_music.h>
#include <bn_span.h>
#include <bn_string_view.h>

namespace sym::scn
{

namespace
{

constexpr auto FADE_IN_TRANSITIONS =
    fx::transitions::kinds::TRANSPARENCY | fx::transitions::kinds::BGS_MOSAIC | fx::transitions::kinds::SPRITES_MOSAIC;
constexpr auto FADE_OUT_TRANSITIONS = FADE_IN_TRANSITIONS | fx::transitions::kinds::MUSIC_VOLUME;

constexpr int FADE_DURATION_UPDATES = 30;

constexpr const ldtk::entity& TEXTS_ENTITY = sys::get_config_entity(ldtk::gen::entity_ident::system_texts);

constexpr bn::span<const bn::string_view> TEXTS_LUT =
    TEXTS_ENTITY.get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_demo_end_notes)
        .get<bn::span<const bn::string_view>>();

constexpr bn::fixed_point TEXT_START_POS = [] constexpr -> bn::fixed_point {
    const auto& raw_pos =
        TEXTS_ENTITY.get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_demo_end_notes_position)
            .get<bn::span<const bn::fixed>>();

    return bn::fixed_point(raw_pos[0], raw_pos[1]);
}();

constexpr bn::fixed LINE_SPACING =
    TEXTS_ENTITY.get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_demo_end_notes_line_spacing)
        .get<bn::fixed>();

auto get_text(ldtk::gen::lang_kind lang, int paragraph) -> const bn::string_view&
{
    BN_ASSERT(lang < ldtk::gen::lang_kind::max_count, "Invalid lang: ", (int)lang);
    BN_ASSERT(0 <= paragraph && paragraph < 2, "Invalid paragraph: ", paragraph);

    return TEXTS_LUT[2 * (int)lang + paragraph];
}

} // namespace

demo_end::demo_end(fx::transitions& transitions, sys::save_data& save_data,
                   ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed, sys::text_generators& text_generators)
    : _transitions(transitions), _save_data(save_data), _lang_changed(lang_changed), _text_generators(text_generators)
{
    transitions.set_alpha(FADE_IN_TRANSITIONS, 1);
    transitions.start(FADE_IN_TRANSITIONS, FADE_DURATION_UPDATES, 0);

    draw_texts(save_data.language());

    bn::music::stop();
    bn::dmg_music::stop();

    sys::music::play(ldtk::gen::music_kind::past);
}

bool demo_end::update(scene_stack& scene_stack_)
{
    switch (_state)
    {
    case state::FADE_IN:
        update_fade_in();
        break;
    case state::READY:
        update_ready();
        break;
    case state::FADE_OUT:
        update_fade_out(scene_stack_);
        break;
    default:
        BN_ERROR("Invalid state: ", (int)_state);
    }

    return false;
}

void demo_end::update_fade_in()
{
    if (_transitions.done())
        transit_to_ready();
}

void demo_end::update_ready()
{
    if (bn::keypad::r_pressed())
    {
        _restart_game = true;
        transit_to_fade_out();
    }
    else if (bn::keypad::any_pressed())
    {
        _restart_game = false;
        transit_to_fade_out();
    }
}

void demo_end::update_fade_out(scene_stack& scene_stack_)
{
    if (_transitions.done())
    {
        scene_stack_.reserve_pop();

        if (_restart_game)
        {
            _save_data.reset_stage();
            scene_stack_.reserve_push<game>(_transitions, _save_data, _lang_changed, _text_generators);
        }
        else
        {
            scene_stack_.reserve_push<splash>(_transitions, _save_data, _lang_changed, _text_generators);
        }
    }
}

void demo_end::transit_to_ready()
{
    _state = state::READY;
}

void demo_end::transit_to_fade_out()
{
    _transitions.start(FADE_OUT_TRANSITIONS, FADE_DURATION_UPDATES, 1);

    _state = state::FADE_OUT;
}

void demo_end::draw_texts(ldtk::gen::lang_kind lang)
{
    // This extended text generator supports '\n'
    ibn::sprite_text_generator& text_generator = _text_generators.get(lang);

    const auto prev_palette = text_generator.palette_item(); // Copy prev by value, not reference!
    const auto& yellow_palette = sys::text_generators::get_palette(sys::text_generators::text_color::YELLOW);
    text_generator.set_palette_item(yellow_palette);

    // ... with line spacing!
    text_generator.generate(TEXT_START_POS, get_text(lang, 0), _text_sprites, LINE_SPACING);

    text_generator.set_palette_item(prev_palette);

    // +2 lines spacing for next paragraph
    text_generator.generate(TEXT_START_POS.x(), _text_sprites.back().y() + 2 * LINE_SPACING, get_text(lang, 1),
                            _text_sprites, LINE_SPACING);
}

} // namespace sym::scn
