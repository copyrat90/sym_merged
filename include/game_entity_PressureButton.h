#pragma once

#include "game_entity_IButtonEntity.h"

#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class PressureButton final : public IButtonEntity
{
public:
    PressureButton(bn::fixed_point position, int textNumber, bool isOnByDefault);

    PressureButton(PressureButton&& other) = delete;
    PressureButton& operator=(PressureButton&& other) = delete;

    PressureButton(const PressureButton& other) = default;
    PressureButton& operator=(const PressureButton& other) = default;

    void AllocateGraphicResource(int z_order) final;
    void FreeGraphicResource() final;

    void Update() final;

    [[nodiscard]] bool CanButtonBeToggled() final;
    /**
     * @brief Toggles button (and also animates it)
     *
     * `CanButtonBeToggled()` needs to be checked first before using this function.
     *
     * @return `true` if button is on
     */
    [[maybe_unused]] bool ToggleButtonOn() final;

private:
    bn::optional<bn::sprite_animate_action<2>> action_;

    void InitButtonOnAction_();
    void InitButtonOffAction_();
};

} // namespace sym::game::entity
