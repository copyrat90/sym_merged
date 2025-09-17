// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "fx/transitions.h"
#include "scn/scene_stack.h"
#include "scn/splash.h"
#include "sys/save_data.h"
#include "sys/text_generators.h"

#include "ibn_observer.h"

#include "ldtk_core.h"

#include <bn_core.h>

#include "ldtk_gen_enums.h"

namespace
{
sym::sys::save_data save_data;
}

void save_on_language_changed(ldtk::gen::lang_kind);

int main()
{
    bn::core::init();
    ldtk::core::init();

    // Transitions manager to deal with actions for transparency, mosaic, intensity, etc.
    sym::fx::transitions transitions;

    // Let's load the save data beforehand.
    save_data.load();

    // Language changed subject.
    // You can observe this via `lang_changed.attach(observer)`
    ibn::subject<void(ldtk::gen::lang_kind)> lang_changed;

    // When language changed, Save the config to SRAM
    ibn::observer<void(ldtk::gen::lang_kind)> lang_changed_observer;
    lang_changed_observer.callback.set(save_on_language_changed);
    lang_changed.attach(lang_changed_observer);

    // Shared text generators.
    sym::sys::text_generators text_generators;

    // The scene stack
    sym::scn::scene_stack scene_stack;
    scene_stack.reserve_push<sym::scn::splash>(transitions, save_data, lang_changed, text_generators);

    while (true)
    {
        scene_stack.update();
        transitions.update();
        bn::core::update();
    }
}

void save_on_language_changed(ldtk::gen::lang_kind lang)
{
    save_data.set_language(lang);
    save_data.save();
}
