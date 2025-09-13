// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/title.h"

#include <bn_string_view.h>

namespace sym::scn
{

title::title(fx::transitions& transitions, sys::save_data& save_data) : _transitions(transitions), _save_data(save_data)
{
}

bool title::update(scene_stack&)
{
    return false;
}

} // namespace sym::scn
