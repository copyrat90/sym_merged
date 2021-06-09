#pragma once

#include "game_entity_IEntity.h"

namespace sym::game::entity
{

class INumberTextEntity : public IEntity
{
public:
    virtual ~INumberTextEntity() = 0;

    INumberTextEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                      bn::fixed_point relativeNumberTextPosition, const bn::sprite_item* spriteItem = nullptr);

    INumberTextEntity(INumberTextEntity&& other);
    INumberTextEntity& operator=(INumberTextEntity&& other);

    INumberTextEntity(const INumberTextEntity& other) = delete;
    INumberTextEntity& operator=(const INumberTextEntity& other) = delete;

    void FreeGraphicResource();
    void AllocateGraphicResource(int z_order);

    void SetCamera(const bn::camera_ptr& camera);

protected:
    bn::optional<bn::sprite_ptr> numberSprite_;
    int textNumber_;
    bn::fixed_point relativeNumberTextPosition_;
};

} // namespace sym::game::entity
