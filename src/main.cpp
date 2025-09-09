// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "ldtk_core.h"

#include <bn_core.h>
#include <bn_dmg_music.h>

int main()
{
    bn::core::init();
    ldtk::core::init();

    // This game uses some DMG music, so we're setting its volume to FULL
    bn::dmg_music::set_master_volume(bn::dmg_music_master_volume::FULL);

    while (true)
    {
        bn::core::update();
    }
}
