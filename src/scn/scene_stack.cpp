// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/scene_stack.h"

namespace sym::scn
{

void scene_stack::update()
{
    // Update scenes
    for (auto iter = _scenes.rbegin(); iter != _scenes.rend(); ++iter)
    {
        // Break if the upper scene don't want to update the scene below
        if (!(*iter)->update(*this))
            break;
    }

    // Apply reserves
    for (auto iter = _reserved.begin(); iter != _reserved.end(); ++iter)
    {
        // If `nullptr`, it's a pop
        if ((*iter))
        {
            // Apply pushes
            _scenes.push_back(std::move(*iter));
        }
        else
        {
            // Apply pops
            _scenes.pop_back();
        }
    }
    _reserved.clear();
}

void scene_stack::reserve_pop()
{
    // `nullptr` means pop
    _reserved.emplace_back(nullptr);
}

} // namespace sym::scn
