// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene.h"

namespace sym::fx
{
class transitions;
}

namespace sym::scn
{

class game final : public scene
{
public:
    game(fx::transitions&);

    bool update(scene_stack&) override;

private:
    fx::transitions& _transitions;
};

}
