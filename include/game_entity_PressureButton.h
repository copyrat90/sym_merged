#pragma once

#include "game_entity_IButtonEntity.h"

#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>

#include "game_entity_Symbol.h"

namespace sym::game::entity
{

class PressureButton final : public IButtonEntity
{
public:
    PressureButton(bn::fixed_point position, int textNumber, bool isOnByDefault, bool isVerticallyReversed,
                   bn::optional<entity::Symbol::Type> requiredSymbolType);

    PressureButton(PressureButton&& other) = delete;
    PressureButton& operator=(PressureButton&& other) = delete;

    PressureButton(const PressureButton& other) = default;
    PressureButton& operator=(const PressureButton& other) = default;

    bn::fixed_rect GetInteractRange() const final;

    void AllocateGraphicResource(int z_order) final;
    void FreeGraphicResource() final;

    void Update() final;

    void SetBlendingEnabled(bool isEnable) final;
    void SetMosaicEnabled(bool isEnable) final;
    void SetCamera(const bn::camera_ptr& camera) final;

    [[nodiscard]] bool CanButtonBeToggled() final;
    /**
     * @brief Toggles button (and also animates it)
     *
     * `CanButtonBeToggled()` needs to be checked first before using this function.
     *
     * @return `true` if button is on
     */
    [[maybe_unused]] bool ToggleButtonOn() final;

    bn::optional<entity::Symbol::Type> GetRequiredSymbolType() const;

private:
    bn::optional<bn::sprite_animate_action<2>> action_;

    bool isOnCeil_;
    bn::optional<entity::Symbol::Type> requiredSymbolType_;
    bn::optional<bn::sprite_ptr> requiredSymbolSprite_;

    void InitButtonOnAction_();
    void InitButtonOffAction_();

    void AllocateRequiredSymbolPlaceholderGraphic_(int z_order);
};

} // namespace sym::game::entity
