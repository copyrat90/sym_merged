#include "game_entity_IOpenableEntity.h"

namespace sym::game::entity
{

IOpenableEntity::~IOpenableEntity() = default;

IOpenableEntity::IOpenableEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                                 bn::fixed_point relativeNumberTextPosition, bool isOpened,
                                 const bn::sprite_item* spriteItem)
    : INumberTextEntity(position, relativeInteractRange, textNumber, relativeNumberTextPosition, spriteItem),
      isOpened_(isOpened)
{
}

IOpenableEntity::IOpenableEntity(IOpenableEntity&& other)
    : INumberTextEntity(bn::move(other)), isOpened_(other.isOpened_)
{
}

IOpenableEntity& IOpenableEntity::operator=(IOpenableEntity&& other)
{
    INumberTextEntity::operator=(bn::move(other));
    isOpened_ = other.isOpened_;
    return *this;
}

bool IOpenableEntity::GetOpened() const
{
    return isOpened_;
}

void IOpenableEntity::SetOpened(bool isOpened)
{
    isOpened_ = isOpened;
}

bool IOpenableEntity::ToggleOpened()
{
    return isOpened_ = !isOpened_;
}

} // namespace sym::game::entity
