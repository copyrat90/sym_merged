#include "SplashScene.h"

#include <bn_blending_actions.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_optional.h>

#include "bn_regular_bg_items_bg_splash1.h"
#include "bn_regular_bg_items_bg_splash2.h"

namespace sym
{

SplashScene::SplashScene()
    : bgs_{bn::regular_bg_items::bg_splash1.create_bg(0, 0), bn::regular_bg_items::bg_splash2.create_bg(0, 0)}
{
    bgs_[0].set_visible(isShowingFirstBg_);
    for (auto& bg : bgs_)
        bg.set_blending_enabled(true);
}

bn::optional<SceneType> SplashScene::Update()
{
    SwapBgWhenNeeded();

    if (bn::keypad::a_pressed())
    {
        bgSwapCounter_ = 0;
    }

    if (0 >= bgSwapCounter_)
    {
        return Transition();
    }

    return bn::nullopt;
}

void SplashScene::SwapBgWhenNeeded()
{
    if (0 >= bgSwapUpdateCounter_--)
    {
        bgSwapUpdateCounter_ = BG_SWAP_UPDATE_COUNT;
        --bgSwapCounter_;
        SwapBg();
    }
}

void SplashScene::SwapBg()
{
    isShowingFirstBg_ = !isShowingFirstBg_;
    bgs_[0].set_visible(isShowingFirstBg_);
    bgs_[1].set_visible(!isShowingFirstBg_);
}

bn::optional<SceneType> SplashScene::Transition()
{
    if (!transparentAction_)
    {
        transparentAction_ = bn::blending_transparency_alpha_to_action(FADE_UPDATE_COUNT, 0);
    }

    if (transparentAction_->done())
    {
        return SceneType::TITLE;
    }

    transparentAction_->update();
    return bn::nullopt;
}

} // namespace sym
