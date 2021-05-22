#include "scene_Splash.h"

#include <bn_assert.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_optional.h>

#include "bn_regular_bg_items_bg_splash1.h"
#include "bn_regular_bg_items_bg_splash2.h"
#include "helper_keypad.h"

namespace sym::scene
{

Splash::Splash()
    : bgs_{bn::regular_bg_items::bg_splash1.create_bg(0, 0), bn::regular_bg_items::bg_splash2.create_bg(0, 0)},
      fadeIn_(effect::Transition::Types::TRANSPARENCY, effect::Transition::Direction::IN, FADE_IN_UPDATE_COUNT),
      fadeOut_(effect::Transition::Types::TRANSPARENCY, effect::Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT)
{
    bgs_[0].set_visible(isShowingFirstBg_);
    bgs_[1].set_visible(!isShowingFirstBg_);

    for (auto& bg : bgs_)
        bg.set_blending_enabled(true);
    fadeIn_.Init();
}

Splash::~Splash()
{
    // Turn off transparency blending
    bn::blending::set_transparency_alpha(1);
}

bn::optional<Type> Splash::Update()
{
    SwapBgWhenNeeded_();

    using kt = bn::keypad::key_type;
    using sym::helper::keypad::operator|;
    FetchPressAnyOfTheseKeys_(kt::A | kt::B | kt::START);

    using Transition = effect::Transition;
    switch (fadeIn_.GetState())
    {
    case Transition::State::NOT_READY:
        BN_ERROR("fadeIn_ somehow not initialized. should not reach here...");
        break;
    case Transition::State::ONGOING:
        fadeIn_.Update();
        break;
    case Transition::State::DONE:
        switch (fadeOut_.GetState())
        {
        case Transition::State::NOT_READY:
            if (isPressedKey_ || fadeOutStartCounter_ <= 0)
            {
                fadeOut_.Init();
            }
            break;
        case Transition::State::ONGOING:
            fadeOut_.Update();
            break;
        case Transition::State::DONE:
            return Type::TITLE;
            break;
        default:
            BN_ERROR("Unknown fadeOut_::State: ", (int)fadeOut_.GetState());
            break;
        }
        break;
    default:
        BN_ERROR("Unknown fadeIn_::State: ", (int)fadeIn_.GetState());
        break;
    }

    return bn::nullopt;
}

void Splash::SwapBgWhenNeeded_()
{
    if (0 >= bgSwapUpdateCounter_--)
    {
        bgSwapUpdateCounter_ = BG_SWAP_UPDATE_COUNT;
        if (0 < fadeOutStartCounter_)
            --fadeOutStartCounter_;
        SwapBg_();
    }
}

void Splash::SwapBg_()
{
    isShowingFirstBg_ = !isShowingFirstBg_;
    bgs_[0].set_visible(isShowingFirstBg_);
    bgs_[1].set_visible(!isShowingFirstBg_);
}

void Splash::FetchPressAnyOfTheseKeys_(bn::keypad::key_type theseKeys)
{
    isPressedKey_ |= bn::keypad::pressed(theseKeys);
}

} // namespace sym::scene
