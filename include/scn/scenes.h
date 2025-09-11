// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/demo_end.h"
#include "scn/game.h"
#include "scn/splash.h"
#include "scn/title.h"

#include <algorithm>

namespace sym::scn
{

inline constexpr int MAX_SCENE_SIZE = std::max({
    sizeof(splash),
    sizeof(title),
    sizeof(game),
    sizeof(demo_end),
});

}
