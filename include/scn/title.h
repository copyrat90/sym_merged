// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene.h"

namespace sym::fx
{
class transitions;
}
namespace sym::sys
{
class save_data;
}

namespace sym::scn
{

class title final : public scene
{
public:
    title(fx::transitions&, sys::save_data&);

    bool update(scene_stack&) override;

private:
    fx::transitions& _transitions;
    sys::save_data& _save_data;
};

}
