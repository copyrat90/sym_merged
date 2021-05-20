#pragma once

#include "scene_Scene.h"

#include <bn_array.h>
#include <bn_optional.h>
#include <bn_regular_bg_ptr.h>
#include <bn_sprite_ptr.h>

#include "effect_Transition.h"

namespace sym::scene
{

class Title final : public Scene
{
public:
    Title();
    ~Title() = default;
    [[nodiscard]] bn::optional<Type> Update() final;

private:
    bn::array<bn::sprite_ptr, 2> cursor_;
    bn::regular_bg_ptr bg_;
    effect::Transition fadeIn_;
    effect::Transition fadeOut_;

    static constexpr int CURSOR_HORIZONTAL_OFFSET = 30;
    static constexpr int BUTTON_WIDTH = 20;
    static constexpr int FADE_IN_UPDATE_COUNT = 30;
    static constexpr int FADE_OUT_UPDATE_COUNT = 30;
};

} // namespace sym::scene
