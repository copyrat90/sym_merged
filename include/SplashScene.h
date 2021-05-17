#pragma once

#include "Scene.h"

#include <bn_array.h>
#include <bn_blending_actions.h>
#include <bn_regular_bg_ptr.h>

namespace sym
{

class SplashScene : public Scene
{
public:
    SplashScene();
    ~SplashScene() = default;
    [[nodiscard]] bn::optional<SceneType> Update() final;

private:
    void SwapBgWhenNeeded();
    void SwapBg();
    /**
     * @brief Update the alpha transition action
     *
     * @return bn::optional<SceneType> Next scene type when transition is done, otherwise bn::nullopt
     */
    [[nodiscard]] bn::optional<SceneType> Transition();

    static constexpr int BG_SWAP_UPDATE_COUNT = 30;
    static constexpr int BG_SWAP_COUNT = 8;
    static constexpr int FADE_UPDATE_COUNT = 60;

    bn::array<bn::regular_bg_ptr, 2> bgs_;
    int bgSwapCounter_ = BG_SWAP_COUNT;
    int bgSwapUpdateCounter_ = BG_SWAP_UPDATE_COUNT;
    bool isShowingFirstBg_ = false;
    bool isFadeDone_ = false;
    bn::optional<bn::blending_transparency_alpha_to_action> transparentAction_;
};

} // namespace sym
