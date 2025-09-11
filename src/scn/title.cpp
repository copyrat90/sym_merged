// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/title.h"

namespace sym::scn
{

title::title(fx::transitions& transitions) : _transitions(transitions)
{
}

bool title::update(scene_stack&)
{
    return false;
}

} // namespace sym::scn
