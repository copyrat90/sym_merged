// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene_ptr.h"

#include <bn_vector.h>
#include <bn_generic_pool.h>

#include <concepts>
#include <utility>

namespace sym::scn
{

class scene_stack final
{
public:
    void update();

public:
    template <std::derived_from<scene> Scene, typename... Args>
    void reserve_push(Args&&... args)
    {
        _reserved.emplace_back(&_scene_pool.create<Scene>(std::forward<Args>(args)...), scene_deleter(_scene_pool));
    }

    void reserve_pop();

private:
    scene_pool_t _scene_pool;
    bn::vector<scene_ptr, MAX_SCENE_COUNT> _scenes;

    bn::vector<scene_ptr, MAX_SCENE_COUNT * 2> _reserved;
};

}
