#pragma once

#include "scene_IScene.h"

#include <bn_array.h>
#include <bn_keypad.h>
#include <bn_regular_bg_ptr.h>

#include "effect_Transition.h"

namespace sym::scene
{

class Splash final : public IScene
{
public:
    Splash();
    ~Splash();
    [[nodiscard]] bn::optional<Type> Update() final;

private:
    bn::array<bn::regular_bg_ptr, 2> bgs_;
    bn::optional<bn::blending_transparency_alpha_to_action> transparentAction_;
    effect::Transition fadeIn_;
    effect::Transition fadeOut_;

    static constexpr int BG_SWAP_UPDATE_COUNT = 30;
    static constexpr int FADE_OUT_START_COUNT = 8;
    static constexpr int FADE_IN_UPDATE_COUNT = 60;
    static constexpr int FADE_OUT_UPDATE_COUNT = 60;

    int fadeOutStartCounter_ = FADE_OUT_START_COUNT;
    int bgSwapUpdateCounter_ = BG_SWAP_UPDATE_COUNT;
    bool isShowingFirstBg_ = false;
    bool isPressedKey_ = false;

    void SwapBgWhenNeeded_();
    void SwapBg_();

    void FetchPressAnyOfTheseKeys_(bn::keypad::key_type theseKeys);
};

} // namespace sym::scene
