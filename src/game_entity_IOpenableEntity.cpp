#include "game_entity_IOpenableEntity.h"

namespace sym::game::entity
{

IOpenableEntity::~IOpenableEntity() = default;

IOpenableEntity::IOpenableEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                                 bn::fixed_point relativeNumberTextPosition, bool isOpenedByDefault,
                                 const bn::sprite_item* spriteItem)
    : INumberTextEntity(position, relativeInteractRange, textNumber, relativeNumberTextPosition, spriteItem),
      isOpenedByDefault_(isOpenedByDefault), isOpened_(isOpenedByDefault)
{
}

void IOpenableEntity::AllocateGraphicResource(int z_order)
{
    INumberTextEntity::AllocateGraphicResource(z_order);
    if (isOpenedByDefault_)
    {
        numberSprite_->set_horizontal_flip(true);
        numberSprite_->set_vertical_flip(true);
    }
}

bool IOpenableEntity::GetOpened() const
{
    return isOpened_;
}

// void IOpenableEntity::SetOpened(bool isOpened)
// {
//     isOpened_ = isOpened;
// }

bool IOpenableEntity::ToggleOpened()
{
    return isOpened_ = !isOpened_;
}

} // namespace sym::game::entity
