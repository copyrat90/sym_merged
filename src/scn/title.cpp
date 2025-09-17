// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/title.h"

#include "fx/transitions.h"
#include "scn/demo_end.h"
#include "scn/game.h"
#include "scn/scene_stack.h"
#include "sys/configs.h"
#include "sys/music.h"
#include "sys/save_data.h"
#include "sys/sound.h"
#include "sys/text_generators.h"

#include "ldtk_gen_enums.h"

#include <bn_array.h>
#include <bn_assert.h>
#include <bn_dmg_music.h>
#include <bn_fixed_point.h>
#include <bn_keypad.h>
#include <bn_music.h>
#include <bn_regular_bg_builder.h>
#include <bn_span.h>
#include <bn_sprite_builder.h>
#include <bn_string_view.h>

#include "bn_regular_bg_items_bg_title.h"
#include "bn_sprite_items_spr_cursor_star.h"

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
    TEXTS_ENTITY.get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_title_options)
        .get<bn::span<const bn::string_view>>();

constexpr bn::array<bn::fixed_point, (int)title::menu_option::MAX_COUNT> TEXTS_POS_LUT = [] {
    bn::array<bn::fixed_point, (int)title::menu_option::MAX_COUNT> result;
    const auto raw_poss =
        TEXTS_ENTITY.get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_title_options_positions)
            .get<bn::span<const bn::fixed>>();

    for (int i = 0; i < (int)title::menu_option::MAX_COUNT; ++i)
        result[i] = bn::fixed_point(raw_poss[2 * i], raw_poss[2 * i + 1]);

    return result;
}();

constexpr bn::array<bn::fixed_point, (int)ldtk::gen::lang_kind::max_count> CURSOR_OFFSETS = [] {
    bn::array<bn::fixed_point, (int)ldtk::gen::lang_kind::max_count> result;
    const auto raw_offsets =
        TEXTS_ENTITY
            .get_field(ldtk::gen::entity_field_ident::ENTITY_system_texts_FIELD_title_options_cursor_left_offset)
            .get<bn::span<const bn::fixed>>();

    for (int i = 0; i < (int)ldtk::gen::lang_kind::max_count; ++i)
        result[i] = bn::fixed_point(raw_offsets[2 * i], raw_offsets[2 * i + 1]);

    return result;
}();

auto create_bg() -> bn::regular_bg_ptr
{
    bn::regular_bg_builder builder(bn::regular_bg_items::bg_title);
    builder.set_blending_top_enabled(true);
    builder.set_mosaic_enabled(true);
    return builder.release_build();
}

auto create_cursors() -> bn::array<bn::sprite_ptr, 2>
{
    bn::sprite_builder builder(bn::sprite_items::spr_cursor_star);
    builder.set_blending_enabled(true);
    builder.set_mosaic_enabled(true);

    bn::array<bn::sprite_ptr, 2> result = {builder.build(), builder.build()};
    return result;
}

auto get_text(ldtk::gen::lang_kind lang, title::menu_option menu_option) -> const bn::string_view&
{
    BN_ASSERT(lang < ldtk::gen::lang_kind::max_count, "Invalid lang: ", (int)lang);
    BN_ASSERT(menu_option < title::menu_option::MAX_COUNT, "Invalid menu option: ", (int)menu_option);

    return TEXTS_LUT[(int)lang * (int)title::menu_option::MAX_COUNT + (int)menu_option];
}

auto get_text_pos(title::menu_option menu_option) -> const bn::fixed_point&
{
    BN_ASSERT(menu_option < title::menu_option::MAX_COUNT, "Invalid menu option: ", (int)menu_option);

    return TEXTS_POS_LUT[(int)menu_option];
}

auto get_cursor_offset(ldtk::gen::lang_kind lang) -> const bn::fixed_point&
{
    BN_ASSERT(lang < ldtk::gen::lang_kind::max_count, "Invalid lang: ", (int)lang);

    return CURSOR_OFFSETS[(int)lang];
}

} // namespace

title::title(fx::transitions& transitions, sys::save_data& save_data,
             ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed, sys::text_generators& text_generators)
    : _transitions(transitions), _save_data(save_data), _lang_changed(lang_changed), _text_generators(text_generators),
      _bg(create_bg()), _cursor_sprites(create_cursors())
{
    _lang_changed_observer.callback.set<&title::on_lang_changed>(*this);
    _lang_changed.attach(_lang_changed_observer);

    redraw_texts(save_data.language());

    _transitions.set_alpha(FADE_IN_TRANSITIONS, 1);
    _transitions.start(FADE_IN_TRANSITIONS, FADE_DURATION_UPDATES, 0);

    bn::music::stop();
    bn::dmg_music::stop();

    sys::music::play(ldtk::gen::music_kind::detour);
}

bool title::update(scene_stack& scene_stack_)
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

void title::update_fade_in()
{
    handle_up_down_press();

    if (_transitions.done())
        transit_to_ready();
}

void title::update_ready()
{
    handle_up_down_press();
    handle_a_press();
}

void title::update_fade_out(scene_stack& scene_stack_)
{
    if (_transitions.done())
    {
        scene_stack_.reserve_pop();

        switch (_menu_option)
        {
        case menu_option::START:
            scene_stack_.reserve_push<game>(_transitions, _save_data, _lang_changed, _text_generators);
            break;
        case menu_option::THANKS:
            scene_stack_.reserve_push<demo_end>(_transitions, _save_data, _lang_changed, _text_generators);
            break;
        default:
            BN_ERROR("Invalid menu option to fade out: ", (int)_menu_option);
        }
    }
}

void title::transit_to_ready()
{
    _state = state::READY;
}

void title::transit_to_fade_out()
{
    _transitions.start(FADE_OUT_TRANSITIONS, FADE_DURATION_UPDATES, 1);

    _state = state::FADE_OUT;
}

void title::handle_up_down_press()
{
    int menu_index = static_cast<int>(_menu_option);
    if (bn::keypad::up_pressed())
        menu_index = (menu_index - 1 + (int)menu_option::MAX_COUNT) % (int)menu_option::MAX_COUNT;
    if (bn::keypad::down_pressed())
        menu_index = (menu_index + 1) % (int)menu_option::MAX_COUNT;

    _menu_option = static_cast<menu_option>(menu_index);

    if (bn::keypad::up_pressed() || bn::keypad::down_pressed())
        sys::sound::play(ldtk::gen::sound_kind::menu_cursor);

    redraw_cursors(_save_data.language());
}

void title::handle_a_press()
{
    if (bn::keypad::a_pressed())
    {
        switch (_menu_option)
        {
        case menu_option::START:
            sys::sound::play(ldtk::gen::sound_kind::menu_select);
            transit_to_fade_out();
            break;

        case menu_option::LANGUAGE:
            sys::sound::play(ldtk::gen::sound_kind::menu_select);
            _save_data.set_next_language();
            _lang_changed.notify(_save_data.language());
            break;

        case menu_option::THANKS:
            if (_save_data.demo_cleared())
            {
                sys::sound::play(ldtk::gen::sound_kind::menu_select);
                transit_to_fade_out();
            }
            else
            {
                sys::sound::play(ldtk::gen::sound_kind::error);
            }
            break;

        default:
            BN_ERROR("Invalid menu option: ", (int)_menu_option);
        }
    }
}

void title::redraw_texts(ldtk::gen::lang_kind lang)
{
    // Clear the previous text sprites
    for (auto& option_text : _options_text_sprites)
        option_text.clear();

    // Re-generate the text sprites
    auto& text_generator = _text_generators.get(lang);

    const auto prev_palette = text_generator.palette_item(); // Copy prev by value, not reference!
    const auto& gray_palette = sys::text_generators::get_palette(sys::text_generators::text_color::GRAY);

    text_generator.generate(get_text_pos(menu_option::START), get_text(lang, menu_option::START),
                            _options_text_sprites[(int)menu_option::START]);
    text_generator.generate(get_text_pos(menu_option::LANGUAGE), get_text(lang, menu_option::LANGUAGE),
                            _options_text_sprites[(int)menu_option::LANGUAGE]);
    if (_save_data.demo_cleared())
    {
        text_generator.generate(get_text_pos(menu_option::THANKS), get_text(lang, menu_option::THANKS),
                                _options_text_sprites[(int)menu_option::THANKS]);
    }
    else
    {
        text_generator.set_palette_item(gray_palette);
        text_generator.generate(get_text_pos(menu_option::THANKS), "???",
                                _options_text_sprites[(int)menu_option::THANKS]);
    }

    text_generator.set_palette_item(prev_palette);

    redraw_cursors(lang);
}

void title::redraw_cursors(ldtk::gen::lang_kind lang)
{
    // Move the cursors to the text start/end positions
    const auto& leftmost_text_sprite = _options_text_sprites[(int)_menu_option].front();
    const auto& cursor_offset = get_cursor_offset(lang);

    const bn::fixed_point left_cursor_pos{
        leftmost_text_sprite.x() - leftmost_text_sprite.shape_size().width() / 2 + cursor_offset.x(),
        leftmost_text_sprite.y() + cursor_offset.y(),
    };
    const bn::fixed_point right_cursor_pos(-left_cursor_pos.x(), left_cursor_pos.y());

    _cursor_sprites[0].set_position(left_cursor_pos);
    _cursor_sprites[1].set_position(right_cursor_pos);
}

void title::on_lang_changed(ldtk::gen::lang_kind lang)
{
    redraw_texts(lang);
}

} // namespace sym::scn
