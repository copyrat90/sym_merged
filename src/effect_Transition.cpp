#include "effect_Transition.h"

#include <bn_assert.h>
#include <bn_bgs_mosaic.h>
#include <bn_blending.h>
#include <bn_sprites_mosaic.h>

namespace sym::effect
{

Transition::Transition(Types types, Direction direction, int updateCount)
    : types_(types), direction_(direction), updateCountDown_(updateCount)
{
    if ((static_cast<int>(types & Types::FADE) > 0) &&
        ((static_cast<int>(types & Types::TRANSPARENCY) > 0) || (static_cast<int>(types & Types::INTENSITY))))
        BN_ERROR("FADE and other blendings cannot be enabled at the same time!");
}

Transition::~Transition()
{
    Destroy();
}

void Transition::Init()
{
    if (static_cast<int>(types_ & Types::FADE) > 0)
    {
        bn::blending::set_fade_alpha((direction_ == Direction::IN) ? 1 : 0);
        fadeAction_ = bn::blending_fade_alpha_to_action(updateCountDown_, (direction_ == Direction::IN) ? 0 : 1);
    }
    else
    {
        if (static_cast<int>(types_ & Types::TRANSPARENCY) > 0)
        {
            bn::blending::set_transparency_alpha((direction_ == Direction::IN) ? 0 : 1);
            transparencyAction_ =
                bn::blending_transparency_alpha_to_action(updateCountDown_, (direction_ == Direction::IN) ? 1 : 0);
        }
        if (static_cast<int>(types_ & Types::INTENSITY) > 0)
        {
            bn::blending::set_intensity_alpha((direction_ == Direction::IN) ? 1 : 0);
            intensityAction_ =
                bn::blending_intensity_alpha_to_action(updateCountDown_, (direction_ == Direction::IN) ? 0 : 1);
        }
    }
    if (static_cast<int>(types_ & Types::SPRITE_MOSAIC) > 0)
    {
        bn::sprites_mosaic::set_stretch((direction_ == Direction::IN) ? 1 : 0);
        spriteMosaicAction_ =
            bn::sprites_mosaic_stretch_to_action(updateCountDown_, (direction_ == Direction::IN) ? 0 : 1);
    }
    if (static_cast<int>(types_ & Types::BG_MOSAIC) > 0)
    {
        bn::bgs_mosaic::set_stretch((direction_ == Direction::IN) ? 1 : 0);
        bgMosaicAction_ = bn::bgs_mosaic_stretch_to_action(updateCountDown_, (direction_ == Direction::IN) ? 0 : 1);
    }
    state_ = State::ONGOING;
}

void Transition::Update()
{
    if (state_ == State::NOT_READY)
        BN_ERROR("Cannot update Transition when it is not initialized");
    if (state_ == State::DONE)
        BN_ERROR("Cannot update Transition when it is done");

    if (transparencyAction_)
        transparencyAction_->update();
    else
    {
        if (fadeAction_)
            fadeAction_->update();
        if (intensityAction_)
            intensityAction_->update();
    }
    if (spriteMosaicAction_)
        spriteMosaicAction_->update();
    if (bgMosaicAction_)
        bgMosaicAction_->update();

    if (--updateCountDown_ <= 0)
    {
        state_ = State::DONE;
        Destroy();
    }
}

void Transition::Destroy()
{
    transparencyAction_.reset();
    fadeAction_.reset();
    intensityAction_.reset();
    spriteMosaicAction_.reset();
    bgMosaicAction_.reset();
}

} // namespace sym::effect
