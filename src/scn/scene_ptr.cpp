// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#include "scn/scene_ptr.h"

namespace sym::scn
{

scene_deleter::scene_deleter(scene_pool_t& pool) : _pool(&pool)
{
}

void scene_deleter::operator()(scene* scene_) const
{
    _pool->destroy(*scene_);
}

} // namespace sym
