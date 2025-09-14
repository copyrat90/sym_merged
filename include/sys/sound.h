// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "ldtk_gen_enums.h"

#include <bn_sound_handle.h>

namespace sym::sys::sound
{

auto play(ldtk::gen::sound_kind) -> bn::sound_handle;

}
