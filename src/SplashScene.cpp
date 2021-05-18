#include "SplashScene.h"

#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_blending_actions.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_optional.h>

#include "KeypadHelper.h"
#include "bn_regular_bg_items_bg_splash1.h"
#include "bn_regular_bg_items_bg_splash2.h"

namespace sym
{

SplashScene::SplashScene()
    : bgs_{bn::regular_bg_items::bg_splash1.create_bg(0, 0), bn::regular_bg_items::bg_splash2.create_bg(0, 0)}
{
    bgs_[0].set_visible(isShowingFirstBg_);
    bgs_[1].set_visible(!isShowingFirstBg_);

    for (auto& bg : bgs_)
        bg.set_blending_enabled(true);
    bn::blending::set_transparency_alpha(0);
    transparentAction_ = bn::blending_transparency_alpha_to_action(FADE_IN_UPDATE_COUNT, 1);
}

bn::optional<SceneType> SplashScene::Update()
{
    SwapBgWhenNeeded_();

    using kt = bn::keypad::key_type;
    using sym::helper::operator|;
    FetchPressAnyOfTheseKeys_(kt::A | kt::B | kt::START);

    switch (state_)
    {
    case SplashState::FADE_IN:
        UpdateFadeIn_();
        break;
    case SplashState::PRESENT:
        UpdatePresent_();
        break;
    case SplashState::FADE_OUT:
        UpdateFadeOut_();
        break;
    case SplashState::DONE:
        return SceneType::TITLE;
    default:
        BN_ERROR("Unknown SplashState: ", (int)state_);
        break;
    }

    return bn::nullopt;
}

void SplashScene::SwapBgWhenNeeded_()
{
    if (0 >= bgSwapUpdateCounter_--)
    {
        bgSwapUpdateCounter_ = BG_SWAP_UPDATE_COUNT;
        if (0 < fadeOutStartCounter_)
            --fadeOutStartCounter_;
        SwapBg_();
    }
}

void SplashScene::SwapBg_()
{
    isShowingFirstBg_ = !isShowingFirstBg_;
    bgs_[0].set_visible(isShowingFirstBg_);
    bgs_[1].set_visible(!isShowingFirstBg_);
}

void SplashScene::FetchPressAnyOfTheseKeys_(bn::keypad::key_type theseKeys)
{
    isPressedKey_ |= bn::keypad::pressed(theseKeys);
}

void SplashScene::UpdateFadeIn_()
{
    if (transparentAction_->done())
    {
        state_ = SplashState::PRESENT;
        transparentAction_.reset();
    }
    else
    {
        transparentAction_->update();
    }
}

void SplashScene::UpdatePresent_()
{
    if (isPressedKey_ || (0 >= fadeOutStartCounter_))
    {
        state_ = SplashState::FADE_OUT;
    }
}

void SplashScene::UpdateFadeOut_()
{
    if (!transparentAction_)
        transparentAction_ = bn::blending_transparency_alpha_to_action(FADE_OUT_UPDATE_COUNT, 0);

    if (transparentAction_->done())
        state_ = SplashState::DONE;
    else
        transparentAction_->update();
}

} // namespace sym
