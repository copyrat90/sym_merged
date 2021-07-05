#include "game_system_StageNameSplash.h"

#include <bn_assert.h>
#include <bn_display.h>

#include "constant.h"
#include "global.h"
#include "helper_sprite.h"
#include "scene_GameState.h"

#include "bn_sprite_palette_items_pal_menu_header.h"

namespace sym::game::system
{

namespace
{

constexpr int START_POS_X = -bn::display::width();
constexpr int CENTER_POS_X = 0;
constexpr int END_POS_X = bn::display::width();

constexpr int NAME_POS_Y = -12;
constexpr int SUB_NAME_POS_Y = 12;

constexpr bn::fixed DELAY_BETWEEN_NAME_AND_SUBNAME_SECONDS = 0.5;

constexpr int DELAY_BETWEEN_NAME_AND_SUBNAME_UPDATE_COUNT = (DELAY_BETWEEN_NAME_AND_SUBNAME_SECONDS * 60).integer();
constexpr int SHOW_UP_UPDATE_COUNT = 90;
constexpr int WAIT_CENTER_UPDATE_COUNT = 150;
constexpr int FLY_AWAY_UPDATE_COUNT = 60;

constexpr bn::fixed SHOW_UP_DX = bn::fixed(CENTER_POS_X - START_POS_X) / SHOW_UP_UPDATE_COUNT;
constexpr bn::fixed FLY_AWAY_DX = bn::fixed(END_POS_X - CENTER_POS_X) / FLY_AWAY_UPDATE_COUNT;

void MoveSprites_(bn::fixed& posX, bn::vector<bn::sprite_ptr, StageNameSplash::SENTENCE_SPRITE_SIZE>& sprites,
                  bn::fixed dx)
{
    posX += dx;
    for (auto& sprite : sprites)
        sprite.set_x(sprite.x() + dx);
}

} // namespace

StageNameSplash::StageNameSplash(scene::GameState& state)
    : ISystem(state), nameCountDown_(SHOW_UP_UPDATE_COUNT),
      subNameCountDown_(DELAY_BETWEEN_NAME_AND_SUBNAME_UPDATE_COUNT), namePosX_(START_POS_X), subNamePosX_(START_POS_X)
{
    RedrawSprites();
}

void StageNameSplash::Update()
{
    if (subNameState_ == State::DONE)
    {
        if (!subNameSprites_.empty())
        {
            Destroy();
        }
        return;
    }

    switch (nameState_)
    {
    case State::SHOW_UP:
        MoveSprites_(namePosX_, nameSprites_, SHOW_UP_DX);
        if (--nameCountDown_ <= 0)
        {
            nameCountDown_ = WAIT_CENTER_UPDATE_COUNT;
            nameState_ = State::WAIT_CENTER;
        }
        break;
    case State::WAIT_CENTER:
        if (--nameCountDown_ <= 0)
        {
            nameCountDown_ = FLY_AWAY_UPDATE_COUNT;
            nameState_ = State::FLY_AWAY;
        }
        break;
    case State::FLY_AWAY:
        MoveSprites_(namePosX_, nameSprites_, FLY_AWAY_DX);
        if (--nameCountDown_ <= 0)
        {
            nameState_ = State::DONE;
        }
        break;
    case State::DONE:
        break;
    case State::NOT_STARTED:
    default:
        BN_ERROR("Invalid StageNameSplash::nameState_: ", static_cast<int>(nameState_));
    }

    switch (subNameState_)
    {
    case State::NOT_STARTED:
        if (--subNameCountDown_ <= 0)
        {
            subNameCountDown_ = SHOW_UP_UPDATE_COUNT;
            subNameState_ = State::SHOW_UP;
        }
        break;
    case State::SHOW_UP:
        MoveSprites_(subNamePosX_, subNameSprites_, SHOW_UP_DX);
        if (--subNameCountDown_ <= 0)
        {
            subNameCountDown_ = WAIT_CENTER_UPDATE_COUNT;
            subNameState_ = State::WAIT_CENTER;
        }
        break;
    case State::WAIT_CENTER:
        if (--subNameCountDown_ <= 0)
        {
            subNameCountDown_ = FLY_AWAY_UPDATE_COUNT;
            subNameState_ = State::FLY_AWAY;
        }
        break;
    case State::FLY_AWAY:
        MoveSprites_(subNamePosX_, subNameSprites_, FLY_AWAY_DX);
        if (--subNameCountDown_ <= 0)
        {
            subNameState_ = State::DONE;
        }
        break;
    case State::DONE:
        break;
    default:
        BN_ERROR("Invalid StageNameSplash::subNameState_: ", static_cast<int>(subNameState_));
    }
}

void StageNameSplash::Destroy()
{
    nameState_ = State::DONE;
    subNameState_ = State::DONE;
    nameSprites_.clear();
    subNameSprites_.clear();
}

void StageNameSplash::RedrawSprites()
{
    if (subNameState_ == State::DONE)
        return;

    using namespace global;
    const bool isEnglish = setting::GetLang() == setting::Lang::ENG;
    const bn::string_view& stageName = isEnglish ? state_.stageInfo.stageNameEng : state_.stageInfo.stageNameKor;
    const bn::string_view& stageSubName =
        isEnglish ? state_.stageInfo.stageSubNameEng : state_.stageInfo.stageSubNameKor;

    nameSprites_.clear();
    subNameSprites_.clear();
    auto* const textGen = GetTextGen();
    auto prevPal = textGen->palette_item();
    textGen->set_palette_item(bn::sprite_palette_items::pal_menu_header);
    textGen->generate(namePosX_, NAME_POS_Y, stageName, nameSprites_);
    textGen->set_palette_item(prevPal);
    textGen->generate(subNamePosX_, SUB_NAME_POS_Y, stageSubName, subNameSprites_);
    for (auto& sprite : nameSprites_)
    {
        sprite.set_blending_enabled(true);
        sprite.set_mosaic_enabled(true);
        sprite.set_z_order(constant::STAGE_NAME_SPLASH_Z_ORDER);
    }
    for (auto& sprite : subNameSprites_)
    {
        sprite.set_blending_enabled(true);
        sprite.set_mosaic_enabled(true);
        sprite.set_z_order(constant::STAGE_NAME_SPLASH_Z_ORDER);
    }
}

} // namespace sym::game::system
