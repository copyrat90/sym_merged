#include "scene_DemoEnd.h"

#include <bn_assert.h>
#include <bn_keypad.h>
#include <bn_string_view.h>

#include "bn_music_items.h"
#include "bn_sprite_items_spr_ingame_protagonist_star.h"
#include "constant.h"
#include "global.h"

namespace sym::scene
{

namespace
{

constexpr int FADE_IN_UPDATE_COUNT = 30;
constexpr int FADE_OUT_UPDATE_COUNT = 30;

constexpr int LINE_COUNT = 5;

constexpr bn::string_view TEXTS_ENG[LINE_COUNT] = {
    "That's it for this demo.", "Thanks for playing!",     "Press R to restart from start",
    "Press any key else to",    "go back to title screen",
};
constexpr bn::string_view TEXTS_KOR[LINE_COUNT] = {
    "데모는 여기까지입니다.", "플레이해주셔서 감사합니다!", "R키 눌러 처음부터 재시작",
    "다른 아무 키나 눌러",    "타이틀 화면으로 돌아가기",
};
constexpr bn::fixed TEXT_SPRITE_Y_POS[LINE_COUNT] = {-50, -30, 10, 30, 50};

} // namespace

DemoEnd::DemoEnd(scene::Param& sceneParam)
    : IScene(sceneParam), fadeIn_(effect::Transition::Types::FADE | effect::Transition::Types::SPRITE_MOSAIC |
                                      effect::Transition::Types::BG_MOSAIC,
                                  effect::Transition::Direction::IN, FADE_IN_UPDATE_COUNT),
      fadeOut_(effect::Transition::Types::FADE | effect::Transition::Types::SPRITE_MOSAIC |
                   effect::Transition::Types::BG_MOSAIC,
               effect::Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT)
{
    using namespace global::setting;
    auto* const textGen = global::GetTextGen();
    const auto& texts = GetLang() == Lang::ENG ? TEXTS_ENG : TEXTS_KOR;
    auto prevPal = textGen->palette_item();
    textGen->set_palette_item(bn::sprite_items::spr_ingame_protagonist_star.palette_item());
    for (int i = 0; i < LINE_COUNT; ++i)
    {
        if (i == 2)
            textGen->set_palette_item(prevPal);
        textGen->generate(0, TEXT_SPRITE_Y_POS[i], texts[i], textSprites_[i]);
        for (auto& sprite : textSprites_[i])
        {
            sprite.set_blending_enabled(true);
            sprite.set_mosaic_enabled(true);
        }
    }

    bn::music_items::music_past.play(constant::volume::music_past);
    volumeFadeOut_.emplace(FADE_OUT_UPDATE_COUNT, 0);

    fadeIn_.Init();
}

DemoEnd::~DemoEnd()
{
    bn::blending::set_fade_alpha(0);
}

bn::optional<Type> DemoEnd::Update()
{
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
            if (waitTillGetInput_ > 0)
                --waitTillGetInput_;
            else
            {
                if (bn::keypad::r_pressed())
                {
                    sceneParam_.SetCurrentStage(game::stage::Id::W1_S0);
                    global::SetCurrentStage(game::stage::Id::W1_S0);
                    reservedSceneType_ = scene::Type::GAME;
                    fadeOut_.Init();
                }
                else if (bn::keypad::any_pressed())
                {
                    reservedSceneType_ = scene::Type::SPLASH;
                    fadeOut_.Init();
                }
            }
            break;
        case Transition::State::ONGOING:
            fadeOut_.Update();
            if (!volumeFadeOut_->done())
                volumeFadeOut_->update();
            break;
        case Transition::State::DONE:
            return reservedSceneType_;
        default:
            BN_ERROR("Invalid fadeOut_ state: ", static_cast<int>(fadeOut_.GetState()));
        }
        break;
    default:
        BN_ERROR("Invalid fadeIn_ state: ", static_cast<int>(fadeIn_.GetState()));
    }
    return bn::nullopt;
}

} // namespace sym::scene
