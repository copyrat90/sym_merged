#pragma once

#include "Scene.h"

#include <bn_array.h>
#include <bn_blending_actions.h>
#include <bn_keypad.h>
#include <bn_regular_bg_ptr.h>

namespace sym
{

class SplashScene final : public Scene
{
public:
    SplashScene();
    ~SplashScene() = default;
    [[nodiscard]] bn::optional<SceneType> Update() final;

private:
    bn::array<bn::regular_bg_ptr, 2> bgs_;
    bn::optional<bn::blending_transparency_alpha_to_action> transparentAction_;

    static constexpr int BG_SWAP_UPDATE_COUNT = 30;
    static constexpr int FADE_OUT_START_COUNT = 8;
    static constexpr int FADE_IN_UPDATE_COUNT = 60;
    static constexpr int FADE_OUT_UPDATE_COUNT = 60;

    enum class SplashState
    {
        FADE_IN,
        PRESENT,
        FADE_OUT,
        DONE
    };
    SplashState state_ = SplashState::FADE_IN;
    int fadeOutStartCounter_ = FADE_OUT_START_COUNT;
    int bgSwapUpdateCounter_ = BG_SWAP_UPDATE_COUNT;
    bool isShowingFirstBg_ = false;
    bool isPressedKey_ = false;

    void SwapBgWhenNeeded_();
    void SwapBg_();

    void FetchPressAnyOfTheseKeys_(bn::keypad::key_type theseKeys);

    /**
     * @brief Update when state_ is SplashState::FADE_IN
     *
     */
    void UpdateFadeIn_();

    /**
     * @brief Update when state_ is SplashState::FADE_OUT
     *
     */
    void UpdateFadeOut_();

    /**
     * @brief Update when state_ is SplashState::PRESENT
     *
     */
    void UpdatePresent_();
};

} // namespace sym
