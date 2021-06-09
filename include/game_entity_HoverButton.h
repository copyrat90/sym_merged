#pragma once

#include "game_entity_IButtonEntity.h"

#include <bn_optional.h>
#include <bn_sprite_animate_actions.h>

namespace sym::game::entity
{

class HoverButton final : public IButtonEntity
{
public:
    HoverButton(bn::fixed_point position, int textNumber, bool isOnByDefault);

    HoverButton(HoverButton&& other) = delete;
    HoverButton& operator=(HoverButton&& other) = delete;

    HoverButton(const HoverButton& other) = delete;
    HoverButton& operator=(const HoverButton& other) = delete;

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
