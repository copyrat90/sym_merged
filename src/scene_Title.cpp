#include "scene_Title.h"

#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_fixed.h>
#include <bn_keypad.h>
#include <bn_music_actions.h>

#include "bn_music_items.h"
#include "bn_regular_bg_items_bg_title.h"
#include "bn_sound_items.h"
#include "bn_sprite_items_spr_cursor_star.h"
#include "bn_sprite_palette_items_pal_menu_header.h"
#include "constant.h"
#include "global.h"
#include "helper_sprite.h"

namespace sym::scene
{

Title::Title(scene::Param& sceneParam)
    : IScene(sceneParam), cursor_{bn::sprite_items::spr_cursor_star.create_sprite(0, 0),
                                  bn::sprite_items::spr_cursor_star.create_sprite(0, 0)},
      bg_(bn::regular_bg_items::bg_title.create_bg(0, 0)),
      fadeIn_(effect::Transition::Types::FADE | effect::Transition::Types::SPRITE_MOSAIC |
                  effect::Transition::Types::BG_MOSAIC,
              effect::Transition::Direction::IN, FADE_IN_UPDATE_COUNT),
      fadeOut_(effect::Transition::Types::FADE | effect::Transition::Types::SPRITE_MOSAIC |
                   effect::Transition::Types::BG_MOSAIC,
               effect::Transition::Direction::OUT, FADE_OUT_UPDATE_COUNT)
{
    bg_.set_blending_enabled(true);
    bg_.set_mosaic_enabled(true);
    for (auto& cursor : cursor_)
    {
        cursor.set_blending_enabled(true);
        cursor.set_mosaic_enabled(true);
    }

    RedrawMenuTextSprites_();
    UpdateCursorSpritePosition_();

    bn::music_items::music_detour.play(constant::volume::music_detour);
    volumeAction_.emplace(FADE_OUT_UPDATE_COUNT, 0);
    fadeIn_.Init();
}

Title::~Title()
{
    bn::blending::set_fade_alpha(0);
}

bn::optional<Type> Title::Update()
{
    using Transition = effect::Transition;
    switch (fadeIn_.GetState())
    {
    case Transition::State::NOT_READY:
        BN_ERROR("fadeIn_ somehow not initialized. should not reach here...");
        break;
    case Transition::State::ONGOING:
        fadeIn_.Update();
        HandleUpDownPress_();
        break;
    case Transition::State::DONE:
        switch (fadeOut_.GetState())
        {
        case Transition::State::NOT_READY:
            HandleUpDownPress_();
            HandleAPress_();
            break;
        case Transition::State::ONGOING:
            fadeOut_.Update();
            if (!volumeAction_->done())
                volumeAction_->update();
            break;
        case Transition::State::DONE:
            return reservedNextScene_;
            break;
        default:
            BN_ERROR("Unknown fadeOut_::State : ", static_cast<int>(fadeOut_.GetState()));
            break;
        }
        break;
    default:
        BN_ERROR("Unknown fadeIn_::State : ", static_cast<int>(fadeIn_.GetState()));
        break;
    }

    return bn::nullopt;
}

void Title::HandleUpDownPress_()
{
    int cursorMoveDirection = 0;
    if (bn::keypad::up_pressed())
    {
        bn::sound_items::sfx_menu_cursor.play();
        cursorMoveDirection = -1;
    }
    else if (bn::keypad::down_pressed())
    {
        bn::sound_items::sfx_menu_cursor.play();
        cursorMoveDirection = 1;
    }

    AdvanceCursorPointingOption_(cursorMoveDirection);
    UpdateCursorSpritePosition_();
}

void Title::HandleAPress_()
{
    if (bn::keypad::a_pressed())
    {
        switch (cursorPointingOption_)
        {
        case MenuOption::START:
            // if (!global::GetSeenOpening())
            //     reservedNextScene_ = scene::Type::OPENING;
            // else
            //     reservedNextScene_ = scene::Type::SELECT_STAGE;
            bn::sound_items::sfx_menu_select.play();
            reservedNextScene_ = scene::Type::GAME;
            fadeOut_.Init();
            break;
        case MenuOption::LANGUAGE:
            bn::sound_items::sfx_menu_select.play();
            using namespace global::setting;
            switch (GetLang())
            {
            case Lang::ENG:
                SetLang(Lang::KOR);
                break;
            case Lang::KOR:
                SetLang(Lang::ENG);
                break;
            default:
                BN_ERROR("Unknown Lang : ", static_cast<int>(GetLang()));
                break;
            }
            RedrawMenuTextSprites_();
            UpdateCursorSpritePosition_();
            break;
        case MenuOption::CREDIT:
            bn::sound_items::sfx_error.play();
            // reservedNextScene_ = scene::Type::CREDIT;
            break;
        case MenuOption::MENU_OPTION_TOTAL_COUNT:
        default:
            BN_ERROR("Invalid MenuOption : ", static_cast<int>(cursorPointingOption_));
            break;
        }
    }
}

void Title::AdvanceCursorPointingOption_(int amount)
{
    int cursorIdx = static_cast<int>(cursorPointingOption_);
    cursorIdx += (amount + MENU_OPTION_TOTAL_COUNT);
    cursorIdx %= MENU_OPTION_TOTAL_COUNT;
    cursorPointingOption_ = static_cast<MenuOption>(cursorIdx);
}

void Title::UpdateCursorSpritePosition_()
{
    using namespace global;

    const int cursorIdx = static_cast<int>(cursorPointingOption_);
    const int langIdx = static_cast<int>(setting::GetLang());

    bn::fixed left = helper::sprite::Left(menuTextSprites_[cursorIdx].front());
    left += CURSOR_OFFSET[langIdx].x();

    const bn::fixed right = -left;
    const bn::fixed y = MENU_STRING_POS[langIdx][cursorIdx].y() + CURSOR_OFFSET[langIdx].y();

    cursor_[0].set_position({left, y});
    cursor_[1].set_position({right, y});
}

void Title::RedrawMenuTextSprites_()
{
    using namespace global;
    const int langIdx = static_cast<int>(setting::GetLang());
    auto* const textGen = global::GetTextGen();

    for (auto& menuText : menuTextSprites_)
        menuText.clear();

    auto prevPal = textGen->palette_item();
    for (int i = 0; i < MENU_OPTION_TOTAL_COUNT; ++i)
    {
        if (i == 2)
            textGen->set_palette_item(bn::sprite_palette_items::pal_menu_header);
        textGen->generate(MENU_STRING_POS[langIdx][i], MENU_STRINGS[langIdx][i], menuTextSprites_[i]);
    }
    textGen->set_palette_item(prevPal);

    for (auto& menuText : menuTextSprites_)
    {
        for (auto& sprite : menuText)
        {
            sprite.set_blending_enabled(true);
            sprite.set_mosaic_enabled(true);
        }
    }
}

} // namespace sym::scene
