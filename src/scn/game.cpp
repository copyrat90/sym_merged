// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/game.h"

namespace sym::scn
{

game::game(fx::transitions& transitions, sys::save_data& save_data,
           ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed, sys::text_generators& text_generators)
    : _transitions(transitions), _save_data(save_data), _lang_changed(lang_changed), _text_generators(text_generators)
{
    _lang_changed_observer.callback.set<&game::on_lang_changed>(*this);
    _lang_changed.attach(_lang_changed_observer);
}

bool game::update(scene_stack&)
{
    return false;
}

void game::on_lang_changed(ldtk::gen::lang_kind lang)
{
    ((void)lang);
}

} // namespace sym::scn
