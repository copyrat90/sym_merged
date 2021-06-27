#include "effect_Transition.h"

#include <bn_assert.h>
#include <bn_bgs_mosaic.h>
#include <bn_blending.h>
#include <bn_sprites_mosaic.h>

namespace sym::effect
{

Transition::Transition(Types types, Direction direction, int updateCount)
    : types_(types), direction_(direction), updateCount_(updateCount), updateCountDown_(updateCount)
{
    BN_ASSERT(!((static_cast<int>(types & Types::FADE) > 0) &&
                ((static_cast<int>(types & Types::TRANSPARENCY) > 0) || (static_cast<int>(types & Types::INTENSITY)))),
              "FADE and other blendings cannot be enabled at the same time!");
    BN_ASSERT(updateCount_ > 0, "updateCount should be positive number");
}

Transition::Transition() : Transition(static_cast<Types>(0), Direction::NONE, 1)
{
}

Transition::~Transition()
{
    Destroy();
}

void Transition::Init()
{
    BN_ASSERT(static_cast<int>(types_) != 0, "Cannot Init empty transition");
    BN_ASSERT(direction_ != Direction::NONE, "Cannot Init empty transition");
    BN_ASSERT(updateCount_ > 0, "updateCount should be positive number");

    updateCountDown_ = updateCount_;
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

void Transition::Init(Types types, Direction direction, int updateCount)
{
    Set(types, direction, updateCount);
    Init();
}

void Transition::Update()
{
    BN_ASSERT(state_ != State::NOT_READY, "Cannot update Transition when it is not initialized");
    BN_ASSERT(state_ != State::DONE, "Cannot update Transition when it is done");

    if (fadeAction_)
        fadeAction_->update();
    else
    {
        if (transparencyAction_)
            transparencyAction_->update();
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

void Transition::Set(Types types, Direction direction, int updateCount)
{
    BN_ASSERT(state_ != State::ONGOING, "Can't change transition options when it is ongoing");
    types_ = types;
    direction_ = direction;
    updateCount_ = updateCount;
    updateCountDown_ = updateCount;
}

Transition::Types Transition::GetTypes() const
{
    return types_;
}

void Transition::SetTypes(Types types)
{
    BN_ASSERT(state_ != State::ONGOING, "Can't change transition types when it is ongoing");
    types_ = types;
}

Transition::Direction Transition::GetDirection() const
{
    return direction_;
}

void Transition::SetDirection(Direction direction)
{
    BN_ASSERT(state_ != State::ONGOING, "Can't change direction when transition is ongoing");
    direction_ = direction;
}

int Transition::GetUpdateCount() const
{
    return updateCount_;
}

void Transition::SetUpdateCount(int updateCount)
{
    BN_ASSERT(state_ != State::ONGOING, "Can't change update count when transition is ongoing");
    updateCount_ = updateCount;
}

int Transition::GetRemainingUpdateCount() const
{
    return updateCountDown_;
}

} // namespace sym::effect
