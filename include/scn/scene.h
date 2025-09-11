// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

namespace sym::scn
{

class scene_stack;

class scene
{
public:
    virtual ~scene() = default;

    /// @brief Updates the scene.
    /// @return `true` if `update()` should be called for the next scene below the stack.
    virtual bool update(scene_stack&) = 0;
};

} // namespace sym
