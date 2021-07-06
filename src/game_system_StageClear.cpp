#include "game_system_StageClear.h"

#include <cstdint>

#include <bn_assert.h>
#include <bn_music.h>
#include <bn_sound.h>
#include <bn_sprite_text_generator.h>
#include <bn_string_view.h>

#include "constant.h"
#include "global.h"
#include "helper_math.h"
#include "scene_GameState.h"

#include "bn_sound_items.h"
#include "bn_sprite_items_spr_ingame_protagonist_star.h"

namespace sym::game::system
{

namespace
{

constexpr int ENTER_BLACK_HOLE_STATE_COUNT = 75;
constexpr int ENTER_BLACK_HOLE_ACTION_UPDATE_COUNT = 50;
constexpr int TEXT_SHOW_UP_UPDATE_COUNT = 60;
constexpr int WAIT_BEFORE_SCREEN_FADE_OUT = 120;
constexpr int FADE_OUT_UPDATE_COUNT = 30;
constexpr int WAIT_ON_BLACK_SCREEN_COUNT = 15;

constexpr bn::fixed TEXT_SPRITES_Y_POS[2] = {-10, 10};

constexpr bn::fixed FINAL_SCALE = 0.01;
constexpr bn::fixed ENTER_BLACK_HOLE_POS_DELTA = bn::fixed(1) / ENTER_BLACK_HOLE_ACTION_UPDATE_COUNT;

constexpr bn::string_view TEXT_ENG[2] = {
    "S T A G E",
    "C L E A R",
};
constexpr bn::string_view TEXT_KOR[2] = {
    "스테이지",
    "클리어",
};

[[nodiscard]] bn::optional<game::stage::Id> GetNextStage_(game::stage::Id prevStage)
{
    int nextIdx = static_cast<uint8_t>(prevStage) + 1;
    if (nextIdx >= static_cast<uint8_t>(game::stage::Id::STAGE_ID_TOTAL_COUNT))
        return bn::nullopt;
    return static_cast<game::stage::Id>(nextIdx);
}

} // namespace

StageClear::StageClear(scene::GameState& state) : ISystem(state)
{
}

void StageClear::Update()
{
    switch (clearState_)
    {
    case ActionState::NOT_STARTED:
        if (CheckCollideWithBlackHole_())
        {
            state_.isClearOngoing = true;
            state_.player.SetControllable(false);
            state_.player.SetGravityEnabled(false);
            const auto playerPos = state_.player.GetPosition();
            const auto blackHolePos = state_.blackHole->GetPosition();
            const auto leftSymbolPos =
                (state_.symbolsInHands[0]) ? state_.symbolsInHands[0]->GetPosition() : bn::fixed_point{0, 0};
            const auto rightSymbolPos =
                (state_.symbolsInHands[1]) ? state_.symbolsInHands[1]->GetPosition() : bn::fixed_point{0, 0};
            using helper::math::operator*;
            playerDeltaPos_ = ENTER_BLACK_HOLE_POS_DELTA * (blackHolePos - playerPos);
            leftSymbolDeltaPos_ = ENTER_BLACK_HOLE_POS_DELTA * (blackHolePos - leftSymbolPos);
            rightSymbolDeltaPos_ = ENTER_BLACK_HOLE_POS_DELTA * (blackHolePos - rightSymbolPos);

            {
                auto playerSprite = state_.player.GetSprite();
                // if (playerSprite)
                playerScaleAction_.emplace(*playerSprite, ENTER_BLACK_HOLE_ACTION_UPDATE_COUNT, FINAL_SCALE);
            }
            if (state_.symbolsInHands[0])
            {
                auto leftSymbolSprite = state_.symbolsInHands[0]->GetSprite();
                // if (leftSymbolSprite)
                leftSymbolScaleAction_.emplace(*leftSymbolSprite, ENTER_BLACK_HOLE_ACTION_UPDATE_COUNT, FINAL_SCALE);
            }
            if (state_.symbolsInHands[1])
            {
                auto rightSymbolSprite = state_.symbolsInHands[1]->GetSprite();
                // if (rightSymbolSprite)
                rightSymbolScaleAction_.emplace(*rightSymbolSprite, ENTER_BLACK_HOLE_ACTION_UPDATE_COUNT, FINAL_SCALE);
            }

            bn::music::stop();
            bn::sound::stop_all();
            bn::sound_items::sfx_enter_black_hole.play(constant::volume::sfx_enter_black_hole);
            updateCountDown_ = ENTER_BLACK_HOLE_STATE_COUNT;

            clearState_ = ActionState::ENTER_BLACK_HOLE;
        }
        break;
    case ActionState::ENTER_BLACK_HOLE:
        if (--updateCountDown_ >= 0)
        {
            if (!playerScaleAction_->done())
            {
                playerScaleAction_->update();
                state_.player.SetPosition(state_.player.GetPosition() + playerDeltaPos_);
                if (state_.symbolsInHands[0] && !leftSymbolScaleAction_->done())
                {
                    auto& symbol = state_.symbolsInHands[0];
                    leftSymbolScaleAction_->update();
                    symbol->SetPosition(symbol->GetPosition() + leftSymbolDeltaPos_);
                }
                if (state_.symbolsInHands[1] && !rightSymbolScaleAction_->done())
                {
                    auto& symbol = state_.symbolsInHands[1];
                    rightSymbolScaleAction_->update();
                    symbol->SetPosition(symbol->GetPosition() + rightSymbolDeltaPos_);
                }
            }
            else
            {
                state_.player.SetVisible(false);
                if (state_.symbolsInHands[0])
                    state_.symbolsInHands[0]->SetVisible(false);
                if (state_.symbolsInHands[1])
                    state_.symbolsInHands[1]->SetVisible(false);
            }
        }
        else
        {
            // disable player & symbols in hands
            playerScaleAction_.reset();
            leftSymbolScaleAction_.reset();
            rightSymbolScaleAction_.reset();

            // sfx
            bn::sound::stop_all();
            bn::sound_items::sfx_stage_clear.play(constant::volume::sfx_stage_clear);

            // draw texts & set blending, mosaic
            {
                using namespace global::setting;
                auto* const textGen = global::GetTextGen();
                const auto& texts = (GetLang() == Lang::ENG) ? TEXT_ENG : TEXT_KOR;
                auto prevPal = textGen->palette_item();
                textGen->set_palette_item(bn::sprite_items::spr_ingame_protagonist_star.palette_item());
                for (int i = 0; i < textSprites_.size(); ++i)
                {
                    textGen->generate(0, TEXT_SPRITES_Y_POS[i], texts[i], textSprites_[i]);
                    for (auto& sprite : textSprites_[i])
                    {
                        sprite.set_blending_enabled(true);
                        sprite.set_mosaic_enabled(true);
                    }
                }
                textGen->set_palette_item(prevPal);
            }

            // set transition
            state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::NONE);
            state_.transition.SetMosaicAppliedItems(Transition::AppliedItems::NONE);
            state_.transition.InitIn(Transition::Types::SPRITE_MOSAIC | Transition::Types::TRANSPARENCY,
                                     TEXT_SHOW_UP_UPDATE_COUNT, WAIT_BEFORE_SCREEN_FADE_OUT);
            state_.transition.SetDoneEventHandler([this] {
                state_.transition.SetBlendingAppliedItems(Transition::AppliedItems::ALL);
                state_.transition.SetMosaicAppliedItems(Transition::AppliedItems::ALL);
                state_.transition.InitOut(Transition::Types::FADE | Transition::Types::BG_MOSAIC |
                                              Transition::Types::SPRITE_MOSAIC,
                                          FADE_OUT_UPDATE_COUNT, WAIT_ON_BLACK_SCREEN_COUNT);
                state_.transition.SetDoneEventHandler([this] {
                    auto nextStage = GetNextStage_(state_.sceneParam_.GetCurrentStage());
                    if (nextStage)
                    {
                        state_.sceneParam_.SetCurrentStage(*nextStage);
                        state_.nextScene = scene::Type::GAME;
                    }
                    else
                    {
                        // only for jam release
                        state_.sceneParam_.SetCurrentStage(game::stage::Id::W1_S0);
                        state_.nextScene = scene::Type::ENDING;
                    }
                });
            });

            clearState_ = ActionState::SPRITE_ACTION_DONE;
        }
        break;
    case ActionState::SPRITE_ACTION_DONE:
        break;
    default:
        BN_ERROR("Invalid StageClear::State: ", static_cast<int>(clearState_));
    }
}

bool StageClear::CheckCollideWithBlackHole_()
{
    if (!state_.blackHole || state_.currentZoneIdx != state_.blackHoleZoneIdx)
        return false;
    return state_.player.GetPhysicsCollider().intersects(state_.blackHole->GetInteractRange());
}

} // namespace sym::game::system
