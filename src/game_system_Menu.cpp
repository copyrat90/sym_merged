#include "game_system_Menu.h"

#include <bn_assert.h>
#include <bn_blending.h>
#include <bn_keypad.h>
#include <bn_sound.h>
#include <bn_string_view.h>

#include "constant.h"
#include "global.h"
#include "helper_sprite.h"
#include "scene_GameState.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_spr_cursor_star.h"
#include "bn_sprite_palette_items_pal_menu_header.h"

namespace sym::game::system
{

namespace
{

constexpr bn::fixed PAUSE_FADE_ALPHA = 0.7;
constexpr int STAGE_RESTART_FADE_UPDATE_COUNT = 30;
constexpr bn::fixed MENU_STAGE_NAME_Y_POS = -62;
constexpr bn::fixed MENU_STAGE_SUB_NAME_Y_POS = -45;

constexpr bn::fixed MENU_OPTION_START_Y_POS = -20;
constexpr bn::fixed MENU_OPTION_DY = 20;

constexpr bn::string_view MENU_STRINGS_ENG[Menu::MENU_OPTION_TOTAL_COUNT] = {"Continue", "Restart this room",
                                                                             "Restart the stage", "언 어 : English"};
constexpr bn::string_view MENU_STRINGS_KOR[Menu::MENU_OPTION_TOTAL_COUNT] = {"계속하기", "현재 방 재시작",
                                                                             "스테이지 재시작", "뷁웱:한국어"};

constexpr bn::fixed_point CURSOR_OFFSET_ENG = {-13, -2};
constexpr bn::fixed_point CURSOR_OFFSET_KOR = {-13, -1};

} // namespace

Menu::Menu(scene::GameState& state)
    : ISystem(state), cursorSprites_{bn::sprite_items::spr_cursor_star.create_sprite(0, 0),
                                     bn::sprite_items::spr_cursor_star.create_sprite(0, 0)}
{
    cursorSprites_[0].set_z_order(constant::MENU_Z_ORDER);
    cursorSprites_[1].set_z_order(constant::MENU_Z_ORDER);
    RedrawMenuTextSprites_();
    UpdateCursorSpritePosition_();
    SetVisible_(false);
}

void Menu::Update()
{
    if (state_.transition.IsOngoing())
        return;

    if (bn::keypad::start_pressed() || (state_.isPaused && bn::keypad::b_pressed()))
        TogglePause_();
    else if (state_.isPaused)
    {
        if (bn::keypad::a_pressed())
        {
            switch (cursorPointingOption_)
            {
            case MenuOption::CONTINUE:
                TogglePause_();
                break;
            case MenuOption::RESTART_ZONE:
                RestartZone_();
                break;
            case MenuOption::RESTART_STAGE:
                RestartStage_();
                break;
            case MenuOption::CHANGE_LANGUAGE:
                ChangeLanguage_();
                break;
            // case MenuOption::BACK_TO_STAGE_SELECT:
            //     GoBackToStageSelect_();
            //     break;
            default:
                BN_ERROR("Invalid MenuOption: ", static_cast<int>(cursorPointingOption_));
            }
        }
        else if (bn::keypad::up_pressed())
        {
            bn::sound_items::sfx_menu_cursor.play(constant::volume::sfx_menu_cursor);
            AdvanceCursorPointingOption_(-1);
            UpdateCursorSpritePosition_();
        }
        else if (bn::keypad::down_pressed())
        {
            bn::sound_items::sfx_menu_cursor.play(constant::volume::sfx_menu_cursor);
            AdvanceCursorPointingOption_(1);
            UpdateCursorSpritePosition_();
        }
    }
}

void Menu::SetVisible_(bool isVisible)
{
    cursorSprites_[0].set_visible(isVisible);
    cursorSprites_[1].set_visible(isVisible);
    for (auto& line : menuOptionSprites_)
        for (auto& sprite : line)
            sprite.set_visible(isVisible);
    for (auto& line : stageNameSprites_)
        for (auto& sprite : line)
            sprite.set_visible(isVisible);
}

void Menu::SetBlendingAndMosaic_(bool isEnabled)
{
    cursorSprites_[0].set_blending_enabled(isEnabled);
    cursorSprites_[1].set_blending_enabled(isEnabled);
    cursorSprites_[0].set_mosaic_enabled(isEnabled);
    cursorSprites_[1].set_mosaic_enabled(isEnabled);
    for (auto& line : stageNameSprites_)
        for (auto& sprite : line)
        {
            sprite.set_blending_enabled(isEnabled);
            sprite.set_mosaic_enabled(isEnabled);
        }
    for (auto& line : menuOptionSprites_)
        for (auto& sprite : line)
        {
            sprite.set_blending_enabled(isEnabled);
            sprite.set_mosaic_enabled(isEnabled);
        }
}

void Menu::RedrawMenuTextSprites_()
{
    using namespace global::setting;
    const bool isEnglish = GetLang() == Lang::ENG;
    const bn::string_view& stageName = isEnglish ? state_.stageInfo.stageNameEng : state_.stageInfo.stageNameKor;
    const bn::string_view& stageSubName =
        isEnglish ? state_.stageInfo.stageSubNameEng : state_.stageInfo.stageSubNameKor;
    const bn::string_view(&menuStrings)[MENU_OPTION_TOTAL_COUNT] = (isEnglish) ? MENU_STRINGS_ENG : MENU_STRINGS_KOR;

    for (auto& line : menuOptionSprites_)
        line.clear();
    for (auto& line : stageNameSprites_)
        line.clear();
    auto* const textGen = global::GetTextGen();
    auto prevPal = textGen->palette_item();
    textGen->set_palette_item(bn::sprite_palette_items::pal_menu_header);
    textGen->generate(0, MENU_STAGE_NAME_Y_POS, stageName, stageNameSprites_[0]);
    textGen->generate(0, MENU_STAGE_SUB_NAME_Y_POS, stageSubName, stageNameSprites_[1]);
    textGen->set_palette_item(prevPal);
    for (int i = 0; i < MENU_OPTION_TOTAL_COUNT; ++i)
    {
        const bn::fixed y = MENU_OPTION_START_Y_POS + i * MENU_OPTION_DY;
        textGen->generate(0, y, menuStrings[i], menuOptionSprites_[i]);
    }
    for (auto& line : menuOptionSprites_)
        for (auto& sprite : line)
            sprite.set_z_order(constant::MENU_Z_ORDER);
    for (auto& line : stageNameSprites_)
        for (auto& sprite : line)
            sprite.set_z_order(constant::MENU_Z_ORDER);
}

void Menu::UpdateCursorSpritePosition_()
{
    using namespace global::setting;
    const bn::fixed_point& cursorOffset = GetLang() == Lang::ENG ? CURSOR_OFFSET_ENG : CURSOR_OFFSET_KOR;
    const int idx = static_cast<int>(cursorPointingOption_);
    const bn::fixed y = MENU_OPTION_START_Y_POS + idx * (MENU_OPTION_DY) + cursorOffset.y();
    const bn::fixed x = helper::sprite::Left(menuOptionSprites_[idx].front()) + cursorOffset.x();

    cursorSprites_[0].set_position(x, y);
    cursorSprites_[1].set_position(-x, y);
}

void Menu::TogglePause_()
{
    state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::ALL);
    SetBlendingAndMosaic_(false);
    if (state_.isPaused)
    {
        bn::sound_items::sfx_menu_select.play(constant::volume::sfx_menu_select);
        bn::blending::set_fade_alpha(0);
        SetVisible_(false);
        state_.isPaused = false;
    }
    else
    {
        bn::sound::stop_all();
        bn::sound_items::sfx_menu_open.play(constant::volume::sfx_menu_open);
        cursorPointingOption_ = MenuOption::CONTINUE;
        UpdateCursorSpritePosition_();
        bn::blending::set_fade_alpha(PAUSE_FADE_ALPHA);
        SetVisible_(true);
        state_.isPaused = true;
    }
}

void Menu::AdvanceCursorPointingOption_(int amount)
{
    int cursorIdx = static_cast<int>(cursorPointingOption_);
    cursorIdx += (amount + MENU_OPTION_TOTAL_COUNT);
    cursorIdx %= MENU_OPTION_TOTAL_COUNT;
    cursorPointingOption_ = static_cast<MenuOption>(cursorIdx);
}

void Menu::RestartZone_()
{
    bn::sound_items::sfx_menu_select.play(constant::volume::sfx_menu_select);
    state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::ALL);
    state_.transition.SetMosaicAppliedItems(Transition::AppliedItems::ALL);
    SetVisible_(false);
    state_.zoneSwitch.InitTransition(state_.initialExitInfo, true);
    state_.isPaused = false;
}

void Menu::RestartStage_()
{
    bn::sound_items::sfx_menu_select.play(constant::volume::sfx_menu_select);
    state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::ALL);
    state_.transition.SetMosaicAppliedItems(Transition::AppliedItems::ALL);
    SetVisible_(false);

    state_.transition.InitOut(Transition::Types::FADE | Transition::Types::BG_MOSAIC | Transition::Types::SPRITE_MOSAIC,
                              STAGE_RESTART_FADE_UPDATE_COUNT);
    state_.transition.SetDoneEventHandler([this]() { state_.nextScene = scene::Type::GAME; });
}

void Menu::ChangeLanguage_()
{
    bn::sound_items::sfx_menu_select.play(constant::volume::sfx_menu_select);
    using namespace global::setting;
    SetLang(GetLang() == Lang::ENG ? Lang::KOR : Lang::ENG);
    state_.triggerInteraction.RedrawSignMessages();
    state_.stageNameSplash.RedrawSprites();
    RedrawMenuTextSprites_();
    UpdateCursorSpritePosition_();
}

void Menu::GoBackToStageSelect_()
{
    // TODO
}

} // namespace sym::game::system
