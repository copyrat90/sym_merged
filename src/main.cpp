// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "fx/transitions.h"
#include "scn/scene_stack.h"
#include "scn/splash.h"
#include "sys/save_data.h"

#include "ldtk_core.h"

#include <bn_core.h>

int main()
{
    bn::core::init();
    ldtk::core::init();

    // Transitions manager to deal with actions for transparency, mosaic, intensity, etc.
    sym::fx::transitions transitions;

    // Some "global" datas
    sym::sys::save_data save_data;
    save_data.load();

    // The scene stack
    sym::scn::scene_stack scene_stack;
    scene_stack.reserve_push<sym::scn::splash>(transitions, save_data);

    while (true)
    {
        scene_stack.update();
        transitions.update();
        bn::core::update();
    }
}
