#pragma once

#include "game_entity_INumberTextEntity.h"

namespace sym::game::entity
{

class IOpenableEntity : public INumberTextEntity
{
public:
    virtual ~IOpenableEntity() = 0;

    IOpenableEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                    bn::fixed_point relativeNumberTextPosition, bool isOpened,
                    const bn::sprite_item* spriteItem = nullptr);

    IOpenableEntity(IOpenableEntity&& other);
    IOpenableEntity& operator=(IOpenableEntity&& other);

    IOpenableEntity(const IOpenableEntity& other) = delete;
    IOpenableEntity& operator=(const IOpenableEntity& other) = delete;

    bool GetOpened() const;
    void SetOpened(bool);
    [[maybe_unused]] bool ToggleOpened();

private:
    bool isOpened_;
};

} // namespace sym::game::entity
