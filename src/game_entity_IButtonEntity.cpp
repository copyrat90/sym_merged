#include "game_entity_IButtonEntity.h"

#include <bn_assert.h>

namespace sym::game::entity
{

IButtonEntity::~IButtonEntity() = default;

IButtonEntity::IButtonEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                             bn::fixed_point relativeNumberTextPosition, bool isOnByDefault,
                             const bn::sprite_item* spriteItem)
    : INumberTextEntity(position, relativeInteractRange, textNumber, relativeNumberTextPosition, spriteItem),
      isButtonOn_(isOnByDefault)
{
}

IButtonEntity::IButtonEntity(IButtonEntity&& other) : INumberTextEntity(bn::move(other)), isButtonOn_(other.isButtonOn_)
{
}

IButtonEntity& IButtonEntity::operator=(IButtonEntity&& other)
{
    INumberTextEntity::operator=(bn::move(other));
    isButtonOn_ = other.isButtonOn_;
    SetGraphicToButtonState_();
    return *this;
}

bool IButtonEntity::GetButtonOn() const
{
    return isButtonOn_;
}

// void IButtonEntity::SetButtonOn(bool isButtonOn)
// {
//     isButtonOn_ = isButtonOn;
// }

bool IButtonEntity::ToggleButtonOn()
{
    BN_ASSERT(CanButtonBeToggled(), "Button can't be toggled when action is not done");
    return isButtonOn_ = !isButtonOn_;
}

} // namespace sym::game::entity
