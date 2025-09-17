// SPDX-FileCopyrightText: Copyright 2021-2025 Guyeon Yu <copyrat90@gmail.com>
// SPDX-License-Identifier: MIT

#pragma once

#include "scn/scene.h"

#include "ibn_observer.h"

#include "ldtk_gen_enums.h"

namespace sym::fx
{
class transitions;
}
namespace sym::sys
{
class save_data;
class text_generators;
} // namespace sym::sys

namespace sym::scn
{

class demo_end final : public scene
{
public:
    demo_end(fx::transitions&, sys::save_data&, ibn::subject<void(ldtk::gen::lang_kind)>& lang_changed,
             sys::text_generators&);

    bool update(scene_stack&) override;

private:
    void on_lang_changed(ldtk::gen::lang_kind);

private:
    fx::transitions& _transitions;
    sys::save_data& _save_data;
    ibn::subject<void(ldtk::gen::lang_kind)>& _lang_changed;
    sys::text_generators& _text_generators;

    ibn::observer<void(ldtk::gen::lang_kind)> _lang_changed_observer;
};

} // namespace sym::scn
