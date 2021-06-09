#pragma once

#include "game_entity_INumberTextEntity.h"

namespace sym::game::entity
{

class IButtonEntity : public INumberTextEntity
{
public:
    ~IButtonEntity() = 0;

    IButtonEntity(bn::fixed_point position, bn::fixed_rect relativeInteractRange, int textNumber,
                  bn::fixed_point relativeNumberTextPosition, bool isOnByDefault,
                  const bn::sprite_item* spriteItem = nullptr);

    IButtonEntity(IButtonEntity&& other);
    IButtonEntity& operator=(IButtonEntity&& other);

    IButtonEntity(const IButtonEntity& other) = delete;
    IButtonEntity& operator=(const IButtonEntity& other) = delete;

    [[nodiscard]] bool GetButtonOn() const;
    // void SetButtonOn(bool isButtonOn);
    [[nodiscard]] virtual bool CanButtonBeToggled() = 0;
    /**
     * @brief `CanButtonBeToggled()` needs to be checked first before using this function.
     * `IButtonEntity::ToggleButtonOn()` must be called somewhere in the Button implementation.
     *
     * @return `true` if button is on
     */
    [[maybe_unused]] virtual bool ToggleButtonOn() = 0;

private:
    bool isButtonOn_;

    void SetGraphicToButtonState_();
};

} // namespace sym::game::entity
