#pragma once

#include "game_system_ISystem.h"

namespace sym::game::system
{

/**
 * @brief Manages trigger interactions, but not colliders.
 * (e.g. symbol pick-up & put-down, button toggle).
 * Switching zones stuff (doors and exits) are dealt in `system::ZoneSwitch` instead.
 *
 */
class TriggerInteraction final : public ISystem
{
public:
    TriggerInteraction(scene::GameState& state);

    void Update() final;

private:
    void HoverButtonPlayerInteract_();
    void HoverButtonThrownSymbolInteract_();

    void UpdateKeyLastingCount_();
    [[nodiscard]] bool IsLKeyPressLasts_() const;
    [[nodiscard]] bool IsRKeyPressLasts_() const;
    void ResetLKeyPress_();
    void ResetRKeyPress_();
    void ToggleOpenedHoverButtonAssociatedOpenables_(int hoverButtonIdx);
};

} // namespace sym::game::system
