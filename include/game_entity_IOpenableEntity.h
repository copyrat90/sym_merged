#pragma once

#include "game_entity_INumberTextEntity.h"

namespace sym::game::entity
{

class IOpenableEntity : public INumberTextEntity
{
public:
    virtual ~IOpenableEntity() = 0;

    IOpenableEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                    bn::fixed_point relativeNumberTextPosition, bool isOpenedByDefault,
                    const bn::sprite_item* spriteItem = nullptr);

    IOpenableEntity(IOpenableEntity&& other) = delete;
    IOpenableEntity& operator=(IOpenableEntity&& other) = delete;

    IOpenableEntity(const IOpenableEntity& other) = delete;
    IOpenableEntity& operator=(const IOpenableEntity& other) = delete;

    void AllocateGraphicResource(int z_order) override;

    [[nodiscard]] bool GetOpened() const;
    // void SetOpened(bool);

    /**
     * @brief `IOpenableEntity::ToggleOpened()` must be called somewhere in the Openable implementation.
     *
     * @return `true` if it is opened
     */
    [[maybe_unused]] virtual bool ToggleOpened() = 0;

protected:
    const bool isOpenedByDefault_;
    bool isOpened_;
};

} // namespace sym::game::entity
